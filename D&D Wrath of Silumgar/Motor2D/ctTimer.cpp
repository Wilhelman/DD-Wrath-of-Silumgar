// ----------------------------------------------------
// ctTimer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "ctTimer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
ctTimer::ctTimer()
{
}

// ---------------------------------------------
void ctTimer::Start()
{
	started_at = SDL_GetTicks();
}

// ---------------------------------------------

void ctTimer::SetStartTime(uint32 time) {
	started_at = SDL_GetTicks() + time;
}

// ---------------------------------------------
uint32 ctTimer::Read() const
{
	return SDL_GetTicks() - started_at;
}

// ---------------------------------------------
float ctTimer::ReadSec() const
{
	return float(SDL_GetTicks() - started_at) / 1000.0f;
}