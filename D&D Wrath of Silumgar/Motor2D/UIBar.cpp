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
		previous_width =200;
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
	//-----------------------------------------------------------------------------

	previous_width = max_width;
}

void UIBar::Update()
{
	
	//Podemos colocar que vaya bajando el ancho de la barra amarilla conforme pasen los segundos
	// y cuando llege a un valor proximo a 0 deletearlo
	//-------------------------------------------------------------

	//Destroy the yellowbar after 500ms
	/*if (yellow_bar != nullptr && yellow_bar_time.ReadMs() > 500) {
		App->gui->DeleteUIElement(*yellow_bar);
		yellow_bar = nullptr;
	}
	*/
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
			yellow_bar_rect.w = (previous_width-current_width);


			App->render->Blit(tex, yellow_bar_position.x, yellow_bar_position.y, &yellow_bar_rect, 2.0f, 0.0, alpha);
			
			speed += 1.0f/3;
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

//Em esto se puede hacer en "10 líneas"
void UIBar::LowerBar(int quantity)
{
	//Lower width of the bar when losing hp/mana
	if (bar != nullptr)
	{
		if ((current_quantity - quantity) >= 0)
			current_width = CalculateBarWidth(quantity);
		else
			current_width = CalculateBarWidth(-current_quantity);

		bar->current_rect.w = current_width;
		

		/*if (quantity<0) {
			if ((current_quantity - quantity) >= 0) {
				current_width = CalculateBarWidth(quantity);
				App->gui->DeleteUIElement(*upper_bar);
				if (bar_type == LIFEBAR) {
					upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 1,107,current_width,bar_height });
					DrawYellowBar();
				}
				else if (bar_type == MANABAR) {
					upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,current_width,bar_height });
					DrawYellowBar();
				}
				else if (bar_type == ENEMYLIFEBAR) {
					upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 1,110,current_width,bar_height });
					DrawYellowBar();
				}
			}
			else {
				current_width = CalculateBarWidth(-current_quantity);
				App->gui->DeleteUIElement(*upper_bar);
				if (bar_type == LIFEBAR) {
					upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 1,107,current_width,bar_height });
					DrawYellowBar();
				}
				else if (bar_type == MANABAR) {
					upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,current_width,bar_height });
					DrawYellowBar();
				}
				else if (bar_type == ENEMYLIFEBAR) {
					upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 1,110,current_width,bar_height });
					DrawYellowBar();
				}
			}
		}
		else if (quantity>0){
			if (lower_bar != nullptr) {
				if ((current_quantity + quantity) < max_capacity) {
					current_width = CalculateBarWidth(quantity);
					App->gui->DeleteUIElement(*upper_bar);
					if (bar_type == LIFEBAR) {
						upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 1,107,current_width,bar_height });
					}
					else if (bar_type == MANABAR) {
						upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,current_width,bar_height });
					}
					else if (bar_type == ENEMYLIFEBAR) {
						upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 1,110,current_width,bar_height });
						DrawYellowBar();
					}
				}
				else {
					current_width = CalculateBarWidth((max_capacity - current_quantity));
					App->gui->DeleteUIElement(*upper_bar);
					if (bar_type == LIFEBAR) {
						upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 1,107,current_width,bar_height });
					}
					else if (bar_type == MANABAR) {
						upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,current_width,bar_height });
					}
					else if (bar_type == ENEMYLIFEBAR) {
						upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 1,110,current_width,bar_height });
						DrawYellowBar();
					}
				}
			}
		}*/

	}
}
//------------------------------------------------------------------------------

//Con un valor fijo se puede aumentar de una manera mucho mas rápida y facil
/*void UIBar::RecoverBar(int quantity)
{
	//Recover width of the bar when wining hp/mana
	if (lower_bar != nullptr) {
		if ((current_quantity + quantity) < max_capacity) {
			current_width = CalculateBarWidth(quantity);
			App->gui->DeleteUIElement(*upper_bar);
			if (bar_type == LIFEBAR) {
				upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 1,107,current_width,bar_height });
			}
			else if (bar_type == MANABAR) {
				upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,current_width,bar_height });
			}
			if (bar_type == ENEMYLIFEBAR) {
				upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 1,110,current_width,bar_height });
			}
		}
		else {
			current_width = CalculateBarWidth((max_capacity - current_quantity));
			App->gui->DeleteUIElement(*upper_bar);
			if (bar_type == LIFEBAR) {
				upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 1,107,current_width,bar_height });
			}
			else if (bar_type == MANABAR) {
				upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 317,444,current_width,bar_height });
			}
			if (bar_type == ENEMYLIFEBAR) {
				upper_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 1,110,current_width,bar_height });
			}
		}
	}
}*/
//----------------------------------------------------------------------------------------

//Si hacemos lo de que vaya bajando poco a poco esto se va a la mierda
/*void UIBar::DrawYellowBar() {
	//Draw a yellow bar showing what you've lost
	if (yellow_bar != nullptr) {
		App->gui->DeleteUIElement(*yellow_bar);
	}
	if (current_width > 0) {
		yellow_bar = App->gui->AddUIImage(bar_pos.x + current_width, bar_pos.y, { 583,130,(previous_width - current_width),bar_height });
	}
	else {
		yellow_bar = App->gui->AddUIImage(bar_pos.x, bar_pos.y, { 583,130,(previous_width),bar_height });
	}
	yellow_bar_time.Start();
}*/
//-------------------------------------------------

/*void UIBar::DeleteElements() {
	App->gui->DeleteUIElement(*lower_bar);
	lower_bar = nullptr;
	App->gui->DeleteUIElement(*upper_bar);
	upper_bar = nullptr;
	App->gui->DeleteUIElement(*yellow_bar);
	yellow_bar = nullptr;
}*/

//Esto se puede rehacer entero en "3 líneas"

int UIBar::CalculateBarWidth(int quantity) {
	//Calculate the new bar width when losing/wining hp/mana quantity 

	//int new_width = current_width;
	previous_width = current_width;
	//int new_quantity = (current_quantity + quantity);
	current_quantity = current_quantity + quantity;

	//new_width = (new_quantity * max_width) / max_capacity;

	return ((current_quantity*max_width)/max_capacity);
}
//---------------------------------

int UIBar::CurrentQuantity() {

	return current_quantity;
}

//Se puede hacer en una sola función
void UIBar::MakeElementsInvisible() {
	
	bar->non_drawable = true;
}

void UIBar::MakeElementsVisible() {

	bar->non_drawable = false;
}
//---------------------------------------