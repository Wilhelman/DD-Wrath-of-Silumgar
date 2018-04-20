
#include "UIElement.h"


UIElement::UIElement(int x, int y, UI_Type type, UIElement* parent) : screen_position(x,y), type(type), parent(parent), current_state(STATE_NORMAL)
{
	
}

UIElement::~UIElement()
{
	App->tex->UnLoad(texture);
}

void UIElement::Update()
{
	if (parent == nullptr)
		local_position = screen_position;
	else
	{
		screen_position.x = parent->screen_position.x - local_position.x;
		screen_position.y = parent->screen_position.y - local_position.y;
	}

	// Esto lo podemos meter en el ctGui??(Y así borramos el include input)
	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		debug_draw = !debug_draw;

	if(callback != nullptr)
		callback->OnUITrigger(this, current_state);

}

void UIElement::Draw(SDL_Texture* sprites)
{
	if (!non_drawable)
	{
		App->render->Blit(sprites, screen_position.x, screen_position.y, &current_rect, 2.0f, 0.0, this->alpha);
		//Podriamos hacer una función debug draw, que se llame desde el postUpdate de ctGui así no interfiere con este
		//y no hace mil veces el if de comprobación solo lo haría 1 vez
		if (debug_draw) {
			SDL_Rect tmp_rect{ screen_position.x,screen_position.y,current_rect.w,current_rect.h };
			App->render->DrawQuad(tmp_rect, 255, 0, 0, 255, false);
		}
		//---------------------------------------------------------
	}

}

iPoint UIElement::GetLocalPosition() const
{
	return this->local_position;
}

iPoint UIElement::GetScreenPosition() const
{
	return this->screen_position;
}

void UIElement::SetParent(UIElement * parent)
{
	this->parent = parent;
}

UIElement * UIElement::GetParent() const
{
	return this->parent;
}

SDL_Rect UIElement::GetRect() const 
{
	return current_rect;
}
