#include "BatSynthFilter.h"


double BatSynthFilter::lopass(double in, double cutoff)
{
    output = outputs[0] + cutoff * (in - outputs[0]);
    outputs[0] = output;
    return output;
}

double BatSynthFilter::hipass(double in, double cutoff)
{
    output = input - (outputs[0] + cutoff * (in - outputs[0]));
    outputs[0] = output;
    return output;
}

double BatSynthFilter::lores(double in, double cutoff1, double resonance)
{
    cutoff = cutoff1;
    if (cutoff < 20) cutoff = 20;
    if (cutoff > (BatSynthDSP::sampleRate / 2.22)) cutoff = (BatSynthDSP::sampleRate / 2.22);
    if (resonance < 1. || cutoff1 < 225) resonance = 1.;
    z = cos(TWOPI * cutoff / BatSynthDSP::sampleRate);
    c = 2 - 2 * z;
    double r = (sqrt(2.0) * sqrt(-pow((z - 1.0), 3.0)) + resonance * (z - 1)) / (resonance * (z - 1));
    x = x + (in - y) * c;
    y = y + x;
    x = x * r;
    output = y;
    return output;
}

double BatSynthFilter::hires(double in, double cutoff1, double resonance)
{
    cutoff = cutoff1;
    if (cutoff < 10) cutoff = 10;
    if (cutoff > (BatSynthDSP::sampleRate)) cutoff = (BatSynthDSP::sampleRate);
    if (resonance < 1.) resonance = 1;
    z = cos(TWOPI * cutoff / BatSynthDSP::sampleRate);
    c = 2 - 2 * z;
    double r = (sqrt(2.0) * sqrt(-pow((z - 1.0), 3.0)) + resonance * (z - 1)) / (resonance * (z - 1));
    x = x + (in - y) * c;
    y = y + x;
    x = x * r;
    output = in - y;
    return output;
}

double BatSynthFilter::bandpass(double in, double cutoff1, double resonance)
{
    cutoff = cutoff1;
    if (cutoff > (BatSynthDSP::sampleRate * 0.5)) cutoff = (BatSynthDSP::sampleRate * 0.5);
    if (resonance >= 1.) resonance = 0.999999;
    z = cos(TWOPI * cutoff / BatSynthDSP::sampleRate);
    inputs[0] = (1 - resonance) * (sqrt(resonance * (resonance - 4.0 * pow(z, 2.0) + 2.0) + 1));
    inputs[1] = 2 * z * resonance;
    inputs[2] = pow((resonance * -1), 2);

    output = inputs[0] * in + inputs[1] * outputs[1] + inputs[2] * outputs[2];
    outputs[2] = outputs[1];
    outputs[1] = output;
    return output;
}