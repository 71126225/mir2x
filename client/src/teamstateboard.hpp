#pragma once
#include "widget.hpp"
#include "sysconst.hpp"
#include "labelboard.hpp"
#include "protocoldef.hpp"
#include "tritexbutton.hpp"

class ProcessRun;
class TeamStateBoard: public Widget
{
    private:
        const int m_startX = 19;
        const int m_startY = 79;

        const int m_lineSpace  = 10;
        const int m_uidRegionW = 220;

    private:
        const uint8_t  m_font      = 2;
        const uint8_t  m_fontSize  = 20;
        const uint8_t  m_fontStyle = 0;

        const uint32_t m_fontColor       = colorf::WHITE + colorf::A_SHF(255);
        const uint32_t m_selectedBGColor = colorf::RED   + colorf::A_SHF(100);

    private:
        std::vector<uint64_t> m_uidList;

    private:
        TritexButton m_enableTeam;

    private:
        TritexButton m_createTeam;
        TritexButton m_addMember;
        TritexButton m_deleteMember;
        TritexButton m_refresh;

    private:
        TritexButton m_close;

    private:
        ProcessRun *m_processRun;

    public:
        TeamStateBoard(int, int, ProcessRun *, Widget * = nullptr, bool = false);

    public:
        void drawEx(int, int, int, int, int, int) const override;

    public:
        bool processEvent(const SDL_Event &, bool) override;

    public:
        int lineHeight() const
        {
            return XMLTypeset(m_uidRegionW, LALIGN_LEFT, false, m_font, m_fontSize, m_fontStyle).getDefaultFontHeight() + m_lineSpace;
        }
};
