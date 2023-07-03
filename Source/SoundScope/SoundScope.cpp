/*
  ==============================================================================

    SoundScope.cpp
    Created: 3 Jul 2023 11:03:21am
    Author:  tytu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SoundScope.h"

//==============================================================================
SoundScope::SoundScope(): m_spectrogram(std::make_unique<Spectrogram>())
{
    addAndMakeVisible(*m_spectrogram.get());
    startTimerHz(refreshRate);
}

SoundScope::~SoundScope()
{
}

void SoundScope::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("SoundScope", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void SoundScope::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill){
    m_spectrogram.get()->getNextAudioBlock(bufferToFill);
}

void timerCallback() {
    m_spectrogram.get()->timerCallback();
}