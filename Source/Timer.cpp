// ----------------------------------------------------
// Timer.cpp
// Body for CPU Tick Timer class
// ----------------------------------------------------

#include "Timer.h"

// ---------------------------------------------
Timer::Timer()
	: running(false)
	, started_at(0)
	, stopped_at(0)
	, time(0.0f)
{
	Start();
}

// ---------------------------------------------
void Timer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Resume()
{
	running = true;
}

// ---------------------------------------------
void Timer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
}

// ---------------------------------------------
void Timer::Restart()
{
	running = false;
	started_at = 0;
	stopped_at = 0; 
}

// ---------------------------------------------
Uint32 Timer::Read()
{
	if(running == true)
	{
		return SDL_GetTicks() - started_at;
	}
	else
	{
		return stopped_at - started_at;
	}
}

float Timer::ReadSec()
{
	if (running == true)
	{
		time = float(SDL_GetTicks() - started_at) / 1000.0f;
		return time;
	}
	else
	{
		time = float(stopped_at - started_at) / 1000.0f;
		return time;
	}
}


