/*
  ==============================================================================

    Spectrogram.h
    Created: 3 Jul 2023 10:20:25am
    Author:  tytu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FFTWrapper;

//==============================================================================
/*
*/
class Spectrogram  : public Component, Value::Listener
{
public:
    Spectrogram(FFTWrapper* fftWrapper);
    ~Spectrogram() override;

    // display
    void paint (Graphics&) override;
    void resized() override;

    void drawNextLineOfSpectrogram(float* data, int dataSize);

    virtual void valueChanged(Value& value) override;

private:
    Image spectrogramImage;
    int fftSize;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Spectrogram)
};
