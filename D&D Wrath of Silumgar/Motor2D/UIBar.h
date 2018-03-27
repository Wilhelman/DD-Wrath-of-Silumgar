#ifndef __UIBAR_H__
#define __UIBAR_H__

//#include "j1Fonts.h"
#include "ctGui.h"
#include "UIElement.h"

struct ctPerfTimer;

class UIBar : public UIElement
{
public:

	UIElement* lower_bar = nullptr;
	UIElement* upper_bar = nullptr;
	UIElement* yellow_bar = nullptr;
	int max_width = 570;
	int previous_width = 570;
	int current_width = 570;
	int max_capacity = 0;
	int current_quantity = 0;
	iPoint bar_pos = {0,0};
	UI_Type bar_type;
	ctPerfTimer yellow_bar_time;


public:
	UIBar(int x, int y, int max_capacity, UI_Type type, ctModule* callback, UIElement* parent = nullptr);

	void Update();
	void LowerBar(int quantity);
	void RecoverBar(int quantity);
	void DrawYellowBar();

	int CalculateBarWidth(int quantity);

};

#endif //__UIBAR_H__