#ifndef _STIMER_H

#define _STIMER_H

#include <windows.h>

namespace spider3d
{

class STimer
{
public:

	void start();

	void stop();

    void tick();

	float value();
    
	void setCurrentTime(float time);

	float getCurrentTime();

	float getIncreasingDuration();

	float getDecreasingDuration();
	
	void setIncreasingDuration(float);

	void setDecreasingDuration(float);

	STimer();

	STimer(unsigned , float);

	float fps;

private:

	float currentTime , startTime;
    float diffTime;
	unsigned loopCount;
	float increasingDuration;
	float decreasingDuration;

	void initTimer();


};

}

#endif