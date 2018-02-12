#ifndef __CTPERFTIMER_H__
#define __CTPERFTIMER_H__

#include "ctDefs.h"

class ctPerfTimer
{
public:

	// Constructor
	ctPerfTimer();

	void Start();
	double ReadMs() const;
	uint64 ReadTicks() const;

private:
	uint64	started_at = 0;
	static uint64 frequency;
};

#endif //__CTPERFTIMER_H__