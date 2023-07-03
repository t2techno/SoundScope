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
          spectrogramImage (juce::Image::RGB, 512, 512, true)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

Spectrogram::~Spectrogram()
{
}

void Spectrogram::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);

    g.setOpacity (1.0f);
    g.drawImage (spectrogramImage, getLocalBounds().toFloat());
}

void drawNextLineOfSpectrogram(){
    auto rightHandEdge = spectrogramImage.getWidth() - 1;
    auto imageHeiht    = spectrogramImage.getHeight();

    // move image left 1 pixel
    spectrogramImage.moveImageSection(0, 0, 1, 0, rightHandEdge, imageHeight);

    // render FFT data
    forwardFFT.performFrequencyOnlyForwardTransform(fftData.data());

    // find max value to scale output
    auto maxLevel = FloatVectorOperations::findMinAndMax(fftData.data(), fftSize/2);
    for (auto y=1; y< imageHeight; ++y){
      // log value of as it better represents frequency spectrum
      auto skewedPropertionY = 1.0f - std::exp(std::log((float)y / (float)imageHeight)* 0.2f);
      auto fftDataIndex = (size_t) juce::jlimit(0, fftSize/2, (int)(skewProportionY * fftSize/2));
      auto level = juce::jmap(fftData[fftDataIndex], 0.0f, juce::jmax(maxLevel.getEnd(), 1e-5f), 0.0f, 1.0f);
    }
}

void Spectrogram::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void Spectrogram::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
    if(bufferToFill.buffer->getNumChannels() > 0){
        auto* channelData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

        for(auto i=0; i<bufferTofill.numSamples; ++i){
            pushNextSampleIntoFifo(channelData[i]);
        }
    }
}

void pushNextSampleIntoFifo(float sample) noexcept {

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
void timerCallback() {
    if (nextFFTBlockReady) {
        drawNextLineOfSpectrogram();
        nextFFTBlockReady = false;
        repaint();
    }
}