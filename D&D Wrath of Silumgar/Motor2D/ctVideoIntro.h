#ifndef __ctVIDEOINTRO_H__
#define __ctVIDEOINTRO_H__

#include "ctModule.h"
#include "ctTimer.h"

class ctVideoIntro : public ctModule
{
public:

	ctVideoIntro();

	// Destructor
	virtual ~ctVideoIntro();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:
	ctTimer introTime;
private:
	bool is_active = false;
};


#endif // __ctVIDEOINTRO_H__