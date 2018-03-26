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
#include "ctWorldMap.h"
#include "j1Map.h"
#include "Cleric.h"

#include "ctFadeToBlack.h"



ctCombat::ctCombat() : ctModule()
{
	name = "combat";
}

// Destructor
ctCombat::~ctCombat()
{}

// Called before render is available

bool ctCombat::Awake()
{
	LOG("Loading Combat");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool ctCombat::Start()
{
	bool ret = true;

	//Displaying map
	App->map->sceneName = this->scene_name;
	LOG("%s", App->map->sceneName.c_str());
	App->map->Load(App->map->sceneName.c_str());
	App->map->LayersSetUp();

	App->entities->SpawnEntity(50, 100, CLERIC);

	return ret;
}

// Called each loop iteration
bool ctCombat::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ctCombat::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fadeToBlack->FadeIsOver())
		App->fadeToBlack->FadeToBlackBetweenModules(this, App->world_map, 1.0f);

	// Draw everything --------------------------------------
	App->map->Draw();

	return true;
}

// Called each loop iteration
bool ctCombat::PostUpdate()
{
	bool ret = true;


	return ret;
}

// Called before quitting
bool ctCombat::CleanUp()
{
	LOG("Freeing combat");

	//todo: despawn entities

	App->map->CleanUp();

	if(App->entities->GetCleric() != nullptr)
		App->entities->GetCleric()->to_destroy = true;

	return true;
}

bool ctCombat::Load(pugi::xml_node& load)
{
	bool ret = true;

	return ret;
}

bool ctCombat::Save(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}

void ctCombat::OnUITrigger(UIElement* elementTriggered, UI_State ui_state)
{
}

void ctCombat::SetSceneName(string new_scene_name)
{
	scene_name = new_scene_name;
}
