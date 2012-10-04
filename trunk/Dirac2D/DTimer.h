#ifndef _DTimer_H

#define _DTimer_H


#include "definitions.h"

#ifdef WIN32
#include <windows.h>
#else if defined(MACOSX) || defined(iOS)
#include <sys/time.h>
#endif


BEGIN_NAMESPACE_DIRAC2D
	
	class DTimer
	{
	public:
		
		void start();
		
		void stop();
		
		void tick();
		
		float value();
		
		void setCurrentTime(float time);
		
		double getCurrentTime();
		
		float getIncreasingDuration();
		
		float getDecreasingDuration();
		
		void setIncreasingDuration(float);
		
		void setDecreasingDuration(float);
		
		DTimer();
		
		DTimer(unsigned , float);
		
		
	private:
		
		unsigned int fps;
		double currentTime , startTime;
		double diffTime;
		
		unsigned loopCount;
		float increasingDuration;
		float decreasingDuration;
		
		void initTimer();
		
		double m_start_sec;
		double m_start_msec;
		
	};

END_NAMESPACE_DIRAC2D


#endif