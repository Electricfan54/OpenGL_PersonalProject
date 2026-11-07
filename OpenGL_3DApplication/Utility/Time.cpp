#include "Time.h"

Time::Time()
	: deltaTime(0)
{
}

void Time::Start()
{
	startTime = std::chrono::high_resolution_clock::now();
}

void Time::End()
{
	endTime = std::chrono::high_resolution_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
}
