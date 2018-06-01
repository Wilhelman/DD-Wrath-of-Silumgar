#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctTextures.h"
#include "ctAudio.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "ctFadeToBlack.h"
#include "ctMainMenu.h"
#include "ctAbout.h"
#include "ctGui.h"
#include "j1Language.h"

#include "UIImage.h"
#include "UIButton.h"
#include "UILabel.h"
#include "UITextBox.h"
#include "ctEntities.h"

ctAbout::ctAbout() : ctModule()
{
	name = "about";
}

// Destructor
ctAbout::~ctAbout()
{}

// Called before render is available

bool ctAbout::Awake()
{

	LOG("Loading j1Credits");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool ctAbout::Start()
{
	bool ret = true;

	credits_over = false;

	if (!App->audio->PlayMusic("audio/music/D&D Shadow Over Mystara - Song 13 Sprite  Theme (Stage 3-B).ogg"))
	{
		//ret = false;
		LOG("Error playing music in ctAbout Start");
	}

	uint w, h;
	App->win->GetWindowSize(w, h);
	int licence_x = (-400 + App->win->GetWScalade()*w / 2) / 2;//first number is width/2

	about_lbl = (UILabel*)App->gui->AddUILabel(licence_x, 10+y_delay, "ABOUT", { 255,255,255,255 }, 22, nullptr, nullptr, "Fonts/FritzQuadrata Regular.ttf");
	
	
	std::string licence;
	licence=("    Team Member                              Role                             Github\nAlex Campamar Redol                        UI                              Acaree\n\nGuillermo García Subirana              Team Lead                   Wilhelman\n\nRicardo Gutiérrez Llenas               Management                 Ricardogll\n\nManel Mourelo Montero                     QA                        manelmourelo\n\nAlfonso Sánchez-Cortés Puerta            Code                           Siitoo\n\nMarc Tarrés i Urbieta                     Art & Audio                   MAtaur00\n\nVictor Tirado Fernández                    Design                      VictorTirado\n\nMIT License - Copyright(c) 2017\nPermission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the ''Software''), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :\nThe above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED ''AS IS'', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");

	std::string esc;
	esc = "ESC to return";
	esc_lbl = (UILabel*)App->gui->AddUILabel(192+(-29 + App->win->GetWScalade()*w / 2) / 2, y_delay+345, esc, { 200,200,200,255 }, 10,nullptr,nullptr,"Fonts/FritzQuadrata Regular.ttf");

	
	//licence_lbl = (UILabel*)App->gui->AddUILabel(50, 50, licence, { 255,255,255,255 }, 10); // ,200);
	licence_lbl = (UITextBox*)App->gui->AddUITextBox(licence_x, 35+y_delay, 20, 800, licence, { 255,255,255,255 },nullptr, "Fonts/FritzQuadrata Regular.ttf");
	//licence_lbl->interactable = false;
	//labels.push_back(licence_lbl);

	/*p2SString the_team;
	the_team.create("THE TEAM");

	team_lbl = (UILabel*)App->ui->AddUILabel(50, h / App->win->GetScale() + licence_lbl->GetRect().h + 20, the_team, { 255,255,255,255 }, 20);
	team_lbl->interactable = false;
	labels.PushBack(team_lbl);

	p2SString ggs;
	ggs.create("Garcia Subirana, Guillermo's responsability and Github account:                                                    In charge of all code related to IA, pathfinding, preservation of changes, interaction between modules and other parts of code.");

	UILabel* ggs_lbl = (UILabel*)App->ui->AddUILabel(50, h / App->win->GetScale() + licence_lbl->GetRect().h + team_lbl->GetRect().h + 40, ggs, { 255,255,255,255 }, 10, 198);
	ggs_lbl->interactable = false;
	labels.PushBack(ggs_lbl);

	git_ggs_btn = (UIButton*)App->ui->AddUIButton(10, h / App->win->GetScale() + licence_lbl->GetRect().h + team_lbl->GetRect().h + ggs_lbl->GetRect().h / 2 + 17, { 0, 455, 28, 32 }, { 87, 304, 40, 44 }, { 28, 455, 28, 29 }, this);
	buttons.PushBack(git_ggs_btn);


	p2SString vmg;
	vmg.create("Maso Garcia, Victor's responsability and Github account:                                                         Management of all tasks / team organization. In charge of sprites and audio and some modules of code.");

	UILabel* vmg_lbl = (UILabel*)App->ui->AddUILabel(50, ggs_lbl->GetLocalPosition().y + 30 + ggs_lbl->GetRect().h + 10, vmg, { 255,255,255,255 }, 10, 198);
	vmg_lbl->interactable = false;
	labels.PushBack(vmg_lbl);

	git_vmg_btn = (UIButton*)App->ui->AddUIButton(10, vmg_lbl->GetLocalPosition().y + 8, { 0, 455, 28, 32 }, { 87, 304, 40, 44 }, { 28, 455, 28, 29 }, this);
	buttons.PushBack(git_vmg_btn);
	*/

	std::string hold;
	hold="Hold space to skip";

	hold_lbl = (UILabel*)App->gui->AddUILabel((-41 + App->win->GetWScalade()*w / 2) / 2, 5, hold, { 255,255,255,255 }, 10, nullptr, nullptr, "Fonts/FritzQuadrata Regular.ttf");
	//hold_lbl->SetLocalPosition(w / App->win->GetScale() / 2 - hold_lbl->GetRect().w / 2, hold_lbl->GetLocalPosition().y);
	
	
	

	//esc_lbl = (UILabel*)App->gui->AddUILabel((-29 + App->win->GetWScalade()*w / 2) / 2, licence_lbl->screen_position.y+licence_lbl->current_rect.h - 230, esc, { 255,255,255,255 }, 10);
	//esc_lbl->SetLocalPosition(w / App->win->GetScale() / 2 - esc_lbl->GetRect().w / 2, vmg_lbl->GetLocalPosition().y + vmg_lbl->GetRect().h + 20);
	
	//labels.PushBack(esc_lbl);

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

	int holding_space = 1;

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {
		holding_space = 4;
		
	}

	if (credits_over)
		holding_space = 0;

	//for (int i = 0; i < labels.size(); i++) 
	//{
	//	labels[i]->SetLocalPosition(labels[i]->GetLocalPosition().x, labels[i]->GetLocalPosition().y - scroll_speed * holding_space);
	//}
	licence_lbl->screen_position.y -= scroll_speed * holding_space;
	about_lbl->screen_position.y -= scroll_speed * holding_space;
	esc_lbl->screen_position.y -= scroll_speed * holding_space;
	//git_ggs_btn->SetLocalPosition(git_ggs_btn->GetLocalPosition().x, git_ggs_btn->GetLocalPosition().y - scroll_speed * holding_space);
	//git_vmg_btn->SetLocalPosition(git_vmg_btn->GetLocalPosition().x, git_vmg_btn->GetLocalPosition().y - scroll_speed * holding_space);

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && App->fadeToBlack->FadeIsOver())
	{
		App->fadeToBlack->FadeToBlackBetweenModules(this, App->main_menu, 1.0f);
	}

	if (about_lbl->screen_position.y < 6) {
		hold_lbl->screen_position.y -= scroll_speed;
	}

	if (about_lbl->screen_position.y < -20) {
		credits_over = true;
	}
	
	
	return true;
}



// Called before quitting
bool ctAbout::CleanUp()
{
	LOG("Freeing j1Credits");
	App->gui->DeleteAllUIElements();
	App->gui->DeleteUIElement(*hold_lbl);
	hold_lbl = nullptr;
	App->gui->DeleteUIElement(*licence_lbl);
	licence_lbl = nullptr;
	App->gui->DeleteUIElement(*esc_lbl);
	esc_lbl = nullptr;

	//git_ggs_btn = nullptr;
	//git_vmg_btn = nullptr;

/*	for (int i = 0; i < labels.size(); i++)
	{
		labels[i] = nullptr;
	}
	labels.clear();*/ 

	/*for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i] = nullptr;
	}
	buttons.clear();*/

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

//void ctAbout::OnUITrigger(UIElement* elementTriggered, UI_State ui_state) {
//	if (elementTriggered->type == BUTTON)
//	{
//		UIButton* tmpBtn = (UIButton*)elementTriggered;
//
//		switch (ui_state)
//		{
//		case STATE_LEFT_MOUSE_RELEASED:
//		case STATE_NORMAL:
//		{
//			tmpBtn->SetLocalPosition(tmpBtn->GetLocalPosition().x, tmpBtn->GetLocalPosition().y - BUTTON_PUSH_OFFSET);
//			if (tmpBtn->last_state == STATE_LEFT_MOUSE_PRESSED && App->fadeToBlack->FadeIsOver())
//			{
//
//				if (tmpBtn == git_ggs_btn) {
//					ShellExecute(GetActiveWindow(), "open", "https://github.com/Wilhelman/", NULL, NULL, SW_SHOWNORMAL);
//				}
//				else if (tmpBtn == git_vmg_btn) {
//					ShellExecute(GetActiveWindow(), "open", "https://github.com/nintervik", NULL, NULL, SW_SHOWNORMAL);
//				}
//
//			}
//			tmpBtn->UpdateButtonWithSelfRect(tmpBtn->btn_normal);
//			break;
//		}
//		case STATE_MOUSE_ENTER:
//		{
//			for (int i = 0; i < buttons.Count(); i++)
//			{
//				if (buttons[i]->current_state == STATE_FOCUSED)
//				{
//					buttons[i]->current_state = STATE_NORMAL;
//					tmpBtn->UpdateButtonWithSelfRect(tmpBtn->btn_normal);
//					break;
//				}
//			}
//
//			tmpBtn->UpdateButtonWithSelfRect(tmpBtn->btn_focused);
//			if (tmpBtn->button_lbl != nullptr)
//				tmpBtn->button_lbl->SetLocalPosition(tmpBtn->button_lbl->GetLocalPosition().x + BUTTON_HOVER_OFFSET, tmpBtn->button_lbl->GetLocalPosition().y + BUTTON_HOVER_OFFSET);
//			tmpBtn->SetLocalPosition(tmpBtn->GetLocalPosition().x - BUTTON_HOVER_OFFSET, tmpBtn->GetLocalPosition().y - BUTTON_HOVER_OFFSET);
//			break;
//		}
//		case STATE_MOUSE_LEAVE:
//			tmpBtn->UpdateButtonWithSelfRect(tmpBtn->btn_normal);
//			if (tmpBtn->last_state == STATE_LEFT_MOUSE_PRESSED)
//			{
//				tmpBtn->SetLocalPosition(tmpBtn->GetLocalPosition().x, tmpBtn->GetLocalPosition().y - BUTTON_PUSH_OFFSET);
//			}
//			else
//			{
//				if (tmpBtn->button_lbl != nullptr)
//					tmpBtn->button_lbl->SetLocalPosition(tmpBtn->button_lbl->GetLocalPosition().x - BUTTON_HOVER_OFFSET, tmpBtn->button_lbl->GetLocalPosition().y - BUTTON_HOVER_OFFSET);
//				tmpBtn->SetLocalPosition(tmpBtn->GetLocalPosition().x + BUTTON_HOVER_OFFSET, tmpBtn->GetLocalPosition().y + BUTTON_HOVER_OFFSET);
//			}
//			break;
//		case STATE_LEFT_MOUSE_PRESSED:
//			tmpBtn->UpdateButtonWithSelfRect(tmpBtn->btn_pressed);
//			if (tmpBtn->button_lbl != nullptr)
//				tmpBtn->button_lbl->SetLocalPosition(tmpBtn->button_lbl->GetLocalPosition().x - BUTTON_HOVER_OFFSET, tmpBtn->button_lbl->GetLocalPosition().y - BUTTON_HOVER_OFFSET);
//			tmpBtn->SetLocalPosition(tmpBtn->GetLocalPosition().x + BUTTON_HOVER_OFFSET, tmpBtn->GetLocalPosition().y + BUTTON_HOVER_OFFSET + BUTTON_PUSH_OFFSET);
//			break;
//		case STATE_NO_DEF:
//			break;
//		default:
//			break;
//		}
//	}
//}