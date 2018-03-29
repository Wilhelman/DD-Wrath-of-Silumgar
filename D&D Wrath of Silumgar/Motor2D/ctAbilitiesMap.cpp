#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctTextures.h"
#include "ctAudio.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "ctEntities.h"
#include "j1Map.h"

#include "ctAbilitiesMap.h"
#include "ctCombat.h"

#include "ctFadeToBlack.h"


ctAbiltiesMap::ctAbiltiesMap() : ctModule()
{
	name = "abilities_map";
}

// Destructor
ctAbiltiesMap::~ctAbiltiesMap()
{}

// Called before render is available

bool ctAbiltiesMap::Awake(pugi::xml_node& config)
{
	LOG("Loading Abilities Map");
	bool ret = true;

	abilities_map_tmx = config.child("abilities_map_tmx").attribute("name").as_string();
	name_spritesheet_abilities_map = config.child("spritesheet").attribute("name").as_string();

	return ret;
}

// Called before the first frame
bool ctAbiltiesMap::Start()
{
	bool ret = true;

	spritesheet_abilities_map = App->tex->Load(name_spritesheet_abilities_map.c_str());

	if (spritesheet_abilities_map == NULL) {
		LOG("Fail to load spritesheet in WorldMap!");
		ret = false;
	}

	return ret;
}

// Called each loop iteration
bool ctAbiltiesMap::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ctAbiltiesMap::Update(float dt)
{



	// Draw everything --------------------------------------
	App->map->Draw();


	return true;
}

// Called each loop iteration
bool ctAbiltiesMap::PostUpdate()
{
	bool ret = true;


	return ret;
}

// Called before quitting
bool ctAbiltiesMap::CleanUp()
{
	LOG("Freeing ctWorldMap");

	

	App->map->CleanUp();

	return true;
}

bool ctAbiltiesMap::Load(pugi::xml_node& load)
{
	bool ret = true;

	return ret;
}

bool ctAbiltiesMap::Save(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}

void ctAbiltiesMap::OnUITrigger(UIElement* elementTriggered, UI_State ui_state)
{
}



