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

	continue_label = App->gui->AddUILabel(35, 10, "Continue", { 255,255,255,255 }, 15, this);
	new_game_label = App->gui->AddUILabel(35, 30, "New Game", { 255,255,255,255 }, 15, this);
	settings_label = App->gui->AddUILabel(35, 50, "Settings", { 255,255,255,255 }, 15, this);
	about_label = App->gui->AddUILabel(35, 70, "About", { 255,255,255,255 }, 15, this);
	quit_label = App->gui->AddUILabel(35, 90, "Quit", { 255,255,255,255 }, 15, this);
	arrow = App->gui->AddUIImage(50, 10, { 0,0,5,5 }, this);
	
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

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fadeToBlack->FadeIsOver())
		App->fadeToBlack->FadeToBlackBetweenModules(this, App->world_map,1.0f);

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
	continue_label = nullptr;
	new_game_label = nullptr;
	settings_label = nullptr;
	about_label = nullptr;
	quit_label = nullptr;
	arrow = nullptr;
	
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