#pragma once

#include <chrono>

template< typename TDuration, char TTimeSuffix  >
class ScopeTimer
{
public:
	ScopeTimer(const char* inMessage) : mMessage(inMessage), mStart( std::chrono::high_resolution_clock::now() ) { }
	~ScopeTimer();

private:
	const char*													mMessage;
	std::chrono::time_point<std::chrono::high_resolution_clock>	mStart;
};

template< typename TDuration, char TTimeSuffix  >
ScopeTimer<TDuration, TTimeSuffix>::~ScopeTimer()
{
    auto end		= std::chrono::high_resolution_clock::now();
	auto delta		= end - mStart;
	auto duration	= std::chrono::duration_cast< TDuration >(delta);

	printf("%s: %lld%cs\n", mMessage, duration.count(), TTimeSuffix);
}

typedef ScopeTimer< std::chrono::nanoseconds,	'n' > ScopeTimerNs;
typedef ScopeTimer< std::chrono::microseconds,	'u' > ScopeTimerUs;
typedef ScopeTimer< std::chrono::milliseconds,	'm' > ScopeTimerMs;
typedef ScopeTimer< std::chrono::seconds,		'\0' > ScopeTimerSec;