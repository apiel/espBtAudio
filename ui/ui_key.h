#ifndef UI_KEY_H_
#define UI_KEY_H_

#include <M5Core2.h>

#include "ui_color.h"
#include "ui_area_rect.h"
#include "ui_component.h"

class UI_Key : public UI_Component
{
protected:
    UI_Area_Rect area;
    const uint16_t *color;
    const char *name = NULL;

    void init(uint8_t _value, const uint16_t *_color, const char *_name)
    {
        value = _value;
        color = _color;
        name = _name;
    }

public:
    bool isOn = false;
    uint8_t value = 0;

    UI_Key(uint16_t _x, uint16_t _y, uint8_t _value = 0,
           const uint16_t *_color = &UI_THEME_BLUE[0], const char *_name = NULL) : area(_x, _y, 45, 45)
    {
        init(_value, _color, _name);
    }

    UI_Key(uint16_t _x, uint16_t _y, uint16_t _w, uint16_t _h, uint8_t _value = 0,
           const uint16_t *_color = &UI_THEME_BLUE[0], const char *_name = NULL) : area(_x, _y, _w, _h)
    {
        init(_value, _color, _name);
    }

    void render()
    {
        if (isOn)
        {
            M5.Lcd.fillRect(area.x, area.y, area.w, area.h, color[1]);
        }
        else
        {
            M5.Lcd.fillRect(area.x, area.y, area.w, area.h, UI_BACKGROUND);
        }
        M5.Lcd.drawRect(area.x, area.y, area.w, area.h, color[0]);
        if (name)
        {
            M5.Lcd.setCursor(area.x + 5, area.y + 5);
            M5.Lcd.setTextColor(color[1], UI_BACKGROUND);
            M5.Lcd.println(name);
        }
    }

    bool update(Event &e)
    {
        if (e.type == E_TOUCH || e.type == E_MOVE || e.type == E_RELEASE)
        {
            if (e.type != E_RELEASE && area.in(e))
            {
                if (!isOn)
                {
                    isOn = true;
                    render();
                    return true;
                }
            }
            else if (isOn)
            {
                isOn = false;
                render();
                return true;
            }
        }
        return false;
    }
};

#endif
