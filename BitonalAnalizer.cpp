
#include "BitonalAnalizer.h"

BitonalAnalizer::BitonalAnalizer(float f1, float d1, float f2, float d2, float sample_rate, int window, int hz) :
	f1(f1), d1(d1),
	f2(f2), d2(d2),
	sample_rate(sample_rate), window(window), hz(hz),
	buffer_size((int) ((d1 + d2) * 2 * hz)), // 2x cycle
	buffer_current_size(0), buffer_index(0),
	match(false)
{
	frequency_resolution = sample_rate / window;
	bin_f1 = bin(f1); 
	bin_f2 = bin(f2);

	buffer = new pair[buffer_size];
}

BitonalAnalizer::~BitonalAnalizer() {}

void BitonalAnalizer::start()
{
	startTimerHz(hz);
}

void BitonalAnalizer::timerCallback() 
{
	double_buffer.read(fftData);
	updateQueue();
	analize();
	alert();
}

void BitonalAnalizer::updateQueue()
{
	buffer[buffer_index] = { fftData[bin_f1], fftData[bin_f2] };
	buffer_index = (buffer_index + 1) % buffer_size;
	buffer_current_size = jlimit(0, buffer_size, ++buffer_current_size);
}

void BitonalAnalizer::analize()
{
	float threshold = 1.0f;
	float f1_score = 0.0f;
	float f2_score = 0.0f;
	float f1_min_score = d1 * hz * 0.8f;
	float f2_min_score = d2 * hz * 0.8f;

	bool f1_prev = false;
	bool f2_prev = false;
	for (int i = 0; i < buffer_current_size; i++)
	{
		pair p = buffer[i];
		if (p.f1 >= threshold && p.f2 < threshold)
		{
			if (!f1_prev)
				f1_score = 1;
			else
				f1_score++;
			f1_prev = true;
		}
		else 
			f1_prev = false;

		if (p.f2 >= threshold && p.f1 < threshold)
		{
			if (!f2_prev)
				f2_score = 1;
			else
				f2_score++;
			f2_prev = true;
		}
		else
			f2_prev = false;

		match = f1_score >= f1_min_score || f2_score >= f2_min_score;
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
	if (f1 <= frequency_resolution / 2.0f)
		return 0;
	else if (f1 >= sample_rate - frequency_resolution / 2.0f)
		return window - 1;
	else
		return (int) ((f / frequency_resolution) / 2.0f);
}

