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



ctAbout::ctAbout() : ctModule()
{
	name = "main_menu";
}

// Destructor
ctAbout::~ctAbout()
{}

// Called before render is available

bool ctAbout::Awake()
{
	LOG("Loading Main Menu");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool ctAbout::Start()
{
	bool ret = true;

	//if (!App->audio->PlayMusic("audio/music/Visager_End_Credits.ogg")) {
	//	//ret = false;
	//	LOG("Error playing music in j1MainMenu Start");
	//}

	return ret;
}

// Called each loop iteration
bool ctAbout::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ctAbout::Update(float dt)
{
	if (first_update == true) {
		back= App->gui->AddUILabel(125, 120, "Back", { 255,255,255,255 }, nullptr);
		arrow = App->gui->AddUIImage(arrow_x, arrow_y, { 50,50,5,5 }, this, nullptr);
		first_update = false;
	}

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		this->quit_pressed = true;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
		if (arrow_y < 120) {
			App->gui->DeleteUIElement(*arrow);
			arrow_y += 20;
			arrow = App->gui->AddUIImage(arrow_x, arrow_y, { 0,0,5,5 }, this, nullptr);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		if (arrow_y > 60) {
			App->gui->DeleteUIElement(*arrow);
			arrow_y -= 20;
			arrow = App->gui->AddUIImage(arrow_x, arrow_y, { 0,0,5,5 }, this, nullptr);
		}
	}
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		if (arrow_y == 120) {
			LOG("Back Pressed");
			App->main_menu->active = true;
			App->gui->DeleteUIElement(*back);
			App->gui->DeleteUIElement(*arrow);
			arrow_y = 20;
			first_update = true;
			this->active = false;
			//App->audio->StopMusic();
		}
	}
	//if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fadeToBlack->FadeIsOver())
	//App->fadeToBlack->FadeToBlackBetweenModules(this, this, 1.0f);

	return true;
}

// Called each loop iteration
bool ctAbout::PostUpdate()
{
	bool ret = true;

	if (quit_pressed)
		ret = false;

	return ret;
}

// Called before quitting
bool ctAbout::CleanUp()
{
	LOG("Freeing main_menu");

	return true;
}

bool ctAbout::Load(pugi::xml_node& load)
{
	bool ret = true;

	return ret;
}

bool ctAbout::Save(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}

void ctAbout::OnUITrigger(UIElement* elementTriggered, UI_State ui_state)
{

}