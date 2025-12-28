#pragma once
#include <JuceHeader.h>

class BatSynth : public juce::Synthesiser
{
public:
    static constexpr size_t tableSize = 2048;
    std::array<float, tableSize> wavetable;
    
    void setCustomWaveform (const std::vector<float>& src)
    {
        jassert(src.size() == wavetable.size());
        std::copy(src.begin(), src.end(), wavetable.begin());
    }

};
