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

//==============================================================================
Spectrogram::Spectrogram(): forwardFFT (fftOrder),
          spectrogramImage (Image::RGB, 512, 512, true)
{
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

void Spectrogram::drawNextLineOfSpectrogram(){
    auto rightHandEdge = spectrogramImage.getWidth() - 1;
    auto imageHeight = spectrogramImage.getHeight();

    // move image left 1 pixel
    spectrogramImage.moveImageSection(0, 0, 1, 0, rightHandEdge, imageHeight);

    // render FFT data
    forwardFFT.performFrequencyOnlyForwardTransform(fftData.data());

    // find max value to scale output
    auto maxLevel = FloatVectorOperations::findMinAndMax(fftData.data(), fftSize/2);
    for (auto y=1; y< imageHeight; ++y){
      // log value of as it better represents frequency spectrum
      auto skewedProportionY = 1.0f - std::exp(std::log((float)y / (float)imageHeight)* 0.2f);
      auto fftDataIndex = (size_t) jlimit(0, fftSize/2, (int)(skewedProportionY * fftSize/2));
      auto level = jmap(fftData[fftDataIndex], 0.0f, jmax(maxLevel.getEnd(), 1e-5f), 0.0f, 1.0f);

      spectrogramImage.setPixelAt(rightHandEdge, y, juce::Colour::fromHSV(level, 1.0f, level, 1.0f));
    }
}

void Spectrogram::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void Spectrogram::fillBuffer(const AudioSourceChannelInfo& bufferToFill) {
    if(bufferToFill.buffer->getNumChannels() > 0){
        auto* channelData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

        for(auto i=0; i<bufferToFill.numSamples; ++i){
            pushNextSampleIntoFifo(channelData[i]);
        }
    }
}

void Spectrogram::pushNextSampleIntoFifo(float sample) noexcept {

    // enough data to render
    if(fifoIndex == fftSize){
        if(!nextFFTBlockReady){
            std::fill(fftData.begin(), fftData.end(), 0.0f);
            std::copy(fifo.begin(), fifo.end(), fftData.begin());
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[(size_t) fifoIndex++] = sample;
}

// used in timer on parent class
void Spectrogram::timerCallback() {
    if (nextFFTBlockReady) {
        drawNextLineOfSpectrogram();
        nextFFTBlockReady = false;
        repaint();
    }
}