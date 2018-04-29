#include "ctApp.h"
#include "UIPauseMenu.h"
#include "ctLog.h"
#include "ctGui.h"
#include "UIImage.h"
#include "ctRender.h"


UIPauseMenu::UIPauseMenu(int x, int y, UI_Type type, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;

	background = new UIImage(x, y, IMAGE, {0,0,484,324}, nullptr, this);

	
}

UIPauseMenu::~UIPauseMenu() {

	App->gui->DeleteUIElement(*background);
	background = nullptr;
}

void UIPauseMenu::Draw(SDL_Texture* sprites)
{
	App->render->Blit(sprites, background->screen_position.x, background->screen_position.y, &background->current_rect);
	//App->render->Blit(Dialog_Text->texture, Dialog_Text->GetLocalPosition().x, Dialog_Text->GetLocalPosition().y, &Dialog_Text->current_rect);
}