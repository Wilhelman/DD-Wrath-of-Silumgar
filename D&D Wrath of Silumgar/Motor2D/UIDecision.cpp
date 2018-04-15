#include "ctApp.h"
#include "UIDecision.h"
//#include <algorithm>
#include "UITextBox.h"

//TODO change rect according to decision_number or do an enum or something
UIDecision::UIDecision(int x, int y, int decision_number, UI_Type type, UIElement* &arrow, std::vector<UIElement*> &options, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	int size = 15;
	std::string text = "Decided to set out on a new adventure with your weapons in hand, you leave the village without looking back, looking for the origin of the attack. But you stumble upon the first decision you must take which will mark your destiny and path.";
	std::string text_options[] = { "!A. Follow the footprints that comes from    the forest.", "!B.Take a shortcut through the cave, dark, narrow and full of threats. Even the most experienced adventurers can not get out without a wound. "};
	this->callback = callback;
	

	//Dont delete. Will be useful later
	/*aux_element = App->gui->AddUIImage(x, y, { 0,648,264,169 }, nullptr, this);
	this->image_border = aux_element;

	aux_element = App->gui->AddUIImage(x, y + 165, { 0,485,264,162 }, nullptr, this);
	this->text_border = aux_element;

	aux_element = App->gui->AddUITextBox(x + 20, y + 165 + 26, size, 222, text, { 255,255,255,255 }, this);
	this->text_decision = aux_element;*/


	//TO SOLVE: if we use AddUIImage or text box, when we create one UIDecision element 3 elements are pushed to GUI list: the image, the textbox and the UIDecision. We should use constructor from each element and only push to the list one element UIDecision.
	
	int xE = App->win->screen_surface->w / App->win->GetHScalade() / 9;
	int yE = App->win->screen_surface->h / App->win->GetHScalade() / 50;
	text_border = App->gui->AddUIImage(xE, yE, { 843,484,264,280 }, nullptr, this);

	
	

	aux_element = App->gui->AddUITextBox( xE + 20, yE + 24, size, 222, text, { 255,255,255,255 }, this);
	this->text_decision = aux_element;



	int extra_h = 0;

	for (int i = 1; i >= 0; i--) {

		if (!text_options[i].empty() && !text_options[i].empty()) { //text box for every option
			aux_element = App->gui->AddUITextBox(xE + 20, 208 - extra_h, 14, 260, text_options[i], { 255,255,255,255 }, this,Third_Font);   //Old: 300 - extra_h 
																																  //this->ui_options[i] = aux_element;
			options.push_back(aux_element);
			extra_h += 40;
		}

	}


	number_of_options = std::count(text.begin(), text.end(), '!'); //checking if there is any ! to know how many options
																   //TODO: look if there is other symbol to use (\t if we change the font maybe).
	int extra_lines = std::count(text.begin(), text.end(), '\n');

	int option_A_height = 16 * number_of_options;  //16 is the height of a line with 15 size. 8 is half.

												   //aux_element = App->gui->AddUIImage(x + 15, y + 165 + 26 + this->text_decision->current_rect.h - option_A_height - 8 * extra_lines + 16, { 1333, 272, 7, 14 }, nullptr, this);
	arrow = App->gui->AddUIImage(xE + 10, options.back()->screen_position.y, { 1333, 272, 7, 14 }, nullptr, options.back()); // y + 165 + 26 + this->text_decision->current_rect.h - option_A_height - 8 * extra_lines + 16
	this->arrow = arrow;

	first_option = options.back();

	this->options = options;

}

