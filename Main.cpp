
#include "../JuceLibraryCode/JuceHeader.h"
#include "Window.h"
#include "AudioProcessing.h"
#include "DoubleBuffer.h"
#include "BitonalAnalizer.h"

#include <thread>
#include <mutex>

#define BUFFER_SIZE(n)	1 << n
#define SAMPLE_RATE		22050

using namespace std;

DoubleBuffer double_buffer;

class AudioRecognizer : public JUCEApplication
{
public:
	AudioRecognizer() {}

	const String getApplicationName() override { return ProjectInfo::projectName; }
	const String getApplicationVersion() override { return ProjectInfo::versionString; }
	bool moreThanOneInstanceAllowed() override { return true; }

	void initialise(const String& cmd) override
	{
		bool spectrogram = cmd.contains("-sp");

		police = new BitonalAnalizer(1258.0f, 0.75f, 1880.0f, 0.75f, SAMPLE_RATE, FFT_SIZE);
		audioProcessing = new AudioProcessing(BUFFER_SIZE(6), SAMPLE_RATE);
		if(spectrogram)
			window = new Window();

		police.get()->start();

		if (spectrogram)
			window.get()->start();
	}

	void shutdown() override
	{
		police = nullptr;
		window = nullptr;
		audioProcessing = nullptr;
	}

	void systemRequestedQuit() override
	{
		quit();
	}

private:
	ScopedPointer<AudioProcessing> audioProcessing;
	ScopedPointer<Window> window;

	ScopedPointer<BitonalAnalizer> police/*, ambulance, firefighters*/;
};

// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(AudioRecognizer);