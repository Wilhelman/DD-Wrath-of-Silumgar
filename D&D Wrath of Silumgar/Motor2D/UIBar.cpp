#include "ctApp.h"
#include "UIBar.h"
#include "ctLog.h"
#include "ctInput.h"


UIBar::UIBar(int x, int y, int max_capacity ,UI_Type type, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;
	
	if (type == LIFEBAR) {
		lower_bar = App->gui->AddUIImage(x, y, { 570,107,max_width,22 });
		upper_bar = App->gui->AddUIImage(x, y, { 0,107,max_width,22 });
	}
	else if (type == MANABAR) {
		lower_bar = App->gui->AddUIImage(x, y, { 0,129,max_width,22 });
		upper_bar = App->gui->AddUIImage(x, y, { 317,444,max_width,22 });
	}

	LOG("UIBar created in x:%i, y:%i", x, y);
}

void UIBar::Update()
{
}