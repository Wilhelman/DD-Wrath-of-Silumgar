#ifndef __UILABEL_H__
#define __UILABEL_H__

#include "UIElement.h"
#include <string>

#include "ctFonts.h"



class UILabel : public UIElement
{
private:
	std::string text;
	bool blitUI = false;
public:
	UILabel(int x, int y, UI_Type type, std::string text, SDL_Color color, int size = DEFAULT_FONT_SIZE, ctModule* callback = nullptr, const char* path = DEFAULT_FONT, UIElement* parent = nullptr,bool UIBlit = false);

	void Update();
	void Draw(SDL_Texture* sprites);
};

#endif //__UILABEL_H__