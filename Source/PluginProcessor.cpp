/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "SoundScope/SoundScope.h";

//==============================================================================
SoundScopeAudioProcessor::SoundScopeAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

SoundScopeAudioProcessor::~SoundScopeAudioProcessor()
{
}

//==============================================================================
const juce::String SoundScopeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SoundScopeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SoundScopeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SoundScopeAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SoundScopeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SoundScopeAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SoundScopeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SoundScopeAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SoundScopeAudioProcessor::getProgramName (int index)
{
    return {};
}

void SoundScopeAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SoundScopeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SoundScopeAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SoundScopeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void SoundScopeAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // prevent garbage sound/data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    AudioSourceChannelInfo bufferToFill;
    bufferToFill.buffer = &buffer;
    bufferToFill.startSample = 0;
    bufferToFill.numSamples = buffer.getNumSamples();

    // need to verify this is threadsafe
    editor = getEditor();
    if(editor != nullptr) {
        // passes forward to the various visualization utilities
        editor->getScope()->fillBuffer(bufferToFill);
    }
    editor = nullptr;
}

//==============================================================================
bool SoundScopeAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SoundScopeAudioProcessor::createEditor()
{
    return new SoundScopeAudioProcessorEditor (*this);
}

SoundScopeAudioProcessorEditor* SoundScopeAudioProcessor::getEditor()
{
    return static_cast<SoundScopeAudioProcessorEditor*>(getActiveEditor());
}

//==============================================================================
void SoundScopeAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SoundScopeAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SoundScopeAudioProcessor();
}
