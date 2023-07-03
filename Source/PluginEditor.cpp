/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoundScopeAudioProcessorEditor::SoundScopeAudioProcessorEditor (SoundScopeAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), m_soundScope(std::make_unique<SoundScope>())
{
    addAndMakeVisible(*m_soundScope.get());
    setSize (400, 300);
}

SoundScopeAudioProcessorEditor::~SoundScopeAudioProcessorEditor()
{
}

//==============================================================================
void SoundScopeAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void SoundScopeAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

SoundScope* getScope(){ return m_soundScope.get();}