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
		max_width = max_player_bar_width;
		current_width = max_width;
		previous_width = max_width;
		bar_height = player_bar_height;
		lower_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 570,107,max_width,bar_height });
		upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 0,107,max_width,bar_height });
	}
	else if (type == MANABAR) {
		max_width = max_player_bar_width;
		current_width = max_width;
		previous_width = max_width;
		bar_height = player_bar_height;
		lower_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 0,129,max_width,bar_height });
		upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,max_width,bar_height });
	}
	else if (type == ENEMYLIFEBAR) {
		max_width = max_enemy_bar_width;
		current_width = max_width;
		previous_width = max_width;
		bar_height = enemy_bar_height;
		lower_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 0,129,max_width,bar_height });
		upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,max_width,bar_height });
	}

	LOG("UIBar created in x:%i, y:%i", x, y);
}

void UIBar::Update()
{
	//Destroy the yellowbar after 500ms
	if (yellow_bar != nullptr && yellow_bar_time.ReadMs() > 500) {
		App->gui->DeleteUIElement(*yellow_bar);
		yellow_bar = nullptr;
	}
	if (current_quantity <=0 && bar_type == LIFEBAR) {
		DeleteElements();
		lower_bar = nullptr;
		upper_bar = nullptr;
		yellow_bar = nullptr;
	}
}

void UIBar::LowerBar(int quantity)
{
	//Lower width of the bar when losing hp/mana
	if (lower_bar != nullptr) {
		if ((current_quantity - quantity) >= 0) {
			current_width = CalculateBarWidth(-quantity);
			App->gui->DeleteUIElement(*upper_bar);
			if (bar_type == LIFEBAR || bar_type == ENEMYLIFEBAR) {
				upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 0,107,current_width,bar_height });
				DrawYellowBar();
			}
			else if (bar_type == MANABAR) {
				upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,current_width,bar_height });
				DrawYellowBar();
			}
		}
		else {
			current_width = CalculateBarWidth(-current_quantity);
			App->gui->DeleteUIElement(*upper_bar);
			if (bar_type == LIFEBAR || bar_type == ENEMYLIFEBAR) {
				upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 0,107,current_width,bar_height });
				DrawYellowBar();
			}
			else if (bar_type == MANABAR) {
				upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,current_width,bar_height });
				DrawYellowBar();
			}
		}
	}
}

void UIBar::RecoverBar(int quantity)
{
	//Recover width of the bar when wining hp/mana
	if (lower_bar != nullptr) {
		if ((current_quantity + quantity) < max_capacity) {
			current_width = CalculateBarWidth(quantity);
			App->gui->DeleteUIElement(*upper_bar);
			if (bar_type == LIFEBAR || bar_type == ENEMYLIFEBAR) {
				upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 0,107,current_width,bar_height });
			}
			else if (bar_type == MANABAR) {
				upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,current_width,bar_height });
			}
		}
		else {
			current_width = CalculateBarWidth((max_capacity - current_quantity));
			App->gui->DeleteUIElement(*upper_bar);
			if (bar_type == LIFEBAR || bar_type == ENEMYLIFEBAR) {
				upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 0,107,current_width,bar_height });
			}
			else if (bar_type == MANABAR) {
				upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,current_width,bar_height });
			}
		}
	}
}

void UIBar::DrawYellowBar() {
	//Draw a yellow bar showing what you've lost
	if (yellow_bar != nullptr) {
		App->gui->DeleteUIElement(*yellow_bar);
	}
	yellow_bar = App->gui->AddUIImage(bar_pos.x+current_width, bar_pos.y, { 582,129,(previous_width-current_width),bar_height });
	yellow_bar_time.Start();
}

void UIBar::DeleteElements() {
	App->gui->DeleteUIElement(*lower_bar);
	App->gui->DeleteUIElement(*upper_bar);
	App->gui->DeleteUIElement(*yellow_bar);
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

int UIBar::CurrentQuantity() {
	return current_quantity;
}