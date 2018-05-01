#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctTextures.h"
#include "ctAudio.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "ctEntities.h"
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

	if (!App->audio->PlayMusic(App->audio->SettingsBSO.c_str(), 1)) {
		LOG("Error playing music in ctMainMenu Start");
	}

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
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		App->audio->PlayFx(App->audio->mm_movement_fx);
		NavigateDown(labels);
	}
	//Go up
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		App->audio->PlayFx(App->audio->mm_movement_fx);
		NavigateUp(labels);
	}
	//Execute
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->gamepad.A == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		/*App->audio->PlayFx(menu_select_fx);*/
		ExecuteComand(labels);
	}
	//TurnUp volume
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->gamepad.CROSS_RIGHT == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		App->audio->PlayFx(App->audio->mm_movement_fx);
		TurnUp(labels);
	}
	//TurnDownVolume
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->gamepad.CROSS_LEFT == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		App->audio->PlayFx(App->audio->mm_movement_fx);
		TurnDown(labels);
	}

	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN || App->input->gamepad.B == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		fx_volume_label->current_state = STATE_NORMAL;
		back_label->current_state = STATE_EXECUTED;
		ExecuteComand(labels);
	}

	return true;
}

// Called each loop iteration
bool ctSettings::PostUpdate()
{
	bool ret = true;

	if (quit_pressed || App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool ctSettings::CleanUp()
{
	LOG("Freeing main_menu");

	App->audio->PauseMusic();

	App->gui->DeleteUIElement(*background);
	background = nullptr;
	App->gui->DeleteUIElement(*music_volume_label);
	music_volume_label = nullptr;
	App->gui->DeleteUIElement(*fx_volume_label);
	fx_volume_label = nullptr;
	App->gui->DeleteUIElement(*back_label);
	back_label = nullptr;
	App->gui->DeleteUIElement(*music_volume);
	music_volume = nullptr;
	App->gui->DeleteUIElement(*fx_volume);
	fx_volume = nullptr;
	App->gui->DeleteUIElement(*arrow);
	arrow = nullptr;

	for (int i = 0; i < labels.size(); i++)
	{
		App->gui->DeleteUIElement(*labels[i]);

	}
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
		App->audio->PlayFx(App->audio->mm_select_fx);
		if(App->fadeToBlack->FadeIsOver())
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
		Mix_Volume(-1, fx_volume_value);
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
		Mix_Volume(-1, fx_volume_value);
	}
}

uint ctSettings::NumberToPercentage(uint num, uint max_num) {
	uint percentage;

	percentage = (num * 100) / max_num;

	return percentage;
}