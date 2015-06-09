#include "Time.h"

#include <thread>
#include <chrono>

namespace Time
{
	void SleepThread(long long inMillisecondDuration)
	{
	    std::chrono::milliseconds milli(inMillisecondDuration);
	    std::this_thread::sleep_for(milli);
	}
}
