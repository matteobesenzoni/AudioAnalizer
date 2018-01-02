#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Spectrogram.h"

class Window : public DocumentWindow
{
public:
	Window() : DocumentWindow(ProjectInfo::projectName,
		LookAndFeel::getDefaultLookAndFeel()
			.findColour(ResizableWindow::backgroundColourId),
		DocumentWindow::allButtons)
	{
		setUsingNativeTitleBar(true);
		spectrogram = new Spectrogram();
		setContentOwned(spectrogram.get(), true);
		setResizable(true, true);
		centreWithSize(getWidth(), getHeight());
		setVisible(true);
	}

	~Window()
	{
		spectrogram = nullptr;
	}

	void closeButtonPressed() override
	{
		JUCEApplication::getInstance()->systemRequestedQuit();
	}

	void start()
	{
		spectrogram.get()->start();
	}

private:
	ScopedPointer<Spectrogram> spectrogram;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Window)
};
