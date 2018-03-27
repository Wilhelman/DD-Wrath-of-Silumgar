#ifndef __UILABEL_H__
#define __UILABEL_H__

#include "UIElement.h"
#include <string>

#include "ctFonts.h"



class UILabel : public UIElement
{
private:
	std::string text;
public:
	UILabel(int x, int y, UI_Type type, std::string text, SDL_Color color, int size = DEFAULT_FONT_SIZE, ctModule* callback = nullptr, const char* path = DEFAULT_FONT, UIElement* parent = nullptr);

	void Update();
};

#endif //__UILABEL_H__