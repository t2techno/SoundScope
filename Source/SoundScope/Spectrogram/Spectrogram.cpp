/*
  ==============================================================================

    Spectrogram.cpp
    Created: 3 Jul 2023 10:20:25am
    Author:  tytu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "../../PluginEditor.h"
#include "Spectrogram.h"
#include "../FFTWrapper/FFTWrapper.h"

//==============================================================================
Spectrogram::Spectrogram(FFTWrapper* fftWrapper): spectrogramImage (Image::RGB, 512, 512, true),
                                                  fftSize(fftWrapper->getFFTSize())
{
    fftWrapper->getValue().addListener(this);
    setSize(700, 500);
}

Spectrogram::~Spectrogram()
{
}

void Spectrogram::paint (Graphics& g)
{
    g.fillAll (Colours::black);

    g.setOpacity (1.0f);
    g.drawImage (spectrogramImage, getLocalBounds().toFloat());

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("Spectrogram", getLocalBounds(),
        juce::Justification::top, true);   // draw some placeholder text
}

void Spectrogram::valueChanged(Value& value) 
{
    MemoryBlock* binArray = value.getValue().getBinaryData();
    float* floatArray = static_cast<float*>(binArray->getData());
    drawNextLineOfSpectrogram(floatArray, fftSize);
}

void Spectrogram::drawNextLineOfSpectrogram(float* data, int dataSize)
{

    // note: dataSize is actually half the size of the array.
    //       the fft stuff requires a double size array
    //       but below operations don't need all of it
    auto rightHandEdge = spectrogramImage.getWidth() - 1;
    auto imageHeight = spectrogramImage.getHeight();

    // move image left 1 pixel
    spectrogramImage.moveImageSection(0, 0, 1, 0, rightHandEdge, imageHeight);

    // find max value to scale output
    auto maxLevel = FloatVectorOperations::findMinAndMax(data, dataSize);
    for (auto y=1; y< imageHeight; ++y){
      // log value of as it better represents frequency spectrum
      auto skewedProportionY = 1.0f - std::exp(std::log((float)y / (float)imageHeight)* 0.2f);
      auto fftDataIndex = (size_t) jlimit(0, dataSize, (int)(skewedProportionY * dataSize));
      auto level = jmap(data[fftDataIndex], 0.0f, jmax(maxLevel.getEnd(), 1e-5f), 0.0f, 1.0f);

      spectrogramImage.setPixelAt(rightHandEdge, y, juce::Colour::fromHSV(level, 1.0f, level, 1.0f));
    }
    repaint();
}

void Spectrogram::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}