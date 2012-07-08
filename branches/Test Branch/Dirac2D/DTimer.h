#ifndef _DTIMER_H_
#define _DTIMER_H_

#include "definitions.h"

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

BEGIN_NAMESPACE_DIRAC2D

class DTimer
{
public:

	void start();

	void stop();

    void tick();

	dfloat value();
    
	void setCurrentTime(dfloat time);

	dfloat getCurrentTime();

	dfloat getIncreasingDuration();

	dfloat getDecreasingDuration();
	
	void setIncreasingDuration(dfloat);

	void setDecreasingDuration(dfloat);

	DTimer();

	DTimer(duint32 , dfloat);

	dfloat fps;

private:

	dfloat currentTime , startTime;
    dfloat diffTime;
	duint32 loopCount;
	dfloat increasingDuration;
	dfloat decreasingDuration;

	void initTimer();


};

END_NAMESPACE_DIRAC2D

#endif