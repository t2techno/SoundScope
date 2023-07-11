/*
  ==============================================================================

    FFTWrapper.h
    Created: 4 Jul 2023 3:09:43pm
    Author:  tytu1

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FFTWrapper {

public:
    FFTWrapper();
    ~FFTWrapper();

    void pushNextSampleIntoFifo(float sample) noexcept;
    void fillBuffer(const AudioSourceChannelInfo& bufferToFill);
    void forwardFrequencyFFT();

    std::atomic_bool nextFFTBlockReady = false;

    Value& getValue();
    const int& getFFTSize();


private:
    dsp::FFT forwardFFT;

    static constexpr auto fftOrder = 10;
    static constexpr auto fftSize = 1 << fftOrder;
    static constexpr size_t memoryBlockSize = fftSize * 2 * sizeof(float);

    std::array<float, fftSize> fifo;
    std::array<float, fftSize * 2> fftData;
    int fifoIndex = 0;

    Value broadcastValue;
    MemoryBlock broadcastMemoryBlock = MemoryBlock(memoryBlockSize);
};