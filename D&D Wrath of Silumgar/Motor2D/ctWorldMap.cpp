#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctTextures.h"
#include "ctAudio.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "ctEntities.h"

#include "ctWorldMap.h"

#include "ctFadeToBlack.h"



ctWorldMap::ctWorldMap() : ctModule()
{
	name = "world_map";
}

// Destructor
ctWorldMap::~ctWorldMap()
{}

// Called before render is available

bool ctWorldMap::Awake(pugi::xml_node& config)
{
	LOG("Loading World Map");
	bool ret = true;

	spritesheet_name = config.child("spritesheet").attribute("name").as_string();

	//read rects from node
	for (pugi::xml_node rects = config.child("rects").child("rect"); rects && ret; rects = rects.next_sibling("rect"))
	{
		std::string tmp(rects.attribute("name").as_string());

		if (tmp == "background")
			LoadRect(rects, &background);
	}

	return ret;
}

// Called before the first frame
bool ctWorldMap::Start()
{
	bool ret = true;

	spritesheet = App->tex->Load(spritesheet_name.c_str());

	if (spritesheet == NULL) {
		LOG("Fail to load spritesheet in WorldMap!");
		ret = false;
	}

	return ret;
}

// Called each loop iteration
bool ctWorldMap::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ctWorldMap::Update(float dt)
{

	// Draw everything --------------------------------------
	App->render->Blit(spritesheet, 0, 0, &background, 1.0f); // map's background

	return true;
}

// Called each loop iteration
bool ctWorldMap::PostUpdate()
{
	bool ret = true;


	return ret;
}

// Called before quitting
bool ctWorldMap::CleanUp()
{
	LOG("Freeing ctWorldMap");

	return true;
}

bool ctWorldMap::Load(pugi::xml_node& load)
{
	bool ret = true;

	return ret;
}

bool ctWorldMap::Save(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}

void ctWorldMap::OnUITrigger(UIElement* elementTriggered, UI_State ui_state)
{
}

void ctWorldMap::LoadRect(pugi::xml_node rect_node, SDL_Rect* rect)
{
	rect->x = rect_node.attribute("x").as_float();
	rect->y = rect_node.attribute("y").as_float();
	rect->w = rect_node.attribute("width").as_float();
	rect->h = rect_node.attribute("height").as_float();
}