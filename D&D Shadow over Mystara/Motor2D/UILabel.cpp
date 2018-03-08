#include "ctApp.h"
#include "UILabel.h"
#include "ctLog.h"
#include "ctGui.h"
#include "ctFonts.h"

UILabel::UILabel(int x, int y, UI_Type type, std::string text, Color color, UIElement* parent) : UIElement(x,y,type,parent)
{
	this->text = text;
	this->text_color = color;

	//todo implement fonts
	if (color == YELLOW)
		texture = App->fonts->Print(text.c_str(), { 254, 203,0,255 }, App->fonts->default);
	else if (color == GREY)
		texture = App->fonts->Print(text.c_str(), { 134, 136, 138,255 }, App->fonts->default);

	int width = 0, height = 0;
	App->fonts->CalcSize(this->text.c_str(), width, height, App->fonts->default);
	current_rect.w = width;
	current_rect.h = height;
}

void UILabel::Update()
{

}
