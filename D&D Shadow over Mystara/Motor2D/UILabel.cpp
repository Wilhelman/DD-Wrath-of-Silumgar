#include "ctApp.h"
#include "UILabel.h"
#include "ctLog.h"
#include "ctGui.h"

UILabel::UILabel(int x, int y, UI_Type type, std::string text, Color color, UIElement* parent) : UIElement(x,y,type,parent)
{
	this->text = text;
	this->text_color = color;

	//todo implement fonts
	/*if (color == YELLOW)
		texture = App->font->Print(text.GetString(), { 254, 203,0,255 }, App->font->default);
	else if (color == GREY)
		texture = App->font->Print(text.GetString(), { 134, 136, 138,255 }, App->font->default);

	int width = 0, height = 0;
	App->font->CalcSize(this->text.GetString(), width, height, App->font->default);
	current_rect.w = width;
	current_rect.h = height;*/
}

void UILabel::Update()
{

}
