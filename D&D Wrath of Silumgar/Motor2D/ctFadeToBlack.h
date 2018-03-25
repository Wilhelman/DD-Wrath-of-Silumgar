#ifndef __ctFADETOBLACK_H__
#define __ctFADETOBLACK_H__

#include "ctModule.h"
#include "SDL\include\SDL_rect.h"
#include <string>

class ctFadeToBlack : public ctModule
{
public:
	ctFadeToBlack();
	~ctFadeToBlack();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PostUpdate();

	bool FadeToBlack(float time = 2.0f);
	bool FadeToBlackBetweenModules(ctModule* module_off, ctModule* module_on, float time = 2.0f, bool andLoad = false);
	bool FadeToKnowBlack(std::string lvlToFade, float time = 2.0f);
	bool FadeIsOver();

private:

	enum FadeStep
	{
		NONE_FADE,
		FADE_TO_BLACK,
		FADE_FROM_BLACK
	} current_step = FadeStep::NONE_FADE;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen = { 0, 0, 0, 0 };

	ctModule* moduleOff = nullptr;
	ctModule* moduleOn = nullptr;
	bool have_to_load = false;

public:
	std::string lvlName;
	bool F1 = false;
};

#endif //__MODULEFADETOBLACK_H__