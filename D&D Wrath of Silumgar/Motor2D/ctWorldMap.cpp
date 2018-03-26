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

#include "ctWorldMap.h"
#include "ctCombat.h"

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

	world_map_tmx = config.child("world_map_tmx").attribute("name").as_string();

	//read rects from node
	for (pugi::xml_node map_element = config.child("world_map_elements").child("map_element"); map_element && ret; map_element = map_element.next_sibling("map_element"))
	{
		WorldMapElement* tmp_map_element = new WorldMapElement();

		tmp_map_element->tier = map_element.child("tier").attribute("type").as_uint();
		tmp_map_element->scene_name = map_element.child("scene").attribute("name").as_string();

		//TODO FOR for each entity in xml and pushback it to the vector


		map_elements.push_back(tmp_map_element);
		
	}

	//TO DELETE 1
	/*spritesheet_name = config.child("spritesheet").attribute("name").as_string();

	//read rects from node
	for (pugi::xml_node rects = config.child("rects").child("rect"); rects && ret; rects = rects.next_sibling("rect"))
	{
		std::string tmp(rects.attribute("name").as_string());

		if (tmp == "background")
			LoadRect(rects, &background);
	}*/

	return ret;
}

// Called before the first frame
bool ctWorldMap::Start()
{
	bool ret = true;

	//TO DELETE 1
	/*spritesheet = App->tex->Load(spritesheet_name.c_str());

	if (spritesheet == NULL) {
		LOG("Fail to load spritesheet in WorldMap!");
		ret = false;
	}*/

	//Displaying map
	App->map->sceneName = world_map_tmx.c_str();
	App->map->Load(App->map->sceneName.c_str());
	App->map->LayersSetUp();

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

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fadeToBlack->FadeIsOver()) {

		WorldMapElement* tmp_map_element = map_elements.back();

		App->combat->SetSceneName(tmp_map_element->scene_name);

		App->fadeToBlack->FadeToBlackBetweenModules(this, App->combat,1.0f);

	}

	// Draw everything --------------------------------------
	App->map->Draw();

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

	//TODO CLEAN THIS
	/*std::vector<WorldMapElement*>::const_iterator it_map_elements = map_elements.begin();

	while (it_map_elements != map_elements.end()) {
		delete *it_map_elements;
		it_map_elements++;
	}

	map_elements.clear();*/

	App->map->CleanUp();

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