
#include "BitonalAnalizer.h"

BitonalAnalizer::BitonalAnalizer(float freq1, float freq1_t, float freq2, float freq2_t, float sample_rate, int fft_window, int hz) :
	freq1(freq1), freq1_t(freq1_t),
	freq2(freq2), freq2_t(freq2_t),
	sample_rate(sample_rate), fft_window(fft_window), hz(hz),
	buffer_size((int) ((freq1_t + freq2_t) * 2 * hz)), // 2x cycle
	buffer_current_size(0), buffer_index(0),
	match(false)
{
	frequency_resolution = sample_rate / fft_window;
	freq1_bin = bin(freq1); 
	freq2_bin = bin(freq2);

	buffer = new pair[buffer_size];
}

BitonalAnalizer::~BitonalAnalizer() 
{
	delete[] buffer;
}

void BitonalAnalizer::start()
{
	startTimerHz(hz);
}

void BitonalAnalizer::timerCallback() 
{
	double_buffer.read(fft_data);
	updateQueue();
	analize();
	alert();
}

void BitonalAnalizer::updateQueue()
{
	buffer[buffer_index] = { fft_data[freq1_bin], fft_data[freq2_bin] };
	buffer_index = (buffer_index + 1) % buffer_size;
	buffer_current_size = min(++buffer_current_size, buffer_size);
}

void BitonalAnalizer::analize()
{
	const float db_threshold = 1.0f;
	const float freq1_threshold = 0.8f;
	const float freq2_threshold = 0.8f;
	const float freq1_min_score = freq1_t * hz * freq1_threshold;
	const float freq2_min_score = freq2_t * hz * freq2_threshold;

	float freq1_score = 0.0f;
	float freq2_score = 0.0f;

	bool freq1_prev = false;
	bool freq2_prev = false;
	for (int i = 0; i < buffer_current_size; i++)
	{
		pair p = buffer[i];
		if (p.freq1 >= db_threshold && p.freq2 < db_threshold)
		{
			if (!freq1_prev)
				freq1_score = 1;
			else
				freq1_score++;
			freq1_prev = true;
		}
		else 
			freq1_prev = false;

		if (p.freq2 >= db_threshold && p.freq1 < db_threshold)
		{
			if (!freq2_prev)
				freq2_score = 1;
			else
				freq2_score++;
			freq2_prev = true;
		}
		else
			freq2_prev = false;

		match = freq1_score >= freq1_min_score || freq2_score >= freq2_min_score;
	}
}

void BitonalAnalizer::alert()
{
	if (match)
	{
		// MATCH!!!
	}
}

int BitonalAnalizer::bin(float f)
{
	if (freq1 <= frequency_resolution / 2.0f)
		return 0;
	else if (freq1 >= sample_rate - frequency_resolution / 2.0f)
		return fft_window - 1;
	else
		return (int) ((f / frequency_resolution) / 2.0f);
}

