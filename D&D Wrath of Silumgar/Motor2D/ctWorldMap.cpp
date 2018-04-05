#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctTextures.h"
#include "ctAudio.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "ctEntities.h"
#include "ctTaskManager.h"
#include "j1Map.h"

#include "ctWorldMap.h"
#include "ctCombat.h"
#include "Entity.h"

#include "MiniHeroes.h"

#include "ctFadeToBlack.h"

//randomize libs
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */



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

	/* initialize random seed: */
	srand(time(NULL));

	world_map_tmx = config.child("world_map_tmx").attribute("name").as_string();
	name_spritesheet_world_map = config.child("spritesheet").attribute("name").as_string();

	//read rects from node
	for (pugi::xml_node map_element = config.child("world_map_elements").child("map_element"); map_element && ret; map_element = map_element.next_sibling("map_element"))
	{
		WorldMapElement* tmp_map_element = new WorldMapElement();

		tmp_map_element->tier = map_element.child("tier").attribute("type").as_uint();
		tmp_map_element->scene_name = map_element.child("scene").attribute("name").as_string();
		tmp_map_element->icon_rect = { map_element.child("icon_coords").attribute("x").as_int(), map_element.child("icon_coords").attribute("y").as_int(), map_element.child("icon_coords").attribute("width").as_int(), map_element.child("icon_coords").attribute("height").as_int() } ;

		//TODO 
		//FOR for each entity in xml and pushback it to the vector


		all_map_elements.push_back(tmp_map_element);
		
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
	spritesheet_world_map = App->tex->Load(name_spritesheet_world_map.c_str());

	if (spritesheet_world_map == NULL) {
		LOG("Fail to load spritesheet in WorldMap!");
		ret = false;
	}

	App->entities->SpawnEntity(40,170, MINIHEROES);

	//Displaying map
	App->map->sceneName = world_map_tmx.c_str();
	App->map->Load(App->map->sceneName.c_str());
	App->map->LayersSetUp();

	avatar = (Entity*)App->entities->GetMiniheroes();

	if (!map_generated)
		GenerateNewRandomlyMap();


	//Decision call example

	//decision = (UIDecision*)App->gui->AddUIDecision(50, 0, 1, arrow, options, this); 
	//(*options.rbegin())->current_state = STATE_FOCUSED;
	//arrow->SetParent(*options.rbegin());

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


	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && decision!=nullptr)
	{
		NavigateUp(options);
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && decision != nullptr)
	{
		NavigateDown(options);
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		
		App->task_manager->AddTask(new MoveForward(avatar, { avatar->position.x + 40,avatar->position.y - 20 }));
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		
		App->task_manager->AddTask(new MoveForward(avatar, { avatar->position.x + 40,avatar->position.y + 20 }));
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fadeToBlack->FadeIsOver()) {

		WorldMapElement* tmp_map_element = final_map_elements.back();

		App->combat->SetSceneName(tmp_map_element->scene_name);

		App->fadeToBlack->FadeToBlackBetweenModules(this, App->combat,1.0f);

	}

	if (App->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		App->render->scale_factor += 0.1;
	}
	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
	{
		App->render->scale_factor -= 0.1;
	}

	// Draw everything --------------------------------------
	App->map->Draw();

	for (int i = 0; i < final_map_elements.size(); i++)
	{
		App->render->Blit(spritesheet_world_map, final_map_elements.at(i)->coords_in_map.x, final_map_elements.at(i)->coords_in_map.y, &final_map_elements.at(i)->icon_rect, 1.0f);
	}

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

	if (App->entities->GetMiniheroes() != nullptr)
		App->entities->GetMiniheroes()->to_destroy = true;

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

void ctWorldMap::GenerateNewRandomlyMap()
{
	//this gets the coords of the tile to set the element maps
	App->map->setAllLogicForMap();

	vector<WorldMapElement*> tier_1_vec;
	vector<WorldMapElement*> tier_2_vec;
	vector<WorldMapElement*> tier_3_vec;

	for (int k = 0; k < this->all_map_elements.size(); k++) {
		switch (this->all_map_elements.at(k)->tier)
		{
		case 1:
			tier_1_vec.push_back(this->all_map_elements.at(k));
			break;
		case 2:
			tier_2_vec.push_back(this->all_map_elements.at(k));
			break;
		case 3:
			tier_3_vec.push_back(this->all_map_elements.at(k));
			break;
		default:
			break;
		}
	}

	int random_number = -1;
	int last_random = -1;

	//TIER 1 randomize (useless but whatever)
	for (int i = 0; i < App->map->tier_1_coords.size(); i++) {
		/* generate secret number: */
		int random_number = (rand() % tier_1_vec.size());

		tier_1_vec.at(random_number)->coords_in_map = App->map->tier_1_coords.at(i);
		
		final_map_elements.push_back(tier_1_vec.at(random_number));
	}

	//TIER 2 randomize
	random_number = -1;
	last_random = -1;
	for (int i = 0; i < App->map->tier_2_coords.size(); i++) {
		/* generate secret number: */
		while(last_random == random_number)
			random_number = rand() % tier_2_vec.size();
		last_random = random_number;

		tier_2_vec.at(random_number)->coords_in_map = App->map->tier_2_coords.at(i);

		final_map_elements.push_back(tier_2_vec.at(random_number));
	}

	//TIER 3 randomize
	random_number = -1;
	last_random = -1;
	for (int i = 0; i < App->map->tier_3_coords.size(); i++) {
		/* generate secret number: */
		while (random_number == last_random) {

			random_number = rand() % tier_3_vec.size();
			while (tier_3_vec.at(random_number)->coords_in_map.y != 0 && tier_3_vec.at(random_number)->coords_in_map.x != 0)
				random_number = rand() % tier_3_vec.size();

		}
		last_random = random_number;

		tier_3_vec.at(last_random)->coords_in_map = App->map->tier_3_coords.at(i);

		final_map_elements.push_back(tier_3_vec.at(last_random));
	}



	map_generated = true;
}

void ctWorldMap::NavigateDown(std::vector<UIElement*> &current_vector) {
	std::vector<UIElement*>::const_iterator it_vector = current_vector.begin();
	while (it_vector != current_vector.end()) {
		if ((*it_vector)->current_state == STATE_FOCUSED) {
			if ((*it_vector) != current_vector.back()) {
				(*it_vector)->current_state = STATE_NORMAL;
				it_vector++;
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
				break;
			}
			else
			{

				(*it_vector)->current_state = STATE_NORMAL;
				it_vector = current_vector.begin();
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
			}
		}
		it_vector++;
	}
}

void ctWorldMap::NavigateUp(std::vector<UIElement*> &current_vector) {
	std::vector<UIElement*>::const_iterator it_vector = current_vector.begin();
	while (it_vector != current_vector.end()) {
		if ((*it_vector)->current_state == STATE_FOCUSED) {
			if ((*it_vector) != current_vector.front()) {
				(*it_vector)->current_state = STATE_NORMAL;
				it_vector--;
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
				break;
			}
			else
			{
				(*it_vector)->current_state = STATE_NORMAL;
				it_vector = current_vector.end() - 1;
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
			}
		}
		it_vector++;
	}
}