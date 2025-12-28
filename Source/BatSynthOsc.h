#pragma once
#include "BatSynth.h"
#include "BatSynthDSP.h"

class BatSynthOsc
{
public:
    double sinewave(double frequency);
    double saw(double frequency);
    double sawr(double frequency);
    double triangle(double frequency);
    double square(double frequency);
    double sawwane(double frequency);
    double sawwax(double frequency);
    double sawpulse(double frequency, double duty);
    double pulse(double frequency, double duty);
    double noise();
    double softdistsine(double frequency);
    double harddistsine(double frequency);
    double customwave(double frequency, const float* waveTable, int size);

private:
    void advancePhase(double frequency);

    double phase = 0.0;
};