#pragma once

#include "JuceHeader.h"
#include "DoubleBuffer.h"

#include <queue>
#include <algorithm>

using namespace std;

class BitonalAnalizer : private Timer
{
public:
	BitonalAnalizer(DoubleBuffer *double_buffer, float freq1, float freq1_t, float freq2, float freq2_t, float sample_rate, int ftt_window, int hz = 20);
	~BitonalAnalizer();

	void start();

private:
	void timerCallback() override;

	void updateQueue();
	float analize(const float db_threshold, const float freq_threshold);
	float analize1(const float db_threshold, const float freq_threshold);
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

	DoubleBuffer *double_buffer;
};
