#include "ctApp.h"
#include "UIBar.h"
#include "ctLog.h"
#include "ctInput.h"
#include "ctPerfTimer.h"

UIBar::UIBar(int x, int y, int max_capacity, UI_Type type, ctModule* callback, Entity* entity, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->entity = entity;
	this->callback = callback;
	bar_type = type;
	bar_pos.x = x;
	bar_pos.y = y;
	this->max_capacity = max_capacity;
	current_quantity = max_capacity;


	if (type == LIFEBAR) {
		max_width = 160;
		current_width = 160;
		previous_width = 160;
		bar_height = 11;

		yellow_bar_rect = { 583,130,max_width,bar_height };
		lower_bar_rect = { 571,107,max_width,bar_height };
		bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 1,107,max_width,bar_height });
	}
	else if (type == MANABAR) {
		max_width = 160;
		current_width = 160;
		previous_width = 200;
		bar_height = 11;

		yellow_bar_rect = { 583,130,max_width,bar_height };
		lower_bar_rect = { 0,129,max_width,bar_height };
		bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 318,445,max_width,bar_height });
	}
	else if (type == ENEMYLIFEBAR) {
		max_width = 50;
		current_width = 50;
		previous_width = 50;
		bar_height = 5;

		yellow_bar_rect = { 583,130,max_width,bar_height };
		lower_bar_rect = { 571,107,max_width,bar_height };
		bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 1,107,max_width,bar_height });
	}

	previous_width = max_width;
}



void UIBar::Draw(SDL_Texture* tex)
{
	if (bar->non_drawable == false)
	{
		if (current_width != max_width)
		{
			iPoint lower_bar_position = iPoint(screen_position.x + max_width - (max_width - current_width), screen_position.y);

			lower_bar_rect.w = max_width - current_width;

			App->render->Blit(tex, lower_bar_position.x, lower_bar_position.y, &lower_bar_rect, 2.0f, 0.0, alpha);
		}

		if (previous_width > current_width) {

			iPoint yellow_bar_position = iPoint(screen_position.x + max_width - (max_width - current_width), screen_position.y);
			yellow_bar_rect.w = (previous_width - current_width);


			App->render->Blit(tex, yellow_bar_position.x, yellow_bar_position.y, &yellow_bar_rect, 2.0f, 0.0, alpha);

			speed += 1.0f / 3;
			if (speed > 1)
			{
				previous_width -= speed;
				speed = 0;
			}
		}
		else
		{
			previous_width = current_width;
		}

		App->render->Blit(tex, screen_position.x, screen_position.y, &current_rect, 2.0f, 0.0, alpha);
	}
}

void UIBar::LowerBar(int quantity)
{

	if (bar != nullptr)
	{
		if ((current_quantity - quantity) >= 0)
			current_width = CalculateBarWidth(quantity);
		else
			current_width = CalculateBarWidth(-current_quantity);

		bar->current_rect.w = current_width;

	}
}



int UIBar::CalculateBarWidth(int quantity)
{
	previous_width = current_width;

	current_quantity = current_quantity + quantity;

	return ((current_quantity*max_width) / max_capacity);
}


int UIBar::CurrentQuantity()
{
	return current_quantity;
}


void UIBar::MakeElementsInvisible() {

	bar->non_drawable = true;
}

void UIBar::MakeElementsVisible() {

	bar->non_drawable = false;
}
