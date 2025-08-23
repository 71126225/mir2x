#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <functional>

#include "colorf.hpp"
#include "totype.hpp"
#include "pngtexdb.hpp"
#include "sdldevice.hpp"
#include "processrun.hpp"
#include "cbleft.hpp"

extern PNGTexDB *g_progUseDB;
extern SDLDevice *g_sdlDevice;

CBLeft::CBLeft(ProcessRun *argProc, Widget *argParent, bool argAutoDelete)
    : Widget
      {
          DIR_UPLEFT,
          0,
          0,
          178,
          133,

          {},
          argParent,
          argAutoDelete,
      }

    , m_processRun(argProc)

    , m_bgFull
      {
          DIR_UPLEFT,
          0,
          0,

          {},
          {},

          [](const Widget *)
          {
              return g_progUseDB->retrieve(0X00000012);
          },
      }

    , m_bg
      {
          DIR_UPLEFT,
          0,
          0,

          &m_bgFull,

          0,
          0,
          [this](const Widget *){ return w(); },
          [this](const Widget *){ return h(); },

          {},

          this,
          false,
      }

    , m_hpFull
      {
          DIR_UPLEFT,
          0,
          0,

          {},
          {},

          [](const Widget *)
          {
              return g_progUseDB->retrieve(0X00000018);
          },
      }

    , m_hp
      {
          DIR_DOWNLEFT,
          33,
          20,

          &m_hpFull,

          0,
          [this](const Widget *)
          {
              return m_hp.h() - m_hp.gfxCropH();
          },

          m_hpFull.w(),
          [this](const Widget *) -> int
          {
              if(auto myHero = m_processRun->getMyHero()){
                  return to_dround(m_hpFull.h() * myHero->getHealthRatio().at(0));
              }
              return 0;
          },

          {},

          this,
          false,
      }

    , m_mpFull
      {
          DIR_UPLEFT,
          0,
          0,

          {},
          {},

          [](const Widget *)
          {
              return g_progUseDB->retrieve(0X00000018);
          },
      }

    , m_mp
      {
          DIR_DOWNLEFT,
          73,
          20,

          &m_mpFull,

          0,
          [this](const Widget *)
          {
              return m_hp.h() - m_hp.gfxCropH();
          },

          m_mpFull.w(),
          [this](const Widget *) -> int
          {
              if(auto myHero = m_processRun->getMyHero()){
                  return to_dround(m_mpFull.h() * myHero->getHealthRatio().at(0));
              }
              return 0;
          },

          {},

          this,
          false,
      }

    , m_levelBarFull
      {
          DIR_UPLEFT,
          0,
          0,

          {},
          {},

          [](const ImageBoard *)
          {
              return g_progUseDB->retrieve(0X000000A0);
          },

          false,
          false,
          0,

          [this](const Widget *)
          {
              const double  ratio = m_processRun->getMyHero()->getLevelRatio();
              const uint8_t red   = static_cast<uint8_t>(to_dround(255 * ratio));
              return colorf::RGBA(red, 255 - red, 0, 255);
          },
      }

    , m_levelBar
      {
          DIR_DOWN,
          152,
          20,

          &m_levelBarFull,

          0,
          [this](const Widget *)
          {
              return m_levelBarFull.h() - m_levelBar.gfxCropH();
          },

          m_levelBarFull.w(),
          [this](const Widget *) -> int
          {
              if(auto myHero = m_processRun->getMyHero()){
                  return to_dround(m_levelBarFull.h() * myHero->getLevelRatio());
              }
              return 0;
          },

          {},

          this,
          false,
      }

    , m_inventoryBarFull
      {
          DIR_UPLEFT,
          0,
          0,

          {},
          {},

          [](const ImageBoard *)
          {
              return g_progUseDB->retrieve(0X000000A0);
          },

          false,
          false,
          0,

          [this](const Widget *)
          {
              const double  ratio = m_processRun->getMyHero()->getLevelRatio();
              const uint8_t red   = static_cast<uint8_t>(to_dround(255 * ratio));
              return colorf::RGBA(red, 255 - red, 0, 255);
          },
      }

    , m_inventoryBar
      {
          DIR_DOWN,
          152,
          20,

          &m_inventoryBarFull,

          0,
          [this](const Widget *)
          {
              return m_inventoryBarFull.h() - m_inventoryBar.gfxCropH();
          },

          m_inventoryBarFull.w(),
          [this](const Widget *) -> int
          {
              if(auto myHero = m_processRun->getMyHero()){
                  return to_dround(m_inventoryBarFull.h() * myHero->getLevelRatio());
              }
              return 0;
          },

          {},

          this,
          false,
      }

    , m_buttonQuickAccess
      {
          DIR_UPLEFT,
          148,
          2,
          {SYS_U32NIL, 0X0B000000, 0X0B000001},
          {
              SYS_U32NIL,
              SYS_U32NIL,
              0X01020000 + 105,
          },

          nullptr,
          nullptr,
          nullptr,
          [this](Widget *)
          {
              if(auto p = m_processRun->getWidget("QuickAccessBoard")){
                  p->flipShow();
              }
          },

          0,
          0,
          0,
          0,

          true,
          false,
          true,

          this,
      }

    , m_buttonClose
      {
          DIR_UPLEFT,
          8,
          72,
          {SYS_U32NIL, 0X0000001E, 0X0000001F},
          {
              SYS_U32NIL,
              SYS_U32NIL,
              0X01020000 + 105,
          },

          nullptr,
          nullptr,
          nullptr,
          [](Widget *)
          {
              std::exit(0);
          },

          0,
          0,
          0,
          0,

          true,
          false,
          true,

          this,
      }

    , m_buttonMinize
      {
          DIR_UPLEFT,
          109,
          72,
          {SYS_U32NIL, 0X00000020, 0X00000021},
          {
              SYS_U32NIL,
              SYS_U32NIL,
              0X01020000 + 105,
          },

          nullptr,
          nullptr,
          nullptr,
          nullptr,

          0,
          0,
          0,
          0,

          true,
          false,
          true,

          this,
      }

    , m_mapGLocFull
      {
          DIR_UPLEFT,
          0, // need reset
          0,

          getMapGLocStr().c_str(),
          1,
          12,
          0,

          colorf::WHITE + colorf::A_SHF(255),
      }

    , m_mapGLoc
      {
          DIR_NONE,
          68,
          113,

          &m_mapGLocFull,

          0,
          0,
          m_mapGLocFull.w(),
          m_mapGLocFull.h(),

          {},

          this,
          false,
      }
{}

void CBLeft::updateMapGLoc()
{
    m_mapGLocFull.setText(u8"%s", getMapGLocStr().c_str());
}

std::u8string CBLeft::getMapGLocStr() const
{
    if(uidf::isMap(m_processRun->mapUID())){
        if(const auto &mr = DBCOM_MAPRECORD(m_processRun->mapID())){
            const auto mapNameFull = std::string(to_cstr(mr.name));
            const auto mapNameBase = mapNameFull.substr(0, mapNameFull.find('_'));

            if(auto myHero = m_processRun->getMyHero()){
                return str_printf(u8"%s: %d %d", mapNameBase.c_str(), myHero->x(), myHero->y());
            }
            else{
                return str_printf(u8"%s", mapNameBase.c_str());
            }
        }
    }
    return {};
}
