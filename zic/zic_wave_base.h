#ifndef ZIC_WAVE_BASE_H_
#define ZIC_WAVE_BASE_H_

#ifndef SAMPLE_RATE
#define SAMPLE_RATE 44100
#endif

#ifndef DELTA_TIME
#define DELTA_TIME 1.0f / 44100
#endif

#ifndef MAX_FREQUENCY
#define MAX_FREQUENCY SAMPLE_RATE * 0.25
#endif

#ifndef AMPLITUDE_PEAK
#define AMPLITUDE_PEAK 32767
#endif

#ifndef between
#define between(x, a, b) (((a) <= (x)) && ((x) <= (b)))
#endif

#ifndef FREQ_MULT
#define FREQ_MULT 100
#endif

#ifndef FREQ_DIV
#define FREQ_DIV 1.0f / FREQ_MULT
#endif

#ifndef FREQ_PI
#define FREQ_PI M_PI / FREQ_MULT
#endif

// Q format https://www.pathpartnertech.com/representing-decimal-data-in-q-format/

class Zic_Wave_Base
{
protected:
    float time = 0.0;

    virtual float sample(uint32_t *freq);

    uint32_t frequency = 103.82617439443122f * FREQ_MULT; // C3
    uint16_t amplitude = AMPLITUDE_PEAK * 0.1;
    // float pitch = 1.0f;

    // Pre-calculation
    uint32_t freq = frequency; // * pitch;

    uint16_t phase = 0; // 0 to 360

    // TODO
    // see https://teropa.info/blog/2016/08/10/frequency-and-pitch.html
    void updateFreq()
    {
        freq = frequency; // and do something with pitch
    }

public:
    float amplitudeMod = 1.0f;
    // float pitchMod = 1.0f;

    int16_t next()
    {
        uint32_t _freq = freq; // need to do something with picth modulation: pitchMod;
        uint16_t _amp = amplitudeMod * amplitude;
        time += DELTA_TIME;

        // might divide AMPLITUDE_PEAK by 1000 and then multiply wavetable by 1000?
        return _amp * sample(&_freq);
    }

    /**
     * @brief Set the Phase of the wave
     *
     * @param value between 0° and 360°
     */
    void setPhase(uint16_t value)
    {
        phase = between(value, 0, 360);
    }

    /**
     * @brief Get the Phase of the wave
     *
     * @return uint16_t
     */
    uint16_t getPhase()
    {
        return phase;
    }

    /**
     * @brief Set the Amplitude of the wave
     *
     * @param value between 0 and 32767
     */
    void setAmplitude(uint16_t value)
    {
        amplitude = between(value, 0, AMPLITUDE_PEAK);
    }

    /**
     * @brief Set the Amplitude of the wave using percentage
     *
     * @param value between 0.0f and 1.0f
     */
    void setAmplitudePct(float value)
    {
        amplitude = between(value, 0.0f, 1.0f) * AMPLITUDE_PEAK;
    }

    /**
     * @brief Get the Amplitude of the wave
     *
     * @return uint16_t
     */
    uint16_t getAmplitude()
    {
        return amplitude;
    }

    /**
     * @brief Get the Amplitude Pct of the wave in percentage
     *
     * @return float
     */
    float getAmplitudePct()
    {
        return (float)amplitude / (float)AMPLITUDE_PEAK;
    }

    /**
     * @brief Set the Frequency of the wave. Use zic_note.h to assign notes.
     *
     * @param value
     */
    void setFrequency(float value)
    {
        frequency = value * FREQ_MULT;
        updateFreq();
    }

    /**
     * @brief Get the Frequency of the wave
     *
     * @return float
     */
    float getFrequency()
    {
        return frequency * FREQ_DIV;
    }

    // /**
    //  * @brief Set the Pitch of the wave
    //  *
    //  * @param value
    //  */
    // void setPitch(float value)
    // {
    //     pitch = value;
    //     updateFreq();
    // }

    // /**
    //  * @brief Get the Pitch of the wave
    //  *
    //  * @return float
    //  */
    // float getPitch()
    // {
    //     return pitch;
    // }
};

#endif
