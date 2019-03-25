/*
  ==============================================================================

	This file was auto-generated!

	It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessor::DelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	)
#endif
{
	addParameter(drywet = new AudioParameterFloat("drywet", "dry wet", 0, 1.0, .5));
	addParameter(feedback = new AudioParameterFloat("feedback", "feedback", 0, .98, .5));
	addParameter(delay = new AudioParameterFloat("drywet", "dry wet", 0, 2000, 1000));
	mWritePosition = 0;
	smooth = 0;
}

DelayAudioProcessor::~DelayAudioProcessor()
{

}

//==============================================================================
const String DelayAudioProcessor::getName() const
{
	return JucePlugin_Name;
}

bool DelayAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool DelayAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool DelayAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double DelayAudioProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

float linit(float x, float x1, float phase) {
	return (1 - phase)*x + phase * x1;
}

int DelayAudioProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
				// so this should be at least 1, even if you're not really implementing programs.
}

int DelayAudioProcessor::getCurrentProgram()
{
	return 0;
}

void DelayAudioProcessor::setCurrentProgram(int index)
{
}

const String DelayAudioProcessor::getProgramName(int index)
{
	return {};
}

void DelayAudioProcessor::changeProgramName(int index, const String& newName)
{
}

//==============================================================================
void DelayAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	float fs = sampleRate;
	DelaybufferLength = (int)fs * 2;
	mWritePosition = 0;
	mReadPosition = 0;
	mDelayBufferLeft.setSize(1, DelaybufferLength, false, true);
	mDelayBufferRight.setSize(1, DelaybufferLength, false, true);
	feedbackleft = 0;
	feedbackright = 0;
	smooth = *delay;

}

void DelayAudioProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TestAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void DelayAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();


	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());
	const int bufferlength = buffer.getNumSamples();
	float* leftChannel = buffer.getWritePointer(0);
	float* rightChannel = buffer.getWritePointer(1);

	for (int sample = 0;sample < bufferlength; ++sample)
	{
		smooth = smooth - .001*(smooth - *delay);
		mDelayBufferLeft.setSample(0, mWritePosition, leftChannel[sample] + feedbackleft);
		mDelayBufferRight.setSample(0, mWritePosition, rightChannel[sample] + feedbackright);
		mReadPosition = mWritePosition - smooth * 44.1;
		if (mReadPosition < 0) {
			mReadPosition += DelaybufferLength;
		}
		int rint1 = (int)mReadPosition;
		float phas = mReadPosition - rint1;
		int rint2 = rint1 + 1;
		if (rint2 >= DelaybufferLength) {
			rint2 = -DelaybufferLength;
		}
		float roat2 = mReadPosition - rint2;
		float delayleft = (1 - phas)*mDelayBufferLeft.getSample(0, rint1) + phas * mDelayBufferLeft.getSample(0, rint2);
		float delayright = (1 - phas)*mDelayBufferRight.getSample(0, rint1) + phas * mDelayBufferRight.getSample(0, rint2);
		feedbackleft = *feedback*delayleft;
		feedbackright = *feedback*delayright;
		float outl = (1 - *drywet)*buffer.getSample(0, sample) + *drywet*feedbackleft;
		float outr = (1 - *drywet)*buffer.getSample(1, sample) + *drywet*feedbackright;
		buffer.addSample(0, sample, outl);
		buffer.addSample(1, sample, outr);
		mWritePosition++;
		if (mWritePosition > DelaybufferLength) {
			mWritePosition = 0;
		}
	}
}

//==============================================================================
bool DelayAudioProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayAudioProcessor::createEditor()
{
	return new DelayAudioProcessorEditor(*this);
}

//==============================================================================
void DelayAudioProcessor::getStateInformation(MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
}

void DelayAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new DelayAudioProcessor();
}
