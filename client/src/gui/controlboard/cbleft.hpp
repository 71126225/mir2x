#pragma once
#include <cstdint>
#include <functional>
#include "widget.hpp"
#include "labelboard.hpp"
#include "imageboard.hpp"
#include "tritexbutton.hpp"
#include "gfxcropboard.hpp"

class ProcessRun;
class CBLeft: public Widget
{
    private:
        ProcessRun *m_processRun;

    private:
        ImageBoard   m_bgFull;
        GfxCropBoard m_bg;

        ImageBoard   m_hpFull;
        GfxCropBoard m_hp;

        ImageBoard   m_mpFull;
        GfxCropBoard m_mp;

        ImageBoard   m_levelBarFull;
        GfxCropBoard m_levelBar;

        ImageBoard   m_inventoryBarFull;
        GfxCropBoard m_inventoryBar;

    private:
        TritexButton m_buttonQuickAccess;

    private:
        TritexButton m_buttonClose;
        TritexButton m_buttonMinize;

    private:
        LabelBoard   m_mapGLocFull;
        GfxCropBoard m_mapGLoc;

    public:
        CBLeft( ProcessRun *,
                Widget * = nullptr,
                bool     = false);

    public:
        void updateMapGLoc();
        std::u8string getMapGLocStr() const;
};
