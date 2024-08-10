#pragma once

#include "..\..\AxGlobals.h"

class AXDLLCLASS AxTimer
{
private:
	long long timeAnchor;
	float lastSecondTime;
    int lastSecondTicks;
	float tickOutTime;

public:
    float time, actualTime, elapsedTime, elapsedActualTime, ticksPerSecond, ticksLastSecond, speed;
	unsigned long long ticksTotal;

	AxTimer(void);
	~AxTimer(void);

	void Reset();
	void Tick();
	void TickOut(float time);

	static long long GetFrequency();
	static long long GetAnchor();
	static float GetAnchorTime(long long anchor);
};