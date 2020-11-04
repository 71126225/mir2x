/*
 * =====================================================================================
 *
 *       Filename: charobject.hpp
 *        Created: 04/10/2016 12:05:22
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

#include <map>
#include <list>
#include <deque>
#include <vector>

#include "typecast.hpp"
#include "svobuf.hpp"
#include "fflerror.hpp"
#include "servermap.hpp"
#include "damagenode.hpp"
#include "actionnode.hpp"
#include "cachequeue.hpp"
#include "servicecore.hpp"
#include "protocoldef.hpp"
#include "serverobject.hpp"

enum _RangeType: uint8_t
{
    RANGE_VIEW,
    RANGE_MAP,
    RANGE_SERVER,

    RANGE_VISIBLE,
    RANGE_ATTACK,
    RANGE_TRACETARGET,
};

// cache entry for charobject location
// should be visible for CharObject and its derived classes
struct COLocation
{
    uint64_t UID;
    uint32_t MapID;
    uint32_t RecordTime;

    int X;
    int Y;
    int Direction;

    COLocation(
            uint64_t nUID        = 0,
            uint32_t nMapID      = 0,
            uint32_t nRecordTime = 0,

            int nX = -1,
            int nY = -1,
            int nDirection = DIR_NONE)
        : UID(nUID)
        , MapID(nMapID)
        , RecordTime(nRecordTime)
        , X(nX)
        , Y(nY)
        , Direction(nDirection)
    {}
};

class CharObject: public ServerObject
{
    protected:
        class COPathFinder final: public AStarPathFinder
        {
            private:
                friend class CharObject;

            private:
                const CharObject *m_CO;

            private:
                const int m_checkCO;

            private:
                mutable std::map<uint32_t, int> m_cache;

            public:
                COPathFinder(const CharObject *, int);
               ~COPathFinder() = default;

            private:
               int GetGrid(int, int) const;
        };

    protected:
        enum QueryType: int
        {
            QUERY_NONE    = 0,
            QUERY_OK      = 1,
            QUERY_ERROR   = 2,
            QUERY_PENDING = 3,
        };

    protected:
        enum SpeedType: int
        {
            SPEED_NONE = 0,
            SPEED_MOVE,
            SPEED_ATTACK,
        };

    protected:
        struct Offender
        {
            uint64_t UID = 0;
            uint32_t Damage = 0;
            uint32_t ActiveTime = 0;

            Offender(uint64_t nUID = 0, uint32_t nDamage = 0, uint32_t nActiveTime = 0)
                : UID(nUID)
                , Damage(nDamage)
                , ActiveTime(nActiveTime)
            {}
        };

        struct Target
        {
            uint64_t UID = 0;
            uint32_t ActiveTime = 0;

            Target(uint64_t nUID = 0, uint32_t nActiveTime = 0)
                : UID(nUID)
                , ActiveTime(nActiveTime)
            {}
        };

    protected:
        const ServiceCore *m_serviceCore;
        const ServerMap   *m_map;

    protected:
        const ServerMap *GetServerMap() const
        {
            return m_map;
        }

    protected:
        // list of all COs *this* CO can see
        // 1. used for path finding
        // 2. directly report to these COs for action non-moving
        // 3. need to report to map if moving
        // 4. part of these COs are neighbors if close enough
        // 5. don't remove COs in this list if expired, otherwise action in (2) may miss
        std::vector<COLocation> m_inViewCOList;

    protected:
        int m_X;
        int m_Y;
        int m_direction;

    protected:
        int m_HP;
        int m_HPMax;
        int m_MP;
        int m_MPMax;

    protected:
        bool     m_moveLock;
        bool     m_attackLock;
        uint32_t m_lastMoveTime;
        uint32_t m_lastAttackTime;

    protected:
        int      m_lastAction;
        uint32_t m_lastActionTime;

    protected:
        Target m_target;

    protected:
        std::vector<Offender> m_offenderList;

    public:
        CharObject(ServiceCore *,       // service core
                ServerMap *,            // server map
                uint64_t,               // uid
                int,                    // map x
                int,                    // map y
                int);                   // direction
       ~CharObject() = default;

    protected:
        int X() const { return m_X; }
        int Y() const { return m_Y; }

    protected:
        int HP()    const { return m_HP; }
        int MP()    const { return m_MP; }
        int HPMax() const { return m_HPMax; }
        int MPMax() const { return m_MPMax; }

    protected:
        int Direction() const
        {
            return m_direction;
        }

        uint32_t MapID() const
        {
            return m_map ? m_map->ID() : 0;
        }

        uint64_t MapUID() const
        {
            return m_map->UID();
        }

    public:
        virtual bool update() = 0;
        virtual bool InRange(int, int, int) = 0;

    public:
        bool NextLocation(int *, int *, int, int);
        bool NextLocation(int *pX, int *pY, int nDistance)
        {
            return NextLocation(pX, pY, Direction(), nDistance);
        }

    public:
        uint64_t Activate() override;

    protected:
        virtual void ReportCORecord(uint64_t) = 0;

    protected:
        void DispatchHealth();
        void DispatchAttack(uint64_t, int);

    protected:
        virtual void DispatchAction(          const ActionNode &);
        virtual void DispatchAction(uint64_t, const ActionNode &);

    protected:
        virtual int OneStepReach(int, int, int *, int *);

    protected:
        virtual int Speed(int) const;

    protected:
        virtual bool canMove();

    protected:
        void retrieveLocation(uint64_t, std::function<void(const COLocation &)>, std::function<void()> = []{});

    protected:
        bool requestMove(
                int,                                // x
                int,                                // y
                int,                                // speed
                bool,                               // allowHalfMove
                bool,                               // removeMonster: force monster on (x, y) go to somewhere else to make room
                std::function<void()> = nullptr,    // fnOnOK
                std::function<void()> = nullptr);   // fnOnError

    protected:
        bool requestSpaceMove(
                int,                                // x
                int,                                // y
                bool,                               // strictMove
                std::function<void()> = nullptr,    // fnOnOK
                std::function<void()> = nullptr);   // fnOnError

        bool requestMapSwitch(
                uint32_t,                           // mapID
                int,                                // x
                int,                                // y
                bool,                               // strictMove
                std::function<void()> = nullptr,    // fnOnOK
                std::function<void()> = nullptr);   // fnOnError

    protected:
        void AddOffenderDamage(uint64_t, int);
        void DispatchOffenderExp();

    protected:
        virtual bool CanAct();
        virtual bool canAttack();

    protected:
        virtual void SetLastAction(int);

    protected:
        virtual bool DCValid(int, bool) = 0;

    protected:
        virtual DamageNode GetAttackDamage(int) = 0;

    protected:
        virtual bool StruckDamage(const DamageNode &) = 0;

    protected:
        void addMonster(uint32_t, int, int, bool);

    protected:
        virtual bool goDie()   = 0;
        virtual bool goGhost() = 0;

    protected:
        virtual int MaxStep() const
        {
            return 1;
        }

        virtual int MoveSpeed()
        {
            return SYS_DEFSPEED;
        }

    protected:
        // estimate how many hops we need
        // this function checks map but can't check CO
        // if we found one-hop distance we need send move request to servermap
        // return: 
        //          -1: invalid
        //           0: no move needed
        //           1: one-hop can reach
        //           2: more than one-hop can reach
        int estimateHop(int, int);

    protected:
        int AttackSpeed() const
        {
            return SYS_DEFSPEED;
        }

        int MagicSpeed() const
        {
            return SYS_DEFSPEED;
        }

        int Horse() const
        {
            return 0;
        }

    protected:
        std::array<PathFind::PathNode, 3>    GetChaseGrid(int, int, int) const;
        std::vector<PathFind::PathNode> GetValidChaseGrid(int, int, int) const;

    protected:
        void GetValidChaseGrid(int, int, int, svobuf<PathFind::PathNode, 3> &) const;

    protected:
        int CheckPathGrid(int, int, uint32_t = 0) const;
        double OneStepCost(const CharObject::COPathFinder *, int, int, int, int, int) const;

    protected:
        bool InView(uint32_t, int, int) const;

    protected:
        void SortInViewCO();
        void RemoveInViewCO(uint64_t);
        void AddInViewCO(const COLocation &);
        void AddInViewCO(uint64_t, uint32_t, int, int, int);
        void ForeachInViewCO(std::function<void(const COLocation &)>);

    protected:
        COLocation &GetInViewCORef(uint64_t);
        COLocation *GetInViewCOPtr(uint64_t);

    protected:
        virtual void checkFriend(uint64_t, std::function<void(int)>) = 0;

    protected:
        void QueryFinalMaster(uint64_t, std::function<void(uint64_t)>);

    protected:
        bool isOffender(uint64_t);

    protected:
        virtual void On_MPK_QUERYFRIENDTYPE(const MessagePack &);

    protected:
        bool isPlayer()  const;
        bool isMonster() const;
};
