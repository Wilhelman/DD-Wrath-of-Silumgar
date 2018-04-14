#include "ctApp.h"
#include "UIDialogBox.h"
#include "UIImage.h"
#include "UITextBox.h"
#include "ctLog.h"
#include "ctRender.h"
#include "ctGui.h"
#include "ctFonts.h"

UIDialogBox::UIDialogBox(int x, int y, UI_Type type, std::string text, SDL_Color color, int size, uint32 box_width, const char* path, UIElement* parent) : UIElement(x, y, type, parent)
{

	//Dialog_Image = App->gui->AddUIImage(x, y, { 843,484,264,273 }, nullptr);
	//Dialog_Text = App->gui->AddUITextBox(2, 2, size, box_width, text, color, Dialog_Image, Second_Font);
	Dialog_Image = new UIImage(x, y, IMAGE, { 840,768,448,169 }, nullptr, this); 
	Dialog_Text = new UITextBox(20, 220, TEXTBOX, text, color, 15, 428, path, Dialog_Image); 
}

void UIDialogBox::Draw(SDL_Texture* sprites) 
{
	App->render->Blit(sprites, Dialog_Image->GetLocalPosition().x, Dialog_Image->GetLocalPosition().y, &Dialog_Image->current_rect);
	App->render->Blit(Dialog_Text->texture, Dialog_Text->GetLocalPosition().x, Dialog_Text->GetLocalPosition().y, &Dialog_Text->current_rect);


}