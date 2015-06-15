#include "Timer.h"


Timer::Timer()
	: mLastClockTime( Clock::now() )
	, mDeltaTime	( 0.0 )
	, mTimeElapsed	( 0.0 )
{
}

void Timer::Start()
{
	mLastClockTime	= Clock::now();
	mDeltaTime		= 0.0;
	mTimeElapsed	= 0.0;
}

void Timer::Tick()
{
	auto now		= Clock::now();
	auto delta		= now - mLastClockTime;
	auto nanosec	= std::chrono::duration_cast< std::chrono::nanoseconds >(delta).count();
	
	mLastClockTime	= now;
	mDeltaTime		= (float) nanosec / 1000000000.0f;
	mTimeElapsed	+= mDeltaTime;
}
