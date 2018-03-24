// ----------------------------------------------------
// ctPerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "ctPerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 ctPerfTimer::frequency = 0;

// ---------------------------------------------
ctPerfTimer::ctPerfTimer()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();

	Start();
}

// ---------------------------------------------
void ctPerfTimer::Start()
{
	started_at = SDL_GetPerformanceCounter();
}

// ---------------------------------------------
double ctPerfTimer::ReadMs() const
{
	return 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}

// ---------------------------------------------
uint64 ctPerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - started_at;
}