#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctAudio.h"
#include "ctCutsceneManager.h"
#include "ctFinalScene.h"
#include "ctFadeToBlack.h"
#include "ctMainMenu.h"


//randomize libs
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */



ctFinalScene::ctFinalScene() : ctModule()
{
	name = "final_scene";
}

// Destructor
ctFinalScene::~ctFinalScene()
{}

// Called before render is available

bool  ctFinalScene::Awake(pugi::xml_node& config)
{
	LOG("Loading World Map");
	bool ret = true;



	return ret;
}

// Called before the first frame
bool  ctFinalScene::Start()
{
	bool ret = true;
	App->entities->SpawnEntity(50, 125, CLERIC);
	App->entities->SpawnEntity(150, 125, WARRIOR);
	App->entities->SpawnEntity(250, 125, DWARF);
	App->entities->SpawnEntity(350, 125, ELF);

	//App->entities->GetLich()->animation = &App->entities->GetLich()->death;

	if (!App->audio->PlayMusic(App->audio->CutsceneBSO.c_str(), 1)) {
		LOG("Error playing music in cutscene Start");
	}
	App->cutscene_manager->ChargeCutscene(FINAL_CUTSCENE);
	App->cutscene_manager->StartCutscene();

	return ret;
}

// Called each loop iteration
bool  ctFinalScene::PreUpdate()
{
	
	return true;
}

// Called each loop iteration
bool  ctFinalScene::Update(float dt)
{
	if (!App->cutscene_manager->isActive()) {
		App->fadeToBlack->FadeToBlackBetweenModules(this, App->main_menu, 1.0f);
	}
	return true;
}

// Called each loop iteration
bool  ctFinalScene::PostUpdate()
{
	bool ret = true;
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	return ret;
}

// Called before quitting
bool  ctFinalScene::CleanUp()
{
	LOG("Freeing ctWorldMap");

	


	return true;
}

bool  ctFinalScene::Load(pugi::xml_node& load)
{
	bool ret = true;

	return ret;
}

bool  ctFinalScene::Save(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}

void  ctFinalScene::OnUITrigger(UIElement* elementTriggered, UI_State ui_state)
{

}

