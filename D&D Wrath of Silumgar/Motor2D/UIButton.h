#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

//#include "j1Fonts.h"
#include "UIElement.h"

class UIButton : public UIElement
{
public:
	SDL_Rect btn_normal;
	SDL_Rect btn_focused;
	SDL_Rect btn_pressed;

public:
	UIButton(int x, int y, UI_Type type, SDL_Rect normal_rect, SDL_Rect focused_rect, SDL_Rect pressed_rect, ctModule* callback, UIElement* parent = nullptr);

	void UpdateButtonWithSelfRect(SDL_Rect self_rect);
};

#endif //__UIIMAGE_H__