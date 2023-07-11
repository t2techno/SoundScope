/*
  ==============================================================================

    FFTWrapper.cpp
    Created: 4 Jul 2023 3:09:43pm
    Author:  tytu1

  ==============================================================================
*/

#include "FFTWrapper.h"

FFTWrapper::FFTWrapper() : forwardFFT(fftOrder), broadcastValue(Value())
{
}

FFTWrapper::~FFTWrapper()
{
}

Value& FFTWrapper::getValue() {
    return broadcastValue;
}

const int& FFTWrapper::getFFTSize() {
    return fftSize;
}

// compute FFT data
void FFTWrapper::forwardFrequencyFFT()
{
    forwardFFT.performFrequencyOnlyForwardTransform(fftData.data());
    broadcastMemoryBlock.copyFrom(fftData.data(), 0, memoryBlockSize);
    broadcastValue.setValue(broadcastMemoryBlock);
}

void FFTWrapper::fillBuffer(const AudioSourceChannelInfo& bufferToFill) {
    if (bufferToFill.buffer->getNumChannels() > 0) {
        auto* channelData = bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample);

        for (auto i = 0; i < bufferToFill.numSamples; ++i) {
            pushNextSampleIntoFifo(channelData[i]);
        }
    }
}

void FFTWrapper::pushNextSampleIntoFifo(float sample) noexcept {

    // enough data to render
    if (fifoIndex == fftSize) {
        if (!nextFFTBlockReady) {
            std::fill(fftData.begin(), fftData.end(), 0.0f);
            std::copy(fifo.begin(), fifo.end(), fftData.begin());
            nextFFTBlockReady = true;
        }

        fifoIndex = 0;
    }

    fifo[(size_t)fifoIndex++] = sample;
}