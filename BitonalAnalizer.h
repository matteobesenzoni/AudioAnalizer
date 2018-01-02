#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DoubleBuffer.h"

#include <queue>

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

	const float f1, d1;
	const float f2, d2;

	const float sample_rate;
	const int window;
	const int hz;
	float frequency_resolution;

	float fftData[FFT_SIZE * 2];
	int bin_f1, bin_f2;

	struct pair {
		float f1;
		float f2;
	};

	pair *buffer;
	int buffer_index;
	int buffer_current_size;
	const int buffer_size;

	bool match;
};
