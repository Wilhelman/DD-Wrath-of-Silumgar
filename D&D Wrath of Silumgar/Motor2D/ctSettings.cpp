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
	labels_bg = App->gui->AddUIImage(15, -1, { 314, 1046, 160, 135 }, this);
	music_volume_label = App->gui->AddUILabel(35, 10, "Music Volume", { 255,255,255,255 }, 25, this);
	music_volume = App->gui->AddUILabel(150, 10, music_volume_char, { 255,255,255,255 }, 25, this);
	fx_volume_label = App->gui->AddUILabel(35, 30, "Fx Volume", { 255,255,255,255 }, 25, this);
	fx_volume = App->gui->AddUILabel(150, 30, fx_volume_char, { 255,255,255,255 }, 25, this);
	controls_label = App->gui->AddUILabel(35, 50, "Controls:", { 255,255,255,255 }, 25, this);
	select_button_label = App->gui->AddUILabel(55, 70, "Select   ", { 255,255,255,255 }, 25, this);
	select_button_image = App->gui->AddUIImage(125, 70, { 1360, 224, 17, 17 }, this);
	back_button_label = App->gui->AddUILabel(55, 90, "Back    ", { 255,255,255,255 }, 25, this);
	back_button_image = App->gui->AddUIImage(125, 90, { 1342, 242, 17, 17 }, this);
	back_label = App->gui->AddUILabel(35, 110, "Back to Menu", { 255,255,255,255 }, 25, this);
	arrow = App->gui->AddUIImage(-10, 0, { 1333, 272, 7, 14 }, this);
	music_volume_label->current_state = STATE_FOCUSED;
	arrow->SetParent(music_volume_label);
	labels.push_back(music_volume_label);
	labels.push_back(fx_volume_label);
	labels.push_back(select_button_label);
	labels.push_back(back_button_label);
	labels.push_back(back_label);

	if(App->main_menu->key_select == 0)
		select_button_image = App->gui->AddUIImage(125, 70, { 1360, 224, 17, 17 }, this);
	else if(App->main_menu->key_select == 1)
		select_button_image = App->gui->AddUIImage(125, 70, { 1342, 242, 17, 17 }, this);
	else if (App->main_menu->key_select == 2)
		select_button_image = App->gui->AddUIImage(125, 70, { 1324, 224, 17, 17 }, this);
	else if (App->main_menu->key_select == 3)
		select_button_image = App->gui->AddUIImage(125, 70, { 1342, 206, 17, 17 }, this);

	if (App->main_menu->key_back == 0)
		back_button_image = App->gui->AddUIImage(125, 90, { 1360, 224, 17, 17 }, this);
	else if (App->main_menu->key_back == 1)
		back_button_image = App->gui->AddUIImage(125, 90, { 1342, 242, 17, 17 }, this);
	else if (App->main_menu->key_back == 2)
		back_button_image = App->gui->AddUIImage(125, 90, { 1324, 224, 17, 17 }, this);
	else if (App->main_menu->key_back == 3)
		back_button_image = App->gui->AddUIImage(125, 90, { 1342, 206, 17, 17 }, this);



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
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN || App->input->gamepad.JOYSTICK_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		App->audio->PlayFx(App->audio->mm_movement_fx);
		NavigateDown(labels);
	}
	//Go up
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN || App->input->gamepad.JOYSTICK_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		App->audio->PlayFx(App->audio->mm_movement_fx);
		NavigateUp(labels);
	}
	//Execute
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->GetGamepadButton(App->main_menu->key_select) == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		/*App->audio->PlayFx(menu_select_fx);*/
		if ((*select_button_label).current_state == STATE_NORMAL && (*back_button_label).current_state == STATE_NORMAL) {
			ExecuteComand(labels);
		}
	}

	//Change Controlls
	if (App->input->gamepad.A == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		if ((*select_button_label).current_state == STATE_FOCUSED) {
			App->main_menu->key_select = 0;
			ChangeControlImages(select_button_image);
			if (App->main_menu->key_select == App->main_menu->key_back) {
				App->main_menu->key_back = 1;
				ChangeControlImages(back_button_image);
			}
		}
		else if ((*back_button_label).current_state == STATE_FOCUSED) {
			App->main_menu->key_back = 0;
			ChangeControlImages(back_button_image);
			if (App->main_menu->key_select == App->main_menu->key_back) {
				App->main_menu->key_select = 1;
				ChangeControlImages(select_button_image);
			}
		}
	}
	if (App->input->gamepad.B == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		if ((*select_button_label).current_state == STATE_FOCUSED) {
			App->main_menu->key_select = 1;
			ChangeControlImages(select_button_image);
			if (App->main_menu->key_select == App->main_menu->key_back) {
				App->main_menu->key_back = 0;
				ChangeControlImages(back_button_image);
			}
		}
		else if ((*back_button_label).current_state == STATE_FOCUSED) {
			App->main_menu->key_back = 1;
			ChangeControlImages(back_button_image);
			if (App->main_menu->key_select == App->main_menu->key_back) {
				App->main_menu->key_select = 0;
				ChangeControlImages(select_button_image);
			}
		}
	}
	if (App->input->gamepad.X == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		if ((*select_button_label).current_state == STATE_FOCUSED) {
			App->main_menu->key_select = 3;
			ChangeControlImages(select_button_image);
			if (App->main_menu->key_select == App->main_menu->key_back) {
				App->main_menu->key_back = 1;
				ChangeControlImages(back_button_image);
			}
		}
		else if ((*back_button_label).current_state == STATE_FOCUSED) {
			App->main_menu->key_back = 3;
			ChangeControlImages(back_button_image);
			if (App->main_menu->key_select == App->main_menu->key_back) {
				App->main_menu->key_select = 0;
				ChangeControlImages(select_button_image);
			}
		}
	}
	if (App->input->gamepad.Y == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		if ((*select_button_label).current_state == STATE_FOCUSED) {
			App->main_menu->key_select = 2;
			ChangeControlImages(select_button_image);
			if (App->main_menu->key_select == App->main_menu->key_back) {
				App->main_menu->key_back = 1;
				ChangeControlImages(back_button_image);
			}
		}
		else if ((*back_button_label).current_state == STATE_FOCUSED) {
			App->main_menu->key_back = 2;
			ChangeControlImages(back_button_image);
			if (App->main_menu->key_select == App->main_menu->key_back) {
				App->main_menu->key_select = 0;
				ChangeControlImages(select_button_image);
			}
		}
	}

	//TurnUp volume
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->gamepad.CROSS_RIGHT == GAMEPAD_STATE::PAD_BUTTON_REPEAT || App->input->gamepad.JOYSTICK_RIGHT == GAMEPAD_STATE::PAD_BUTTON_REPEAT) {
		//App->audio->PlayFx(App->audio->mm_movement_fx);
		TurnUp(labels);
	}
	//TurnDownVolume
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->gamepad.CROSS_LEFT == GAMEPAD_STATE::PAD_BUTTON_REPEAT || App->input->gamepad.JOYSTICK_LEFT == GAMEPAD_STATE::PAD_BUTTON_REPEAT) {
		//App->audio->PlayFx(App->audio->mm_movement_fx);
		TurnDown(labels);
	}

	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN /*|| App->input->gamepad.B == GAMEPAD_STATE::PAD_BUTTON_DOWN*/) {
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
	App->gui->DeleteUIElement(*controls_label);
	controls_label = nullptr;
	App->gui->DeleteUIElement(*select_button_image);
	select_button_image = nullptr;
	App->gui->DeleteUIElement(*back_button_image);
	back_button_image = nullptr;
	App->gui->DeleteUIElement(*labels_bg);
	labels_bg = nullptr;

	for (int i = 0; i < labels.size(); i++)
	{
		App->gui->DeleteUIElement(*labels[i]);

	}
	
	labels.clear();
	App->gui->DeleteAllUIElements();
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
		if (music_volume_value <= 127) {
			music_volume_value += 1;
			uint music_num = NumberToPercentage(music_volume_value, max_volume);
			char music_volume_char[(((sizeof music_num) * CHAR_BIT) + 2) / 3 + 2];
			sprintf_s(music_volume_char, "%d", music_num);
			App->gui->DeleteUIElement(*music_volume);
			music_volume = App->gui->AddUILabel(150, 10, music_volume_char, { 255,255,255,255 }, 25, this);
		}
		Mix_VolumeMusic(music_volume_value);
	}
	else if (fx_volume_label->current_state == STATE_FOCUSED) {
		if (fx_volume_value <= 127) {
			fx_volume_value += 1;
			uint fx_num = NumberToPercentage(fx_volume_value, max_volume);
			char fx_volume_char[(((sizeof fx_num) * CHAR_BIT) + 2) / 3 + 2];
			sprintf_s(fx_volume_char, "%d", fx_num);
			App->gui->DeleteUIElement(*fx_volume);
			fx_volume = App->gui->AddUILabel(150, 30, fx_volume_char, { 255,255,255,255 }, 25, this);
		}
		Mix_Volume(-1, fx_volume_value);
	}
}

void ctSettings::TurnDown(std::vector<UIElement*> &current_vector) {
	if (music_volume_label->current_state == STATE_FOCUSED) {
		if (music_volume_value >= 1) {
			music_volume_value -= 1;
			uint music_num = NumberToPercentage(music_volume_value, max_volume);
			char music_volume_char[(((sizeof music_num) * CHAR_BIT) + 2) / 3 + 2];
			sprintf_s(music_volume_char, "%d", music_num);
			App->gui->DeleteUIElement(*music_volume);
			music_volume = App->gui->AddUILabel(150, 10, music_volume_char, { 255,255,255,255 }, 25, this);
		}
		Mix_VolumeMusic(music_volume_value);
	}
	else if (fx_volume_label->current_state == STATE_FOCUSED) {
		if (fx_volume_value >= 1) {
			fx_volume_value -= 1;
			uint fx_num = NumberToPercentage(fx_volume_value, max_volume);
			char fx_volume_char[(((sizeof fx_num) * CHAR_BIT) + 2) / 3 + 2];
			sprintf_s(fx_volume_char, "%d", fx_num);
			App->gui->DeleteUIElement(*fx_volume);
			fx_volume = App->gui->AddUILabel(150, 30, fx_volume_char, { 255,255,255,255 }, 25, this);
		}
		Mix_Volume(-1, fx_volume_value);
	}
}

uint ctSettings::NumberToPercentage(uint num, uint max_num) {
	uint percentage;

	percentage = (num * 100) / max_num;

	return percentage;
}

void ctSettings::ChangeControlImages(UIElement* control) {
	if (control == select_button_image) {
		if (select_button_image != nullptr) {
			App->gui->DeleteUIElement(*select_button_image);
		}
		if (App->main_menu->key_select == 0) {
			select_button_image = App->gui->AddUIImage(125, 70, { 1360, 224, 17, 17 }, this);
		}
		else if (App->main_menu->key_select == 1) {
			select_button_image = App->gui->AddUIImage(125, 70, { 1342, 242, 17, 17 }, this);
		}
		else if (App->main_menu->key_select == 2) {
			select_button_image = App->gui->AddUIImage(125, 70, { 1324, 224, 17, 17 }, this);
		}
		else if (App->main_menu->key_select == 3) {
			select_button_image = App->gui->AddUIImage(125, 70, { 1342, 206, 17, 17 }, this);
		}
	}
	else if (control == back_button_image) {
		if (back_button_image != nullptr) {
			App->gui->DeleteUIElement(*back_button_image);
		}
		if (App->main_menu->key_back == 0) {
			back_button_image = App->gui->AddUIImage(125, 90, { 1360, 224, 17, 17 }, this);
		}
		else if (App->main_menu->key_back == 1) {
			back_button_image = App->gui->AddUIImage(125, 90, { 1342, 242, 17, 17 }, this);
		}
		else if (App->main_menu->key_back == 2) {
			back_button_image = App->gui->AddUIImage(125, 90, { 1324, 224, 17, 17 }, this);
		}
		else if (App->main_menu->key_back == 3) {
			back_button_image = App->gui->AddUIImage(125, 90, { 1342, 206, 17, 17 }, this);
		}
	}
}