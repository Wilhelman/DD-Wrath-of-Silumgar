#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctTextures.h"
#include "ctAudio.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "ctEntities.h"
#include "ctCombat.h"
#include "j1Map.h"
#include "ctMap.h"

#include "ctFadeToBlack.h"



ctMap::ctMap() : ctModule()
{
	name = "map";
}

// Destructor
ctMap::~ctMap()
{}

// Called before render is available

bool ctMap::Awake()
{
	LOG("Loading Map");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool ctMap::Start()
{
	bool ret = true;

	/*if (!App->audio->PlayMusic("audio/music/8-bit_Detective.ogg")) {
	//ret = false;
	LOG("Error playing music in j1MainMenu Start");
	}*/

	return ret;
}

// Called each loop iteration
bool ctMap::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ctMap::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		this->quit_pressed = true;

	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN) {
		App->combat->active = true;
		LOG("X pressed");
		App->map->Load("TestTavern.tmx");
		this->active = false;
	}

	//if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fadeToBlack->FadeIsOver())
	//App->fadeToBlack->FadeToBlackBetweenModules(this, this, 1.0f);

	return true;
}

// Called each loop iteration
bool ctMap::PostUpdate()
{
	bool ret = true;

	if (quit_pressed)
		ret = false;

	return ret;
}

// Called before quitting
bool ctMap::CleanUp()
{
	LOG("Freeing map");

	return true;
}

bool ctMap::Load(pugi::xml_node& load)
{
	bool ret = true;

	return ret;
}

bool ctMap::Save(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}

void ctMap::OnUITrigger(UIElement* elementTriggered, UI_State ui_state)
{
}