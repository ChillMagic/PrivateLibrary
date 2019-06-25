// timer.h
// * PrivateLibrary

#pragma once
#ifndef _PRILIB_TIMER_H_
#define _PRILIB_TIMER_H_
#include "macro.h"
#include <ctime>

PRILIB_BEGIN
class Timer
{
public:
	Timer() : time(std::clock()) {}
	auto detTime() {
		std::clock_t currTime = std::clock();
		auto det = currTime - time;
		time = currTime;
		return det;
	}

	std::clock_t time;
};
PRILIB_END

#endif
