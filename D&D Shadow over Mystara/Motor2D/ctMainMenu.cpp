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
#include "ctSettings.h"
#include "ctAbout.h"
#include "ctMap.h"
#include "j1Map.h"

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

	continue_label = App->gui->AddUILabel(35, 10, "Continue", { 255,255,255,255 }, nullptr,this);
	continue_label->SetState(STATE_FOCUSED);
	new_game_label = App->gui->AddUILabel(35, 30, "New Game", { 255,255,255,255 }, nullptr,this);
	settings_label = App->gui->AddUILabel(35, 50, "Settings", { 255,255,255,255 }, nullptr,this);
	about_label = App->gui->AddUILabel(35, 70, "About", { 255,255,255,255 }, nullptr,this);
	quit_label = App->gui->AddUILabel(35, 90, "Quit", { 255,255,255,255 }, nullptr,this);
	arrow = App->gui->AddUIImage(50, 10, { 0,0,5,5 }, this, nullptr);
	
	if (!App->audio->PlayMusic("audio/music/Visager_End_Credits.ogg")) {
		//ret = false;
		LOG("Error playing music in j1MainMenu Start");
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
	if (continue_label->current_state == STATE_EXECUTED)
	{
		App->combat->active = true;
		App->gui->DeleteAllUIElements();
		this->active = false;
	}
	else if (new_game_label->current_state == STATE_EXECUTED)
	{
		App->combat->active = true;
		App->gui->DeleteAllUIElements();
		this->active = false;
	}
	else if (settings_label->current_state == STATE_EXECUTED)
	{
		App->settings->active = true;
		App->gui->DeleteAllUIElements();
		this->active = false;
	}
	else if (about_label->current_state == STATE_EXECUTED)
	{
		App->about->active = true;
		App->gui->DeleteAllUIElements();
		this->active = false;
	}
	else if (quit_label->current_state == STATE_EXECUTED)
	{
		quit_pressed = true;
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

void ctMainMenu::OnUITrigger(UIElement* elementTriggered)
{
	
	switch (elementTriggered->current_state)
	{

	case STATE_NORMAL:
		break;
	case STATE_FOCUSED:
		arrow->SetParent(elementTriggered);
		break;
	case STATE_EXECUTED:
		break;

	}

}