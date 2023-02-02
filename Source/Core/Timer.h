#pragma once

#include <chrono>

class FTimer
{
public:
	void Start()
	{
		StartTime = std::chrono::system_clock::now();
		bRunning = true;
	}

	void Stop()
	{
		EndTime = std::chrono::system_clock::now();
		bRunning = false;
	}

	double ElapsedMilliseconds()
	{
		std::chrono::time_point<std::chrono::system_clock> _EndTime;
		if (bRunning)
		{
			_EndTime = std::chrono::system_clock::now();
		}
		else
		{
			_EndTime = EndTime;
		}
		return (double)std::chrono::duration_cast<std::chrono::milliseconds>(_EndTime - StartTime).count();
	}

	double ElapsedSeconds()
	{
		return ElapsedMilliseconds() / 1000.0;
	}

private:
	bool bRunning = false;
	std::chrono::time_point<std::chrono::system_clock> StartTime;
	std::chrono::time_point<std::chrono::system_clock> EndTime;
};
