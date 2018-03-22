#include "ctApp.h"
#include "UIElement.h"
#include "ctRender.h"
#include "ctGui.h"
#include "ctInput.h"
#include "UIButton.h"


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
}

void UIElement::Update()
{

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		debug_draw = !debug_draw;

	if (current_state == STATE_FOCUSED)
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
			current_state = STATE_EXECUTED;
	


	callback->OnUITrigger(this);

}

void UIElement::Draw(SDL_Texture* sprites)
{
	if (current_rect.w > 0 && current_rect.h > 0) 
	{
		switch (this->type)
		{
		case IMAGE:
			App->render->Blit(sprites, local_position.x, local_position.y, &current_rect);
		case BUTTON:
			App->render->Blit(sprites, local_position.x, local_position.y, &current_rect);
				break;
		case LABEL:
			if(parent == nullptr)
			App->render->Blit(texture, local_position.x, local_position.y, &current_rect);
			else
			App->render->Blit(texture, local_position.x, local_position.y, &current_rect);
			break;
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
	if (this->parent != parent)
	{
		this->parent = parent;
		local_position.x = screen_position.x - parent->screen_position.x;
		local_position.y = screen_position.y + parent->screen_position.y;
	}
}

UIElement * UIElement::GetParent() const
{
	return this->parent;
}

SDL_Rect UIElement::GetRect() const 
{
	return current_rect;
}

void UIElement::SetState(UI_State state)
{
	current_state = state;
}