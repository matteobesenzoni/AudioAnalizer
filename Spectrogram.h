#pragma once

#include "JuceHeader.h"
#include "DoubleBuffer.h"

#include <mutex>

extern DoubleBuffer double_buffer;

using namespace std;

class Spectrogram : public Component, private Timer
{
public:
	Spectrogram();
	~Spectrogram();

	void start();

private:
	void paint(Graphics&) override;
	void resized() override;

	void timerCallback() override;

	void drawNextFrame();

	float fft_data[FFT_SIZE * 2];
	Image spectrogram_image;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Spectrogram)
};