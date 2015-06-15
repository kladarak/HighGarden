#pragma once

#include <chrono>

class Timer
{
public:
	Timer();

	void	Start();
	void	Tick();
	float	GetDeltaTime() const	{ return mDeltaTime; }
	float	GetTimeElapsed() const	{ return mTimeElapsed; }

private:
	typedef std::chrono::system_clock Clock;
	typedef std::chrono::time_point<Clock> ClockTime;
	ClockTime	mLastClockTime;
	float		mDeltaTime;
	float		mTimeElapsed;
};
