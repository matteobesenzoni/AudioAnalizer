
#include "AudioProcessing.h"

#include <list>

AudioProcessing::AudioProcessing(int newBufferSize, double newSampleRate) : 
	forwardFFT((int)log2(FFT_SIZE)),
	fifoIndex(0)
{
	setAudioChannels(2, 0);  // 2 in, 0 out

	deviceManager.getAudioDeviceSetup(customDeviceSetup); // current setup

	customDeviceSetup.bufferSize = newBufferSize;
	customDeviceSetup.sampleRate = newSampleRate;
	String error = deviceManager.setAudioDeviceSetup(customDeviceSetup, true);

	if (!error.isEmpty())
		cout << error << endl;

	cout << endl 
		 << " - Buffer Size "	<< customDeviceSetup.bufferSize << endl
		 << " - Sample Rate "	<< customDeviceSetup.sampleRate << endl
		 << " - FFT Window  "	<< FFT_SIZE						<< endl;
}

AudioProcessing::~AudioProcessing()
{
	shutdownAudio();
}

void AudioProcessing::prepareToPlay(int, double) {}

void AudioProcessing::releaseResources() {}

void AudioProcessing::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	if (bufferToFill.buffer->getNumChannels() > 0)
	{
		const float* input = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);

		for (int i = 0; i < bufferToFill.numSamples; ++i)
		{
			if (fifoIndex == FFT_SIZE)
			{
				// clear old data and copy new buffer
				zeromem(fftData, sizeof(fftData));
				memcpy(fftData, fifo, sizeof(fifo));

				// do FFT
				forwardFFT.performFrequencyOnlyForwardTransform(fftData);

				// update double buffer
				double_buffer.write(fftData);

				// reset fifo index
				fifoIndex = 0;
			}
			// add sample
			fifo[fifoIndex++] = input[i];
		}
	}
}