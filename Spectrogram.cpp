
#include "Spectrogram.h"

Spectrogram::Spectrogram(DoubleBuffer *double_buffer) :
	double_buffer(double_buffer),
	spectrogram_image(Image::RGB, 512, 512, true)
{
	setOpaque(false);
	setSize(700, 500);
}

Spectrogram::~Spectrogram() {}

void Spectrogram::start()
{
	startTimerHz(60);
}

void Spectrogram::paint(Graphics& g)
{
	g.fillAll(Colours::black);

	g.setOpacity(1.0f);
	g.drawImage(spectrogram_image, getLocalBounds().toFloat());
}

void Spectrogram::resized()
{

}

void Spectrogram::timerCallback()
{
	double_buffer->read(fft_data);
	drawNextFrame();
	repaint();
}

void Spectrogram::drawNextFrame()
{
	// image bounds
	const int right_edge = spectrogram_image.getWidth() - 1;
	const int image_h = spectrogram_image.getHeight();

	// shift image << 1px
	spectrogram_image.moveImageSection(0, 0, 1, 0, right_edge, image_h);

	// lowest/highest intensity
	Range<float> max_level = FloatVectorOperations::findMinAndMax(fft_data, FFT_SIZE / 2);

	for (int y = 1; y < image_h; ++y)
	{
		const float skewed_y = 1.0f - exp(log(y / (float)image_h) * 0.2f); // log scale represetation | 1.0 - e^[ ln(y / imageHeight) * 0.2 ]
		const int fft_data_index = jlimit(0, FFT_SIZE / 2, (int)(skewed_y * FFT_SIZE / 2)); // limit fftData index to half it's size (second half not used for spectrogram)
		const float level = jmap(fft_data[fft_data_index], 0.0f, jmax(max_level.getEnd(), 1e-5f), 0.0f, 1.0f); // map intensity to [0,1] scale based on highest value

		Colour colour = Colour::fromHSV(level, 1.0f, level, 1.0f);
		spectrogram_image.setPixelAt(right_edge, y, colour);
	}
}