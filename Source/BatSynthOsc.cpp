#include "BatSynthOsc.h"


double BatSynthOsc::noise()
{
    return (rand() / (float)RAND_MAX) * 2.0 - 1.0;
}

double BatSynthOsc::sinewave(double freq)
{
    advancePhase(freq);
    return sin(phase * (TWOPI));
}

double BatSynthOsc::square(double freq)
{
    advancePhase(freq);
    if (phase <= 0.5) return 1;
    return -1;
}

double BatSynthOsc::pulse(double freq, double duty)
{
    if (duty < 0.05) duty = 0.05;
    if (duty > 0.45) duty = 0.45;
    advancePhase(freq);
    if (phase <= duty) return 1;
    return -1;
}

double BatSynthOsc::saw(double freq)
{
    advancePhase(freq);
    return phase * 2.0 - 1.0;
}

double BatSynthOsc::sawr(double freq)
{
    return -saw(freq);
}

double BatSynthOsc::softdistsine(double freq)
{
    advancePhase(freq);
    return tanh(1.2 * sinewave(freq));
}

double BatSynthOsc::harddistsine(double freq)
{
    advancePhase(freq);
    return tanh(1.3 * sinewave(freq));
}

double BatSynthOsc::customwave(double freq, const float* waveTable, int size)
{
    advancePhase(freq);

    // phase [0,1) -> index [0, tableSize)
    double index = phase * (double)size;

    int i0 = (int)index;
    double frac = index - (double)i0;

    // wrap i0, i1
    int i1;
    if (i0 >= size)
        i0 -= size;

    i1 = i0 + 1;
    if (i1 >= size)
        i1 -= size;

    // fetch samples (assuming customTable is double or castable)
    double s0 = waveTable[i0];
    double s1 = waveTable[i1];

    // linear interpolation
    double sample = s0 + frac * (s1 - s0);

    return sample;
}

double BatSynthOsc::triangle(double freq)
{
    advancePhase(freq);
    if (phase <= 0.5) return (phase - 0.25) * 4.;
    return ((1.0 - phase) - 0.25) * 4.;
}

double BatSynthOsc::sawwane(double freq)
{
    advancePhase(freq);
    return phase * phase * 2 - 1;
}

double BatSynthOsc::sawwax(double freq)
{
    advancePhase(freq);
    return sqrt(phase) * 2 - 1;
}

double BatSynthOsc::sawpulse(double freq, double duty)
{
    if (duty < 0.05) duty = 0.05;
    else if (duty > 0.45) duty = 0.45;
    advancePhase(freq);
    if (phase < duty) return -1;
    else if (phase > (1 - duty)) return 1;
    else return phase * 2 - 1;
}

void BatSynthOsc::advancePhase(double freq)
{
    phase += freq / BatSynthDSP::sampleRate;
    if (phase >= 1.0) phase -= 1.0;
}