#include "ctApp.h"
#include "UIDecision.h"
#include "UITextBox.h"
#include "ctWorldMap.h"

//TODO change rect according to decision_number or do an enum or something
UIDecision::UIDecision(int x, int y, UI_Type type, UIElement* &arrow, std::string decision, WorldMapElement &option_1, WorldMapElement &option_2, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	int size = 15;
	std::string text = decision.c_str();
	std::string text_options[] = { option_1.option.c_str(),option_2.option.c_str() };
	this->callback = callback;
	
	choice_01 = &option_1;
	choice_02 = &option_2;
	//Dont delete. Will be useful later
	/*aux_element = App->gui->AddUIImage(x, y, { 0,648,264,169 }, nullptr, this);
	this->image_border = aux_element;

	aux_element = App->gui->AddUIImage(x, y + 165, { 0,485,264,162 }, nullptr, this);
	this->text_border = aux_element;

	aux_element = App->gui->AddUITextBox(x + 20, y + 165 + 26, size, 222, text, { 255,255,255,255 }, this);
	this->text_decision = aux_element;*/

	int xE = App->win->screen_surface->w / App->win->GetHScalade() / 9;
	int yE = App->win->screen_surface->h / App->win->GetHScalade() / 50;
	text_border = App->gui->AddUIImage(xE, yE, { 843,484,264,280 }, nullptr, this);

	aux_element = App->gui->AddUITextBox( xE + 20, yE + 24, 12, 290, text, { 255,255,255,255 }, this, Second_Font);
	this->text_decision = aux_element;

	int extra_h = 0;

	for (int i = 1; i >= 0; i--) {

		if (!text_options[i].empty() && !text_options[i].empty()) { //text box for every option
			aux_element = App->gui->AddUITextBox(xE + 20, 208 - extra_h, 12, 245, text_options[i], { 255,255,255,255 }, this,Second_Font);   //Old: 300 - extra_h 
			if(i == 1)
				this->option_1 = aux_element;
			else
				this->option_2 = aux_element;

			extra_h += 40;
		}

	}

	int extra_lines = std::count(text.begin(), text.end(), '\n');

	int option_A_height = 16 * 2;  //16 is the height of a line with 15 size. 8 is half.

	arrow = App->gui->AddUIImage(xE + 10, this->option_1->screen_position.y, { 1333, 272, 7, 14 }, nullptr, this->option_1); // y + 165 + 26 + this->text_decision->current_rect.h - option_A_height - 8 * extra_lines + 16
	
	this->arrow = arrow;

	App->world_map->options.push_back(this->option_1);
	App->world_map->options.push_back(this->option_2);
}

UIDecision::~UIDecision(){
	App->gui->DeleteUIElement(*text_border);
	App->gui->DeleteUIElement(*text_decision);
	App->gui->DeleteUIElement(*arrow);
	App->gui->DeleteUIElement(*option_1);
	App->gui->DeleteUIElement(*option_2);

	App->tex->UnLoad(texture);
}

