#include "BatSynthAudioProcessor.h"
#include "BatSynthAudioProcessorEditor.h"
#include "../../Common/LogMsSlider.h"
#include "../../Common/PitchSlider.h"
#include "../../Common/FreqSlider.h"
#include "../../Common/PercentSlider.h"

float BatSynthAudioProcessorEditor::scale = 1.0f;

BatSynthAudioProcessorEditor::BatSynthAudioProcessorEditor (BatSynthAudioProcessor& p)
:   BaseAudioProcessorEditor { p }
,   presetBar                { p }
,   processor                { p }
,   waveScopeComponent       { processor.audioBufferQueue }
,   spectrumScopeComponent   { processor.audioBufferQueue, lookAndFeel }
{
    startTimerHz(60); // refresh at 30 fps
    addAllGUIComponents();
}

void BatSynthAudioProcessorEditor::addAllGUIComponents()
{    
    setLookAndFeel(&lookAndFeel);
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
    
    auto setupSlider = [&](Slider* slider, Slider::SliderStyle style, float rangeStart, float rangeEnd, float value,
                           Slider::TextEntryBoxPosition position, int textBoxWidth, int textBoxHeight) 
    {
        slider->setSliderStyle(style);
        slider->setRange(rangeStart, rangeEnd);
        slider->setValue(value);
        slider->setTextBoxStyle(position, true, textBoxWidth, textBoxHeight);
        slider->setTextBoxIsEditable(true);
        slider->setLookAndFeel(&lookAndFeel);
        slider->addListener(this);
        addAndMakeVisible(slider);
    };
    
    Font font1("Lucida Console", scale*10.0f, Font::bold);
    Font font2("Lucida Console", scale*9.0f, Font::bold);
    
    std::string sliderlabels[NUM_SLIDERS] = { "Atk", "Dec", "Sus", "Rel", "Pitch", "Gain", "Noise", "Cutoff", "Resonance", 
                                              "LFO Filter", "LFO F. Rate", "LFO Pitch", "LFO P. Rate", 
                                              "Total Gain" };
    
    sliders[0] = std::make_unique<LogMsSlider>(); // attack
    setupSlider(sliders[0].get(), Slider::SliderStyle::Rotary, 0.1f, 5000.0f, 10.0f, Slider::TextBoxBelow, 50, 15);
    sliders[1] = std::make_unique<LogMsSlider>(); // decay
    setupSlider(sliders[1].get(), Slider::SliderStyle::Rotary, 0.1f, 2000.0f, 200.0f, Slider::TextBoxBelow, 50, 15);
    sliders[2] = std::make_unique<juce::Slider>(); // sustain
    setupSlider(sliders[2].get(), Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.8f, Slider::TextBoxBelow, 50, 15);
    sliders[3] = std::make_unique<LogMsSlider>(); // release
    setupSlider(sliders[3].get(), Slider::SliderStyle::Rotary, 0.1f, 5000.0f, 100.0f, Slider::TextBoxBelow, 50, 15);
    sliders[4] = std::make_unique<PitchSlider>();
    setupSlider(sliders[4].get(), Slider::SliderStyle::Rotary, 0.5f, 2.0f, 1.0f, Slider::TextBoxBelow, 50, 15);
    sliders[5] = std::make_unique<PercentSlider>();
    setupSlider(sliders[5].get(), Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.5f, Slider::TextBoxBelow, 50, 15);
    sliders[6] = std::make_unique<PercentSlider>();
    setupSlider(sliders[6].get(), Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.0f, Slider::TextBoxBelow, 50, 15);
    sliders[7] = std::make_unique<FreqSlider>();
    setupSlider(sliders[7].get(), Slider::SliderStyle::Rotary, 50.0f, 9000.0f, 500.0f, Slider::TextBoxBelow, 50, 15);
    sliders[8] = std::make_unique<juce::Slider>();
    setupSlider(sliders[8].get(), Slider::SliderStyle::Rotary, 0.1f, 0.99f, 0.1f, Slider::TextBoxBelow, 50, 15);

    sliders[9] = std::make_unique<PercentSlider>();
    setupSlider(sliders[9].get(), Slider::SliderStyle::Rotary, 0.0f, 0.9f, 0.0f, Slider::TextBoxBelow, 50, 15);
    sliders[10] = std::make_unique<FreqSlider>();
    setupSlider(sliders[10].get(), Slider::SliderStyle::Rotary, 0.5f, 12.0f, 2.0f, Slider::TextBoxBelow, 50, 15);
    sliders[11] = std::make_unique<PercentSlider>();
    setupSlider(sliders[11].get(), Slider::SliderStyle::Rotary, 0.0f, 0.9f, 0.0f, Slider::TextBoxBelow, 50, 15);
    sliders[12] = std::make_unique<FreqSlider>();
    setupSlider(sliders[12].get(), Slider::SliderStyle::Rotary, 0.5f, 12.0f, 2.0f, Slider::TextBoxBelow, 50, 15);
    sliders[13] = std::make_unique<PercentSlider>();
    setupSlider(sliders[13].get(), Slider::SliderStyle::Rotary, 0.0f, 1.0f, 0.8f, Slider::TextBoxBelow, 50, 15);
    
    for (int i = 0; i < NUM_SLIDERS; ++i)
        setupLabel(labels[i], sliderlabels[i], font2, Justification::centred, sliders[i].get(), false);

    std::string comboboxlabels[NUM_COMBOBOXES] = { "Osc 1", "Osc 2" };
    for (int i = 0; i < NUM_COMBOBOXES; ++i)
        setupLabel(labels[i + NUM_SLIDERS], comboboxlabels[i], font1, Justification::centred, &comboboxes[i], false);
    
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
    
    presetBar.setLookAndFeel(&lookAndFeel);
    addAndMakeVisible(presetBar);

    sliders[2]->textFromValueFunction = nullptr;
    sliders[2]->setNumDecimalPlacesToDisplay(2);
    sliders[8]->textFromValueFunction = nullptr;
    sliders[8]->setNumDecimalPlacesToDisplay(2);

    openGLComponent = std::make_unique<OpenGLComponent>();
    addAndMakeVisible(openGLComponent.get());

    image = juce::ImageCache::getFromMemory(BinaryData::bgfile_jpg, BinaryData::bgfile_jpgSize);
    setSize(800, 600);
    lookAndFeel.setWindowScale(scale);
    setLookAndFeel(&lookAndFeel);
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
        sliders[i]->removeListener(this);

    processor.waveScopeDataCollector.removeAllListeners();
    processor.spectrumScopeDataCollector.removeAllListeners();
}

void BatSynthAudioProcessorEditor::paint (Graphics& g) {
    int height = static_cast<int>(scale * 800), width = static_cast<int>(scale * 600);
    g.drawImage(image, 0, 0, height, width, 0, 0, 1600, 1200);
}

void BatSynthAudioProcessorEditor::resized() {
    //scale = 1.0f;
    presetBar.setBounds(0, 5, 800, 25);
    comboboxes[0].setBounds(10, 75, 120, 30);
    comboboxes[1].setBounds(10, 160, 120, 30);
    sliders[4]->setBounds(180, 150, 60, 60);
    sliders[5]->setBounds(260, 150, 60, 60);
    sliders[6]->setBounds(340, 150, 60, 60);
    sliders[0]->setBounds(140, 50, 70, 70);
    sliders[1]->setBounds(220, 50, 70, 70);
    sliders[2]->setBounds(300, 50, 70, 70);
    sliders[3]->setBounds(380, 50, 70, 70);
    sliders[7]->setBounds(460, 50, 80, 80);
    sliders[8]->setBounds(460, 150, 80, 80);
    sliders[9]->setBounds(570, 50, 60, 60);
    sliders[10]->setBounds(640, 50, 60, 60);
    sliders[11]->setBounds(570, 145, 60, 60);
    sliders[12]->setBounds(640, 145, 60, 60);
    sliders[13]->setBounds(710, 85, 80, 80);
    waveScopeComponent.setBounds(15, 235, 380, 180);
    spectrumScopeComponent.setBounds(15, 425, 380, 180);
    openGLComponent->setBounds(400, 235, 380, 360);
}

void BatSynthAudioProcessorEditor::timerCallback() {
    float amplitude = processor.getCurrentAmplitude();
    float frequency = processor.getCurrentFrequency();
    
    lookAndFeel.setGainColorIntensity(amplitude);
    lookAndFeel.setFrequencyColor(frequency);
    openGLComponent->colorVal = frequency;
    openGLComponent->colorIntensity = amplitude;
    repaint();
}
