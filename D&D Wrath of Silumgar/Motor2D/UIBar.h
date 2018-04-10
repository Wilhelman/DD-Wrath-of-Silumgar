#ifndef __UIBAR_H__
#define __UIBAR_H__

//#include "j1Fonts.h"
#include "ctGui.h"
#include "UIElement.h"

struct ctPerfTimer;
struct Entity;

class UIBar : public UIElement
{
public:

	UIElement* lower_bar = nullptr;
	UIElement* upper_bar = nullptr;
	UIElement* yellow_bar = nullptr;
	int player_bar_height = 11;
	int enemy_bar_height = 5;
	int max_player_bar_width = 160;
	int max_enemy_bar_width = 50;
	int bar_height = 22;
	int max_width = 570;
	int previous_width = 200;
	int current_width = 570;
	int max_capacity = 0;
	int current_quantity = 0;
	iPoint bar_pos = {0,0};
	UI_Type bar_type;
	ctPerfTimer yellow_bar_time;

	Entity* entity;


public:
	UIBar(int x, int y, int max_capacity, UI_Type type, ctModule* callback, Entity* entity,UIElement* parent = nullptr);
	~UIBar()
	{
		lower_bar->to_destroy = true;
		upper_bar->to_destroy = true;
		if( yellow_bar != nullptr)
		yellow_bar->to_destroy = true;
	}; 
	void Update();
	void LowerBar(int quantity);
	void RecoverBar(int quantity);
	void DrawYellowBar();
	void DeleteElements();
	int CurrentQuantity();
	void SetBarCurrentQuantity(int new_quantity);

	int CalculateBarWidth(int quantity);

};

#endif //__UIBAR_H__