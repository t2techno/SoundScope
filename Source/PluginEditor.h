/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class SoundScope;

//==============================================================================
/**
*/
class SoundScopeAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SoundScopeAudioProcessorEditor (SoundScopeAudioProcessor&);
    ~SoundScopeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    SoundScope* getScope();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SoundScopeAudioProcessor& audioProcessor;
    std::unique_ptr<SoundScope> m_soundScope;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundScopeAudioProcessorEditor)
};
