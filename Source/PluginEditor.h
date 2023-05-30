/*
  ==============================================================================

 PluginEditor.h
 Created: 3 Sep 2019 11:46:10pm
 Author:  Emmet Hayes
 
 The editor encapsulates the entire user interface of this application. It
 interacts with the Processor, rather than interacting with voices or hardware
 directly. each slider has a value tree slider/combobox attachment to connect to
 the processor to handle changes in DSP effects. All sliders and combo boxes have
 labels attached to them. There is a background image, a scope component, a static
 scale value depending on your device and a styling class called lookAndFeel for
 aesthetic choices about the sliders and combo boxes
  ==============================================================================
*/
#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "CustomFeel.h"

class SynthFrameworkAudioProcessorEditor : public AudioProcessorEditor,
                                           public Slider::Listener, 
                                           public ComboBox::Listener,
                                           public Timer
{
public:
    SynthFrameworkAudioProcessorEditor (SynthFrameworkAudioProcessor&);
    ~SynthFrameworkAudioProcessorEditor();
    void paint (Graphics&) override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;
    void comboBoxChanged(ComboBox* comboBox) override;
    void timerCallback() override;
private:
    void addAllGUIComponents();

    SynthFrameworkAudioProcessor& processor; //accesses the processor object that created the editor
    
    ComboBox comboboxes[NUM_COMBOBOXES]; // osc1, osc2, dist
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> comboboxattachments[NUM_COMBOBOXES];
    Slider sliders[NUM_SLIDERS]; // atk, dcy, sus, rel, osc2pitch, osc2gain, noisegain, filtercut, filterres, lfoInten,
                                 // lfoRate, compRatio, compThr, compAtk, compRel, compGain, distDrive, delTime, delFdbk, delGain, totGain
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sliderattachments[NUM_SLIDERS];
    Label labels[NUM_SLIDERS + NUM_COMBOBOXES]; //sliders first [0-22], then comboboxes [23-25]

    ScopeComponent<float> scopeComponent; //this is the oscilloscope UIwidget
    
    //MidiKeyboardComponent keyboardComponent;
    //MidiKeyboardState keyboardState;
    
    CustomFeel lookAndFeel; //defines drawing of sliders combo boxes and fonts
    juce::Image image; //background image
    juce::JPEGImageFormat* j; //necessary to convert background image format
    static float scale; //affects scale of the entire window

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthFrameworkAudioProcessorEditor)
};
