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
	continue_label = App->gui->AddUILabel(15, 10, "Continue", {255,255,255,255}, nullptr);
	new_game_label = App->gui->AddUILabel(15, 30, "New Game", {255,255,255,255}, nullptr);
	settings_label = App->gui->AddUILabel(15, 50, "Settings", { 255,255,255,255 }, nullptr);
	options_label = App->gui->AddUILabel(15, 70, "Options", { 255,255,255,255 }, nullptr);
	quit_label = App->gui->AddUILabel(15, 90, "Quit", { 255,255,255,255 }, nullptr);

	/*if (!App->audio->PlayMusic("audio/music/8-bit_Detective.ogg")) {
		//ret = false;
		LOG("Error playing music in j1MainMenu Start");
	}*/

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
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		this->quit_pressed = true;

	//if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fadeToBlack->FadeIsOver())
		//App->fadeToBlack->FadeToBlackBetweenModules(this, this, 1.0f);

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