#pragma once

#include "JuceHeader.h"
#include "DoubleBuffer.h"

#include <mutex>

using namespace std;

class AudioProcessing : public AudioAppComponent
{
public:
	AudioProcessing(DoubleBuffer *double_buffer, int new_buffer_size = 0, double new_sample_rate = 0);
	~AudioProcessing();

	void prepareToPlay(int, double) override;
	void releaseResources() override;
	void getNextAudioBlock(const AudioSourceChannelInfo&) override;

private:
	float fft_data[FFT_SIZE * 2];

	float fifo[FFT_SIZE];
	int fifo_index;

	dsp::FFT forward_FFT;

	AudioDeviceManager::AudioDeviceSetup custom_device_setup;

	DoubleBuffer *double_buffer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioProcessing)
};