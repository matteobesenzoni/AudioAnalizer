#pragma once

#include <mutex>

using namespace std;

class DoubleBuffer
{
public:
	DoubleBuffer() :
		buffer1(new float[FFT_SIZE * 2]),
		buffer2(new float[FFT_SIZE * 2]),
		read_p(buffer1),
		write_p(buffer2)
	{
		zeromem(buffer1, buffer_size);
		zeromem(buffer2, buffer_size);
	}

	~DoubleBuffer()
	{
		read_p = nullptr;
		write_p = nullptr;
		delete[] buffer1;
		delete[] buffer2;
	}

	void write(float *data)
	{	
		memcpy(write_p, data, buffer_size);

		if (mtx.try_lock())
		{
			swap(read_p, write_p);
			mtx.unlock();
		}
	}

	void read(float *buffer)
	{
		if (mtx.try_lock())
		{
			memcpy(buffer, read_p, buffer_size);
			mtx.unlock();
		}
	}

private:
	const size_t buffer_size = FFT_SIZE * 2 * sizeof(float);

	float *buffer1, *buffer2, *read_p, *write_p;
	mutex mtx;
};