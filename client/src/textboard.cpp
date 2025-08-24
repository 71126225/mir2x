#include "fontexdb.hpp"
#include "textboard.hpp"

extern FontexDB *g_fontexDB;
TextBoard::TextBoard(
        Widget::VarDir argDir,
        Widget::VarOff argX,
        Widget::VarOff argY,

        std::function<std::string(const Widget *)> argTextFunc,

        uint8_t argFont,
        uint8_t argFontSize,
        uint8_t argFontStyle,

        Widget::VarColor argColor,

        Widget *argParent,
        bool    argAutoDelete)

    : Widget
      {
          std::move(argDir),
          std::move(argX),
          std::move(argY),
          0,
          0,

          {},

          argParent,
          argAutoDelete,
      }

    , m_image
      {
          DIR_UPLEFT,
          0,
          0,

          {}, // image width
          {}, // image height

          [
              this,

              argTextFunc = std::move(argTextFunc),
              argFont,
              argFontSize,
              argFontStyle,

              lastTexture = SDL_Texture *(nullptr),
              lastText    = std::string()

          ](const Widget *widget) -> SDL_Texture *
          {
              if(const auto currText = argTextFunc(this); currText != lastText){
                  g_fontexDB->freeTexture(lastTexture);
              }


              if(!lastTexture){
                  lastText = argTextFunc(this);
                  lastTexture = g_fontexDB->createTexture(argFont, argFontSize, argFontStyle, lastText.c_str());
              }
              else if(const auto s = argTextFunc(this); s != lastText){
                  lastText = s;
                  lastTexture = g_fontexDB->createTexture(argFont, argFontSize, argFontStyle, lastText.c_str());
              }

              return lastTexture;



              const auto text = argTextFunc(widget);
              if(text.empty()){
                  return nullptr;
              }

              return g_fontexDB->createTexture(text, argFont, argFontSize, argFontStyle);
          },

          false,
          false,
          0,

          std::move(argColor),

          nullptr,
          false,
      }

    , m_tpset(&m_image)
{
    if(!argTextFunc){
        throw fflerror("invalid text function");
    }

    if(!g_fontexDB->hasFont(argFont)){
        throw fflerror("invalid font: %hhu", argFont);
    }

    if(!g_fontexDB->hasFontSize(argFontSize)){
        throw fflerror("invalid font size: %hhu", argFontSize);
    }

    if(argFontStyle & ~g_fontexDB->getFontStyleMask()){
        throw fflerror("invalid font style: %hhu", argFontStyle);
    }

    m_image.setW(Widget::VarSize(g_fontexDB->getTextWidth("A", argFont, argFontSize, argFontStyle)));
    m_image.setH(Widget::VarSize(g_fontexDB->getTextHeight(argFont, argFontSize, argFontStyle)));
}
