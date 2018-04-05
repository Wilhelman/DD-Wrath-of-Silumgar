#ifndef __UIDECISION_H__
#define __UIDECISION_H__

//#include "j1Fonts.h"
#include "ctGui.h"
#include "UIElement.h"

struct Option {
	uint option_number;
	iPoint position;
};

class UIDecision : public UIElement
{
public:

	UIElement * image_border;
	UIElement* image_decision;
	UIElement* text_border;
	UIElement* text_decision;
	UIElement* arrow;
	UIElement* ui_options[3];
	UIElement* first_option;
	//std::vector<Option> options;
	std::vector<UIElement*> options;
	int number_of_options = 0;

public:
	UIDecision(int x, int y, int decision_number, UI_Type type, UIElement* &arrow, std::vector<UIElement*> &options, ctModule* callback, UIElement* parent = nullptr);

	std::vector<UIElement*> GetOptionsUIElements() {
		return options;
	}

};

#endif //Questions to Ricardo