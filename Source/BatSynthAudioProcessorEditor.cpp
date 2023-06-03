/*==============================================================================
 PluginEditor.h
 Created: 3 Sep 2019 11:46:10pm
 Author:  Emmet Hayes
 
 The editor's constructor creates a user interface based on the size of your
 device, then procedurally initializes the different components in the window
 with terse yet descriptive detail.
  ==============================================================================*/

#include "BatSynthAudioProcessor.h"
#include "BatSynthAudioProcessorEditor.h"

float BatSynthAudioProcessorEditor::scale = 1.0f;

BatSynthAudioProcessorEditor::BatSynthAudioProcessorEditor (BatSynthAudioProcessor& p)
:   AudioProcessorEditor (&p)
,   processor (p)
,   scopeComponent(processor.audioBufferQueue) 
{
    startTimerHz(30); // refresh at 30 fps
    setSize(800, 600);
    lookAndFeel.setScale(scale);
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
    
    auto setupLabel = [&](Label &label, const juce::String& text, const juce::Font& font, const Justification& jtype,
                          Component* component, bool isAttached) 
    {
        label.setText(text, dontSendNotification);
        label.setFont(font);
        label.setJustificationType(jtype);
        label.attachToComponent(component, isAttached);
    };
    
    auto setupSlider = [&](Slider& slider, Slider::SliderStyle style, float rangeStart, float rangeEnd, float value,
                           Slider::TextEntryBoxPosition position, int textBoxWidth, int textBoxHeight) 
    {
        slider.setSliderStyle(style);
        slider.setRange(rangeStart, rangeEnd);
        slider.setValue(value);
        slider.setTextBoxStyle(position, true, textBoxWidth, textBoxHeight);
        slider.addListener(this);
        addAndMakeVisible(&slider);
    };
    
    Font font1("Lucida Console", scale*10.0f, Font::bold);
    Font font2("Lucida Console", scale*9.0f, Font::bold);
    
    std::string sliderlabels[NUM_SLIDERS] = { "Atk", "Dec", "Sus", "Rel", "Pitch", "Gain", "Noise", "Cutoff", "Resonance", 
                                              "LFO Filter", "LFO F. Rate", "LFO Pitch", "LFO P. Rate", 
                                              "Ratio", "Threshold", "Attack", "Release", "Comp Gain", "Drive", "Time", 
                                              "Feedback", "Delay Gain", "Total Gain" };
    for (int i = 0; i < NUM_SLIDERS; ++i)
        setupLabel(labels[i], sliderlabels[i], font2, Justification::centred, &sliders[i], false);
    
    setupSlider(sliders[0], Slider::SliderStyle::Rotary, 0.1f, 5000.0f, 10.0f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[1], Slider::SliderStyle::Rotary, 0.1f, 2000.0f, 200.0f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[2], Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.8f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[3], Slider::SliderStyle::Rotary, 0.1f, 5000.0f, 100.0f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[4], Slider::SliderStyle::Rotary, 0.5f, 2.0f, 1.0f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[5], Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.5f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[6], Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.0f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[7], Slider::SliderStyle::Rotary, 50.0f, 9000.0f, 500.0f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[8], Slider::SliderStyle::Rotary, 0.1f, 0.99f, 0.1f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[9], Slider::SliderStyle::Rotary, 0.0f, 0.9f, 0.0f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[10], Slider::SliderStyle::Rotary, 0.5f, 12.0f, 2.0f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[11], Slider::SliderStyle::Rotary, 0.0f, 0.9f, 0.0f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[12], Slider::SliderStyle::Rotary, 0.5f, 12.0f, 2.0f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[13], Slider::SliderStyle::Rotary, 1.0f, 10.0f, 2.0f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[14], Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.9f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[15], Slider::SliderStyle::Rotary, 0.5f, 200.0f, 10.0f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[16], Slider::SliderStyle::Rotary, 0.1f, 2.0f, 0.995f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[17], Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.8f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[18], Slider::SliderStyle::Rotary, 0.01f, 6.0f, 1.0f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[19], Slider::SliderStyle::Rotary, 100.0f, 88000.0f, 10000.0f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[20], Slider::SliderStyle::Rotary, 0.0f, 0.9f, 0.5f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[21], Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.5f, Slider::TextBoxBelow, 50, 15);
    setupSlider(sliders[22], Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.8f, Slider::TextBoxBelow, 50, 15);
    
    std::string comboboxlabels[NUM_COMBOBOXES] = { "Osc 1", "Osc 2", "Distortion" };
    for (int i = 0; i < NUM_COMBOBOXES; ++i)
        setupLabel(labels[i + NUM_SLIDERS], comboboxlabels[i], font1, Justification::centred, &comboboxes[i], false);
    
    setupComboBox(comboboxes[0], Justification::centred, "Saw");
    setupComboBox(comboboxes[1], Justification::centred, "Sine");
    setupComboBox(comboboxes[2], Justification::centred, "None", false); // NOT an osc
    
    addAndMakeVisible(&scopeComponent);
    processor.scopeDataCollector.addListener(&scopeComponent);
    //addAndMakeVisible(&keyboardComponent);

    auto setupComboBoxAttachments = [&](ComboBox& comboBox, juce::String treelabel,
                                      std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment>& comboBoxAttachment) 
    {
        comboBoxAttachment.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(
                                     processor.tree, treelabel, comboBox));
    };
    
    auto setupSliderAttachments = [&](Slider& slider, juce::String treelabel,
                                      std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment>& sliderAttachment) 
    {
        sliderAttachment.reset(new AudioProcessorValueTreeState::SliderAttachment(
                                     processor.tree, treelabel, slider));
    };
    
    std::string comboboxtreelabels[NUM_COMBOBOXES] = { "wavetype", "wavetype2", "distortionType" };
    for (int i = 0; i < NUM_COMBOBOXES; ++i)
        setupComboBoxAttachments(comboboxes[i], comboboxtreelabels[i], comboboxattachments[i]);

    std::string slidertreelabels[NUM_SLIDERS] = { "attack", "decay", "sustain", "release", "osc2Pitch", "osc2Gain", "noiseGain", "filterCutoff",
                                                "filterResonance", "lfoFilterIntensity", "lfoFilterRate", "lfoPitchIntensity", "lfoPitchRate",
                                                "compressionRatio", "compressionThreshold", "compressionAttack", "compressionRelease",
                                                "compressionGain", "distortionDrive", "delayTime", "delayFeedback", "delayGain", "totalGain" };
    for (int i = 0; i < NUM_SLIDERS; ++i)
        setupSliderAttachments(sliders[i], slidertreelabels[i], sliderattachments[i]);

    int ndp_sliders[NUM_SLIDERS] = { 0, 0, 2, 0, 3, 3, 3, 0, 2, 2, 2, 2, 2, 1, 2, 1, 1, 2, 2, 0, 2, 2, 2};
    for (int i = 0; i < NUM_SLIDERS; ++i)
    {
        sliders[i].textFromValueFunction = nullptr;
        sliders[i].setNumDecimalPlacesToDisplay(ndp_sliders[i]);
    }
    
    image = juce::ImageCache::getFromMemory(BinaryData::bgfile4_jpg, BinaryData::bgfile4_jpgSize);
}

int BatSynthAudioProcessorEditor::intify(float f)
{
    return static_cast<int>(f);
}

BatSynthAudioProcessorEditor::~BatSynthAudioProcessorEditor() {
    setLookAndFeel(nullptr);
    for (int i = 0; i < NUM_COMBOBOXES; ++i)
        comboboxes[i].removeListener(this);
        
    for (int i = 0; i < NUM_SLIDERS; ++i)
        sliders[i].removeListener(this);

    processor.scopeDataCollector.removeAllListeners();
}

void BatSynthAudioProcessorEditor::paint (Graphics& g) {
    int height = static_cast<int>(scale * 800), width = static_cast<int>(scale * 600);
    g.drawImage(image, 0, 0, height, width, 0, 0, 1600, 1200);
}

void BatSynthAudioProcessorEditor::resized() {
    //scale = 1.0f;
    comboboxes[0].setBounds(10, 30, 120, 30);
    comboboxes[1].setBounds(10, 85, 120, 30);
    sliders[0].setBounds(130, 40, 60, 60);
    sliders[1].setBounds(200, 40, 60, 60);
    sliders[2].setBounds(270, 40, 60, 60);
    sliders[3].setBounds(340, 40, 60, 60);
    sliders[4].setBounds(70, 130, 50, 50);
    sliders[5].setBounds(10, 130, 50, 50);
    sliders[6].setBounds(130, 130, 50, 50);
    sliders[7].setBounds(410, 45, 70, 70);
    sliders[8].setBounds(490, 45, 70, 70);
    sliders[9].setBounds(570, 20, 60, 60);
    sliders[10].setBounds(640, 20, 60, 60);
    sliders[11].setBounds(570, 100, 60, 60);
    sliders[12].setBounds(640, 100, 60, 60);
    sliders[13].setBounds(10, 220, 60, 60);
    sliders[14].setBounds(80, 220, 60, 60);
    sliders[15].setBounds(10, 300, 60, 60);
    sliders[16].setBounds(80, 300, 60, 60);
    sliders[17].setBounds(10, 460, 60, 60);
    comboboxes[2].setBounds(10, 550, 60, 30);
    sliders[18].setBounds(80, 540, 50, 50);
    sliders[19].setBounds(10, 380, 60, 60);
    sliders[20].setBounds(80, 380, 60, 60);
    sliders[21].setBounds(80, 460, 60, 60);
    sliders[22].setBounds(710, 35, 80, 80);
    scopeComponent.setBounds(185, 160, 600, 400);
}

void BatSynthAudioProcessorEditor::timerCallback() {
    float amplitude = processor.getCurrentAmplitude();
    float frequency = processor.getCurrentFrequency();
    
    lookAndFeel.setGainColorIntensity(amplitude);
    lookAndFeel.setFrequencyColor(frequency);
    repaint();
}
