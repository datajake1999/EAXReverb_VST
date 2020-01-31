#ifndef REVERBEFFECTDOUBLE_H
#define REVERBEFFECTDOUBLE_H

#include <stdint.h>
#include "efx-presets.h"

typedef int BOOL;

#define OUTPUT_CHANNELS 2
#define TRUE 1
#define FALSE 0
#define REVERB_BUFFERSIZE (2048u)
#define F_2PI (6.28318530717958647692f)
#define FLT_EPSILON 1.19209290E-07F

#define  MAX_AMBI_COEFFS 4

#define SPEEDOFSOUNDMETRESPERSEC 343.3f

#define GAIN_SILENCE_THRESHOLD  0.00001f

/* This is a user config option for modifying the overall output of the reverb effect. */
#define ReverbBoost 1.0f

/* Effect parameter ranges. */
#define EAXREVERB_MIN_DENSITY                 (0.0f)
#define EAXREVERB_MAX_DENSITY                 (1.0f)

#define EAXREVERB_MIN_DIFFUSION               (0.0f)
#define EAXREVERB_MAX_DIFFUSION               (1.0f)

#define EAXREVERB_MIN_GAIN                    (0.0f)
#define EAXREVERB_MAX_GAIN                    (1.0f)

#define EAXREVERB_MIN_GAINHF                  (0.0f)
#define EAXREVERB_MAX_GAINHF                  (1.0f)

#define EAXREVERB_MIN_GAINLF                  (0.0f)
#define EAXREVERB_MAX_GAINLF                  (1.0f)

#define EAXREVERB_MIN_DECAY_TIME              (0.1f)
#define EAXREVERB_MAX_DECAY_TIME              (20.0f)

#define EAXREVERB_MIN_DECAY_HFRATIO           (0.1f)
#define EAXREVERB_MAX_DECAY_HFRATIO           (2.0f)

#define EAXREVERB_MIN_DECAY_LFRATIO           (0.1f)
#define EAXREVERB_MAX_DECAY_LFRATIO           (2.0f)

#define EAXREVERB_MIN_REFLECTIONS_GAIN        (0.0f)
#define EAXREVERB_MAX_REFLECTIONS_GAIN        (3.16f)

#define EAXREVERB_MIN_REFLECTIONS_DELAY       (0.0f)
#define EAXREVERB_MAX_REFLECTIONS_DELAY       (0.3f)

#define EAXREVERB_MIN_LATE_REVERB_GAIN        (0.0f)
#define EAXREVERB_MAX_LATE_REVERB_GAIN        (10.0f)

#define EAXREVERB_MIN_LATE_REVERB_DELAY       (0.0f)
#define EAXREVERB_MAX_LATE_REVERB_DELAY       (0.1f)

#define EAXREVERB_MIN_ECHO_TIME               (0.075f)
#define EAXREVERB_MAX_ECHO_TIME               (0.25f)

#define EAXREVERB_MIN_ECHO_DEPTH              (0.0f)
#define EAXREVERB_MAX_ECHO_DEPTH              (1.0f)

#define EAXREVERB_MIN_MODULATION_TIME         (0.04f)
#define EAXREVERB_MAX_MODULATION_TIME         (4.0f)

#define EAXREVERB_MIN_MODULATION_DEPTH        (0.0f)
#define EAXREVERB_MAX_MODULATION_DEPTH        (1.0f)

#define EAXREVERB_MIN_AIR_ABSORPTION_GAINHF   (0.892f)
#define EAXREVERB_MAX_AIR_ABSORPTION_GAINHF   (1.0f)

#define EAXREVERB_MIN_HFREFERENCE             (1000.0f)
#define EAXREVERB_MAX_HFREFERENCE             (20000.0f)

#define EAXREVERB_MIN_LFREFERENCE             (20.0f)
#define EAXREVERB_MAX_LFREFERENCE             (1000.0f)

#define EAXREVERB_MIN_ROOM_ROLLOFF_FACTOR     (0.0f)
#define EAXREVERB_MAX_ROOM_ROLLOFF_FACTOR     (10.0f)

#define EAXREVERB_MIN_DECAY_HFLIMIT           FALSE
#define EAXREVERB_MAX_DECAY_HFLIMIT           TRUE

typedef enum FilterTypeDouble {
    /** EFX-style low-pass filter, specifying a gain and reference frequency. */
    Filter_HighShelf_Double,
    /** EFX-style high-pass filter, specifying a gain and reference frequency. */
    Filter_LowShelf_Double,
} FilterTypeDouble;

typedef struct
{
    // The delay lines use sample lengths that are powers of 2 to allow the
    // use of bit-masking instead of a modulus for wrapping.
    uint32_t   Mask;
    double *Line;
} DelayLineDouble;

typedef struct {
    double x[2]; /* History of two last input samples  */
    double y[2]; /* History of two last output samples */
    double a[3]; /* Transfer function coefficients "a" */
    double b[3]; /* Transfer function coefficients "b" */
} FilterStateDouble;

typedef struct {
    // Shared Reverb Properties
    double Density;
    double Diffusion;
    double Gain;
    double GainHF;
    double DecayTime;
    double DecayHFRatio;
    double ReflectionsGain;
    double ReflectionsDelay;
    double LateReverbGain;
    double LateReverbDelay;
    double AirAbsorptionGainHF;
    double RoomRolloffFactor;
    BOOL DecayHFLimit;

    // Additional EAX Reverb Properties
    double GainLF;
    double DecayLFRatio;
    double ReflectionsPan[3];
    double LateReverbPan[3];
    double EchoTime;
    double EchoDepth;
    double ModulationTime;
    double ModulationDepth;
    double HFReference;
    double LFReference;

} ReverbSettingsDouble;

typedef struct {
    // ModulatorDouble delay line.
    DelayLineDouble Delay;

    // The vibrato time is tracked with an index over a modulus-wrapped
    // range (in samples).
    uint32_t    Index;
    uint32_t    Range;

    // The depth of frequency change (also in samples) and its filter.
    double   Depth;
    double   Coeff;
    double   Filter;
} ModulatorDouble;

typedef struct {
    // Output gain for early reflections.
    double   Gain;

    // Early reflections are done with 4 delay lines.
    double   Coeff[4];
    DelayLineDouble Delay[4];
    uint32_t    Offset[4];

    // The gain for each output channel based on 3D panning (only for the
    // EAX path).
    double   PanGain[OUTPUT_CHANNELS];
} EarlyDelayDouble;

typedef struct {
    // Output gain for late reverb.
    double   Gain;

    // Attenuation to compensate for the modal density and decay rate of
    // the late lines.
    double   DensityGain;

    // The feed-back and feed-forward all-pass coefficient.
    double   ApFeedCoeff;

    // Mixing matrix coefficient.
    double   MixCoeff;

    // Late reverb has 4 parallel all-pass filters.
    double   ApCoeff[4];
    DelayLineDouble ApDelay[4];
    uint32_t    ApOffset[4];

    // In addition to 4 cyclical delay lines.
    double   Coeff[4];
    DelayLineDouble Delay[4];
    uint32_t    Offset[4];

    // The cyclical delay lines are 1-pole low-pass filtered.
    double   LpCoeff[4];
    double   LpSample[4];

    // The gain for each output channel based on 3D panning (only for the
    // EAX path).
    double   PanGain[OUTPUT_CHANNELS];
} LateDelayDouble;

typedef struct {
    // Attenuation to compensate for the modal density and decay rate of
    // the echo line.
    double   DensityGain;

    // Echo delay and all-pass lines.
    DelayLineDouble Delay;
    DelayLineDouble ApDelay;

    double   Coeff;
    double   ApFeedCoeff;
    double   ApCoeff;

    uint32_t    Offset;
    uint32_t    ApOffset;

    // The echo line is 1-pole low-pass filtered.
    double   LpCoeff;
    double   LpSample;

    // Echo mixing coefficients.
    double   MixCoeff[2];
} ReverbEchoDouble;

class ReverbEffectDouble
{
    public:
        void Create(uint32_t frequency);
        void Destroy();

        void Process(uint32_t SamplesToDo, const double *SamplesIn, double *SamplesOut);
        void Update(int frequency);

        void LoadPreset(double Density, double Diffusion, double Gain, double GainHF, double GainLF, double DecayTime, double DecayHFRatio, double DecayLFRatio, double ReflectionsGain, double ReflectionsDelay, double ReflectionsPanX, double ReflectionsPanY, double ReflectionsPanZ, double LateReverbGain, double LateReverbDelay, double LateReverbPanX, double LateReverbPanY, double LateReverbPanZ, double EchoTime, double EchoDepth, double ModulationTime, double ModulationDepth, double AirAbsorptionGainHF, double HFReference, double LFReference, double RoomRolloffFactor, int DecayHFLimit);


    private:
    ReverbSettingsDouble settings;

    // All delay lines are allocated as a single buffer to reduce memory
    // fragmentation and management code.
    double  *SampleBuffer;
    uint32_t    TotalSamples;
    double CutoffFrequency;

    // Master effect filters
    FilterStateDouble LpFilter;
    FilterStateDouble HpFilter; // EAX only

    ModulatorDouble Mod;

    // Initial effect delay.
    DelayLineDouble Delay;
    // The tap points for the initial delay.  First tap goes to early
    // reflections, the last to late reverb.
    uint32_t    DelayTap[2];

    EarlyDelayDouble Early;

    // Decorrelator delay line.
    DelayLineDouble Decorrelator;
    // There are actually 4 decorrelator taps, but the first occurs at the
    // initial sample.
    uint32_t    DecoTap[3];

    LateDelayDouble Late;

    ReverbEchoDouble Echo;

    // The current read offset for all delay lines.
    uint32_t Offset;

        /* Temporary storage used when processing, before deinterlacing. */
    double ReverbSamples[REVERB_BUFFERSIZE][4];
    double EarlySamples[REVERB_BUFFERSIZE][4];

    double ambiCoeffs[OUTPUT_CHANNELS][MAX_AMBI_COEFFS];

    void ComputeAmbientGains(double ingain, double gains[OUTPUT_CHANNELS]);
    void ComputeDirectionalGains(const double dir[3], double ingain, double gains[OUTPUT_CHANNELS]);

    void AllocLines(uint32_t frequency);

    inline double EAXModulation(double in);

    inline double EarlyDelayDoubleLineDoubleOut(uint32_t index);
    inline void EarlyReflection(double in, double *out);

    inline double LateAllPassInOut(uint32_t index, double in);
    inline double LateDelayDoubleLineDoubleOut(uint32_t index);

    inline double LateLowPassInOut(uint32_t index, double in);
    inline void LateReverb(const double *in, double *out);

    inline void EAXEcho(double in, double *late);
    inline void EAXVerbPass(double in, double *early, double *late);

    void UpdateDelayLineDouble(double earlyDelay, double lateDelay, uint32_t frequency);
    void UpdateModulatorDouble(double modTime, double modDepth, uint32_t frequency);
    void UpdateEarlyLines(double reverbGain, double earlyGain, double lateDelay);
    void UpdateDecorrelator(double density, uint32_t frequency);
    void UpdateLateLines(double reverbGain, double lateGain, double xMix, double density, double decayTime, double diffusion, double hfRatio, double cw, uint32_t frequency);
    void UpdateEchoLine(double reverbGain, double lateGain, double echoTime, double decayTime, double diffusion, double echoDepth, double hfRatio, double cw, uint32_t frequency);

    void Update3DPanning(const double *ReflectionsPan, const double *LateReverbPan, double Gain);
};

#endif // REVERBEFFECTDOUBLE_H
