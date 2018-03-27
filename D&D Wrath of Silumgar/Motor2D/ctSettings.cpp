#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctTextures.h"
#include "ctAudio.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "ctEntities.h"
#include "UIBar.h"

#include "ctSettings.h"

#include "ctFadeToBlack.h"
#include "ctMainMenu.h"



ctSettings::ctSettings() : ctModule()
{
	name = "settings";
}

// Destructor
ctSettings::~ctSettings()
{}

// Called before render is available

bool ctSettings::Awake()
{
	LOG("Loading Main Menu");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool ctSettings::Start()
{
	bool ret = true;
	uint music_num = NumberToPercentage(music_volume_value, max_volume);
	char music_volume_char[(((sizeof music_num) * CHAR_BIT) + 2) / 3 + 2];
	sprintf_s(music_volume_char, "%d", music_num);

	uint fx_num = NumberToPercentage(fx_volume_value, max_volume);
	char fx_volume_char[(((sizeof fx_num) * CHAR_BIT) + 2) / 3 + 2];
	sprintf_s(fx_volume_char, "%d", fx_num);

	background = App->gui->AddUIImage(0, 0, { 337, 479, 800, 450 }, this);
	music_volume_label = App->gui->AddUILabel(35, 10, "Music Volume", { 255,255,255,255 }, 15, this);
	music_volume = App->gui->AddUILabel(150, 10, music_volume_char, { 255,255,255,255 }, 15, this);
	fx_volume_label = App->gui->AddUILabel(35, 30, "Fx Volume", { 255,255,255,255 }, 15, this);
	fx_volume = App->gui->AddUILabel(150, 30, fx_volume_char, { 255,255,255,255 }, 15, this);
	back_label = App->gui->AddUILabel(35, 50, "Back to Menu", { 255,255,255,255 }, 15, this);
	arrow = App->gui->AddUIImage(-10, 0, { 1333, 272, 7, 14 }, this);
	music_volume_label->current_state = STATE_FOCUSED;
	arrow->SetParent(music_volume_label);
	labels.push_back(music_volume_label);
	labels.push_back(fx_volume_label);
	labels.push_back(back_label);

	if (!App->audio->PlayMusic("audio/music/Visager_End_Credits.ogg")) {
		//ret = false;
		LOG("Error playing music in ctMainMenu Start");
	}

	HPbar = (UIBar*)App->gui->AddUIBar(0,150, 100,LIFEBAR,this);
	ManaBar = (UIBar*)App->gui->AddUIBar(0, 300, 100, MANABAR, this);
	EnemyBar = (UIBar*)App->gui->AddUIBar(0, 350, 100, ENEMYLIFEBAR, this);

	return ret;
}

// Called each loop iteration
bool ctSettings::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ctSettings::Update(float dt)
{

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
	//TurnUp volume
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
		TurnUp(labels);
	}
	//TurnDownVolume
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
		TurnDown(labels);
	}

	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
		HPbar->LowerBar(25);
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		HPbar->RecoverBar(1);
	}
	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
		ManaBar->LowerBar(25);
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		ManaBar->RecoverBar(1);
	}
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN) {
		EnemyBar->LowerBar(25);
	}
	if (App->input->GetKey(SDL_SCANCODE_Y) == KEY_DOWN) {
		EnemyBar->RecoverBar(1);
	}

	return true;
}

// Called each loop iteration
bool ctSettings::PostUpdate()
{
	bool ret = true;

	if (quit_pressed)
		ret = false;

	return ret;
}

// Called before quitting
bool ctSettings::CleanUp()
{
	LOG("Freeing main_menu");

	App->audio->StopMusic();

	App->gui->DeleteAllUIElements();
	background = nullptr;
	music_volume_label = nullptr;
	fx_volume_label = nullptr;
	back_label = nullptr;
	music_volume = nullptr;
	fx_volume = nullptr;
	arrow = nullptr;
	labels.clear();
	
	return true;
}

bool ctSettings::Load(pugi::xml_node& load)
{
	bool ret = true;

	return ret;
}

bool ctSettings::Save(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}

void ctSettings::OnUITrigger(UIElement* elementTriggered, UI_State ui_state)
{

}

void ctSettings::NavigateDown(std::vector<UIElement*> &current_vector) {
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

void ctSettings::NavigateUp(std::vector<UIElement*> &current_vector) {
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

void ctSettings::ExecuteComand(std::vector<UIElement*> &current_vector) {
	for (int i = 0; i < current_vector.size(); i++) {
		if (current_vector.at(i)->current_state == STATE_FOCUSED) {
			current_vector.at(i)->current_state = STATE_EXECUTED;
		}
	}
	
	if (music_volume_label->current_state == STATE_EXECUTED) {
		LOG("music_volume_label pressed");
		music_volume_label->current_state = STATE_FOCUSED;
	}
	if (fx_volume_label->current_state == STATE_EXECUTED) {
		LOG("fx_volume_label pressed");
		fx_volume_label->current_state = STATE_FOCUSED;
	}
	if (back_label->current_state == STATE_EXECUTED) {
		LOG("back_label pressed");
		App->fadeToBlack->FadeToBlackBetweenModules(this, App->main_menu, 1.0f);
	}

}

void ctSettings::TurnUp(std::vector<UIElement*> &current_vector) {
	if (music_volume_label->current_state == STATE_FOCUSED) {
		if (music_volume_value <= 120) {
			music_volume_value += 8;
			uint music_num = NumberToPercentage(music_volume_value, max_volume);
			char music_volume_char[(((sizeof music_num) * CHAR_BIT) + 2) / 3 + 2];
			sprintf_s(music_volume_char, "%d", music_num);
			App->gui->DeleteUIElement(*music_volume);
			music_volume = App->gui->AddUILabel(150, 10, music_volume_char, { 255,255,255,255 }, 15, this);
		}
		Mix_VolumeMusic(music_volume_value);
	}
	else if (fx_volume_label->current_state == STATE_FOCUSED) {
		if (fx_volume_value <= 120) {
			fx_volume_value += 8;
			uint fx_num = NumberToPercentage(fx_volume_value, max_volume);
			char fx_volume_char[(((sizeof fx_num) * CHAR_BIT) + 2) / 3 + 2];
			sprintf_s(fx_volume_char, "%d", fx_num);
			App->gui->DeleteUIElement(*fx_volume);
			fx_volume = App->gui->AddUILabel(150, 30, fx_volume_char, { 255,255,255,255 }, 15, this);
		}
		//Mix_VolumeChunk(Mix_GetChunk(1), fx_volume_value);
	}
}

void ctSettings::TurnDown(std::vector<UIElement*> &current_vector) {
	if (music_volume_label->current_state == STATE_FOCUSED) {
		if (music_volume_value >= 8) {
			music_volume_value -= 8;
			uint music_num = NumberToPercentage(music_volume_value, max_volume);
			char music_volume_char[(((sizeof music_num) * CHAR_BIT) + 2) / 3 + 2];
			sprintf_s(music_volume_char, "%d", music_num);
			App->gui->DeleteUIElement(*music_volume);
			music_volume = App->gui->AddUILabel(150, 10, music_volume_char, { 255,255,255,255 }, 15, this);
		}
		Mix_VolumeMusic(music_volume_value);
	}
	else if (fx_volume_label->current_state == STATE_FOCUSED) {
		if (fx_volume_value >= 8) {
			fx_volume_value -= 8;
			uint fx_num = NumberToPercentage(fx_volume_value, max_volume);
			char fx_volume_char[(((sizeof fx_num) * CHAR_BIT) + 2) / 3 + 2];
			sprintf_s(fx_volume_char, "%d", fx_num);
			App->gui->DeleteUIElement(*fx_volume);
			fx_volume = App->gui->AddUILabel(150, 30, fx_volume_char, { 255,255,255,255 }, 15, this);
		}
		//Mix_VolumeChunk(Mix_GetChunk(1), fx_volume_value);
	}
}

uint ctSettings::NumberToPercentage(uint num, uint max_num) {
	uint percentage;

	percentage = (num * 100) / max_num;

	return percentage;
}