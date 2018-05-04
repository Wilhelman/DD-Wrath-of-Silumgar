#include "ctApp.h"
#include "UITextBox.h"
#include "ctLog.h"
#include "ctRender.h"
#include "ctGui.h"
#include "ctFonts.h"

UITextBox::UITextBox(int x, int y, UI_Type type, std::string text, SDL_Color color, int size, uint32 box_width, const char* path, UIElement* parent) : UIElement(x, y, type, parent)
{
	float lines = 0;


	this->text = text;
	this->text_color = color;
	_TTF_Font* new_font = App->fonts->Load(path, size);

	
	int width_size = 0, height_size = 0;
	App->fonts->CalcSize(text.c_str(), width_size, height_size, new_font);

	if (width_size < box_width)
		box_width = width_size;


	texture = App->fonts->PrintTextBox(text.c_str(), color, new_font, box_width, size);
	App->fonts->Unload(new_font);
	new_font = nullptr;


	current_rect.w = box_width;

	lines = (float)width_size / (float)box_width;


	if (lines != (int)lines)
		lines = (int)lines + 1;

	int lines_jumped = std::count(text.begin(), text.end(), '\n');

	lines += lines_jumped;
	current_rect.h = height_size * (int)lines;

	text_rect = current_rect;
}


void UITextBox::Draw(SDL_Texture* sprites)
{

	if (!non_drawable)
	{
		App->render->UIBlit(texture, screen_position.x, screen_position.y, &current_rect, 2.0f, 0.0, this->alpha);
	}

}

