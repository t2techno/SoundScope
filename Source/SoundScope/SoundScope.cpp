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
#include "FFTWrapper/FFTWrapper.h"

//==============================================================================
SoundScope::SoundScope(): m_FFTWrapper(std::make_unique<FFTWrapper>())
{
    m_spectrogram = std::make_unique<Spectrogram>(m_FFTWrapper.get());
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
    m_FFTWrapper.get()->fillBuffer(bufferToFill);
}

void SoundScope::timerCallback() {
    if (m_FFTWrapper.get()->nextFFTBlockReady) {
        m_FFTWrapper.get()->forwardFrequencyFFT();
        m_FFTWrapper.get()->nextFFTBlockReady = false;
        repaint();
    }
}