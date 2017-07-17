//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTimer.h"

#include "..\..\Tools\Platform\AxPlatformUtils.h"

AxTimer::AxTimer(void)
{
	this->Reset();
}


AxTimer::~AxTimer(void)
{
}

void AxTimer::Reset()
{
	this->time = 0.0f;
	this->actualTime = 0.0f;
	this->elapsedTime = 0.0f;
	this->elapsedActualTime = 0.0f;
	this->ticksPerSecond = 0.0f;
	this->ticksLastSecond = 0.0f;
	this->speed = 1.0f;
	this->ticksTotal = 0;
	
	this->lastSecondTicks = 0;
    this->lastSecondTime = 0.0f;

	this->tickOutTime = 0.0;

	this->timeAnchor = AxTimer::GetAnchor();
}

void AxTimer::Tick()
{
	this->actualTime = AxTimer::GetAnchorTime(this->timeAnchor);
	this->time = this->actualTime * this->speed + this->tickOutTime;
    this->elapsedTime += this->time;
	this->elapsedActualTime += this->actualTime;
    this->ticksPerSecond = 1.0f / this->actualTime;
	this->ticksTotal++;
	this->lastSecondTicks++;
    this->lastSecondTime += this->actualTime;
    if (this->lastSecondTime >= 1.0f)
    {
        this->ticksLastSecond = (float)this->lastSecondTicks / this->lastSecondTime;
        this->lastSecondTicks = 0;
        this->lastSecondTime = 0.0f;
    }
        
	this->tickOutTime = 0.0;

	this->timeAnchor = AxTimer::GetAnchor();
}

void AxTimer::TickOut(float time)
{
	this->tickOutTime = time;
}

long long AxTimer::GetFrequency()
{
	return AxPlatformUtils::GetPerformanceCounterFrequency();
}

long long AxTimer::GetAnchor()
{
	return AxPlatformUtils::GetPerformanceCounter();
}

float AxTimer::GetAnchorTime(long long anchor)
{
	return (float)(AxTimer::GetAnchor() - anchor) / (float)AxTimer::GetFrequency();
}
