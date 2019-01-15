#pragma once

#include <chrono>

struct Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> start, end;
	std::chrono::duration<float> duration;

public:
	Timer();
	~Timer();
};
