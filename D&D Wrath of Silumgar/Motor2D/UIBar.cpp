#include "ctApp.h"
#include "UIBar.h"
#include "ctLog.h"
#include "ctInput.h"
#include "ctPerfTimer.h"

UIBar::UIBar(int x, int y, int max_capacity, UI_Type type, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;
	bar_type = type;
	bar_pos.x = x;
	bar_pos.y = y;
	this->max_capacity = max_capacity;
	current_quantity = max_capacity;

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
	if (yellow_bar != nullptr && yellow_bar_time.ReadMs() > 500) {
		App->gui->DeleteUIElement(*yellow_bar);
		yellow_bar = nullptr;
	}
}

void UIBar::LowerBar(int quantity)
{
	//Lower width of the bar when losing hp/mana
	if ((current_quantity-quantity) >= 0) {
		current_width = CalculateBarWidth(-quantity);
		App->gui->DeleteUIElement(*upper_bar);
		if (bar_type == LIFEBAR) {
			upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 0,107,current_width,22 });
			DrawYellowBar();
		}
		else if (bar_type == MANABAR) {
			upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,current_width,22 });
			DrawYellowBar();
		}
	}
	else {
		current_width = CalculateBarWidth(-current_quantity);
		App->gui->DeleteUIElement(*upper_bar);
		if (bar_type == LIFEBAR) {
			upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 0,107,current_width,22 });
			DrawYellowBar();
		}
		else if (bar_type == MANABAR) {
			upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,current_width,22 });
			DrawYellowBar();
		}
	}
}

void UIBar::RecoverBar(int quantity)
{
	//Recover width of the bar when wining hp/mana
	if ((current_quantity+quantity) < max_capacity) {
		current_width = CalculateBarWidth(quantity);
		App->gui->DeleteUIElement(*upper_bar);
		if (bar_type == LIFEBAR) {
			upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 0,107,current_width,22 });
		}
		else if (bar_type == MANABAR) {
			upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,current_width,22 });
		}
	}
	else {
		current_width = CalculateBarWidth((max_capacity-current_quantity));
		App->gui->DeleteUIElement(*upper_bar);
		if (bar_type == LIFEBAR) {
			upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 0,107,current_width,22 });
		}
		else if (bar_type == MANABAR) {
			upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,current_width,22 });
		}
	}
}

void UIBar::DrawYellowBar() {
	if (yellow_bar != nullptr) {
		App->gui->DeleteUIElement(*yellow_bar);
	}
	yellow_bar = App->gui->AddUIImage(bar_pos.x+current_width, bar_pos.y, { 582,129,(previous_width-current_width),22 });
	yellow_bar_time.Start();
}

int UIBar::CalculateBarWidth(int quantity) {
	//Calculate the new bar width when losing/wining hp/mana quantity 
	int new_width = current_width;
	previous_width = current_width;
	int new_quantity = (current_quantity + quantity);
	current_quantity = new_quantity;

	new_width = (new_quantity * max_width) / max_capacity;

	return new_width;
}