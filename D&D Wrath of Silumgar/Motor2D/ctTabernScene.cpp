#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctAudio.h"
#include "ctWindow.h"
#include "ctEntities.h"
#include "ctTabernScene.h"
#include "ctWorldMap.h"
#include "ctCombat.h"
#include "ctFadeToBlack.h"
#include "ctGui.h"
#include "UIElement.h"
#include "UIImage.h"
#include "UIDecision.h"


//randomize libs
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */



ctTabernScene::ctTabernScene() : ctModule()
{
	name = "world_map";
}

// Destructor
ctTabernScene::~ctTabernScene()
{}

// Called before render is available

bool ctTabernScene::Awake(pugi::xml_node& config)
{
	LOG("Loading World Map");
	bool ret = true;



	return ret;
}

// Called before the first frame
bool ctTabernScene::Start()
{
	bool ret = true;
	Dialog_Box =App->gui->AddUIDialogBox(20, 158, 15, 224, " Usollip, the god of war has been judged and condemned in the great Olympus by the rest of the gods for him having meddled too much in the lives of human beings. In the world of the gods there are few laws, one of these is not to intervene in the world of humans. ", { 255,255,255,255 }, nullptr, Second_Font);
	Text_continue = App->gui->AddUITextBox(320, 293, 10, 224, "Press ENTER or A to continue", { 255,255,255,255 }, nullptr, Second_Font);
	
	if (!App->audio->PlayMusic(App->audio->CutsceneBSO.c_str(), 1)) {
		LOG("Error playing music in cutscene Start");
	}


	return ret;
}

// Called each loop iteration
bool ctTabernScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ctTabernScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN ||App->input->gamepad.A == GAMEPAD_STATE::PAD_BUTTON_DOWN) {

		times_x_pressed++;

		if (times_x_pressed == 1) {
			App->gui->DeleteUIElement(*Dialog_Box);
			Dialog_Box = App->gui->AddUIDialogBox(20, 158, 15, 224, "Usollip bored of having everything he wanted, decided to start playing with them. As god of war, he distributed a great artifact called, Sorez, which contained an unlimited power capable of dominating the whole earth. Once all the nations learned, great conflicts and crusades began to get Sorez.", { 255,255,255,255 }, nullptr, Second_Font);
			App->gui->DeleteUIElement(*Text_continue);
			Text_continue = App->gui->AddUITextBox(320, 293, 10, 224, "Press ENTER or A to continue", { 255,255,255,255 }, nullptr, Second_Font);
			
		}

		if (times_x_pressed == 2) {
			App->gui->DeleteUIElement(*Dialog_Box);
			Dialog_Box = App->gui->AddUIDialogBox(20, 158, 15, 224, "When the gods learned what Usollip was doing, they decided to lock him in hell, from which he could never leave. Sorez finished in the hands of Silumgar Dragon Blood, with this power began to attack all continents. His desire to generate wars has induced the world into chaos.", { 255,255,255,255 }, nullptr, Second_Font);
			App->gui->DeleteUIElement(*Text_continue);
			Text_continue = App->gui->AddUITextBox(320, 293, 10, 224, "Press ENTER or A to continue", { 255,255,255,255 }, nullptr, Second_Font);
			
		}

		if (times_x_pressed == 3) {
			App->gui->DeleteUIElement(*Dialog_Box);
			Dialog_Box = App->gui->AddUIDialogBox(20, 158, 15, 224, "That is why the rest of the Gods: Wilhelman, Kira, Arla, Alfa, Acare, Sito and Rikarudo have guided a small group of heroes to return the balance to the world.", { 255,255,255,255 }, nullptr, Second_Font);
			App->gui->DeleteUIElement(*Text_continue);
			Text_continue = App->gui->AddUITextBox(320, 293, 10, 224, "Press ENTER or A to continue", { 255,255,255,255 }, nullptr, Second_Font);
			
		}

		if (times_x_pressed == 4) {
			App->gui->DeleteUIElement(*Dialog_Box);
			Dialog_Box = App->gui->AddUIDialogBox(20, 158, 15, 224, "We are in Allarad, the last peaceful place of all Mystara. The heroes have been gathered in the tavern of the city through messages and guides sent by the gods themselves, knowing already, what was going to happen.", { 255,255,255,255 }, nullptr, Second_Font);
			App->gui->DeleteUIElement(*Text_continue);
			Text_continue = App->gui->AddUITextBox(320, 293, 10, 224, "Press ENTER or A to continue", { 255,255,255,255 }, nullptr, Second_Font);
			Background = App->gui->AddUIImage(0, -40,{ 524,836,485,200 });
		}

		if (times_x_pressed == 5) {
			App->gui->DeleteUIElement(*Dialog_Box);
			Dialog_Box = App->gui->AddUIDialogBox(20, 158, 15, 224, "This peace would soon end when a great army approached the city of Allard to burn with everything that was in it. The heroes followed the responsibles of fire and they are going to pay that. ", { 255,255,255,255 }, nullptr, Second_Font);
			App->gui->DeleteUIElement(*Text_continue);
			Text_continue = App->gui->AddUITextBox(320, 293, 10, 224, "Press ENTER or A to continue", { 255,255,255,255 }, nullptr, Second_Font);
			App->gui->DeleteUIElement(*Background);
			Background = App->gui->AddUIImage(0, -40, { 1,836,485,200 });
		}

		if (times_x_pressed >= 6) {
			if(App->fadeToBlack->FadeIsOver())
				App->fadeToBlack->FadeToBlackBetweenModules(this, App->world_map, 1.0f);

		}
	}

	return true;
}

// Called each loop iteration
bool ctTabernScene::PostUpdate()
{
	bool ret = true;
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	return ret;
}

// Called before quitting
bool ctTabernScene::CleanUp()
{
	LOG("Freeing ctWorldMap");

	times_x_pressed = 0;
	App->gui->DeleteUIElement(*Dialog_Box);
	App->gui->DeleteUIElement(*Text_continue);
	App->gui->DeleteUIElement(*Background);
	

	return true;
}

bool ctTabernScene::Load(pugi::xml_node& load)
{
	bool ret = true;

	return ret;
}

bool ctTabernScene::Save(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}

void ctTabernScene::OnUITrigger(UIElement* elementTriggered, UI_State ui_state)
{

}

