#ifndef __UIBAR_H__
#define __UIBAR_H__

#include "ctGui.h"
#include "UIElement.h"

struct ctPerfTimer;
struct Entity;

class UIBar : public UIElement
{
public:
	
	UIElement* bar = nullptr;
	
	SDL_Rect lower_bar_rect;
	SDL_Rect yellow_bar_rect;
	
	float speed = 0;

	int bar_height = 22;
	int max_width = 570;

	int previous_width = 200;
	int current_width = 570;

	int max_capacity = 0;
	int current_quantity = 0;

	iPoint bar_pos = {0,0};
	UI_Type bar_type;

	Entity* entity;

public:
	UIBar(int x, int y, int max_capacity, UI_Type type, ctModule* callback, Entity* entity,UIElement* parent = nullptr);
	~UIBar()
	{
		entity = nullptr;
		App->gui->DeleteUIElement(*bar);
	}; 
	
	void Draw(SDL_Texture*tex);
	void LowerBar(int quantity);

	void MakeElementsInvisible();
	void MakeElementsVisible();
	int CurrentQuantity();

	int CalculateBarWidth(int quantity);

};

#endif //__UIBAR_H__