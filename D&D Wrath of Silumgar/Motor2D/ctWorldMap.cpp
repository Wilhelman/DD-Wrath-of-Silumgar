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
#include "ctMainMenu.h"

#include "ctWorldMap.h"
#include "ctCombat.h"
#include "Entity.h"

#include "MiniHeroes.h"

#include "ctFadeToBlack.h"

#include "ctGui.h"
#include "UIElement.h"
#include "UIDecision.h"


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

bool ctWorldMap::Awake(pugi::xml_node& co)
{
	LOG("Loading World Map");
	bool ret = true;

	/* initialize random seed: */
	srand(time(NULL));

	//------------------
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("world_map.xml");
	pugi::xml_node config = doc.child("config").child("world_map");
	//-----------------

	world_map_tmx = config.child("world_map_tmx").attribute("name").as_string();
	name_spritesheet_world_map = config.child("spritesheet").attribute("name").as_string();

	//read rects from node
	for (pugi::xml_node map_element = config.child("world_map_elements").child("map_element"); map_element && ret; map_element = map_element.next_sibling("map_element"))
	{
		WorldMapElement* tmp_map_element = new WorldMapElement();

		tmp_map_element->tier = map_element.child("tier").attribute("type").as_uint();
		tmp_map_element->scene_name = map_element.child("scene").attribute("name").as_string();
		tmp_map_element->icon_rect = { map_element.child("icon_coords").attribute("x").as_int(), map_element.child("icon_coords").attribute("y").as_int(), map_element.child("icon_coords").attribute("width").as_int(), map_element.child("icon_coords").attribute("height").as_int() };
		tmp_map_element->decision = map_element.child("decision").attribute("name").as_string();
		tmp_map_element->option = map_element.child("option").attribute("name").as_string();

		//FOR for each entity in xml and pushback it to the vector
		for (pugi::xml_node entity = map_element.child("entity"); entity && ret; entity = entity.next_sibling("entity"))
		{
			std::string tmp(entity.attribute("name").as_string());
			if (tmp == "KOBOLD")
				tmp_map_element->entities.push_back(EntityType::KOBOLD);
			else if (tmp == "GNOLL")
				tmp_map_element->entities.push_back(EntityType::GNOLL);
			else if (tmp == "GOBLIN")
				tmp_map_element->entities.push_back(EntityType::GOBLIN);
			else if (tmp == "HEAVYGOBLIN")
				tmp_map_element->entities.push_back(EntityType::HEAVY_GOBLIN);
			else if (tmp == "ALCHEMISTGOBLIN")
				tmp_map_element->entities.push_back(EntityType::ALCHEMIST_GOBLIN);
			else if (tmp == "OWLBEAR")
				tmp_map_element->entities.push_back(EntityType::OWLBEAR);
			else if (tmp == "TRITON")
				tmp_map_element->entities.push_back(EntityType::TRITON);
			else if (tmp == "HELLHOUND")
				tmp_map_element->entities.push_back(EntityType::HELLHOUND);
			else if (tmp == "SKELETON")
				tmp_map_element->entities.push_back(EntityType::SKELETON);
			else if (tmp == "DARK_WARRIOR")
				tmp_map_element->entities.push_back(EntityType::DARK_WARRIOR);
			else if (tmp == "TROLL_LEADERS")
				tmp_map_element->entities.push_back(EntityType::TROLL_LEADERS);
			else if (tmp == "DISPLACER_BEAST")
				tmp_map_element->entities.push_back(EntityType::DISPLACER_BEAST);
			else if (tmp == "LICH")
				tmp_map_element->entities.push_back(EntityType::LICH_1);
		}

		all_map_elements.push_back(tmp_map_element);
		
	}



	return ret;
}

// Called before the first frame
bool ctWorldMap::Start()
{
	bool ret = true;

	start_combat_label=App->gui->AddUITextBox(320, 293, 10, 224, "Press ENTER or A to start combat", { 0,0,0,255 }, nullptr, Second_Font);

	//TO DELETE 1
	spritesheet_world_map = App->tex->Load(name_spritesheet_world_map.c_str());

	if (spritesheet_world_map == NULL) {
		LOG("Fail to load spritesheet in WorldMap!");
		ret = false;
	}

	App->entities->SpawnEntity(avatar_position.x,avatar_position.y, MINIHEROES);

	//Displaying map
	App->map->sceneName = world_map_tmx.c_str();
	App->map->Load(App->map->sceneName.c_str());
	App->map->LayersSetUp();

	avatar = (Entity*)App->entities->GetMiniheroes();

	if (!map_generated)
		GenerateNewRandomlyMap();

	// MORE TIER

	switch (App->map->actual_tier)
	{
	case TIER_MAP_1: {
		App->map->actual_tier = TierList::TIER_MAP_1;
		current_map_element = final_map_elements.at(0);
		break;
	}
	case TIER_MAP_2:
		App->map->actual_tier = TierList::TIER_MAP_2;
		break;
	case TIER_MAP_3:
		App->map->actual_tier = TierList::TIER_MAP_3;
		break;
	default:
		break;
	}

	if (App->map->actual_tier != TIER_MAP_1)
		spawn_decision = true;


	if (!App->audio->PlayMusic(App->audio->WorldMapBSO.c_str(), 1)) {

		LOG("Error playing music in ctMainMenu Start");
	}


	if (spawn_decision) {
		switch (App->map->actual_tier)
		{
		case TIER_MAP_2:
			decision = (UIDecision*)App->gui->AddUIDecision(50, 0, DECISION, arrow, current_map_element->decision, *final_map_elements.at(1), *final_map_elements.at(2), this);
			break;
		case TIER_MAP_3:
			if(avatar_position.y > 190)
				decision = (UIDecision*)App->gui->AddUIDecision(50, 0, DECISION, arrow, current_map_element->decision, *final_map_elements.at(4), *final_map_elements.at(5), this);
			else
				decision = (UIDecision*)App->gui->AddUIDecision(50, 0, DECISION, arrow, current_map_element->decision, *final_map_elements.at(3), *final_map_elements.at(4), this);
			break;
		case TIER_MAP_4:
			decision = (UIDecision*)App->gui->AddUIDecision(50, 0, DECISION, arrow, current_map_element->decision, *final_map_elements.at(6), *final_map_elements.at(7), this);
			break;
		case TIER_MAP_5:
			decision = (UIDecision*)App->gui->AddUIDecision(50, 0, DECISION, arrow, current_map_element->decision, *final_map_elements.at(8), *final_map_elements.at(8), this);
			break;
		case TIER_MAP_6:
			decision = (UIDecision*)App->gui->AddUIDecision(50, 0, DECISION, arrow, current_map_element->decision, *final_map_elements.at(9), *final_map_elements.at(10), this);
			break;
		case TIER_MAP_7:
			if (avatar_position.y > 190)
				decision = (UIDecision*)App->gui->AddUIDecision(50, 0, DECISION, arrow, current_map_element->decision, *final_map_elements.at(12), *final_map_elements.at(13), this);
			else
				decision = (UIDecision*)App->gui->AddUIDecision(50, 0, DECISION, arrow, current_map_element->decision, *final_map_elements.at(11), *final_map_elements.at(12), this);
			break;
		case TIER_MAP_8:
			decision = (UIDecision*)App->gui->AddUIDecision(50, 0, DECISION, arrow, current_map_element->decision, *final_map_elements.at(14), *final_map_elements.at(14), this);
			break;
		default:
			break;
		}
		
		decision->option_1->current_state = STATE_FOCUSED;
		arrow->SetParent(decision->option_1);
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

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || App->input->gamepad.Y == GAMEPAD_STATE::PAD_BUTTON_DOWN)
	{
		if (pauseMenu == nullptr) {
			pauseMenu = App->gui->AddUIPauseMenu(0, 0, this, nullptr);
			pause_menu_is_open = true;
		}
		else {
			App->gui->DeleteUIElement(*pauseMenu);
			pauseMenu = nullptr;
			pause_menu_is_open = false;
		}
	}


	if (!pause_menu_is_open)
	{
		if ((App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->gamepad.A == GAMEPAD_STATE::PAD_BUTTON_DOWN) && App->fadeToBlack->FadeIsOver() && decision == nullptr && App->task_manager->aux_task == nullptr) {

			App->combat->SetSceneName(current_map_element->scene_name);
			App->combat->entities_to_spawn = current_map_element->entities;

			if (App->fadeToBlack->FadeIsOver())
				App->fadeToBlack->FadeToBlackBetweenModules(this, App->combat, 1.0f);

		}
		


		/*	IF HAVE TO QUIT THE GAME
			App->main_menu->is_new_game = false;
					if (App->fadeToBlack->FadeIsOver())
						App->fadeToBlack->FadeToBlackBetweenModules(this, App->main_menu, 1.0f);
		*/

		// DECISION
		if (decision != nullptr)
		{

			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && decision != nullptr || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN)
			{
				App->audio->PlayFx(App->audio->wm_walk_fx);
				NavigateUp(options);
			}

			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && decision != nullptr || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN)
			{
				App->audio->PlayFx(App->audio->wm_walk_fx);
				NavigateDown(options);
			}

		}
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
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool ctWorldMap::CleanUp()
{
	LOG("Freeing ctWorldMap");

	App->audio->PauseMusic();

	App->tex->UnLoad(spritesheet_world_map);

	if (dwarf_level_up != nullptr)
		App->gui->DeleteUIElement(*dwarf_level_up);
	if (elf_level_up != nullptr)
		App->gui->DeleteUIElement(*elf_level_up);
	if (warrior_level_up != nullptr)
		App->gui->DeleteUIElement(*warrior_level_up);
	if (cleric_level_up != nullptr)
		App->gui->DeleteUIElement(*cleric_level_up);

	if (condition_lose != nullptr)
		App->gui->DeleteUIElement(*condition_lose);
	if (condition_win != nullptr)
		App->gui->DeleteUIElement(*condition_win);
	
	if (decision != nullptr)
		App->gui->DeleteUIElement(*decision);
	if (arrow != nullptr)
		App->gui->DeleteUIElement(*arrow);
	if (options.size() != 0)
		options.clear();


	if (App->entities->GetMiniheroes() != nullptr)
		App->entities->GetMiniheroes()->to_destroy = true;

	App->gui->DeleteUIElement(*start_combat_label);

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
	if (elementTriggered->type == DECISION)
	{
		if (ui_state == STATE_EXECUTED)
		{

			if (decision->option_1->current_state == STATE_FOCUSED)
			{
				current_map_element = decision->choice_02;
			}
			else
			{
				current_map_element = decision->choice_01;
			}

			//if (App->fadeToBlack->FadeIsOver())
				//App->fadeToBlack->FadeToBlackBetweenModules(this, App->combat, 3.0f);

			App->task_manager->AddTask(new MoveAvatarsToPosition(avatar, iPoint(current_map_element->coords_in_map.x + 5, current_map_element->coords_in_map.y + 30)));
			avatar_position = iPoint(current_map_element->coords_in_map.x + 5, current_map_element->coords_in_map.y + 30);

			App->task_manager->PerformAllTheTasks();

			App->gui->DeleteUIElement(*decision);
			decision = nullptr;
		}
	}


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
	vector<WorldMapElement*> tier_4_vec;
	vector<WorldMapElement*> tier_5_vec;
	vector<WorldMapElement*> tier_6_vec;
	vector<WorldMapElement*> tier_7_vec;
	vector<WorldMapElement*> tier_8_vec;

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
		case 4:
			tier_4_vec.push_back(this->all_map_elements.at(k));
			break;
		case 5:
			tier_5_vec.push_back(this->all_map_elements.at(k));
			break;
		case 6:
			tier_6_vec.push_back(this->all_map_elements.at(k));
			break;
		case 7:
			tier_7_vec.push_back(this->all_map_elements.at(k));
			break;
		case 8:
			tier_8_vec.push_back(this->all_map_elements.at(k));
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
	if (tier_3_vec.size() > 0) {
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
	}

	//TIER 4 randomize
	if (tier_4_vec.size() > 0) {
		random_number = -1;
		last_random = -1;
		for (int i = 0; i < App->map->tier_4_coords.size(); i++) {
			/* generate secret number: */
			while (random_number == last_random) {

				random_number = rand() % tier_4_vec.size();
				while (tier_4_vec.at(random_number)->coords_in_map.y != 0 && tier_4_vec.at(random_number)->coords_in_map.x != 0)
					random_number = rand() % tier_4_vec.size();

			}
			last_random = random_number;

			tier_4_vec.at(last_random)->coords_in_map = App->map->tier_4_coords.at(i);

			final_map_elements.push_back(tier_4_vec.at(last_random));
		}
	}

	//TIER 5 randomize
	if (tier_5_vec.size() > 0) {
		random_number = -1;
		last_random = -1;
		for (int i = 0; i < App->map->tier_5_coords.size(); i++) {
			/* generate secret number: */
			while (random_number == last_random) {

				random_number = rand() % tier_5_vec.size();
				while (tier_5_vec.at(random_number)->coords_in_map.y != 0 && tier_5_vec.at(random_number)->coords_in_map.x != 0)
					random_number = rand() % tier_5_vec.size();

			}
			last_random = random_number;

			tier_5_vec.at(last_random)->coords_in_map = App->map->tier_5_coords.at(i);

			final_map_elements.push_back(tier_5_vec.at(last_random));
		}
	}

	//TIER 6 randomize
	if (tier_6_vec.size() > 0) {
		random_number = -1;
		last_random = -1;
		for (int i = 0; i < App->map->tier_6_coords.size(); i++) {
			/* generate secret number: */
			while (random_number == last_random) {

				random_number = rand() % tier_6_vec.size();
				while (tier_6_vec.at(random_number)->coords_in_map.y != 0 && tier_6_vec.at(random_number)->coords_in_map.x != 0)
					random_number = rand() % tier_6_vec.size();

			}
			last_random = random_number;

			tier_6_vec.at(last_random)->coords_in_map = App->map->tier_6_coords.at(i);

			final_map_elements.push_back(tier_6_vec.at(last_random));
		}
	}

	//TIER 7 randomize
	if (tier_7_vec.size() > 0) {
		random_number = -1;
		last_random = -1;
		for (int i = 0; i < App->map->tier_7_coords.size(); i++) {
			/* generate secret number: */
			while (random_number == last_random) {

				random_number = rand() % tier_7_vec.size();
				while (tier_7_vec.at(random_number)->coords_in_map.y != 0 && tier_7_vec.at(random_number)->coords_in_map.x != 0)
					random_number = rand() % tier_7_vec.size();

			}
			last_random = random_number;

			tier_7_vec.at(last_random)->coords_in_map = App->map->tier_7_coords.at(i);

			final_map_elements.push_back(tier_7_vec.at(last_random));
		}
	}

	//TIER 7 randomize
	if (tier_8_vec.size() > 0) {
		random_number = -1;
		last_random = -1;
		for (int i = 0; i < App->map->tier_8_coords.size(); i++) {
			/* generate secret number: */
			while (random_number == last_random) {

				random_number = rand() % tier_8_vec.size();
				while (tier_8_vec.at(random_number)->coords_in_map.y != 0 && tier_8_vec.at(random_number)->coords_in_map.x != 0)
					random_number = rand() % tier_8_vec.size();

			}
			last_random = random_number;

			tier_8_vec.at(last_random)->coords_in_map = App->map->tier_8_coords.at(i);

			final_map_elements.push_back(tier_8_vec.at(last_random));
		}
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

WorldMapElement* ctWorldMap::SetDecision()
{
	WorldMapElement* tmp = nullptr;

	for (std::vector<WorldMapElement*>::iterator it = final_map_elements.begin(); it != final_map_elements.end(); it++)
	{
		if ((*it)->coords_in_map.x +6 < avatar->position.x || (*it)->coords_in_map.x  >= avatar->position.x)
		{
			if ((*it)->coords_in_map.y + 36 < avatar->position.x || (*it)->coords_in_map.y >= avatar->position.x)
			{
				tmp = (*it);
			}
		}

	}
	
	return tmp;

}

/*std::string ctWorldMap::SetOptions()
{
	std::string tmp[] = { };

	switch (App->map->actual_tier)
	{
	case TIER_MAP_1:
		tmp.=final_map_elements[1]->option.c_str();
		break;
	case TIER_MAP_2:
		App->map->actual_tier = TierList::TIER_MAP_3;
		break;
	case TIER_MAP_3:
		break;
	}

	


	//return tmp;
}*/