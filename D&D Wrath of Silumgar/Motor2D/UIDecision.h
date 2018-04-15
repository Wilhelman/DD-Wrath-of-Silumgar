#ifndef __UIDECISION_H__
#define __UIDECISION_H__

//#include "j1Fonts.h"
#include "ctGui.h"
#include "UIElement.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "ctTextures.h"

struct Option {
	uint option_number;
	iPoint position;
};

class UIDecision : public UIElement
{
public:


	//UIElement* image_decision;
	UIElement* text_border;
	UIElement* text_decision;
	UIElement* arrow;
	/*UIElement* ui_options[2];*/
	UIElement* first_option;
	//std::vector<Option> options;
	std::vector<UIElement*> options;
	int number_of_options = 2;

public:
	UIDecision(int x, int y, int decision_number, UI_Type type, UIElement* &arrow, std::vector<UIElement*> &options, ctModule* callback, UIElement* parent = nullptr);
	UIElement* aux_element;
	std::vector<UIElement*> GetOptionsUIElements() {
		return options;
	}

	~UIDecision()
	{
		//image_border->to_destroy = true;
		//image_decision->to_destroy = true;
		App->gui->DeleteUIElement(*text_border);
		App->gui->DeleteUIElement(*text_decision);
		App->gui->DeleteUIElement(*arrow);
		App->gui->DeleteUIElement(*first_option);
		App->gui->DeleteUIElement(*options[2]);
		App->gui->DeleteUIElement(*aux_element);
		App->tex->UnLoad(texture);

		options.clear();
	}
	//Decision call example

	//decision = (UIDecision*)App->gui->AddUIDecision(50, 0, 1, arrow, options, this); 
	//(*options.rbegin())->current_state = STATE_FOCUSED;
	//arrow->SetParent(*options.rbegin());
};

#endif //Questions to Ricardo