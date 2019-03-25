/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor(DelayAudioProcessor& p)
	: AudioProcessorEditor(&p), processor(p)
{
	// Make sure that before the constructor has finished, you've set the
	// editor's size to whatever you need it to be.
	setSize(400, 300);

	auto& params = processor.getParameters();


	AudioParameterFloat* delay = (AudioParameterFloat*)params.getUnchecked(2);
	delayslid.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	delayslid.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	delayslid.setRange(delay->range.start, delay->range.end);
	delayslid.setValue(*delay);
	addAndMakeVisible(delayslid);
	delayslid.onValueChange = [this, delay] {*delay = delayslid.getValue();};
	delayslid.onDragStart = [delay] {delay->beginChangeGesture();};
	delayslid.onDragEnd = [delay] {delay->endChangeGesture();};
	delayslid.setLookAndFeel(&newlook);


	AudioParameterFloat* feed = (AudioParameterFloat*)params.getUnchecked(1);
	feedbackslid.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	feedbackslid.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	//feedbackslid.setBounds(100, 0, 100, 100);
	feedbackslid.setRange(feed->range.start, feed->range.end);
	feedbackslid.setValue(*feed);
	addAndMakeVisible(feedbackslid);
	feedbackslid.onValueChange = [this, feed] {*feed = feedbackslid.getValue();};
	feedbackslid.onDragStart = [feed] {feed->beginChangeGesture();};
	feedbackslid.onDragEnd = [feed] {feed->endChangeGesture();};
	feedbackslid.setLookAndFeel(&newlook);
	AudioParameterFloat* drywet = (AudioParameterFloat*)params.getUnchecked(0);

	//drywetslid.setBounds(200, 0, 100, 100);
	drywetslid.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
	drywetslid.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 20);
	drywetslid.setRange(drywet->range.start, drywet->range.end);
	drywetslid.setValue(*drywet);
	addAndMakeVisible(drywetslid);
	drywetslid.onValueChange = [this, drywet] {*drywet = drywetslid.getValue();};
	drywetslid.onDragStart = [drywet] {drywet->beginChangeGesture();};
	drywetslid.onDragEnd = [drywet] {drywet->endChangeGesture();};
	drywetslid.setLookAndFeel(&newlook);
}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
}

//==============================================================================
void DelayAudioProcessorEditor::paint(Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(Colours::darkgreen);

	g.setColour(Colours::silver);
	g.setFont(15.0f);
	g.drawFittedText("Delay 2.0 Albert Peyton 2019", getLocalBounds(), Justification::top, 1);
	Rectangle<int> box(getX(), getBottom() - 40, getWidth() / 3, 40);
	g.drawFittedText(TRANS("Time(ms)"), box, Justification::centred, 1);
	box.setX(box.getRight());
	g.drawFittedText(TRANS("Feedback"), box, Justification::centred, 1);
	box.setX(box.getRight());
	g.drawFittedText(TRANS("Dry/Wet"), box, Justification::centred, 1);
}

void DelayAudioProcessorEditor::resized()
{
	auto box = getLocalBounds();
	box.setWidth(getWidth() / 3);
	box.setHeight(getHeight() - 40);
	delayslid.setBounds(box);
	box.setX(box.getRight());
	feedbackslid.setBounds(box);
	box.setX(box.getRight());
	drywetslid.setBounds(box);
}
