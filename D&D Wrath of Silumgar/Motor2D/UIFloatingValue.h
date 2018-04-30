#ifndef __UIFLOATINGVALUE_H__
#define __UIFLOATINGVALUE_H__

//#include "j1Fonts.h"
#include "UIElement.h"

class UIFloatingValue : public UIElement
{
public:

	std::string text;

	void Update();
	void Draw(SDL_Texture* sprites);

public:
	UIFloatingValue(int x, int y, UI_Type type, std::string text, SDL_Color color, int size = DEFAULT_FONT_SIZE, ctModule* callback = nullptr, const char* path = DEFAULT_FONT, UIElement* parent = nullptr);
};

#endif //__UIIMAGE_H__