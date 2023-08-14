#pragma once
#include <JuceHeader.h>
#include "BatSynthAudioProcessor.h"
#include "CustomFeel.h"

class BatSynthAudioProcessorEditor : public AudioProcessorEditor,
                                           public Slider::Listener, 
                                           public ComboBox::Listener,
                                           public Timer
{
public:
    BatSynthAudioProcessorEditor (BatSynthAudioProcessor&);
    ~BatSynthAudioProcessorEditor();
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider*) override {};
    void comboBoxChanged(ComboBox*) override {};
    void timerCallback() override;
private:
    void addAllGUIComponents();
    int intify(float f);

    BatSynthAudioProcessor& processor;
    
    Label labels[NUM_SLIDERS + NUM_COMBOBOXES];
    ComboBox comboboxes[NUM_COMBOBOXES]; // osc1, osc2, dist
    Slider sliders[NUM_SLIDERS]; // atk, dcy, sus, rel, osc2pitch, osc2gain, noisegain, filtercut, filterres, lfoInten,
                                 // lfoRate, compRatio, compThr, compAtk, compRel, compGain, distDrive, delTime, delFdbk, delGain, totGain
    
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> comboboxattachments[NUM_COMBOBOXES];
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sliderattachments[NUM_SLIDERS];

    ScopeComponent<float> scopeComponent;
    
    //MidiKeyboardComponent keyboardComponent;
    //MidiKeyboardState keyboardState;
    
    CustomFeel lookAndFeel;
    juce::Image image;
    juce::JPEGImageFormat* j;
    static float scale;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BatSynthAudioProcessorEditor)
};
