#pragma once
#include "BatSynthDSP.h"

class BatSynthCompressor
{
public:
    double ratio{ 1.0 };
    double threshold{ 0.0 };
    double attackTime{ 0.1 };
    double releaseTime{ 0.1 };
    double envelope{ 0.0 };
    double gain{ 1.0 };

    BatSynthCompressor(double r, double t, double atk, double rel) : ratio(r), threshold(t), attackTime(atk), releaseTime(rel) {}
    double process(double input);
};

class BatSynthLPFilter {
public:
    void setCutoffFrequency(double cutoff) { a = exp(-TWOPI * cutoff); }
    double process(double input) { prevOutput = (1 - a) * input + a * prevOutput; return prevOutput; }

private:
    double a{ 0.0 };
    double prevOutput{ 0.0 };
};