#include "BatSynthAudioProcessor.h"
#include "BatSynthAudioProcessorEditor.h"
#include "../../Common/LogMsSlider.h"
#include "../../Common/PitchSlider.h"
#include "../../Common/FreqSlider.h"
#include "../../Common/PercentSlider.h"

BatSynthAudioProcessorEditor::BatSynthAudioProcessorEditor (BatSynthAudioProcessor& p)
:   BaseAudioProcessorEditor { p }
,   processor                { p }
,   presetBar                { p }
,   waveScopeComponent       { processor.audioBufferQueue }
,   spectrumScopeComponent   { processor.audioBufferQueue, lookAndFeel }
,   keyboardComponent { processor.keyboardState, juce::MidiKeyboardComponent::Orientation::horizontalKeyboard }
{
    startTimerHz(30); // refresh at 30 fps
    setLookAndFeel(&lookAndFeel);
    addAllGUIComponents();
}

void BatSynthAudioProcessorEditor::addAllGUIComponents()
{
    auto setupComboBox = [&](ComboBox &box, const Justification& jtype, const String &initialText,
                             bool isOsc = true) 
    {
        const std::vector<String> options = isOsc ? std::vector<String>({ "Sine", "Saw", "Square", "Soft Distorted Sine", "Hard Distorted Sine", 
                                                                    "Triangle", "Noise", "Sharp Saw", "Thick Saw", "Pulse 0.2", "Pulse 0.3",
                                                                    "Saw Pulse 0.2", "Saw Pulse 0.3" }) :
                                                std::vector<String>({ "None", "Sigmoid", "Tanh", "Arctan10", "FastArcTan10" });
        for (int i = 0; i < options.size(); ++i)
            box.addItem(options[i], i+1);
        box.setJustificationType(jtype);
        box.setText(initialText);
        box.addListener(this);
        addAndMakeVisible(&box);
    };
    
    auto setupLabel = [&](Label &label, const juce::String& text, const Justification& jtype,
                          Component* component, bool isAttached) 
    {
        label.setText(text, dontSendNotification);
        label.setJustificationType(jtype);
        label.attachToComponent(component, isAttached);
    };

    auto setupSlider = [&](Slider* slider, Slider::SliderStyle style, float rangeStart, float rangeEnd, float value)
    {
        slider->setSliderStyle(style);
        slider->setRange(rangeStart, rangeEnd);
        slider->setValue(value);
        slider->addListener(this);
        addAndMakeVisible(slider);
    };

    std::string sliderlabels[NUM_SLIDERS] = { "Atk", "Dec", "Sus", "Rel", "Pitch", "Gain", "Noise", "Cutoff", "Resonance", 
                                              "LFO Filter", "LFO F. Rate", "LFO Pitch", "LFO P. Rate", 
                                              "Total Gain" };

    sliders[0] = std::make_unique<LogMsSlider>(); // attack
    setupSlider(sliders[0].get(), Slider::SliderStyle::Rotary, 0.1f, 5000.0f, 10.0f);
    sliders[1] = std::make_unique<LogMsSlider>(); // decay
    setupSlider(sliders[1].get(), Slider::SliderStyle::Rotary, 0.1f, 2000.0f, 200.0f);
    sliders[2] = std::make_unique<juce::Slider>(); // sustain
    setupSlider(sliders[2].get(), Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.8f);
    sliders[3] = std::make_unique<LogMsSlider>(); // release
    setupSlider(sliders[3].get(), Slider::SliderStyle::Rotary, 0.1f, 5000.0f, 100.0f);
    sliders[4] = std::make_unique<PitchSlider>();
    setupSlider(sliders[4].get(), Slider::SliderStyle::Rotary, 0.5f, 2.0f, 1.0f);
    sliders[5] = std::make_unique<PercentSlider>();
    setupSlider(sliders[5].get(), Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.5f);
    sliders[6] = std::make_unique<PercentSlider>();
    setupSlider(sliders[6].get(), Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.0f);
    sliders[7] = std::make_unique<FreqSlider>();
    setupSlider(sliders[7].get(), Slider::SliderStyle::Rotary, 50.0f, 9000.0f, 500.0f);
    sliders[8] = std::make_unique<juce::Slider>();
    setupSlider(sliders[8].get(), Slider::SliderStyle::Rotary, 0.1f, 0.99f, 0.1f);
    sliders[9] = std::make_unique<PercentSlider>();
    setupSlider(sliders[9].get(), Slider::SliderStyle::Rotary, 0.0f, 0.9f, 0.0f);
    sliders[10] = std::make_unique<FreqSlider>();
    setupSlider(sliders[10].get(), Slider::SliderStyle::Rotary, 0.5f, 12.0f, 2.0f);
    sliders[11] = std::make_unique<PercentSlider>();
    setupSlider(sliders[11].get(), Slider::SliderStyle::Rotary, 0.0f, 0.9f, 0.0f);
    sliders[12] = std::make_unique<FreqSlider>();
    setupSlider(sliders[12].get(), Slider::SliderStyle::Rotary, 0.5f, 12.0f, 2.0f);
    sliders[13] = std::make_unique<PercentSlider>();
    setupSlider(sliders[13].get(), Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.8f);

    for (int i = 0; i < NUM_SLIDERS; ++i)
        setupLabel(labels[i], sliderlabels[i], Justification::centred, sliders[i].get(), false);

    std::string comboboxlabels[NUM_COMBOBOXES] = { "Osc 1", "Osc 2" };
    for (int i = 0; i < NUM_COMBOBOXES; ++i)
        setupLabel(labels[i + NUM_SLIDERS], comboboxlabels[i], Justification::centred, &comboboxes[i], false);

    setupComboBox(comboboxes[0], Justification::centred, "Saw");
    setupComboBox(comboboxes[1], Justification::centred, "Sine");

    addAndMakeVisible(&waveScopeComponent);
    addAndMakeVisible(&spectrumScopeComponent);
    processor.waveScopeDataCollector.addListener(&waveScopeComponent);
    processor.spectrumScopeDataCollector.addListener(&spectrumScopeComponent);

    auto setupComboBoxAttachments = [&](juce::ComboBox& comboBox, juce::String treelabel,
                                      std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>& comboBoxAttachment) 
    {
        comboBoxAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(
                                     processor.apvts, treelabel, comboBox));
    };

    auto setupSliderAttachments = [&](juce::Slider* slider, juce::String treelabel,
                                      std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>& sliderAttachment) 
    {
        sliderAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(
                                     processor.apvts, treelabel, *slider));
    };

    std::string comboboxtreelabels[NUM_COMBOBOXES] = { "wavetype", "wavetype2" };
    for (int i = 0; i < NUM_COMBOBOXES; ++i)
        setupComboBoxAttachments(comboboxes[i], comboboxtreelabels[i], comboboxattachments[i]);

    std::string slidertreelabels[NUM_SLIDERS] = { "attack", "decay", "sustain", "release", "osc2Pitch", "osc2Gain", "noiseGain", "filterCutoff",
                                                "filterResonance", "lfoFilterIntensity", "lfoFilterRate", "lfoPitchIntensity", "lfoPitchRate",
                                                "totalGain" };
    for (int i = 0; i < NUM_SLIDERS; ++i)
        setupSliderAttachments(sliders[i].get(), slidertreelabels[i], sliderattachments[i]);
    
    addAndMakeVisible(presetBar);

    sliders[2]->textFromValueFunction = nullptr;
    sliders[2]->setNumDecimalPlacesToDisplay(2);
    sliders[8]->textFromValueFunction = nullptr;
    sliders[8]->setNumDecimalPlacesToDisplay(2);

    openGLComponent = std::make_unique<OpenGLComponent>();
    addAndMakeVisible(openGLComponent.get());

    addAndMakeVisible(keyboardComponent);
    
    const auto ratio = static_cast<double> (defaultWidth) / defaultHeight;
    setResizable(false, true);
    getConstrainer()->setFixedAspectRatio (ratio);
    getConstrainer()->setSizeLimits(defaultWidth, defaultHeight, defaultWidth * 2, defaultHeight * 2);
    setSize (defaultWidth, defaultHeight);
}


BatSynthAudioProcessorEditor::~BatSynthAudioProcessorEditor() 
{
    setLookAndFeel(nullptr);

    for (int i = 0; i < NUM_COMBOBOXES; ++i)
        comboboxes[i].removeListener(this);
        
    for (int i = 0; i < NUM_SLIDERS; ++i)
        sliders[i]->removeListener(this);

    processor.waveScopeDataCollector.removeAllListeners();
    processor.spectrumScopeDataCollector.removeAllListeners();
}

void BatSynthAudioProcessorEditor::paint (Graphics& g) 
{
    g.fillAll(juce::Colours::black);
}

void BatSynthAudioProcessorEditor::resized() 
{
    scale = static_cast<float> (getWidth()) / defaultWidth;
    auto setBoundsAndApplyScaling = [&](juce::Component* component, int x, int y, int w, int h, bool isSlider = false)
    {
        component->setBounds(static_cast<int>(x * scale), static_cast<int>(y * scale), 
                             static_cast<int>(w * scale), static_cast<int>(h * scale));
        if (isSlider)
            dynamic_cast<juce::Slider*>(component)->setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50 * scale, 15 * scale);
    };
    
    lookAndFeel.setWindowScale(scale);
    setBoundsAndApplyScaling(&presetBar, 0, 5, 800, 25);
    setBoundsAndApplyScaling(&comboboxes[0], 10, 75, 120, 30);
    setBoundsAndApplyScaling(&comboboxes[1], 10, 160, 120, 30);
    setBoundsAndApplyScaling(sliders[4].get(), 180, 150, 60, 60, true);
    setBoundsAndApplyScaling(sliders[5].get(), 260, 150, 60, 60, true);
    setBoundsAndApplyScaling(sliders[6].get(), 340, 150, 60, 60, true);
    setBoundsAndApplyScaling(sliders[0].get(), 140, 50, 70, 70, true);
    setBoundsAndApplyScaling(sliders[1].get(), 220, 50, 70, 70, true);
    setBoundsAndApplyScaling(sliders[2].get(), 300, 50, 70, 70, true);
    setBoundsAndApplyScaling(sliders[3].get(), 380, 50, 70, 70, true);
    setBoundsAndApplyScaling(sliders[7].get(), 460, 50, 80, 80, true);
    setBoundsAndApplyScaling(sliders[8].get(), 460, 150, 80, 80, true);
    setBoundsAndApplyScaling(sliders[9].get(), 570, 50, 60, 60, true);
    setBoundsAndApplyScaling(sliders[10].get(), 640, 50, 60, 60, true);
    setBoundsAndApplyScaling(sliders[11].get(), 570, 145, 60, 60, true);
    setBoundsAndApplyScaling(sliders[12].get(), 640, 145, 60, 60, true);
    setBoundsAndApplyScaling(sliders[13].get(), 710, 85, 80, 80, true);
   #if JUCE_WINDOWS // for now, openGL components are only rendering on windows, which is honestly a huge bummer.
    setBoundsAndApplyScaling(&waveScopeComponent, 15, 235, 380, 130);
    setBoundsAndApplyScaling(&spectrumScopeComponent, 15, 425, 380, 130);
    setBoundsAndApplyScaling(openGLComponent.get(), 400, 235, 380, 310);
    setBoundsAndApplyScaling(&keyboardComponent, 10, 560, 780, 40);
   #elif JUCE_MAC
    setBoundsAndApplyScaling(&waveScopeComponent, 15, 235, 770, 130);
    setBoundsAndApplyScaling(&spectrumScopeComponent, 15, 425, 770, 130);
    setBoundsAndApplyScaling(openGLComponent.get(), 400, 235, 0, 0); // empty
    setBoundsAndApplyScaling(&keyboardComponent, 10, 560, 780, 40);
   #endif
}

void BatSynthAudioProcessorEditor::timerCallback() 
{
    float amplitude = processor.getCurrentAmplitude();
    float frequency = processor.getCurrentFrequency();

    lookAndFeel.setGainColorIntensity(amplitude);
    lookAndFeel.setFrequencyColor(frequency);
    openGLComponent->synthNoteColor = frequency;
    openGLComponent->audioAmplitude = amplitude;
    repaint();
}
