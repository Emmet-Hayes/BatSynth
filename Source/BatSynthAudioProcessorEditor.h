#pragma once
#include <JuceHeader.h>
#include "BatSynthAudioProcessor.h"
#include "../../Common/OpenGLComponent.h"
#include "../../Common/BaseAudioProcessorEditor.h"
#include "../../Common/BatSynthLookAndFeel.h"
#include "../../Common/PresetBar.h"

class BatSynthAudioProcessorEditor : public BaseAudioProcessorEditor
                                   , public Slider::Listener
                                   , public ComboBox::Listener
                                   , public Timer
{
public:
    BatSynthAudioProcessorEditor (BatSynthAudioProcessor&);
    ~BatSynthAudioProcessorEditor();
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider*) override {};
    void comboBoxChanged(juce::ComboBox*) override {};
    void timerCallback() override;
private:
    void addAllGUIComponents();
    void handleOpenGLComponent(float amplitude, float frequency);

    BatSynthLookAndFeel lookAndFeel;

    BatSynthAudioProcessor& processor;
    PresetBar presetBar;
    
    Label labels[NUM_SLIDERS + NUM_COMBOBOXES];
    ComboBox comboboxes[NUM_COMBOBOXES]; // osc1, osc2, dist
    std::unique_ptr<juce::Slider> sliders[NUM_SLIDERS]; // atk, dcy, sus, rel, osc2pitch, osc2gain, noisegain, filtercut, filterres, lfoInten,
                                 // lfoRate, totGain

    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> comboboxattachments[NUM_COMBOBOXES];
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> sliderattachments[NUM_SLIDERS];

    WaveScopeComponent<float> waveScopeComponent;
    SpectrumScopeComponent<float> spectrumScopeComponent;
    std::unique_ptr<OpenGLComponent> openGLComponent;
    bool isOpenGLAvailable = false;
    int tryInitializeOpenGL = 0;
    float scale = 1.0f;
    //MidiKeyboardComponent keyboardComponent;
    //MidiKeyboardState keyboardState;
    
    juce::Image image;
    
    static constexpr int defaultWidth  { 800 };
    static constexpr int defaultHeight { 600 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BatSynthAudioProcessorEditor)
};
