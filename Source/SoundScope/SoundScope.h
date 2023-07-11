/*
  ==============================================================================

    SoundScope.h
    Created: 3 Jul 2023 11:03:21am
    Author:  tytu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class Spectrogram;
class FFTWrapper;

//==============================================================================
/*
*/
class SoundScope  : public Component,
                    private juce::Timer
{
public:
    SoundScope();
    ~SoundScope() override;

    void paint (Graphics&) override;
    void resized() override;

    void fillBuffer(const AudioSourceChannelInfo& bufferToFill);

    void timerCallback() override;

private:
    static constexpr int refreshRate = 60;

    std::unique_ptr<Spectrogram> m_spectrogram;
    std::unique_ptr<FFTWrapper> m_FFTWrapper;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoundScope)
};
