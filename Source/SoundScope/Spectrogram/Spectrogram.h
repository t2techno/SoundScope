/*
  ==============================================================================

    Spectrogram.h
    Created: 3 Jul 2023 10:20:25am
    Author:  tytu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Spectrogram  : public Component
{
public:
    Spectrogram();
    ~Spectrogram() override;

    // display
    void paint (Graphics&) override;
    void resized() override;

    void drawNextLineOfSpectrogram();

    // data
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void pushNextSampleIntoFifo(float sample);

    void timerCallback();

    static constexpr auto fftOrder = 10;
    static constexpr auto fftSize  = 1 << fftOrder;

private:
    dsp::FFT forwardFFT;
    Image spectrogramImage;
 
    std::array<float, fftSize> fifo;
    std::array<float, fftSize * 2> fftData;
    int fifoIndex = 0;
    bool nextFFTBlockReady = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Spectrogram)
};
