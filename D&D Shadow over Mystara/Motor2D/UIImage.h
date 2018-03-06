#ifndef __UIIMAGE_H__
#define __UIIMAGE_H__

#include "UIElement.h"

class UIImage : public UIElement
{
private:
	SDL_Rect img_normal;
	SDL_Rect img_pressed;
	SDL_Rect img_mouse_in;

public:
	UIImage(int x, int y, UI_Type type, SDL_Rect rect, ctModule* callback, UIElement* parent = nullptr);

	void UpdateImageWithCoords(SDL_Rect rect);
};

#endif //__UIIMAGE_H__