#include "ctApp.h"
#include "UILabel.h"
#include "ctLog.h"
#include "ctGui.h"
#include "ctRender.h"
//#include "ctFonts.h"

UILabel::UILabel(int x, int y, UI_Type type, std::string text, SDL_Color color, int size, ctModule* callback, const char* path, UIElement* parent, bool UIBLit) : UIElement(x, y, type, parent)
{
	blitUI = UIBLit;
	this->callback = callback;
	this->text = text;

	App->fonts->size = size;
	_TTF_Font* newFont = App->fonts->Load(path, size);

	texture = App->fonts->Print(text.c_str(), color, newFont);

	int width = 0, height = 0;
	App->fonts->CalcSize(this->text.c_str(), width, height, newFont);
	
	App->fonts->Unload(newFont);

	newFont = nullptr;

	current_rect.w = width;
	current_rect.h = height;
}

void UILabel::Update()
{

}


void UILabel::Draw(SDL_Texture* sprites)
{

	if (!non_drawable)
	{
		if(blitUI == true)
			App->render->UIBlit(texture, screen_position.x, screen_position.y, &current_rect, 2.0f, 0.0, this->alpha);
		else
			App->render->Blit(texture, screen_position.x, screen_position.y, &current_rect, 2.0f, 0.0, this->alpha);
	}

}