#ifndef __GAMETIMER_H__
#define __GAMETIMER_H__

#include "Globals.h"
#include "Dependencies\SDL\include\SDL.h"
#include "Timer.h"

class GameTimer
{
public:

	// Constructor
	GameTimer();

	void Start();
	void Stop();

	Uint32 Read();
	float ReadSec() const;

	bool	running;
	Uint32	started_at;
	Uint32	stopped_at;

	const Timer gameTimer;

private:

};

#endif //__TIMER_H__
