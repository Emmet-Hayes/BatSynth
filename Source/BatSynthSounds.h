#pragma once

#include <JuceHeader.h>


class BatSynthSound : public SynthesiserSound 
{
public:
    bool appliesToNote(int) { return true; }
    bool appliesToChannel(int) { return true; }
};
