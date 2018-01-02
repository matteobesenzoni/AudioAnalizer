#pragma once

#include <mutex>

using namespace std;

class DoubleBuffer
{
public:
	DoubleBuffer() :
		f1(new float[FFT_SIZE * 2]),
		f2(new float[FFT_SIZE * 2]),
		r(f1),
		w(f2)
	{
		zeromem(f1, FFT_SIZE * 2 * sizeof(float));
		zeromem(f2, FFT_SIZE * 2 * sizeof(float));
	}

	~DoubleBuffer()
	{
		r = nullptr;
		w = nullptr;
		delete[] f1;
		delete[] f2;
	}

	void write(float *bufferToRead)
	{	
		memcpy(w, bufferToRead, FFT_SIZE * 2 * sizeof(float));

		if (lock.try_lock())
		{
			swap(r, w);
			lock.unlock();
		}
	}

	void read(float *bufferToFill)
	{
		if (lock.try_lock())
		{
			memcpy(bufferToFill, r, FFT_SIZE * 2 * sizeof(float));
			lock.unlock();
		}
	}

private:
	float *f1, *f2, *r, *w;

	mutex lock;
};