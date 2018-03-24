#include "ctApp.h"
#include "UILabel.h"
#include "ctLog.h"
#include "ctGui.h"
#include "ctFonts.h"

UILabel::UILabel(int x, int y, UI_Type type, std::string text, SDL_Color color, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->text = text;
	this->callback = callback;
	texture = App->fonts->Print(text.c_str(), color, App->fonts->default);

	int width = 0, height = 0;
	App->fonts->CalcSize(this->text.c_str(), width, height, App->fonts->default);
	current_rect.w = width;
	current_rect.h = height;
}

