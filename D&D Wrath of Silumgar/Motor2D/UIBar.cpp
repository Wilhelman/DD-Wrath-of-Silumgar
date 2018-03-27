#include "ctApp.h"
#include "UIBar.h"
#include "ctLog.h"
#include "ctInput.h"


UIBar::UIBar(int x, int y, int max_capacity, UI_Type type, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;

	bar_pos.x = x;
	bar_pos.y = y;
	this->max_capacity = max_capacity;

	if (type == LIFEBAR) {
		lower_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 570,107,max_width,22 });
		upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 0,107,max_width,22 });
	}
	else if (type == MANABAR) {
		lower_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 0,129,max_width,22 });
		upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,max_width,22 });
	}

	LOG("UIBar created in x:%i, y:%i", x, y);
}

void UIBar::Update()
{
}

int UIBar::CalculateBarWidth(int quantity) {
	int new_width = 0;

	return new_width;
}