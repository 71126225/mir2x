#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <SDL2/SDL.h>

#include "colorf.hpp"
#include "widget.hpp"
#include "imageboard.hpp"

class TextBoard: public Widget
{
    private:
        ImageBoard m_image;

    public:
        TextBoard(
                Widget::VarDir,
                Widget::VarOff,
                Widget::VarOff,

                std::function<std::string(const Widget *)>,

                uint8_t =  0,
                uint8_t = 10,
                uint8_t =  0,

                Widget::VarColor = colorf::WHITE + colorf::A_SHF(0XFF),

                Widget * = nullptr,
                bool     = false);

    public:
        void setFont(uint8_t);
        void setFontSize(uint8_t);
        void setFontStyle(uint8_t);
        void setFontColor(uint32_t);

    public:
        void clear()
        {
            m_tpset.clear();
        }

        bool empty() const
        {
            return m_tpset.empty();
        }

    public:
        void drawEx(int argDstX, int argDstY, int argSrcX, int argSrcY, int argSrcW, int argSrcH) const override
        {
            m_tpset.drawEx(argDstX, argDstY, argSrcX, argSrcY, argSrcW, argSrcH);
        }
};
