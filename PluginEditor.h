/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
class newlook : public LookAndFeel_V4
{



	void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
	{
		auto radius = jmin(width / 2, height / 2) - 4.0f;
		auto centreX = x + width * 0.5f;
		auto centreY = y + height * 0.5f;
		auto rx = centreX - radius;
		auto ry = centreY - radius;
		auto rw = radius * 2.0f;
		auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		// fill
		g.setColour(Colours::black);
		g.fillEllipse(rx, ry, rw, rw);
		// outline
		g.setColour(Colours::white);
		g.drawEllipse(rx, ry, rw, rw, 1.0f);
		Path p;
		auto pointerLength = radius * 0.33f;
		auto pointerThickness = 2.0f;
		p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
		p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
		// pointer
		g.setColour(Colours::white);
		g.fillPath(p);
	}

};
//==============================================================================
/**
*/
class DelayAudioProcessorEditor : public AudioProcessorEditor
{
public:
	DelayAudioProcessorEditor(DelayAudioProcessor&);
	~DelayAudioProcessorEditor();

	//==============================================================================
	void paint(Graphics&) override;
	void resized() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	DelayAudioProcessor& processor;
	Slider drywetslid;
	Slider delayslid;
	Slider feedbackslid;
	newlook newlook;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DelayAudioProcessorEditor)
};
