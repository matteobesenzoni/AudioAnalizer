
#include "Spectrogram.h"

Spectrogram::Spectrogram() :
	spectrogramImage(Image::RGB, 512, 512, true)
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
	g.drawImage(spectrogramImage, getLocalBounds().toFloat());
}

void Spectrogram::resized()
{

}

void Spectrogram::timerCallback()
{
	double_buffer.read(fftData);
	drawNextFrame();
	repaint();
}

void Spectrogram::drawNextFrame()
{
	// image bounds
	const int rightHandEdge = spectrogramImage.getWidth() - 1;
	const int imageHeight = spectrogramImage.getHeight();

	// shift image << 1px
	spectrogramImage.moveImageSection(0, 0, 1, 0, rightHandEdge, imageHeight);

	// lowest/highest intensity
	Range<float> maxLevel = FloatVectorOperations::findMinAndMax(fftData, FFT_SIZE / 2);

	for (int y = 1; y < imageHeight; ++y)
	{
		const float skewedProportionY = 1.0f - exp(log(y / (float)imageHeight) * 0.2f); // log scale represetation | 1.0 - e^[ ln(y / imageHeight) * 0.2 ]
		const int fftDataIndex = jlimit(0, FFT_SIZE / 2, (int)(skewedProportionY * FFT_SIZE / 2)); // limit fftData index to half it's size (second half not used for spectrogram)
		const float level = jmap(fftData[fftDataIndex], 0.0f, jmax(maxLevel.getEnd(), 1e-5f), 0.0f, 1.0f); // map intensity to [0,1] scale based on highest value

		Colour colour = Colour::fromHSV(level, 1.0f, level, 1.0f);
		spectrogramImage.setPixelAt(rightHandEdge, y, colour);
	}
}