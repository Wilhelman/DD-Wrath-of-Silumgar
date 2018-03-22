#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctTextures.h"
#include "ctAudio.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "ctEntities.h"
#include "ctCombat.h"
#include "ctMainMenu.h"
#include "ctAbout.h"


#include "ctSettings.h"

#include "ctFadeToBlack.h"



ctSettings::ctSettings() : ctModule()
{
	name = "main_menu";
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
	
	//music_volume = App->gui->AddUILabel(40, 45, "Music volume", { 255,255,255,255 }, nullptr);
	//fx_volume = App->gui->AddUILabel(40, 90, "Fx volume", { 255,255,255,255 }, nullptr);
	//if (!App->audio->PlayMusic("audio/music/Visager_End_Credits.ogg")) {
	//	//ret = false;
	//	LOG("Error playing music in j1MainMenu Start");
	//}

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

	char music_volume_char[(((sizeof music_volume_value) * CHAR_BIT) + 2) / 3 + 2];
	sprintf_s(music_volume_char, "%d", music_volume_value);

	char fx_volume_char[(((sizeof fx_volume_value) * CHAR_BIT) + 2) / 3 + 2];
	sprintf_s(fx_volume_char, "%d", fx_volume_value);

	if (first_update == true)
	{
		music_volume_text = App->gui->AddUILabel(125, 20, "Music volume", { 255,255,255,255 }, nullptr);
		music_volume_num = App->gui->AddUILabel(210, 20, music_volume_char, { 255,255,255,255 }, nullptr);
		fx_volume_text = App->gui->AddUILabel(125, 80, "Fx volume", { 255,255,255,255 }, nullptr);
		fx_volume_num = App->gui->AddUILabel(210, 80, fx_volume_char, { 255,255,255,255 }, nullptr);
		back = App->gui->AddUILabel(125, 140, "Back", { 255,255,255,255 }, nullptr);
		arrow = App->gui->AddUIImage(arrow_x, arrow_y, { 0,0,5,5 }, this, nullptr);

		first_update = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		this->quit_pressed = true;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		if (arrow_y < 140) {
			App->gui->DeleteUIElement(*arrow);
			arrow_y += 60;
			arrow = App->gui->AddUIImage(arrow_x, arrow_y, { 0,0,5,5 }, this, nullptr);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		if (arrow_y > 20) {
			App->gui->DeleteUIElement(*arrow);
			arrow_y -= 60;
			arrow = App->gui->AddUIImage(arrow_x, arrow_y, { 0,0,5,5 }, this, nullptr);
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		if (arrow_y == 140) {
			LOG("Back Pressed");
			App->main_menu->active = true;
			App->gui->DeleteUIElement(*music_volume_text);
			App->gui->DeleteUIElement(*fx_volume_text);
			App->gui->DeleteUIElement(*back);
			App->gui->DeleteUIElement(*arrow);
			App->gui->DeleteUIElement(*music_volume_num);
			App->gui->DeleteUIElement(*fx_volume_num);
			arrow_y = 20;
			first_update = true;
			this->active = false;
			//App->audio->StopMusic();
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {
		if (arrow_y == 20) {
			if (music_volume_value >= 8) {
				music_volume_value -= 8;
				App->gui->DeleteUIElement(*music_volume_num);
				music_volume_num = App->gui->AddUILabel(210, 20, music_volume_char, { 255,255,255,255 }, nullptr);
			}
			Mix_VolumeMusic(music_volume_value);
		}
		else if (arrow_y == 80) {
			if (fx_volume_value >= 8) {
				fx_volume_value -= 8;
				App->gui->DeleteUIElement(*fx_volume_num);
				fx_volume_num = App->gui->AddUILabel(210, 80, fx_volume_char, { 255,255,255,255 }, nullptr);
			}
			//Mix_VolumeChunk(Mix_GetChunk(1), fx_volume_value);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {
		if (arrow_y == 20) {
			if (music_volume_value <= 120) {
				music_volume_value += 8;
				App->gui->DeleteUIElement(*music_volume_num);
				music_volume_num = App->gui->AddUILabel(210, 20, music_volume_char, { 255,255,255,255 }, nullptr);
			}
			Mix_VolumeMusic(music_volume_value);
		}
		else if (arrow_y == 80) {
			if (fx_volume_value <= 120) {
				fx_volume_value += 8;
				App->gui->DeleteUIElement(*fx_volume_num);
				fx_volume_num = App->gui->AddUILabel(210, 80, fx_volume_char, { 255,255,255,255 }, nullptr);
			}
			//Mix_VolumeChunk(Mix_GetChunk(1), fx_volume_value);
		}
	}

	//if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fadeToBlack->FadeIsOver())
	//App->fadeToBlack->FadeToBlackBetweenModules(this, this, 1.0f);

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

void ctSettings::OnUITrigger(UIElement* elementTriggered)
{

}