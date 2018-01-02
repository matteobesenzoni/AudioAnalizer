#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DoubleBuffer.h"

#include <mutex>

extern DoubleBuffer double_buffer;

using namespace std;

class AudioProcessing : public AudioAppComponent
{
public:
	AudioProcessing(int newBufferSize = 0, double newSampleRate = 0);
	~AudioProcessing();

	void prepareToPlay(int, double) override;
	void releaseResources() override;
	void getNextAudioBlock(const AudioSourceChannelInfo&) override;

private:
	float fftData[FFT_SIZE * 2];

	float fifo[FFT_SIZE];
	int fifoIndex;

	dsp::FFT forwardFFT;

	AudioDeviceManager::AudioDeviceSetup customDeviceSetup;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioProcessing)
};