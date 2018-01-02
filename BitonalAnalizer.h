#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DoubleBuffer.h"

#include <queue>
#include <algorithm>

extern DoubleBuffer double_buffer;

using namespace std;

class BitonalAnalizer : private Timer
{
public:
	BitonalAnalizer(float freq1, float freq1_t, float freq2, float freq2_t, float sample_rate, int ftt_window, int hz = 20);
	~BitonalAnalizer();

	void start();

private:
	void timerCallback() override;

	void updateQueue();
	void analize();
	void alert();
	int bin(float);

	const float freq1, freq1_t;
	const float freq2, freq2_t;

	const float sample_rate;
	const int fft_window;
	const int hz;
	float frequency_resolution;

	float fft_data[FFT_SIZE * 2];
	int freq1_bin, freq2_bin;

	struct pair {
		float freq1;
		float freq2;
	};

	pair *buffer;
	int buffer_index;
	size_t buffer_current_size;
	const size_t buffer_size;

	bool match;
};
