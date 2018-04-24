#include "ctApp.h"
#include "UIImage.h"
#include "ctLog.h"
#include "ctGui.h"


UIImage::UIImage(int x, int y, UI_Type type, SDL_Rect rect, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;

	current_rect = rect;
}

void UIImage::UpdateImageWithCoords(SDL_Rect rect) 
{
	current_rect = rect;
}