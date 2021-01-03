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
	void Restart();



	Uint32 Read();
	float ReadSec();

	bool	running;
	Uint32	started_at;
	Uint32	stopped_at;
	float	time;

private:

};

#endif //__TIMER_H__