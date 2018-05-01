#ifndef __UIPAUSEMENU_H__
#define __UIPAUSEMENU_H__

#include "UIElement.h"

class UIPauseMenu : public UIElement
{
private:

	UIElement* background = nullptr;

public:
	UIPauseMenu(int x, int y, UI_Type type, ctModule* callback, UIElement* parent = nullptr);
	~UIPauseMenu();
	void Draw(SDL_Texture* sprites);
};

#endif //__UIPAUSEMENU_H__