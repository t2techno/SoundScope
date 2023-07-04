/*
  ==============================================================================

    SoundScope.cpp
    Created: 3 Jul 2023 11:03:21am
    Author:  tytu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SoundScope.h"

#include "Spectrogram/Spectrogram.h"

//==============================================================================
SoundScope::SoundScope(): m_spectrogram(std::make_unique<Spectrogram>())
{
    addAndMakeVisible(*m_spectrogram.get());
    startTimerHz(refreshRate);
    setSize(700, 500);
}

SoundScope::~SoundScope()
{
}

void SoundScope::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("SoundScope", getLocalBounds(),
                juce::Justification::bottom, true);   // draw some placeholder text
}

void SoundScope::resized()
{
    m_spectrogram.get()->setBounds(getLocalBounds());
}

void SoundScope::fillBuffer(const AudioSourceChannelInfo& bufferToFill){
    m_spectrogram.get()->fillBuffer(bufferToFill);
}

void SoundScope::timerCallback() {
    m_spectrogram.get()->timerCallback();
}