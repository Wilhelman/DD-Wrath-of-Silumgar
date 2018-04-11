#include "ctApp.h"
#include "UIElement.h"
#include "ctRender.h"
#include "ctGui.h"
#include "ctInput.h"
#include "UIButton.h"
#include "ctTextures.h"

UIElement::UIElement(int x, int y, UI_Type type, UIElement* parent) : screen_position(x,y), type(type), parent(parent)
{
	current_state = STATE_NORMAL;

	if (parent == nullptr)
		local_position = screen_position;
	else
	{
		local_position.x = screen_position.x - parent->screen_position.x;
		local_position.y = screen_position.y - parent->screen_position.y;
	}
}

UIElement::~UIElement()
{
	App->tex->UnLoad(texture);
}

void UIElement::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		debug_draw = !debug_draw;

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		if (type == DECISION)
			current_state = STATE_EXECUTED;
	}
	
	
	if (parent == nullptr)
		local_position = screen_position;
	else
	{
		screen_position.x = parent->screen_position.x + local_position.x;
		screen_position.y = parent->screen_position.y + local_position.y;
	}
	
	if(callback != nullptr)
		callback->OnUITrigger(this, current_state);
}

void UIElement::Draw(SDL_Texture* sprites)
{
	if (current_rect.w > 0 && current_rect.h > 0) 
	{
		switch (this->type)
		{
		case IMAGE:
		case BUTTON:
			App->render->Blit(sprites, screen_position.x, screen_position.y, &current_rect);
				break;
		case LABEL:
		case FLOATING_VALUE:
			App->render->Blit(texture, screen_position.x, screen_position.y, &current_rect);
			break;
		case TEXTBOX:
			App->render->Blit(texture, screen_position.x, screen_position.y, &current_rect);
		default:
			break;
		}

		if (debug_draw) {
			SDL_Rect tmp_rect{ screen_position.x,screen_position.y,current_rect.w,current_rect.h };
			App->render->DrawQuad(tmp_rect, 255, 0, 0, 255, false);
		}

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
