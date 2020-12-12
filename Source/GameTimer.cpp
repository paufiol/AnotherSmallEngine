#include "GameTimer.h"

// ---------------------------------------------
GameTimer::GameTimer()
	: running(false)
	, started_at(0)
	, stopped_at(0)
{
	Start();
}

// ---------------------------------------------
void GameTimer::Start()
{
	running = true;
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
void GameTimer::Stop()
{
	running = false;
	stopped_at = SDL_GetTicks();
}

// ---------------------------------------------
Uint32 GameTimer::Read()
{
	if (running == true)
	{
		return SDL_GetTicks() - started_at;
	}
	else
	{
		return stopped_at - started_at;
	}
}

float GameTimer::ReadSec() const
{
	return float(SDL_GetTicks() - started_at) / 1000.0f;
}