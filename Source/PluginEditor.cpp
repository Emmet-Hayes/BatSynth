/*==============================================================================
 PluginEditor.h
 Created: 3 Sep 2019 11:46:10pm
 Author:  Emmet Hayes
 
 The editor's constructor creates a user interface based on the size of your
 device, then procedurally initializes the different components in the window
 with terse yet descriptive detail.
  ==============================================================================*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
float SynthFrameworkAudioProcessorEditor::scale = 1.f;
//==============================================================================
SynthFrameworkAudioProcessorEditor::SynthFrameworkAudioProcessorEditor (SynthFrameworkAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), scopeComponent(processor.audioBufferQueue) {
    addAllGUIComponents();
}

void SynthFrameworkAudioProcessorEditor::addAllGUIComponents() {
    Rectangle<int> deviceScreen = Desktop::getInstance().getDisplays().getMainDisplay().userArea; //get screen size
    double deviceW = deviceScreen.getWidth() / 800., deviceH = deviceScreen.getHeight() / 600.; //800 and 600 is default scale
    scale = (deviceW > deviceH) ? deviceH : deviceW; //make sure window fits on the screen...
    scale *= 0.6f; //don't let scale dominate the DAW space.
    setSize (800*scale, 600*scale); //setting the actual size of the editor
    lookAndFeel.setScale(scale); //sets font sizes of the look and feel aspects based on scale
    setLookAndFeel(&lookAndFeel);
    Font font1("Lucida Console", scale*10.0f, Font::bold);
    Font font2("Lucida Console", scale*9.0f, Font::bold);
    o1Label.setText("Osc 1", dontSendNotification); ////////////
    o1Label.setFont(font1);
    o1Label.attachToComponent(&osc1Gui, false);
    osc1Gui.addItem("Sine", 1);
    osc1Gui.addItem("Saw", 2);
    osc1Gui.addItem("Square", 3);
    osc1Gui.addItem("Soft Distorted Sine", 4);
    osc1Gui.addItem("Hard Distorted Sine", 5);
    osc1Gui.addItem("Triangle", 6);
    osc1Gui.addItem("Noise", 7);
    osc1Gui.addItem("Sharp Saw", 8);
    osc1Gui.addItem("Thick Saw", 9);
    osc1Gui.addItem("Pulse 0.2", 10);
    osc1Gui.addItem("Pulse 0.3", 11);
    osc1Gui.addItem("Saw Pulse 0.2", 12);
    osc1Gui.addItem("Saw Pulse 0.3", 13);
    osc1Gui.setJustificationType(Justification::centred);
    osc1Gui.setText("Saw");
    addAndMakeVisible(&osc1Gui);
    osc1Gui.addListener(this);
    o2Label.setText("Osc 2", dontSendNotification); ////////////
    o2Label.setFont(font1);
    o2Label.attachToComponent(&osc2Gui, false);
    osc2Gui.addItem("Sine", 1);
    osc2Gui.addItem("Saw", 2);
    osc2Gui.addItem("Square", 3);
    osc2Gui.addItem("Soft Distorted Sine", 4);
    osc2Gui.addItem("Hard Distorted Sine", 5);
    osc2Gui.addItem("Triangle", 6);
    osc2Gui.addItem("Noise", 7);
    osc2Gui.addItem("Sharp Saw", 8);
    osc2Gui.addItem("Thick Saw", 9);
    osc2Gui.addItem("Pulse 0.2", 10);
    osc2Gui.addItem("Pulse 0.3", 11);
    osc2Gui.addItem("Saw Pulse 0.2", 12);
    osc2Gui.addItem("Saw Pulse 0.3", 13);
    osc2Gui.setJustificationType(Justification::centred);
    osc2Gui.setText("Sine");
    osc2Gui.addListener(this);
    addAndMakeVisible(&osc2Gui);
    asLabel.setText("Atk", dontSendNotification); ////////////
    asLabel.setFont(font2);
    asLabel.setJustificationType(Justification::centred);
    asLabel.attachToComponent(&attackSlider, false);
    attackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    attackSlider.setRange(0.1f, 5000.0f);
    attackSlider.setValue(0.1f);
    attackSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    attackSlider.addListener(this);
    addAndMakeVisible(&attackSlider);
    dcLabel.setText("Dec", dontSendNotification); ////////////
    dcLabel.setFont(font2);
    dcLabel.setJustificationType(Justification::centred);
    dcLabel.attachToComponent(&decaySlider, false);
    decaySlider.setSliderStyle(Slider::SliderStyle::Rotary);
    decaySlider.setRange(0.1f, 2000.0f);
    decaySlider.setValue(500.f);
    decaySlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    decaySlider.addListener(this);
    addAndMakeVisible(&decaySlider);
    ssLabel.setText("Sus", dontSendNotification); ////////////
    ssLabel.setFont(font2);
    ssLabel.setJustificationType(Justification::centred);
    ssLabel.attachToComponent(&sustainSlider, false);
    sustainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    sustainSlider.setRange(0.0f, 1.0f);
    sustainSlider.setValue(0.8f);
    sustainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sustainSlider.addListener(this);
    addAndMakeVisible(&sustainSlider);
    relLabel.setText("Rel", dontSendNotification); ////////////
    relLabel.setFont(font2);
    relLabel.setJustificationType(Justification::centred);
    relLabel.attachToComponent(&releaseSlider, false);
    releaseSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    releaseSlider.setRange(0.1f, 5000.0f);
    releaseSlider.setValue(0.1f);
    releaseSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    releaseSlider.addListener(this);
    addAndMakeVisible(&releaseSlider);
    o2gLabel.setText("Gain", dontSendNotification); ////////////
    o2gLabel.setFont(font2);
    o2gLabel.setJustificationType(Justification::centred);
    o2gLabel.attachToComponent(&osc2GainSlider, false);
    osc2GainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    osc2GainSlider.setRange(0.0f, 1.0f);
    osc2GainSlider.setValue(0.5f);
    osc2GainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    osc2GainSlider.addListener(this);
    addAndMakeVisible(&osc2GainSlider);
    nzLabel.setText("Noise", dontSendNotification); ////////////
    nzLabel.setFont(font2);
    nzLabel.setJustificationType(Justification::centred);
    nzLabel.attachToComponent(&noiseGainSlider, true);
    noiseGainSlider.setRange(0.0f, 1.0f);
    noiseGainSlider.setValue(0.0f);
    noiseGainSlider.setTextBoxStyle(Slider::TextBoxRight, true, 50, 15);
    noiseGainSlider.addListener(this);
    addAndMakeVisible(&noiseGainSlider);
    o2pLabel.setText("Pitch", dontSendNotification); ////////////
    o2pLabel.setFont(font2);
    o2pLabel.setJustificationType(Justification::centred);
    o2pLabel.attachToComponent(&osc2PitchSlider, false);
    osc2PitchSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    osc2PitchSlider.setRange(0.5f, 2.0f);
    osc2PitchSlider.setValue(1.0f);
    osc2PitchSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    osc2PitchSlider.addListener(this);
    addAndMakeVisible(&osc2PitchSlider);
    fcLabel.setText("Cutoff", dontSendNotification); ////////////
    fcLabel.setFont(font2);
    fcLabel.setJustificationType(Justification::centred);
    fcLabel.attachToComponent(&filterCutSlider, false);
    filterCutSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    filterCutSlider.setRange(50.f, 9000.0f);
    filterCutSlider.setValue(500.f);
    filterCutSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    filterCutSlider.addListener(this);
    addAndMakeVisible(&filterCutSlider);
    frLabel.setText("Resonance", dontSendNotification); ////////////
    frLabel.setFont(font2);
    frLabel.setJustificationType(Justification::centred);
    frLabel.attachToComponent(&filterResSlider, false);
    filterResSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    filterResSlider.setRange(0.01f, 0.99f);
    filterResSlider.setValue(0.1f);
    filterResSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    filterResSlider.addListener(this);
    addAndMakeVisible(&filterResSlider);
    lfiLabel.setText("LFO Intensity", dontSendNotification); ////////////
    lfiLabel.setFont(font2);
    lfiLabel.setJustificationType(Justification::centred);
    lfiLabel.attachToComponent(&lfoFilterIntenSlider, false);
    lfoFilterIntenSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    lfoFilterIntenSlider.setRange(0.0f, 0.9f);
    lfoFilterIntenSlider.setValue(0.0f);
    lfoFilterIntenSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    lfoFilterIntenSlider.addListener(this);
    addAndMakeVisible(&lfoFilterIntenSlider);
    lfrLabel.setText("LFO Rate", dontSendNotification); ////////////
    lfrLabel.setFont(font2);
    lfrLabel.setJustificationType(Justification::centred);
    lfrLabel.attachToComponent(&lfoFilterRateSlider, false);
    lfoFilterRateSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    lfoFilterRateSlider.setRange(0.5f, 12.0f);
    lfoFilterRateSlider.setValue(2.0f);
    lfoFilterRateSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    lfoFilterRateSlider.addListener(this);
    addAndMakeVisible(&lfoFilterRateSlider);
    /*lpiLabel.setText("lfo Pitch Gain", dontSendNotification); ////////////
    lpiLabel.setJustificationType(Justification::centred);
    lpiLabel.attachToComponent(&lfoPitchIntenSlider, false);
    lfoPitchIntenSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    lfoPitchIntenSlider.setRange(0.0f, 3.0f);
    lfoPitchIntenSlider.setValue(0.0f);
    lfoPitchIntenSlider.setNumDecimalPlacesToDisplay(1);
    lfoPitchIntenSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    lfoPitchIntenSlider.addListener(this);
    addAndMakeVisible(&lfoPitchIntenSlider);
    lprLabel.setText("lfo Pitch Rate", dontSendNotification); ////////////
    lprLabel.setJustificationType(Justification::centred);
    lprLabel.attachToComponent(&lfoPitchRateSlider, false);
    lfoPitchRateSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    lfoPitchRateSlider.setRange(0.05f, 4.0f);
    lfoPitchRateSlider.setValue(0.05f);
    lfoPitchRateSlider.setNumDecimalPlacesToDisplay(1);
    lfoPitchRateSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    lfoPitchRateSlider.addListener(this);
    addAndMakeVisible(&lfoPitchRateSlider);*/
    crLabel.setText("Ratio", dontSendNotification); ////////////
    crLabel.setFont(font2);
    crLabel.setJustificationType(Justification::centred);
    crLabel.attachToComponent(&compressRatioSlider, false);
    compressRatioSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    compressRatioSlider.setRange(1.0f, 10.0f);
    compressRatioSlider.setValue(2.0f);
    compressRatioSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    compressRatioSlider.addListener(this);
    addAndMakeVisible(&compressRatioSlider);
    ctLabel.setText("Threshold", dontSendNotification); ////////////
    ctLabel.setFont(font2);
    ctLabel.setJustificationType(Justification::centred);
    ctLabel.attachToComponent(&compressThreshSlider, false);
    compressThreshSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    compressThreshSlider.setRange(0.0f, 1.0f);
    compressThreshSlider.setValue(0.9f);
    compressThreshSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    compressThreshSlider.addListener(this);
    addAndMakeVisible(&compressThreshSlider);
    caLabel.setText("Attack", dontSendNotification); ////////////
    caLabel.setFont(font2);
    caLabel.setJustificationType(Justification::centred);
    caLabel.attachToComponent(&compressAtkSlider, false);
    compressAtkSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    compressAtkSlider.setRange(0.5f, 100.0f);
    compressAtkSlider.setValue(1.0f);
    compressAtkSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    compressAtkSlider.addListener(this);
    addAndMakeVisible(&compressAtkSlider);
    clLabel.setText("Release", dontSendNotification); ////////////
    clLabel.setFont(font2);
    clLabel.setJustificationType(Justification::centred);
    clLabel.attachToComponent(&compressRelSlider, false);
    compressRelSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    compressRelSlider.setRange(0.1f, 2.0f);
    compressRelSlider.setValue(0.995f);
    compressRelSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    compressRelSlider.addListener(this);
    addAndMakeVisible(&compressRelSlider);
    cgLabel.setText("Comp Gain", dontSendNotification); ////////////
    cgLabel.setFont(font2);
    cgLabel.setJustificationType(Justification::centred);
    cgLabel.attachToComponent(&compressGainSlider, false);
    compressGainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    compressGainSlider.setRange(0.0f, 1.0f);
    compressGainSlider.setValue(0.8f);
    compressGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    compressGainSlider.addListener(this);
    addAndMakeVisible(&compressGainSlider);
    ddLabel.setText("Drive", dontSendNotification); ////////////
    ddLabel.setFont(font2);
    ddLabel.setJustificationType(Justification::centred);
    ddLabel.attachToComponent(&distortionDriveSlider, false);
    distortionDriveSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    distortionDriveSlider.setRange(0.01f, 6.0f);
    distortionDriveSlider.setValue(1.0f);
    distortionDriveSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    distortionDriveSlider.addListener(this);
    addAndMakeVisible(&distortionDriveSlider);
    dLabel.setText("Distortion", dontSendNotification); ////////////
    dLabel.setFont(font2);
    dLabel.setJustificationType(Justification::centred);
    dLabel.attachToComponent(&distGui, false);
    distGui.addItem("None", 1);
    distGui.addItem("Sigmoid", 2);
    distGui.addItem("Tanh", 3);
    distGui.addItem("ArcTan10", 4);
    distGui.addItem("FastArcTan10", 5);
    distGui.setJustificationType(Justification::centred);
    distGui.setText("None");
    addAndMakeVisible(&distGui);
    distGui.addListener(this);
    dtLabel.setText("Time", dontSendNotification); ////////////
    dtLabel.setFont(font2);
    dtLabel.setJustificationType(Justification::centred);
    dtLabel.attachToComponent(&delayTimeSlider, false);
    delayTimeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    delayTimeSlider.setRange(100.0f, 88000.0f);
    delayTimeSlider.setValue(10000.0f);
    delayTimeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    delayTimeSlider.addListener(this);
    addAndMakeVisible(&delayTimeSlider);
    dfLabel.setText("Feedback", dontSendNotification); ////////////
    dfLabel.setFont(font2);
    dfLabel.setJustificationType(Justification::centred);
    dfLabel.attachToComponent(&delayFdbkSlider, false);
    delayFdbkSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    delayFdbkSlider.setRange(0.0f, 0.9f);
    delayFdbkSlider.setValue(0.5f);
    delayFdbkSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    delayFdbkSlider.addListener(this);
    addAndMakeVisible(&delayFdbkSlider);
    dgLabel.setText("Delay Gain", dontSendNotification); ////////////
    dgLabel.setFont(font2);
    dgLabel.setJustificationType(Justification::centred);
    dgLabel.attachToComponent(&delayGainSlider, false);
    delayGainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    delayGainSlider.setRange(0.0f, 1.0f);
    delayGainSlider.setValue(1.0f);
    delayGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    delayGainSlider.addListener(this);
    addAndMakeVisible(&delayGainSlider);
    tgLabel.setText("Total Gain", dontSendNotification); ////////////
    tgLabel.setFont(font2);
    tgLabel.setJustificationType(Justification::centred);
    tgLabel.attachToComponent(&totalGainSlider, false);
    totalGainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    totalGainSlider.setRange(0.0f, 1.0f);
    totalGainSlider.setValue(0.1f);
    totalGainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    totalGainSlider.addListener(this);
    addAndMakeVisible(&totalGainSlider);
    addAndMakeVisible(&scopeComponent);
    //addAndMakeVisible(&keyboardComponent);

    //attaching all sliders and combo boxes to keys in the processors value tree
    osc1GuiAttach.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(
      processor.tree, "wavetype", osc1Gui));
    osc2GuiAttach.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(
      processor.tree, "wavetype2", osc2Gui));
    distGuiAttach.reset(new AudioProcessorValueTreeState::ComboBoxAttachment(
      processor.tree, "distortionType", distGui));
    attackSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "attack", attackSlider));
    decaySliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "decay", decaySlider));
    sustainSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "sustain", sustainSlider));
    releaseSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "release", releaseSlider));
    noiseGainSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "noiseGain", noiseGainSlider));
    osc2GainSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "osc2Gain", osc2GainSlider));
    osc2PitchSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "osc2Pitch", osc2PitchSlider));
    filtCutSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "filterCutoff", filterCutSlider));
    filtResSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "filterResonance", filterResSlider));
    lfoFilterIntenSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "lfoFilterIntensity", lfoFilterIntenSlider));
    lfoFilterRateSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "lfoFilterRate", lfoFilterRateSlider));
    //lfoPitchIntenSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      //processor.tree, "lfoPitchIntensity", lfoPitchIntenSlider));
    //lfoPitchRateSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      //processor.tree, "lfoPitchRate", lfoPitchRateSlider)); */
    compressRatioSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "compressionRatio", compressRatioSlider));
    compressThreshSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "compressionThreshold", compressThreshSlider));
    compressAtkSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "compressionAttack", compressAtkSlider));
    compressRelSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "compressionRelease", compressRelSlider));
    compressGainSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "compressionGain", compressGainSlider));
    distortionDriveSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "distortionDrive", distortionDriveSlider));
    delayTimeSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "delayTime", delayTimeSlider));
    delayFdbkSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "delayFeedback", delayFdbkSlider));
    delayGainSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "delayGain", delayGainSlider));
    totalGainSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "totalGain", totalGainSlider));

    //we have to override this function AFTER the slider has an attachment
    attackSlider.textFromValueFunction = nullptr;
    decaySlider.textFromValueFunction = nullptr;
    sustainSlider.textFromValueFunction = nullptr;
    releaseSlider.textFromValueFunction = nullptr;
    osc2PitchSlider.textFromValueFunction = nullptr;
    noiseGainSlider.textFromValueFunction = nullptr;
    filterCutSlider.textFromValueFunction = nullptr;
    osc2GainSlider.textFromValueFunction = nullptr;
    filterResSlider.textFromValueFunction = nullptr;
    lfoFilterIntenSlider.textFromValueFunction = nullptr;
    lfoFilterRateSlider.textFromValueFunction = nullptr;
    /*lfoPitchIntenSlider, lfoPitchRateSlider,*/
    compressRatioSlider.textFromValueFunction = nullptr;
    compressThreshSlider.textFromValueFunction = nullptr;
    compressAtkSlider.textFromValueFunction = nullptr;
    compressRelSlider.textFromValueFunction = nullptr;
    distortionDriveSlider.textFromValueFunction = nullptr;
    delayTimeSlider.textFromValueFunction = nullptr;
    delayFdbkSlider.textFromValueFunction = nullptr;
    delayGainSlider.textFromValueFunction = nullptr;
    compressGainSlider.textFromValueFunction = nullptr;
    totalGainSlider.textFromValueFunction = nullptr;
    
    attackSlider.setNumDecimalPlacesToDisplay(0);
    decaySlider.setNumDecimalPlacesToDisplay(0);
    sustainSlider.setNumDecimalPlacesToDisplay(2);
    releaseSlider.setNumDecimalPlacesToDisplay(0);
    osc2GainSlider.setNumDecimalPlacesToDisplay(3);
    noiseGainSlider.setNumDecimalPlacesToDisplay(3);
    osc2PitchSlider.setNumDecimalPlacesToDisplay(3);
    filterCutSlider.setNumDecimalPlacesToDisplay(0);
    filterResSlider.setNumDecimalPlacesToDisplay(2);
    lfoFilterIntenSlider.setNumDecimalPlacesToDisplay(2);
    lfoFilterRateSlider.setNumDecimalPlacesToDisplay(2);
    compressRatioSlider.setNumDecimalPlacesToDisplay(1);
    compressThreshSlider.setNumDecimalPlacesToDisplay(2);
    compressAtkSlider.setNumDecimalPlacesToDisplay(1);
    compressRelSlider.setNumDecimalPlacesToDisplay(1);
    compressGainSlider.setNumDecimalPlacesToDisplay(2);
    distortionDriveSlider.setNumDecimalPlacesToDisplay(2);
    delayTimeSlider.setNumDecimalPlacesToDisplay(0);
    delayFdbkSlider.setNumDecimalPlacesToDisplay(2);
    delayGainSlider.setNumDecimalPlacesToDisplay(2);
    totalGainSlider.setNumDecimalPlacesToDisplay(2);
    
    juce::File backgroundFile = juce::File::getCurrentWorkingDirectory().getChildFile("../../Images/bgfile4.jpg");
    image = j->loadFrom(backgroundFile); //loads a jpg background :) thx JUCE

}

//release resources 
SynthFrameworkAudioProcessorEditor::~SynthFrameworkAudioProcessorEditor() {
  setLookAndFeel(nullptr);
  osc1Gui.removeListener(this);
  osc2Gui.removeListener(this);
  attackSlider.removeListener(this);
  decaySlider.removeListener(this);
  sustainSlider.removeListener(this);
  releaseSlider.removeListener(this);
  osc2PitchSlider.removeListener(this);
  noiseGainSlider.removeListener(this);
  filterCutSlider.removeListener(this);
  osc2GainSlider.removeListener(this);
  filterResSlider.removeListener(this);
  lfoFilterIntenSlider.removeListener(this);
  lfoFilterRateSlider.removeListener(this);
  //lfoPitchIntenSlider.removeListener(this);
  //lfoPitchRateSlider.removeListener(this);
  compressRatioSlider.removeListener(this);
  compressThreshSlider.removeListener(this);
  compressAtkSlider.removeListener(this);
  compressRelSlider.removeListener(this);
  delayTimeSlider.removeListener(this);
  delayFdbkSlider.removeListener(this);
  delayGainSlider.removeListener(this);
  compressGainSlider.removeListener(this);
  totalGainSlider.removeListener(this);
  
}

//draws the background image, or if the image is missing, a black background
void SynthFrameworkAudioProcessorEditor::paint (Graphics& g) {
  // Our component is opaque, so we must completely fill the background with a solid colour
  juce::Colour c(0, 0, 0);
  g.fillAll(c);
  short height = scale * 800, width = scale * 600;
  g.drawImage(image, 0, 0, height, width, 200, 0, 1600, 1200); //these numbers are specific to this particular image.
}

//here we actually place the widgets on the screen, each placement and distance based on scale set by device
void SynthFrameworkAudioProcessorEditor::resized() {
  osc1Gui.setBounds(10*scale, 30*scale, 120*scale, 30*scale);
  osc2Gui.setBounds(10*scale, 85*scale, 120*scale, 30*scale);
  osc2GainSlider.setBounds(10*scale, 130*scale, 50*scale, 50*scale);
  osc2PitchSlider.setBounds(80*scale, 130*scale, 50*scale, 50*scale);
  noiseGainSlider.setBounds(40*scale, 180*scale, 100*scale, 30*scale);
  attackSlider.setBounds(130*scale, 40*scale, 60*scale, 60*scale);
  decaySlider.setBounds(200*scale, 40*scale, 60*scale, 60*scale);
  sustainSlider.setBounds(270*scale, 40*scale, 60*scale, 60*scale);
  releaseSlider.setBounds(340*scale, 40*scale, 60*scale, 60*scale);
  filterCutSlider.setBounds(410*scale, 45*scale, 70*scale, 70*scale);
  filterResSlider.setBounds(490*scale, 45*scale, 70*scale, 70*scale);
  lfoFilterIntenSlider.setBounds(570*scale, 40*scale, 60*scale, 60*scale);
  lfoFilterRateSlider.setBounds(640*scale, 40*scale, 60*scale, 60*scale);
  //lfoPitchIntenSlider.setBounds(580*scale, 75*scale, 50*scale, 50*scale);
  //lfoPitchRateSlider.setBounds(640*scale, 75*scale, 50*scale, 50*scale);
  totalGainSlider.setBounds(710*scale, 35*scale, 80*scale, 80*scale);
  compressRatioSlider.setBounds(10*scale, 220*scale, 60*scale, 60*scale);
  compressThreshSlider.setBounds(80*scale, 220*scale, 60*scale, 60*scale);
  compressAtkSlider.setBounds(10*scale, 300*scale, 60*scale, 60*scale);
  compressRelSlider.setBounds(80*scale, 300*scale, 60*scale, 60*scale);
  delayTimeSlider.setBounds(10*scale, 380*scale, 60*scale, 60*scale);
  delayFdbkSlider.setBounds(80*scale, 380*scale, 60*scale, 60*scale);
  compressGainSlider.setBounds(10*scale, 460*scale, 60*scale, 60*scale);
  delayGainSlider.setBounds(80*scale, 460*scale, 60*scale, 60*scale);
  distGui.setBounds(10*scale, 550*scale, 60*scale, 30*scale);
  distortionDriveSlider.setBounds(80*scale, 540*scale, 50*scale, 50*scale);
  scopeComponent.setBounds(150*scale, 120*scale, 630*scale, 450*scale);
}

//any time a slider value is changed mutate the value stored within the processor's value tree
void SynthFrameworkAudioProcessorEditor::sliderValueChanged(Slider* slider) {
  if (slider == &osc2GainSlider) processor.setOsc2Gain(osc2GainSlider.getValue());
  if (slider == &noiseGainSlider) processor.setNoiseGain(noiseGainSlider.getValue());
  if (slider == &osc2PitchSlider) processor.setOsc2Pitch(osc2PitchSlider.getValue());
  if (slider == &attackSlider) processor.setAttackTime(attackSlider.getValue());
  if (slider == &decaySlider) processor.setDecayTime(decaySlider.getValue());
  if (slider == &sustainSlider) processor.setSustainTime(sustainSlider.getValue());
  if (slider == &releaseSlider) processor.setReleaseTime(attackSlider.getValue());
  if (slider == &filterCutSlider) processor.setFilterCutoff(filterCutSlider.getValue());
  if (slider == &filterResSlider) processor.setFilterResonance(filterResSlider.getValue());
  if (slider == &lfoFilterIntenSlider) processor.setLfoFilterIntensity(lfoFilterIntenSlider.getValue());
  if (slider == &lfoFilterRateSlider) processor.setLfoFilterRate(lfoFilterRateSlider.getValue());
  //if (slider == &lfoPitchIntenSlider) processor.setLfoPitchIntensity(lfoPitchIntenSlider.getValue());
  //if (slider == &lfoPitchRateSlider) processor.setLfoPitchRate(lfoPitchRateSlider.getValue());
  if (slider == &compressRatioSlider) processor.setCompressionRatio(compressRatioSlider.getValue());
  if (slider == &compressThreshSlider) processor.setCompressionThreshold(compressThreshSlider.getValue());
  if (slider == &compressAtkSlider) processor.setCompressionAttack(compressAtkSlider.getValue());
  if (slider == &compressRelSlider) processor.setCompressionRelease(compressRelSlider.getValue());
  if (slider == &compressGainSlider) processor.setCompressionGain(compressGainSlider.getValue());
  if (slider == &distortionDriveSlider) processor.setDistortionDrive(distortionDriveSlider.getValue());
  if (slider == &delayTimeSlider) processor.setDelayTime(delayTimeSlider.getValue());
  if (slider == &delayFdbkSlider) processor.setDelayFeedback(delayFdbkSlider.getValue());
  if (slider == &delayGainSlider) processor.setDelayGain(delayGainSlider.getValue());
  if (slider == &totalGainSlider) processor.setTotalGain(totalGainSlider.getValue());
}


//this mutates the integer value stored within the processors oscillator type enum
void SynthFrameworkAudioProcessorEditor::comboBoxChanged(ComboBox* comboBox) {
  if (comboBox == &osc1Gui) processor.setOsc1Type(osc1Gui.getItemText(osc1Gui.getSelectedItemIndex()));
  if (comboBox == &osc2Gui) processor.setOsc2Type(osc2Gui.getItemText(osc2Gui.getSelectedItemIndex()));
  if (comboBox == &distGui) processor.setDistType(distGui.getItemText(distGui.getSelectedItemIndex()));
}
