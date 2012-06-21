#include "DTimer.h"
#include <stdio.h>

BEGIN_NAMESPACE_DIRAC2D

//sf32 STimer::currentTime = 0.0f;
//sf32 STimer::startTime = 0.0f;
//sf32 STimer::diffTime = 0.0f;
//sf32 STimer::increasingDuration = 0.0f;
//sf32 STimer::decreasingDuration = 0.0f;
#ifdef WIN32
static LARGE_INTEGER HighPerformanceFreq;
static BOOL HighPerformanceTimerSupport = FALSE;
#else

#endif

DTimer::DTimer()
{
	initTimer();
}

DTimer::DTimer(duint32 loopCount , dfloat increasingDuration)
{
	initTimer();
	DTimer::increasingDuration = increasingDuration;
}

void DTimer::initTimer()
{
	diffTime = 0.0f;
	decreasingDuration = 0.0f;

	increasingDuration = 100;
#ifdef WIN32
	startTime = GetTickCount();
#else
	timeval t;
    gettimeofday(&t, NULL);
	startTime = (t.tv_usec - t.tv_usec) / 1000.0;   // us to ms
#endif
	currentTime = startTime;
#ifdef WIN32
	HighPerformanceTimerSupport = QueryPerformanceFrequency(&HighPerformanceFreq);
#endif
	
}

void DTimer::tick()
{
#ifdef WIN32
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
#else
	timeval t;
    gettimeofday(&t, NULL);
	currentTime = (t.tv_usec - t.tv_usec) / 1000.0;   // us to ms
#endif
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

void DTimer::setCurrentTime(float time)
{
	startTime = time;
}

float DTimer::getCurrentTime()
{
	dfloat time;
#ifdef WIN32
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
#else
	timeval t;
    gettimeofday(&t, NULL);
	time = (t.tv_usec - t.tv_usec) / 1000.0;   // us to ms
#endif
	return time;
}

void DTimer::stop()
{
  
}

float DTimer::value()
{
  return diffTime / increasingDuration;
}

void DTimer::setIncreasingDuration(float increasingDuration)
{
	DTimer::increasingDuration = increasingDuration;
}

float DTimer::getIncreasingDuration()
{
	return increasingDuration;
}

void DTimer::setDecreasingDuration(float decreasingDuration)
{
	DTimer::decreasingDuration = decreasingDuration;
}

float DTimer::getDecreasingDuration()
{
	return decreasingDuration;
}

END_NAMESPACE_DIRAC2D