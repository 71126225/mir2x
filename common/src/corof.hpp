/*
 * =====================================================================================
 *
 *       Filename: corof.hpp
 *        Created: 03/07/2020 12:36:32
 *    Description:
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#pragma once
#include <any>
#include <optional>
#include <coroutine>
#include "fflerror.hpp"
#include "raiitimer.hpp"

namespace corof
{
    class eval_poller;
    class eval_poller_promise;
    template<typename T> class [[nodiscard]] eval_awaiter
    {
        private:
            friend class eval_poller_promise;

        private:
            std::coroutine_handle<eval_poller_promise> m_eval_handle;

        public:
            explicit eval_awaiter(eval_poller &&) noexcept;

        public:
            ~eval_awaiter()
            {
                if(m_eval_handle){
                    m_eval_handle.destroy();
                }
            }

        public:
            eval_awaiter & operator = (eval_awaiter && other) noexcept
            {
                m_eval_handle = other.m_eval_handle;
                other.m_eval_handle = nullptr;
                return *this;
            }

        public:
            bool await_ready() noexcept
            {
                return false;
            }

        public:
            bool await_suspend(std::coroutine_handle<eval_poller_promise> handle) noexcept;
            decltype(auto) await_resume();
    };

    class [[nodiscard]] eval_poller
    {
        public:
            using promise_type = eval_poller_promise;
            using  handle_type = std::coroutine_handle<promise_type>;

        public:
            handle_type m_handle;

        public:
            eval_poller(handle_type handle = nullptr)
                : m_handle(handle)
            {}

            eval_poller(eval_poller&& other) noexcept
                : m_handle(other.m_handle)
            {
                other.m_handle = nullptr;
            }

        public:
            eval_poller & operator = (eval_poller && other) noexcept
            {
                m_handle = other.m_handle;
                other.m_handle = nullptr;
                return *this;
            }

        public:
            ~eval_poller()
            {
                if(m_handle){
                    m_handle.destroy();
                }
            }

        public:
            bool valid() const
            {
                return m_handle.address();
            }

        public:
            inline bool poll_one();

        private:
            static inline handle_type find_handle(handle_type);

        public:
            template<typename T> [[nodiscard]] eval_awaiter<T> to_awaiter()
            {
                fflassert(valid());
                return eval_awaiter<T>(std::move(*this));
            }

            template<typename T> auto sync_eval()
            {
                while(!poll_one()){
                    continue;
                }
                return to_awaiter<T>().await_resume();
            }
    };

    class eval_poller_promise
    {
        private:
            friend class eval_poller;
            template<typename T> friend class eval_awaiter;

        private:
            std::any m_value;
            eval_poller::handle_type m_inner_handle;
            eval_poller::handle_type m_outer_handle;

        public:
            template<typename T> T &get_value()
            {
                return std::any_cast<T &>(m_value);
            }

            auto initial_suspend()
            {
                return std::suspend_never{};
            }

            auto final_suspend() noexcept
            {
                return std::suspend_always{};
            }

            template<typename T> auto return_value(T t)
            {
                m_value = std::move(t);
                return std::suspend_always{};
            }

            eval_poller get_return_object()
            {
                return {std::coroutine_handle<eval_poller_promise>::from_promise(*this)};
            }

            void unhandled_exception()
            {
                std::terminate();
            }

            void rethrow_if_unhandled_exception()
            {
            }
    };

    inline eval_poller::handle_type eval_poller::find_handle(eval_poller::handle_type start_handle)
    {
        fflassert(start_handle);
        auto curr_handle = start_handle;
        auto next_handle = start_handle.promise().m_inner_handle;

        while(curr_handle && next_handle){
            curr_handle = next_handle;
            next_handle = next_handle.promise().m_inner_handle;
        }
        return curr_handle;
    }

    inline bool eval_poller::poll_one()
    {
        fflassert(m_handle);
        handle_type curr_handle = find_handle(m_handle);
        if(curr_handle.done()){
            if(!curr_handle.promise().m_outer_handle){
                return true;
            }

            // jump out for one layer
            // should I call destroy() for done handle?

            curr_handle = curr_handle.promise().m_outer_handle;
            curr_handle.promise().m_inner_handle = nullptr;

            if(curr_handle.done()){
                throw fflerror("linked done handle detected");
            }
        }

        // resume only once and return immediately
        // after resume curr_handle can be in done state, next call to poll_one should unlink it

        curr_handle.resume();
        return m_handle.done();
    }

    template<typename T> eval_awaiter<T>::eval_awaiter(eval_poller &&jmper) noexcept
        : m_eval_handle(jmper.m_handle)
    {
        jmper.m_handle = nullptr;
    }

    template<typename T> inline bool eval_awaiter<T>::await_suspend(std::coroutine_handle<eval_poller_promise> handle) noexcept
    {
        handle       .promise().m_inner_handle = m_eval_handle;
        m_eval_handle.promise().m_outer_handle = handle;
        return true;
    }

    template<typename T> inline decltype(auto) eval_awaiter<T>::await_resume()
    {
        return m_eval_handle.promise().get_value<T>();
    }
}

namespace corof
{
    template<typename T> class async_variable
    {
        private:
            std::optional<T> m_var;

        public:
            template<typename U = T> void assign(U &&u)
            {
                if(m_var.has_value()){
                    throw fflerror("assign value to async_variable twice");
                }
                m_var = std::move(u);
            }

        public:
            async_variable() = default;

        public:
            template<typename U> async_variable(const async_variable<U> &) = delete;
            template<typename U = T> async_variable<T> & operator = (const async_variable<U> &) = delete;

        public:
            auto operator co_await() noexcept
            {
                const auto fnwait = +[](corof::async_variable<T> *p) -> corof::eval_poller
                {
                    while(!p->m_var.has_value()){
                        co_await std::suspend_always{};
                    }
                    co_return p->m_var.value();
                };
                return fnwait(this). template to_awaiter<T>();
            }
    };

    inline auto async_wait(uint64_t msec) noexcept
    {
        const auto fnwait = +[](uint64_t msec) -> corof::eval_poller
        {
            size_t count = 0;
            if(msec == 0){
                co_await std::suspend_always{};
                count++;
            }
            else{
                hres_timer timer;
                while(timer.diff_msec() < msec){
                    co_await std::suspend_always{};
                    count++;
                }
            }
            co_return count;
        };
        return fnwait(msec).to_awaiter<size_t>();
    }

    template<typename T> inline auto delay_value(uint64_t msec, T t) // how about variadic template argument
    {
        const auto fnwait = +[](uint64_t msec, T t) -> corof::eval_poller
        {
            co_await corof::async_wait(msec);
            co_return t;
        };
        return fnwait(msec, std::move(t)). template to_awaiter<T>();
    }
}
