#include "STimer.h"
#include <stdio.h>

namespace spider3d
{

//sf32 STimer::currentTime = 0.0f;
//sf32 STimer::startTime = 0.0f;
//sf32 STimer::diffTime = 0.0f;
//sf32 STimer::increasingDuration = 0.0f;
//sf32 STimer::decreasingDuration = 0.0f;
static LARGE_INTEGER HighPerformanceFreq;
static BOOL HighPerformanceTimerSupport = FALSE;

STimer::STimer()
{
	initTimer();
}

STimer::STimer(unsigned loopCount , float increasingDuration)
{
	initTimer();
	STimer::increasingDuration = increasingDuration;
}

void STimer::initTimer()
{
	diffTime = 0.0f;
	decreasingDuration = 0.0f;

	increasingDuration = 1000;
	startTime = GetTickCount();
	currentTime = startTime;
	HighPerformanceTimerSupport = QueryPerformanceFrequency(&HighPerformanceFreq);
}

void STimer::tick()
{
	LARGE_INTEGER nTime;
	
	if ( HighPerformanceTimerSupport )
	{
	  BOOL queriedOK = QueryPerformanceCounter(&nTime);
	  
	  currentTime = ((nTime.QuadPart) * 1000 / HighPerformanceFreq.QuadPart);

	}
	else
	{
	currentTime = GetTickCount();
	}
	
	static int count = 0;
	count++;
	if ( diffTime > increasingDuration )
	{
		fps = count*1000.0f/diffTime;
		printf("\nFPS = %f" , fps);
        count = 0;
		startTime = currentTime;
	}
	diffTime = ( currentTime - startTime );
}

void STimer::setCurrentTime(float time)
{
	startTime = time;
}

float STimer::getCurrentTime()
{
	float time;
	LARGE_INTEGER nTime;

	if ( HighPerformanceTimerSupport )
	{
	  BOOL queriedOK = QueryPerformanceCounter(&nTime);
	  
	  time = ((nTime.QuadPart) * 1000 / HighPerformanceFreq.QuadPart);

	}
	else
	{
		time = GetTickCount();
	}

	return time;
}

void STimer::stop()
{
  
}

float STimer::value()
{
  return diffTime / increasingDuration;
}

void STimer::setIncreasingDuration(float increasingDuration)
{
	STimer::increasingDuration = increasingDuration;
}

float STimer::getIncreasingDuration()
{
	return increasingDuration;
}

void STimer::setDecreasingDuration(float decreasingDuration)
{
	STimer::decreasingDuration = decreasingDuration;
}

float STimer::getDecreasingDuration()
{
	return decreasingDuration;
}

}