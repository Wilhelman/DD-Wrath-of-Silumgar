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

	Dialog_Image = new UIImage(x, y, IMAGE, { 1075,762,448,169 }, nullptr, this); 
	Dialog_Text = new UITextBox(50, 333, TEXTBOX, text, color, 15, 428, path, Dialog_Image);
}

UIDialogBox::~UIDialogBox() {
	App->gui->DeleteUIElement(*Dialog_Image);
	Dialog_Image = nullptr;
	App->gui->DeleteUIElement(*Dialog_Text);
	Dialog_Text = nullptr;
}

void UIDialogBox::Draw(SDL_Texture* sprites) 
{
	App->render->Blit(sprites, Dialog_Image->screen_position.x, Dialog_Image->screen_position.y, &Dialog_Image->current_rect);
	App->render->Blit(Dialog_Text->texture, Dialog_Text->GetLocalPosition().x, Dialog_Text->GetLocalPosition().y, &Dialog_Text->current_rect);

}