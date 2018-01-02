#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
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

	float fftData[FFT_SIZE * 2];
	Image spectrogramImage;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Spectrogram)
};