#pragma once
#include "BatSynthDSP.h"

class BatSynthFilter
{
public:
    BatSynthFilter() {};
    double cutoff{ 500.0 };
    double resonance{ 1.0 };
    double lores(double input, double cutoff1, double resonance);
    double hires(double input, double cutoff1, double resonance);
    double bandpass(double input, double cutoff1, double resonance);
    double lopass(double input, double cutoff);
    double hipass(double input, double cutoff);

private:
    double gain{ 0.0 };
    double input{ 0.0 };
    double output{ 0.0 };
    double inputs[10]{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double outputs[10]{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    double cutoff1{ 0.0 };
    double x{ 0.0 }; // speed
    double y{ 0.0 }; // pos
    double z{ 0.0 }; // pole
    double c{ 0.0 }; // filter coefficient
};