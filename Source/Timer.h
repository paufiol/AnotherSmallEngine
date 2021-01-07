#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"
#include "Dependencies\SDL\include\SDL.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Resume();
	void Stop();
	void Pause();
	void Restart();



	Uint32 Read();
	float ReadSec();
	
	bool running;
	Uint32	started_at;
	Uint32	stopped_at;
	Uint32	paused_at;
	Uint32	resumed_at;
	Uint32	time;

private:

};

#endif //__TIMER_H__