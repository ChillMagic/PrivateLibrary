// Name:	timer.h
// Date:    11/05/2016
// Version: 1.0.0.0

#pragma once
#ifndef _PRILIB_TIMER_H_
#define _PRILIB_TIMER_H_
#include "macro.h"
#include <ctime>

PRILIB_BEGIN
class Timer
{
public:
	Timer() : time(clock()) {}
	size_t detTime() {
		clock_t currTime = clock();
		size_t det = (size_t)(currTime - time);
		time = currTime;
		return det;
	}

	clock_t time;
};
PRILIB_END

#endif
