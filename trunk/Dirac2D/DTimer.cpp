#include "DTimer.h"
#include <stdio.h>

BEGIN_NAMESPACE_DIRAC2D
	
	//float DTimer::currentTime = 0.0f;
	//float DTimer::startTime = 0.0f;
	//float DTimer::diffTime = 0.0f;
	//float DTimer::increasingDuration = 0.0f;
	//float DTimer::decreasingDuration = 0.0f;
#ifdef WIN32
	static LARGE_INTEGER HighPerformanceFreq;
	static BOOL HighPerformanceTimerSupport = FALSE;
#endif
#if defined(MACOSX) || defined(iOS)
#endif
	
	DTimer::DTimer()
	{
		initTimer();
	}
	
	DTimer::DTimer(unsigned loopCount , float increasingDuration)
	{
		initTimer();
		DTimer::increasingDuration = increasingDuration;
	}
	
	void DTimer::initTimer()
	{
		diffTime = 0.0f;
		decreasingDuration = 0.0f;
		
		increasingDuration = 1000;
		
#ifdef WIN32
		startTime = GetTickCount();
		HighPerformanceTimerSupport = QueryPerformanceFrequency(&HighPerformanceFreq);
#endif
		
#if defined(MACOSX) || defined(iOS)
		timeval t;
		gettimeofday(&t, 0);
		m_start_sec = t.tv_sec;
		m_start_msec = t.tv_usec * 0.001f;
		startTime = (double)t.tv_sec * 1000 + (double)t.tv_usec * 0.001f;
#endif
		currentTime = startTime;
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
#endif
		
#if defined(MACOSX) || defined(iOS)
		timeval t;
		gettimeofday(&t, 0);
		currentTime = (double)t.tv_sec * 1000 + (double)t.tv_usec * 0.001f;;	
#endif
		static long count = 0;
		count++;
		if ( diffTime > increasingDuration )
		{
			fps = count*1000.0/diffTime;
			printf("\nFPS = %d" , fps);
			count = 0;
			startTime = currentTime;
		}
		diffTime = ( currentTime - startTime );
	}
	
	void DTimer::setCurrentTime(float time)
	{
		startTime = time;
	}
	
	double DTimer::getCurrentTime()
	{
		double time;
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
#endif
		
#if defined(MACOSX) || defined(iOS)
		timeval t;
		gettimeofday(&t, 0);
		time =  (double)t.tv_sec  * 1000 + (double)t.tv_usec * 0.001f;
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