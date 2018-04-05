#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctTextures.h"
#include "ctAudio.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "ctEntities.h"

#include "ctMainMenu.h"

#include "ctFadeToBlack.h"
#include "ctWorldMap.h"
#include "ctSettings.h"
#include "ctSkillTree.h"



ctMainMenu::ctMainMenu() : ctModule()
{
	name = "main_menu";
}

// Destructor
ctMainMenu::~ctMainMenu()
{}

// Called before render is available

bool ctMainMenu::Awake()
{
	LOG("Loading Main Menu");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool ctMainMenu::Start()
{
	bool ret = true;

	background = App->gui->AddUIImage(0, 0, { 337, 479, 484, 324 }, this);
	continue_label = App->gui->AddUILabel(35, 10, "Continue", { 255,255,255,255 }, 15, this);
	new_game_label = App->gui->AddUILabel(35, 30, "New Game", { 255,255,255,255 }, 15, this);
	settings_label = App->gui->AddUILabel(35, 50, "Settings", { 255,255,255,255 }, 15, this);
	about_label = App->gui->AddUILabel(35, 70, "About", { 255,255,255,255 }, 15, this);
	quit_label = App->gui->AddUILabel(35, 90, "Quit", { 255,255,255,255 }, 15, this);
	arrow = App->gui->AddUIImage(-10, 0, { 1333, 272, 7, 14 }, this);
	continue_label->current_state = STATE_FOCUSED;
	arrow->SetParent(continue_label);
	labels.push_back(continue_label);
	labels.push_back(new_game_label);
	labels.push_back(settings_label);
	labels.push_back(about_label);
	labels.push_back(quit_label);


	if (!App->audio->PlayMusic("audio/music/Visager_End_Credits.ogg")) {
		//ret = false;
		LOG("Error playing music in ctMainMenu Start");
	}

	return ret;
}

// Called each loop iteration
bool ctMainMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ctMainMenu::Update(float dt)
{

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fadeToBlack->FadeIsOver()) {
		App->fadeToBlack->FadeToBlackBetweenModules(this, App->world_map,1.0f);
	}
		
	//Go down
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		NavigateDown(labels);
	}
	//Go up
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		NavigateUp(labels);
	}
	//Execute
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		ExecuteComand(labels);
	}

	return true;
}

// Called each loop iteration
bool ctMainMenu::PostUpdate()
{
	bool ret = true;

	if (quit_pressed)
		ret = false;

	return ret;
}

// Called before quitting
bool ctMainMenu::CleanUp()
{
	LOG("Freeing main_menu");

	App->audio->StopMusic();

	App->gui->DeleteAllUIElements();
	background = nullptr;
	continue_label = nullptr;
	new_game_label = nullptr;
	settings_label = nullptr;
	about_label = nullptr;
	quit_label = nullptr;
	arrow = nullptr;
	labels.clear();
	
	return true;
}

bool ctMainMenu::Load(pugi::xml_node& load)
{
	bool ret = true;

	return ret;
}

bool ctMainMenu::Save(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}

void ctMainMenu::OnUITrigger(UIElement* elementTriggered, UI_State ui_state)
{

}

void ctMainMenu::NavigateDown(std::vector<UIElement*> &current_vector) {
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

void ctMainMenu::NavigateUp(std::vector<UIElement*> &current_vector) {
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
				it_vector = current_vector.end()-1;
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
			}
		}
		it_vector++;
	}
}

void ctMainMenu::ExecuteComand(std::vector<UIElement*> &current_vector) {
	for (int i = 0; i < current_vector.size(); i++) {
		if (current_vector.at(i)->current_state == STATE_FOCUSED) {
			current_vector.at(i)->current_state = STATE_EXECUTED;
		}
	}
	
	if (continue_label->current_state == STATE_EXECUTED) {
		LOG("continue_label pressed");
		App->fadeToBlack->FadeToBlackBetweenModules(this, App->world_map, 1.0f);
	}
	if (new_game_label->current_state == STATE_EXECUTED) {
		LOG("new_game_label pressed");
		App->fadeToBlack->FadeToBlackBetweenModules(this, App->world_map, 1.0f);
	}
	if (settings_label->current_state == STATE_EXECUTED) {
		LOG("settings_label pressed");
		App->fadeToBlack->FadeToBlackBetweenModules(this, App->settings, 1.0f);
		settings_label->current_state = STATE_FOCUSED;
	}
	if (about_label->current_state == STATE_EXECUTED) {
		LOG("about_label pressed");
		about_label->current_state = STATE_FOCUSED;
	}
	if (quit_label->current_state == STATE_EXECUTED) {
		LOG("quit_label pressed");
		this->quit_pressed = true;
	}

}