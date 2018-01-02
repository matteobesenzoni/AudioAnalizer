
#include "AudioProcessing.h"

#include <list>

AudioProcessing::AudioProcessing(DoubleBuffer *double_buffer, int new_buffer_size, double new_sample_rate) : 
	double_buffer(double_buffer),
	forward_FFT((int)log2(FFT_SIZE)),
	fifo_index(0)
{
	setAudioChannels(2, 0);  // 2 in, 0 out

	deviceManager.getAudioDeviceSetup(custom_device_setup); // current setup

	custom_device_setup.bufferSize = new_buffer_size;
	custom_device_setup.sampleRate = new_sample_rate;
	String error = deviceManager.setAudioDeviceSetup(custom_device_setup, true);

	if (!error.isEmpty())
		cout << error << endl;

	cout << endl 
		 << " - Buffer Size "	<< custom_device_setup.bufferSize << endl
		 << " - Sample Rate "	<< custom_device_setup.sampleRate << endl
		 << " - FFT Window  "	<< FFT_SIZE						<< endl;
}

AudioProcessing::~AudioProcessing()
{
	shutdownAudio();
}

void AudioProcessing::prepareToPlay(int, double) {}

void AudioProcessing::releaseResources() {}

void AudioProcessing::getNextAudioBlock(const AudioSourceChannelInfo& buffer)
{
	if (buffer.buffer->getNumChannels() > 0)
	{
		const float* input = buffer.buffer->getWritePointer(0, buffer.startSample);

		for (int i = 0; i < buffer.numSamples; ++i)
		{
			if (fifo_index == FFT_SIZE)
			{
				// clear old data and copy new buffer
				zeromem(fft_data, sizeof(fft_data));
				memcpy(fft_data, fifo, sizeof(fifo));

				// do FFT
				forward_FFT.performFrequencyOnlyForwardTransform(fft_data);

				// update double buffer
				double_buffer->write(fft_data);

				// reset fifo index
				fifo_index = 0;
			}
			// add sample
			fifo[fifo_index++] = input[i];
		}
	}
}