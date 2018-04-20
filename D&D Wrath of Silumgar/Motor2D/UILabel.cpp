#include "ctApp.h"
#include "UILabel.h"
#include "ctLog.h"
#include "ctGui.h"


UILabel::UILabel(int x, int y, UI_Type type, std::string text, SDL_Color color, int size, ctModule* callback, const char* path, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;
	this->text = text;

	App->fonts->size = size;
	_TTF_Font* newFont = App->fonts->Load(path, size);

	texture = App->fonts->Print(text.c_str(), color, newFont);

	App->fonts->CalcSize(this->text.c_str(), current_rect.w, current_rect.h, newFont);
	
}


