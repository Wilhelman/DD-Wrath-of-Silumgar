#include "ctApp.h"
#include "UIFloatingValue.h"
#include "ctLog.h"
#include "ctGui.h"
#include "ctTextures.h"


UIFloatingValue::UIFloatingValue(int x, int y, UI_Type type, std::string text, SDL_Color color, int size, ctModule* callback, const char* path, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;
	this->text = text;

	App->fonts->size = size;
	_TTF_Font* newFont = App->fonts->Load(path, size);

	texture = App->fonts->Print(text.c_str(), color, newFont);

	int width = 0, height = 0;
	App->fonts->CalcSize(this->text.c_str(), width, height, newFont);
	current_rect.w = width;
	current_rect.h = height;

	if (parent == nullptr)
		screen_position = local_position;
	else
	{
		screen_position.x = parent->screen_position.x + local_position.x;
		screen_position.y = parent->screen_position.y + local_position.y;
	}

}

void UIFloatingValue::Update()
{
	this->screen_position.y--;

	alpha -= 3;
	if (alpha <= 0)
		alpha = 0;
		
}

void UIFloatingValue::Draw(SDL_Texture* sprites)
{
	if (!non_drawable)
	{
		App->render->UIBlit(texture, screen_position.x, screen_position.y, &current_rect, 2.0f, 0.0, this->alpha);
	}

}