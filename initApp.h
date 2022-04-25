#ifndef INIT_APP_H_
#define INIT_APP_H_

#include <BluetoothA2DPSource.h>
#include <math.h>
#include <M5Core2.h>

#include "zic/zic_note.h"
#include "zic/zic_mod_asr.h"

#include "zic/zic_wave_osc.h"
#include "zic/zic_wave_wavetable.h"
#include "zic/zic_wave_doubleWavetable.h"
#include "zic/zic_wave_SDwavetableBank.h"

#include "ui/ui_key.h"
#include "ui/ui_color.h"
#include "ui/ui_slider.h"
#include "ui/ui_knob.h"
#include "ui/ui_toggle.h"

#include "zic/zic_fastTrigo.h"
#include "fastTrigo.h"

BluetoothA2DPSource a2dp_source;
// Zic_Wave_Osc wave;
// Zic_Wave_Wavetable wave;
Zic_Wave_SDWavetableBank wave;
Zic_Mod_Asr asr;

enum
{
    OSC_SLIDER_CROSSFADER,
    OSC_SLIDER_COUNT
};

enum
{
    MODE_KEYBOARD,
    MODE_OSC,
    MODE_COUNT
};

uint8_t mode = MODE_KEYBOARD;

#define KEYS_COUNT 7 * 5
UI_Key keys[KEYS_COUNT] = {
    {0, 0, _C6}, {45, 0, _D6}, {90, 0, _E6}, {135, 0, _F6}, {180, 0, _G6}, {225, 0, _A6}, {270, 0, _B6}, {0, 45, _C5}, {45, 45, _D5}, {90, 45, _E5}, {135, 45, _F5}, {180, 45, _G5}, {225, 45, _A5}, {270, 45, _B5}, {0, 90, _C4}, {45, 90, _D4}, {90, 90, _E4}, {135, 90, _F4}, {180, 90, _G4}, {225, 90, _A4}, {270, 90, _B4}, {0, 135, _C3}, {45, 135, _D3}, {90, 135, _E3}, {135, 135, _F3}, {180, 135, _G3}, {225, 135, _A3}, {270, 135, _B3}, {0, 180, _C2}, {45, 180, _D2}, {90, 180, _E2}, {135, 180, _F2}, {180, 180, _G2}, {225, 180, _A2}, {270, 180, _B2}};

UI_Slider oscSliders[OSC_SLIDER_COUNT] = {
    // {10, &UI_THEME_BLUE[0], getOscName(OSC_SINE)},
    // {55, &UI_THEME_PURPLE[0], getOscName(OSC_SQUARE)},
    // {100, &UI_THEME_GREEN[0], getOscName(OSC_TRIANGLE)},
    {100, &UI_THEME_GREEN[0], "Crossfader"},
    // {145, &UI_THEME_RED[0], getOscName(OSC_SAW)},
    // {190, &UI_THEME_ORANGE[0], getOscName(OSC_NOIZE)},
};

UI_Knob knob(160, 120, 100);
UI_Toggle toggle(10, 10);

int32_t get_data_channels(Frame *frame, int32_t channel_len)
{
    for (int sample = 0; sample < channel_len; ++sample)
    {
        wave.amplitudeMod = asr.next();
        frame[sample].channel1 = wave.next();
        frame[sample].channel2 = frame[sample].channel1;
    }

    return channel_len;
}

void displayKeyboard()
{
    M5.Lcd.fillScreen(UI_BACKGROUND);
    for (uint8_t k = 0; k < KEYS_COUNT; k++)
    {
        keys[k].render();
    }
}

void displayOsc()
{
    M5.Lcd.fillScreen(UI_BACKGROUND);
    // for (uint8_t k = 0; k < OSC_SLIDER_COUNT; k++)
    // {
    //     oscSliders[k].render();
    // }
    knob.render();
    toggle.render();
}

void eventHandler(Event &e)
{
    // Serial.printf("%s %3d,%3d\n", e.typeName(), e.to.x, e.to.y);

    if (mode == MODE_KEYBOARD)
    {
        // TODO might need to find a better way, cause note on doesn't last
        // either fix ASR or find better UI handler
        bool isOn = false;
        bool isOff = false;
        for (uint8_t k = 0; k < KEYS_COUNT; k++)
        {
            if (keys[k].update(e))
            {
                if (keys[k].isOn)
                {
                    wave.frequency = NOTE_FREQ[keys[k].midiNote];
                    asr.on();
                    isOn = true;
                    // Serial.printf("Play note %d\n", keys[k].midiNote);
                }
                else
                {
                    isOff = true;
                }
            }
        }
        if (isOff && !isOn)
        {
            // only if there is not another note on
            asr.off();
        }
    }
    else if (mode == MODE_OSC)
    {
        // for (uint8_t k = 0; k < OSC_SLIDER_COUNT; k++)
        // {
        //     if (oscSliders[k].update(e))
        //     {
        //         if (k == OSC_SLIDER_CROSSFADER)
        //         {
        //             // wave.crossfader = oscSliders[k].value;
        //         }
        //     }
        // }
        if (knob.update(e))
        {
            wave.pos += knob.direction;
            Serial.printf("knob value %d direction %d\n", knob.value, knob.direction);
            // for testing
            if (!toggle.isOn)
            {
                if (knob.active)
                {
                    if (!asr.isOn())
                    {
                        asr.on();
                    }
                }
                else
                {
                    asr.off();
                }
            }
        }
        if (toggle.update(e))
        {
            if (toggle.isOn)
            {
                asr.on();
            }
            else
            {
                asr.off();
            }
        }
    }
}

void render()
{
    if (mode == MODE_OSC)
    {
        displayOsc();
    }
    else
    {
        displayKeyboard();
    }
}

void initApp()
{
    Serial.begin(115200);
    Serial.println("Zic zic");

    M5.begin();
    SD.begin();

    // maybe there should be some default banks
    // cause it's soooooooooo slow!!!!
    M5.Lcd.println("Load wavetable bank...");
    uint8_t ret = wave.load("/01.wav");

    M5.background.addHandler(eventHandler, E_ALL);
    render();

    a2dp_source.start("Geo Speaker", get_data_channels);

    // for (int i = 0; i < 255; i++)
    // {
    //     Serial.printf("%.9f, ", lut[i]);
    // }

    // demoYo();
}

void loopApp()
{
    M5.update();
    // TODO might want to have a screen to select mode
    // long press display screen selector and select mode with dual touch
    // or should it be a short press bringing to mode screen?
    // ...
    // what the point to do dual touch, as it will be anyway pressing screen twice
    // once on screen mode selector, short press could also do something (go back)
    // as db press could do (toggle to prev screen)
    // as long press could do (?)
    if (M5.BtnA.wasPressed())
    {
        mode = (mode + 1) % MODE_COUNT;
        render();
    }
}

#endif