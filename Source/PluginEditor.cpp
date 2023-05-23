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
:   AudioProcessorEditor (&p)
,   processor (p)
,   scopeComponent(processor.audioBufferQueue) 
{
    addAllGUIComponents();
}

void SynthFrameworkAudioProcessorEditor::addAllGUIComponents() 
{
    Rectangle<int> deviceScreen = Desktop::getInstance().getDisplays().getMainDisplay().userArea; //get screen size
    double deviceW = deviceScreen.getWidth() / 800., deviceH = deviceScreen.getHeight() / 600.; //800 and 600 is default scale
    scale = (deviceW > deviceH) ? deviceH : deviceW; //make sure window fits on the screen...
    scale *= 0.6f; //don't let scale dominate the DAW space.
    setSize (800*scale, 600*scale); //setting the actual size of the editor
    lookAndFeel.setScale(scale); //sets font sizes of the look and feel aspects based on scale
    setLookAndFeel(&lookAndFeel);
    Font font1("Lucida Console", scale*10.0f, Font::bold);
    Font font2("Lucida Console", scale*9.0f, Font::bold);
    labels[21].setText("Osc 1", dontSendNotification); ////////////
    labels[21].setFont(font1);
    labels[21].attachToComponent(&comboboxes[0], false);
    comboboxes[0].addItem("Sine", 1);
    comboboxes[0].addItem("Saw", 2);
    comboboxes[0].addItem("Square", 3);
    comboboxes[0].addItem("Soft Distorted Sine", 4);
    comboboxes[0].addItem("Hard Distorted Sine", 5);
    comboboxes[0].addItem("Triangle", 6);
    comboboxes[0].addItem("Noise", 7);
    comboboxes[0].addItem("Sharp Saw", 8);
    comboboxes[0].addItem("Thick Saw", 9);
    comboboxes[0].addItem("Pulse 0.2", 10);
    comboboxes[0].addItem("Pulse 0.3", 11);
    comboboxes[0].addItem("Saw Pulse 0.2", 12);
    comboboxes[0].addItem("Saw Pulse 0.3", 13);
    comboboxes[0].setJustificationType(Justification::centred);
    comboboxes[0].setText("Saw");
    addAndMakeVisible(&comboboxes[0]);
    comboboxes[0].addListener(this);
    labels[22].setText("Osc 2", dontSendNotification); ////////////
    labels[22].setFont(font1);
    labels[22].attachToComponent(&comboboxes[1], false);
    comboboxes[1].addItem("Sine", 1);
    comboboxes[1].addItem("Saw", 2);
    comboboxes[1].addItem("Square", 3);
    comboboxes[1].addItem("Soft Distorted Sine", 4);
    comboboxes[1].addItem("Hard Distorted Sine", 5);
    comboboxes[1].addItem("Triangle", 6);
    comboboxes[1].addItem("Noise", 7);
    comboboxes[1].addItem("Sharp Saw", 8);
    comboboxes[1].addItem("Thick Saw", 9);
    comboboxes[1].addItem("Pulse 0.2", 10);
    comboboxes[1].addItem("Pulse 0.3", 11);
    comboboxes[1].addItem("Saw Pulse 0.2", 12);
    comboboxes[1].addItem("Saw Pulse 0.3", 13);
    comboboxes[1].setJustificationType(Justification::centred);
    comboboxes[1].setText("Sine");
    comboboxes[1].addListener(this);
    addAndMakeVisible(&comboboxes[1]);
    labels[0].setText("Atk", dontSendNotification); ////////////
    labels[0].setFont(font2);
    labels[0].setJustificationType(Justification::centred);
    labels[0].attachToComponent(&sliders[0], false);
    sliders[0].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[0].setRange(0.1f, 5000.0f);
    sliders[0].setValue(0.1f);
    sliders[0].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[0].addListener(this);
    addAndMakeVisible(&sliders[0]);
    labels[1].setText("Dec", dontSendNotification); ////////////
    labels[1].setFont(font2);
    labels[1].setJustificationType(Justification::centred);
    labels[1].attachToComponent(&sliders[1], false);
    sliders[1].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[1].setRange(0.1f, 2000.0f);
    sliders[1].setValue(500.f);
    sliders[1].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[1].addListener(this);
    addAndMakeVisible(&sliders[1]);
    labels[2].setText("Sus", dontSendNotification); ////////////
    labels[2].setFont(font2);
    labels[2].setJustificationType(Justification::centred);
    labels[2].attachToComponent(&sliders[2], false);
    sliders[2].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[2].setRange(0.0f, 1.0f);
    sliders[2].setValue(0.8f);
    sliders[2].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[2].addListener(this);
    addAndMakeVisible(&sliders[2]);
    labels[3].setText("Rel", dontSendNotification); ////////////
    labels[3].setFont(font2);
    labels[3].setJustificationType(Justification::centred);
    labels[3].attachToComponent(&sliders[3], false);
    sliders[3].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[3].setRange(0.1f, 5000.0f);
    sliders[3].setValue(0.1f);
    sliders[3].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[3].addListener(this);
    addAndMakeVisible(&sliders[3]);
    labels[5].setText("Gain", dontSendNotification); ////////////
    labels[5].setFont(font2);
    labels[5].setJustificationType(Justification::centred);
    labels[5].attachToComponent(&sliders[5], false);
    sliders[5].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[5].setRange(0.0f, 1.0f);
    sliders[5].setValue(0.5f);
    sliders[5].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[5].addListener(this);
    addAndMakeVisible(&sliders[5]);
    labels[6].setText("Noise", dontSendNotification); ////////////
    labels[6].setFont(font2);
    labels[6].setJustificationType(Justification::centred);
    labels[6].attachToComponent(&sliders[6], true);
    sliders[6].setRange(0.0f, 1.0f);
    sliders[6].setValue(0.0f);
    sliders[6].setTextBoxStyle(Slider::TextBoxRight, true, 50, 15);
    sliders[6].addListener(this);
    addAndMakeVisible(&sliders[6]);
    labels[4].setText("Pitch", dontSendNotification); ////////////
    labels[4].setFont(font2);
    labels[4].setJustificationType(Justification::centred);
    labels[4].attachToComponent(&sliders[4], false);
    sliders[4].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[4].setRange(0.5f, 2.0f);
    sliders[4].setValue(1.0f);
    sliders[4].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[4].addListener(this);
    addAndMakeVisible(&sliders[4]);
    labels[7].setText("Cutoff", dontSendNotification); ////////////
    labels[7].setFont(font2);
    labels[7].setJustificationType(Justification::centred);
    labels[7].attachToComponent(&sliders[7], false);
    sliders[7].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[7].setRange(50.f, 9000.0f);
    sliders[7].setValue(500.f);
    sliders[7].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[7].addListener(this);
    addAndMakeVisible(&sliders[7]);
    labels[8].setText("Resonance", dontSendNotification); ////////////
    labels[8].setFont(font2);
    labels[8].setJustificationType(Justification::centred);
    labels[8].attachToComponent(&sliders[8], false);
    sliders[8].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[8].setRange(0.01f, 0.99f);
    sliders[8].setValue(0.1f);
    sliders[8].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[8].addListener(this);
    addAndMakeVisible(&sliders[8]);
    labels[9].setText("LFO Intensity", dontSendNotification); ////////////
    labels[9].setFont(font2);
    labels[9].setJustificationType(Justification::centred);
    labels[9].attachToComponent(&sliders[9], false);
    sliders[9].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[9].setRange(0.0f, 0.9f);
    sliders[9].setValue(0.0f);
    sliders[9].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[9].addListener(this);
    addAndMakeVisible(&sliders[9]);
    labels[10].setText("LFO Rate", dontSendNotification); ////////////
    labels[10].setFont(font2);
    labels[10].setJustificationType(Justification::centred);
    labels[10].attachToComponent(&sliders[10], false);
    sliders[10].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[10].setRange(0.5f, 12.0f);
    sliders[10].setValue(2.0f);
    sliders[10].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[10].addListener(this);
    addAndMakeVisible(&sliders[10]);
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
    labels[11].setText("Ratio", dontSendNotification); ////////////
    labels[11].setFont(font2);
    labels[11].setJustificationType(Justification::centred);
    labels[11].attachToComponent(&sliders[11], false);
    sliders[11].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[11].setRange(1.0f, 10.0f);
    sliders[11].setValue(2.0f);
    sliders[11].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[11].addListener(this);
    addAndMakeVisible(&sliders[11]);
    labels[12].setText("Threshold", dontSendNotification); ////////////
    labels[12].setFont(font2);
    labels[12].setJustificationType(Justification::centred);
    labels[12].attachToComponent(&sliders[12], false);
    sliders[12].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[12].setRange(0.0f, 1.0f);
    sliders[12].setValue(0.9f);
    sliders[12].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[12].addListener(this);
    addAndMakeVisible(&sliders[12]);
    labels[13].setText("Attack", dontSendNotification); ////////////
    labels[13].setFont(font2);
    labels[13].setJustificationType(Justification::centred);
    labels[13].attachToComponent(&sliders[13], false);
    sliders[13].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[13].setRange(0.5f, 100.0f);
    sliders[13].setValue(1.0f);
    sliders[13].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[13].addListener(this);
    addAndMakeVisible(&sliders[13]);
    labels[14].setText("Release", dontSendNotification); ////////////
    labels[14].setFont(font2);
    labels[14].setJustificationType(Justification::centred);
    labels[14].attachToComponent(&sliders[14], false);
    sliders[14].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[14].setRange(0.1f, 2.0f);
    sliders[14].setValue(0.995f);
    sliders[14].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[14].addListener(this);
    addAndMakeVisible(&sliders[14]);
    labels[15].setText("Comp Gain", dontSendNotification); ////////////
    labels[15].setFont(font2);
    labels[15].setJustificationType(Justification::centred);
    labels[15].attachToComponent(&sliders[15], false);
    sliders[15].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[15].setRange(0.0f, 1.0f);
    sliders[15].setValue(0.8f);
    sliders[15].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[15].addListener(this);
    addAndMakeVisible(&sliders[15]);
    labels[16].setText("Drive", dontSendNotification); ////////////
    labels[16].setFont(font2);
    labels[16].setJustificationType(Justification::centred);
    labels[16].attachToComponent(&sliders[16], false);
    sliders[16].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[16].setRange(0.01f, 6.0f);
    sliders[16].setValue(1.0f);
    sliders[16].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[16].addListener(this);
    addAndMakeVisible(&sliders[16]);
    labels[23].setText("Distortion", dontSendNotification); ////////////
    labels[23].setFont(font2);
    labels[23].setJustificationType(Justification::centred);
    labels[23].attachToComponent(&comboboxes[2], false);
    comboboxes[2].addItem("None", 1);
    comboboxes[2].addItem("Sigmoid", 2);
    comboboxes[2].addItem("Tanh", 3);
    comboboxes[2].addItem("ArcTan10", 4);
    comboboxes[2].addItem("FastArcTan10", 5);
    comboboxes[2].setJustificationType(Justification::centred);
    comboboxes[2].setText("None");
    addAndMakeVisible(&comboboxes[2]);
    comboboxes[2].addListener(this);
    labels[17].setText("Time", dontSendNotification); ////////////
    labels[17].setFont(font2);
    labels[17].setJustificationType(Justification::centred);
    labels[17].attachToComponent(&sliders[17], false);
    sliders[17].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[17].setRange(100.0f, 88000.0f);
    sliders[17].setValue(10000.0f);
    sliders[17].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[17].addListener(this);
    addAndMakeVisible(&sliders[17]);
    labels[18].setText("Feedback", dontSendNotification); ////////////
    labels[18].setFont(font2);
    labels[18].setJustificationType(Justification::centred);
    labels[18].attachToComponent(&sliders[18], false);
    sliders[18].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[18].setRange(0.0f, 0.9f);
    sliders[18].setValue(0.5f);
    sliders[18].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[18].addListener(this);
    addAndMakeVisible(&sliders[18]);
    labels[19].setText("Delay Gain", dontSendNotification); ////////////
    labels[19].setFont(font2);
    labels[19].setJustificationType(Justification::centred);
    labels[19].attachToComponent(&sliders[19], false);
    sliders[19].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[19].setRange(0.0f, 1.0f);
    sliders[19].setValue(1.0f);
    sliders[19].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[19].addListener(this);
    addAndMakeVisible(&sliders[19]);
    labels[20].setText("Total Gain", dontSendNotification); ////////////
    labels[20].setFont(font2);
    labels[20].setJustificationType(Justification::centred);
    labels[20].attachToComponent(&sliders[20], false);
    sliders[20].setSliderStyle(Slider::SliderStyle::Rotary);
    sliders[20].setRange(0.0f, 1.0f);
    sliders[20].setValue(0.1f);
    sliders[20].setTextBoxStyle(Slider::TextBoxBelow, true, 50, 15);
    sliders[20].addListener(this);
    addAndMakeVisible(&sliders[20]);
    addAndMakeVisible(&scopeComponent);
    //addAndMakeVisible(&keyboardComponent);

    //attaching all sliders and combo boxes to keys in the processors value tree
    comboboxattachments[0].reset(new AudioProcessorValueTreeState::ComboBoxAttachment(
      processor.tree, "wavetype", comboboxes[0]));
    comboboxattachments[1].reset(new AudioProcessorValueTreeState::ComboBoxAttachment(
      processor.tree, "wavetype2", comboboxes[1]));
    comboboxattachments[2].reset(new AudioProcessorValueTreeState::ComboBoxAttachment(
      processor.tree, "distortionType", comboboxes[2]));
    sliderattachments[0].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "attack", sliders[0]));
    sliderattachments[1].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "decay", sliders[1]));
    sliderattachments[2].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "sustain", sliders[2]));
    sliderattachments[3].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "release", sliders[3]));
    sliderattachments[4].reset(new AudioProcessorValueTreeState::SliderAttachment(
        processor.tree, "osc2Pitch", sliders[4]));
    sliderattachments[5].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "noiseGain", sliders[6]));
    sliderattachments[6].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "osc2Gain", sliders[5]));
    sliderattachments[7].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "filterCutoff", sliders[7]));
    sliderattachments[8].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "filterResonance", sliders[8]));
    sliderattachments[9].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "lfoFilterIntensity", sliders[9]));
    sliderattachments[10].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "lfoFilterRate", sliders[10]));
    //lfoPitchIntenSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      //processor.tree, "lfoPitchIntensity", lfoPitchIntenSlider));
    //lfoPitchRateSliderAttach.reset(new AudioProcessorValueTreeState::SliderAttachment(
      //processor.tree, "lfoPitchRate", lfoPitchRateSlider)); */
    sliderattachments[11].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "compressionRatio", sliders[11]));
    sliderattachments[12].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "compressionThreshold", sliders[12]));
    sliderattachments[13].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "compressionAttack", sliders[13]));
    sliderattachments[14].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "compressionRelease", sliders[14]));
    sliderattachments[15].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "compressionGain", sliders[15]));
    sliderattachments[16].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "distortionDrive", sliders[16]));
    sliderattachments[17].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "delayTime", sliders[17]));
    sliderattachments[18].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "delayFeedback", sliders[18]));
    sliderattachments[19].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "delayGain", sliders[19]));
    sliderattachments[20].reset(new AudioProcessorValueTreeState::SliderAttachment(
      processor.tree, "totalGain", sliders[20]));

    //we have to override this function AFTER the slider has an attachment
    sliders[0].textFromValueFunction = nullptr;
    sliders[1].textFromValueFunction = nullptr;
    sliders[2].textFromValueFunction = nullptr;
    sliders[3].textFromValueFunction = nullptr;
    sliders[4].textFromValueFunction = nullptr;
    sliders[6].textFromValueFunction = nullptr;
    sliders[7].textFromValueFunction = nullptr;
    sliders[5].textFromValueFunction = nullptr;
    sliders[8].textFromValueFunction = nullptr;
    sliders[9].textFromValueFunction = nullptr;
    sliders[10].textFromValueFunction = nullptr;
    /*lfoPitchIntenSlider, lfoPitchRateSlider,*/
    sliders[11].textFromValueFunction = nullptr;
    sliders[12].textFromValueFunction = nullptr;
    sliders[13].textFromValueFunction = nullptr;
    sliders[14].textFromValueFunction = nullptr;
    sliders[16].textFromValueFunction = nullptr;
    sliders[17].textFromValueFunction = nullptr;
    sliders[18].textFromValueFunction = nullptr;
    sliders[19].textFromValueFunction = nullptr;
    sliders[15].textFromValueFunction = nullptr;
    sliders[20].textFromValueFunction = nullptr;
    
    sliders[0].setNumDecimalPlacesToDisplay(0);
    sliders[1].setNumDecimalPlacesToDisplay(0);
    sliders[2].setNumDecimalPlacesToDisplay(2);
    sliders[3].setNumDecimalPlacesToDisplay(0);
    sliders[5].setNumDecimalPlacesToDisplay(3);
    sliders[6].setNumDecimalPlacesToDisplay(3);
    sliders[4].setNumDecimalPlacesToDisplay(3);
    sliders[7].setNumDecimalPlacesToDisplay(0);
    sliders[8].setNumDecimalPlacesToDisplay(2);
    sliders[9].setNumDecimalPlacesToDisplay(2);
    sliders[10].setNumDecimalPlacesToDisplay(2);
    sliders[11].setNumDecimalPlacesToDisplay(1);
    sliders[12].setNumDecimalPlacesToDisplay(2);
    sliders[13].setNumDecimalPlacesToDisplay(1);
    sliders[14].setNumDecimalPlacesToDisplay(1);
    sliders[15].setNumDecimalPlacesToDisplay(2);
    sliders[16].setNumDecimalPlacesToDisplay(2);
    sliders[17].setNumDecimalPlacesToDisplay(0);
    sliders[18].setNumDecimalPlacesToDisplay(2);
    sliders[19].setNumDecimalPlacesToDisplay(2);
    sliders[20].setNumDecimalPlacesToDisplay(2);
    
    juce::File backgroundFile = juce::File::getCurrentWorkingDirectory().getChildFile("../../Images/bgfile4.jpg");
    image = j->loadFrom(backgroundFile); //loads a jpg background :) thx JUCE

}

//release resources 
SynthFrameworkAudioProcessorEditor::~SynthFrameworkAudioProcessorEditor() 
{
    setLookAndFeel(nullptr);
    comboboxes[0].removeListener(this);
    comboboxes[1].removeListener(this);
    comboboxes[2].removeListener(this);
    sliders[0].removeListener(this);
    sliders[1].removeListener(this);
    sliders[2].removeListener(this);
    sliders[3].removeListener(this);
    sliders[4].removeListener(this);
    sliders[6].removeListener(this);
    sliders[7].removeListener(this);
    sliders[5].removeListener(this);
    sliders[8].removeListener(this);
    sliders[9].removeListener(this);
    sliders[10].removeListener(this);
    //lfoPitchIntenSlider.removeListener(this);
    //lfoPitchRateSlider.removeListener(this);
    sliders[11].removeListener(this);
    sliders[12].removeListener(this);
    sliders[13].removeListener(this);
    sliders[14].removeListener(this);
    sliders[17].removeListener(this);
    sliders[18].removeListener(this);
    sliders[19].removeListener(this);
    sliders[15].removeListener(this);
    sliders[20].removeListener(this);
}

//draws the background image, or if the image is missing, a black background
void SynthFrameworkAudioProcessorEditor::paint (Graphics& g) 
{
    // Our component is opaque, so we must completely fill the background with a solid colour
    juce::Colour c(0, 0, 0);
    g.fillAll(c);
    short height = scale * 800, width = scale * 600;
    g.drawImage(image, 0, 0, height, width, 200, 0, 1600, 1200); //these numbers are specific to this particular image.
}

//here we actually place the widgets on the screen, each placement and distance based on scale set by device
void SynthFrameworkAudioProcessorEditor::resized() 
{
    comboboxes[0].setBounds(10*scale, 30*scale, 120*scale, 30*scale);
    comboboxes[1].setBounds(10*scale, 85*scale, 120*scale, 30*scale);
    sliders[5].setBounds(10*scale, 130*scale, 50*scale, 50*scale);
    sliders[4].setBounds(80*scale, 130*scale, 50*scale, 50*scale);
    sliders[6].setBounds(40*scale, 180*scale, 100*scale, 30*scale);
    sliders[0].setBounds(130*scale, 40*scale, 60*scale, 60*scale);
    sliders[1].setBounds(200*scale, 40*scale, 60*scale, 60*scale);
    sliders[2].setBounds(270*scale, 40*scale, 60*scale, 60*scale);
    sliders[3].setBounds(340*scale, 40*scale, 60*scale, 60*scale);
    sliders[7].setBounds(410*scale, 45*scale, 70*scale, 70*scale);
    sliders[8].setBounds(490*scale, 45*scale, 70*scale, 70*scale);
    sliders[9].setBounds(570*scale, 40*scale, 60*scale, 60*scale);
    sliders[10].setBounds(640*scale, 40*scale, 60*scale, 60*scale);
    //lfoPitchIntenSlider.setBounds(580*scale, 75*scale, 50*scale, 50*scale);
    //lfoPitchRateSlider.setBounds(640*scale, 75*scale, 50*scale, 50*scale);
    sliders[20].setBounds(710*scale, 35*scale, 80*scale, 80*scale);
    sliders[11].setBounds(10*scale, 220*scale, 60*scale, 60*scale);
    sliders[12].setBounds(80*scale, 220*scale, 60*scale, 60*scale);
    sliders[13].setBounds(10*scale, 300*scale, 60*scale, 60*scale);
    sliders[14].setBounds(80*scale, 300*scale, 60*scale, 60*scale);
    sliders[17].setBounds(10*scale, 380*scale, 60*scale, 60*scale);
    sliders[18].setBounds(80*scale, 380*scale, 60*scale, 60*scale);
    sliders[15].setBounds(10*scale, 460*scale, 60*scale, 60*scale);
    sliders[19].setBounds(80*scale, 460*scale, 60*scale, 60*scale);
    comboboxes[2].setBounds(10*scale, 550*scale, 60*scale, 30*scale);
    sliders[16].setBounds(80*scale, 540*scale, 50*scale, 50*scale);
    scopeComponent.setBounds(150*scale, 120*scale, 630*scale, 450*scale);
}

//any time a slider value is changed mutate the value stored within the processor's value tree
void SynthFrameworkAudioProcessorEditor::sliderValueChanged(Slider* slider) 
{
    //for (int i = 0; i < NUM_SLIDERS; ++i)
    //    if (slider == &sliders[i])
    //        processor.setNewParamValue(sliders[i].getValue(), i);
}

//this mutates the integer value stored within the processors oscillator type enum
void SynthFrameworkAudioProcessorEditor::comboBoxChanged(ComboBox* comboBox) 
{
    //for (int i = 0; i < NUM_COMBOBOXES; ++i)
    //    if (comboBox == &comboboxes[i])
    //        processor.setNewParamType(comboboxes[i].getItemText(comboboxes[i].getSelectedItemIndex()), i);
}
