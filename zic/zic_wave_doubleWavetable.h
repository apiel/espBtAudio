#ifndef ZIC_WAVE_DOUBLE_WAVETABLE_H_
#define ZIC_WAVE_DOUBLE_WAVETABLE_H_

#include "zic_wave_base.h"
#include "wavetables/wavetable_sine.h"
#include "wavetables/wavetable_square.h"

class Zic_Wave_DoubleWavetable : public Zic_Wave_Base
{
protected:
    double sample(float *freq)
    {
        float pos = M_PI * time;
        int i1 = pos * (*freq) * size;
        int i2 = (pos * (*freq + detune) + phase) * size;
        return table1[i1 & (size - 1)] * crossfader + table2[i2 & (size - 1)] * (1.0f - crossfader);
    }

public:
    float *table1 = &wavetableSine[0];
    float *table2 = &wavetableSquare[0];
    float crossfader = 0.5f; // should be between 0.0 and 1.0
    float detune = 0.0f;
    uint16_t size = 2048;
};

#endif
