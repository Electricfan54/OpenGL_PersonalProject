#pragma once
#include <chrono>

class Time
{
private:
	double deltaTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> endTime;


public:

	Time();
	void Start();
	void End();

	const double& DeltaTime() const
	{
		return deltaTime / 1000000;
	}

	void SetDeltaTime(double time)
	{
		deltaTime = time;
	}

};

