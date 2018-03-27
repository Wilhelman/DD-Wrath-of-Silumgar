#ifndef __UIBAR_H__
#define __UIBAR_H__

//#include "j1Fonts.h"
#include "ctGui.h"
#include "UIElement.h"

class UIBar : public UIElement
{
public:

	//570 22
	UIElement* lower_bar = nullptr;
	UIElement* upper_bar = nullptr;
	UIElement* yellow_bar = nullptr;
	int max_width = 570;
	int max_capacity = 0;
	iPoint bar_pos = {0,0};


public:
	UIBar(int x, int y, int max_capacity, UI_Type type, ctModule* callback, UIElement* parent = nullptr);

	void Update();

	int CalculateBarWidth(int quantity);
};

#endif //__UIBAR_H__