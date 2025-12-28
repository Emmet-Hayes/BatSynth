#include "BatSynthCompressor.h"


double BatSynthCompressor::process(double input)
{
    double inputLevel = fabs(input);

    if (inputLevel > threshold) {
        double desiredGain = threshold / inputLevel;
        double dbDesiredGain = log10(desiredGain) * 20.0;
        double dbGainReduction = dbDesiredGain * (1.0 - 1.0 / ratio);
        double desiredEnvelope = pow(10.0, dbGainReduction / 20.0);

        if (desiredEnvelope < envelope)
        {
            double coeff = exp(log(0.01) / (attackTime * BatSynthDSP::sampleRate));
            envelope = coeff * envelope + (1.0 - coeff) * desiredEnvelope;
        }
        else
        {
            double coeff = exp(log(0.01) / (releaseTime * BatSynthDSP::sampleRate));
            envelope = coeff * envelope + (1.0 - coeff) * desiredEnvelope;
        }
    }
    else
    {
        envelope = 1.0;
    }

    gain = envelope;
    return input * gain;
}