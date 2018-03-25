#include <math.h>
#include "ctApp.h"
#include "ctFadeToBlack.h"
#include "ctRender.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"
#include "ctWindow.h"
/*#include "j1Collider.h"
#include "j1Map.h"
#include "Player.h"
#include "j1Entities.h"
#include "j1Pathfinding.h"
#include "j1Particles.h"*/
#include "ctLog.h"


ctFadeToBlack::ctFadeToBlack()
{
	name = "fadeToBlack";
}

ctFadeToBlack::~ctFadeToBlack()
{}

bool ctFadeToBlack::Awake(pugi::xml_node& conf)
{
	uint winWidth, winHeight;

	App->win->GetWindowSize(winWidth, winHeight);

	screen = { 0, 0,  (int)winWidth * (int)App->win->GetScale(), (int)winHeight * (int)App->win->GetScale() };

	return true;
}

// Load assets
bool ctFadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	lvlName = "";
	F1 = false;
	return true;
}

// Update: draw background
bool ctFadeToBlack::PostUpdate()
{
	bool ret = true;
	if (current_step == FadeStep::NONE_FADE)
		return ret;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
	case FadeStep::FADE_TO_BLACK:
	{
		if (now >= total_time)
		{

			if (moduleOn != nullptr && moduleOff != nullptr) {
				this->moduleOff->CleanUp();
				this->moduleOff->active = false;

				this->moduleOn->Start();
				this->moduleOn->active = true;

			}
			
			total_time += total_time;
			start_time = SDL_GetTicks();
			current_step = FadeStep::FADE_FROM_BLACK;

		}
	} break;

	case FadeStep::FADE_FROM_BLACK:
	{
		normalized = 1.0f - normalized;

		if (now >= total_time)
			current_step = FadeStep::NONE_FADE;
	} break;
	}

	// Finally render the black square with alpha on the screen
	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return ret;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool ctFadeToBlack::FadeToBlack(float time)
{
	lvlName = "";
	bool ret = false;
	if (current_step == FadeStep::NONE_FADE)
	{
		current_step = FadeStep::FADE_TO_BLACK;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}

bool ctFadeToBlack::FadeToKnowBlack(std::string lvlToFade, float time)
{
	lvlName = lvlToFade;
	bool ret = false;
	if (current_step == FadeStep::NONE_FADE)
	{
		current_step = FadeStep::FADE_TO_BLACK;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}

bool ctFadeToBlack::FadeToBlackBetweenModules(ctModule* module_off, ctModule* module_on, float time, bool andLoad)
{
	bool ret = false;

	have_to_load = andLoad;
	moduleOff = module_off;
	moduleOn = module_on;

	if (current_step == FadeStep::NONE_FADE)
	{
		current_step = FadeStep::FADE_TO_BLACK;
		start_time = SDL_GetTicks();
		total_time = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}

bool ctFadeToBlack::FadeIsOver() {
	bool ret = true;

	if (current_step == FadeStep::NONE_FADE)
		ret = true;
	else
		ret = false;

	return ret;
}