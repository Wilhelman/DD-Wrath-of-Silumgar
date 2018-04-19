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
	//TEST
	UIElement* bar = nullptr;
	
	SDL_Rect lower_bar_rect;
	SDL_Rect yellow_bar_rect;
	//Need little change
	float speed = 2;

	int bar_height = 22;
	int max_width = 570;

	
	int previous_width = 200;
	int current_width = 570;

	int max_capacity = 0;
	int current_quantity = 0;

	iPoint bar_pos = {0,0};
	UI_Type bar_type;

	//ctPerfTimer yellow_bar_time;

	Entity* entity;

public:
	UIBar(int x, int y, int max_capacity, UI_Type type, ctModule* callback, Entity* entity,UIElement* parent = nullptr);
	~UIBar()
	{
		entity = nullptr;
		App->gui->DeleteUIElement(*bar);
	}; 
	void Update();
	void Draw(SDL_Texture*tex);
	void LowerBar(int quantity);
	//void RecoverBar(int quantity);
	//void DrawYellowBar();
	//void DeleteElements();
	void MakeElementsInvisible();
	void MakeElementsVisible();
	int CurrentQuantity();

	int CalculateBarWidth(int quantity);

};

#endif //__UIBAR_H__