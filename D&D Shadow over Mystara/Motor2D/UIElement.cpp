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

	int mouse_x = 0, mouse_y = 0;
	App->input->GetMousePosition(mouse_x, mouse_y);

	UIElement* element_to_trigger = nullptr;
	element_to_trigger = App->gui->GetElementUnderMouse(mouse_x, mouse_y);
	
	if (element_to_trigger != nullptr && this == element_to_trigger) 
	{
		if (current_state == STATE_LEFT_MOUSE_RELEASED)
			current_state = STATE_NORMAL;

		if (current_state != STATE_MOUSE_ENTER && current_state != STATE_LEFT_MOUSE_PRESSED) 
		{
			current_state = UI_State::STATE_MOUSE_ENTER;
			if (this->type == BUTTON)
				((UIButton*)this)->UpdateButtonWithSelfRect(((UIButton*)this)->btn_focused);
			this->callback->OnUITrigger(this, current_state);
		}

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT && current_state == STATE_MOUSE_ENTER) 
		{
			current_state = STATE_LEFT_MOUSE_PRESSED;
			if (this->type == BUTTON)
				((UIButton*)this)->UpdateButtonWithSelfRect(((UIButton*)this)->btn_pressed);
			this->callback->OnUITrigger(this, current_state);
		}
		else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && current_state == STATE_LEFT_MOUSE_PRESSED) 
		{
			current_state = STATE_LEFT_MOUSE_RELEASED;
			if (this->type == BUTTON)
				((UIButton*)this)->UpdateButtonWithSelfRect(((UIButton*)this)->btn_normal);
			this->callback->OnUITrigger(this, current_state);
		}

		if (current_state == STATE_LEFT_MOUSE_PRESSED && this->draggable) 
		{
			App->input->GetMouseMotion(mouse_motion_x, mouse_motion_y);

			if (mouse_x != tmp_mouse_x || mouse_y != tmp_mouse_y)
			{
				screen_position.x += mouse_motion_x;
				screen_position.y += mouse_motion_y;
				local_position.x += mouse_motion_x;
				local_position.y += mouse_motion_y;
				tmp_mouse_x = mouse_x;
				tmp_mouse_y = mouse_y;
			}
		}
	}
	else 
	{
		if (current_state == UI_State::STATE_MOUSE_ENTER || current_state == STATE_LEFT_MOUSE_PRESSED) 
		{
			current_state = UI_State::STATE_MOUSE_LEAVE;
			if (this->type == BUTTON)
				((UIButton*)this)->UpdateButtonWithSelfRect(((UIButton*)this)->btn_normal);
			this->callback->OnUITrigger(this, current_state);
		}
		else if (current_state == STATE_MOUSE_LEAVE)
			current_state = STATE_NORMAL;
	}

	if (parent == nullptr)
		local_position = screen_position;
	else
	{
		screen_position.x = parent->screen_position.x + local_position.x;
		screen_position.y = parent->screen_position.y + local_position.y;
	}
	
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
			App->render->Blit(texture, screen_position.x, screen_position.y, &current_rect);
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
