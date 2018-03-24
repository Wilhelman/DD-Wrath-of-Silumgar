#ifndef __ctTIMER_H__
#define __ctTIMER_H__

#include "ctDefs.h"

class ctTimer
{
public:

	// Constructor
	ctTimer();

	void Start();
	void SetStartTime(uint32 time);
	uint32 Read() const;
	float ReadSec() const;

private:
	uint32	started_at = 0;
};

#endif //__ctTIMER_H__