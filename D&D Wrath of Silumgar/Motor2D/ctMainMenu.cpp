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
#include "ctTabernScene.h"
#include "ctSettings.h"
#include "ctSkillTree.h"

#include "j1Language.h"



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

	/*background = App->gui->AddUIImage(0, 0, { 337, 479, 484, 324 }, this);
	continue_label = App->gui->AddUILabel(35, 10, App->language->GetDictionary().MM_continue_btn.c_str(), { 255,0,0,255 }, 15, this);
	new_game_label = App->gui->AddUILabel(35, 30, App->language->GetDictionary().MM_new_game_btn.c_str(), { 255,255,255,255 }, 15, this);
	settings_label = App->gui->AddUILabel(35, 50, App->language->GetDictionary().MM_settings_btn.c_str(), { 255,255,255,255 }, 15, this);
	about_label = App->gui->AddUILabel(35, 70, App->language->GetDictionary().MM_about_btn.c_str(), { 255,0,0,255 }, 15, this);
	quit_label = App->gui->AddUILabel(35, 90, App->language->GetDictionary().MM_quit_btn.c_str(), { 255,255,255,255 }, 15, this);
	arrow = App->gui->AddUIImage(-10, 0, { 1333, 272, 7, 14 }, this);
	//continue_label->current_state = STATE_FOCUSED;
	new_game_label->current_state = STATE_FOCUSED;
	//arrow->SetParent(continue_label);
	arrow->SetParent(new_game_label);
	//labels.push_back(continue_label);
	labels.push_back(new_game_label);
	labels.push_back(settings_label);
	//labels.push_back(about_label);
	labels.push_back(quit_label);

	Mix_VolumeMusic(App->settings->music_volume_value);
	Mix_Volume(-1, App->settings->fx_volume_value);*/
	std::string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi finibus arcu sed elit volutpat aliquet.";
	App->gui->AddUILabel(50,100,text, { 255,255,255,255 },12,this);
	App->gui->AddUITextBox(50, 120, 12, 200, text, { 255,255,255,255 });

	if (!App->audio->PlayMusic("audio/music/D&D Shadow Over Mystara - Song 00 Fanfare.ogg",1)) {
		
		LOG("Error playing music in ctMainMenu Start");
	}
	
	menu_move_fx = App->audio->LoadFx("audio/sounds/UI and Menus/MenuMove.wav");
	menu_select_fx = App->audio->LoadFx("audio/sounds/UI and Menus/MenuSelect.wav");
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
		
	//Go down
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		App->audio->PlayFx(menu_move_fx);
		NavigateDown(labels);
	}
	//Go up
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		App->audio->PlayFx(menu_move_fx);
		NavigateUp(labels);
	}
	//Execute
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->gamepad.A == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		ExecuteComand(labels);
	}

	return true;
}

// Called each loop iteration
bool ctMainMenu::PostUpdate()
{
	bool ret = true;

	if (quit_pressed ||App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool ctMainMenu::CleanUp()
{
	LOG("Freeing main_menu");

	if(!App->audio->UnLoadFx(menu_move_fx))
		LOG("Error unloading menu_move_fx");
	LOG("1");
	if(!App->audio->UnLoadFx(menu_select_fx))
		LOG("Error unloading menu_select_fx");

	App->audio->StopMusic();

	App->gui->DeleteUIElement(*arrow);
	arrow = nullptr;
	App->gui->DeleteUIElement(*background);
	background = nullptr;
	App->gui->DeleteUIElement(*continue_label);
	continue_label = nullptr;
	App->gui->DeleteUIElement(*new_game_label);
	new_game_label = nullptr;
	App->gui->DeleteUIElement(*settings_label);
	settings_label = nullptr;
	App->gui->DeleteUIElement(*about_label);
	about_label = nullptr;
	App->gui->DeleteUIElement(*quit_label);
	quit_label = nullptr;

	for (int i = 0; i < labels.size(); i++)
	{
		App->gui->DeleteUIElement(*labels[i]);

	}
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
		App->audio->PlayFx(menu_select_fx);


		if (!App->audio->PlayMusic("audio/music/D&D Shadow Over Mystara - Song 02  Dungeons & Dragons.ogg", 1, 0)) {

			LOG("Error playing music in ctMainMenu Start");
		}
		if (App->fadeToBlack->FadeIsOver())
			App->fadeToBlack->FadeToBlackBetweenModules(this, App->world_map, 5.0f);

		
	}
	if (new_game_label->current_state == STATE_EXECUTED) {
		LOG("new_game_label pressed");

		if (!App->audio->PlayMusic("audio/music/D&D Shadow Over Mystara - Song 02  Dungeons & Dragons.ogg", 1, 0)) {

			LOG("Error playing music in ctMainMenu Start");
		}
		if (App->fadeToBlack->FadeIsOver()) {
			is_new_game = true;
			App->fadeToBlack->FadeToBlackBetweenModules(this, App->tabern_scene, 1.0f);
		}
	}
	if (settings_label->current_state == STATE_EXECUTED) {
		LOG("settings_label pressed");
		if(App->fadeToBlack->FadeIsOver())
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