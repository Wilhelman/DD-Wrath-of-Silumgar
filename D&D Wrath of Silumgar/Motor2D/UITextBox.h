#ifndef __UITEXTBOX_H__
#define __UITEXTBOX_H__

#include "UIElement.h"
#include <string>
#include "ctFonts.h"



class UITextBox : public UIElement
{
private:
	_TTF_Font* normal_font = App->fonts->default;
	std::string text;
	SDL_Color text_color = { 255,255,255,255 };
	int size = 0;
public: //TODOR
	SDL_Rect text_rect;
public:
	UITextBox(int x, int y, UI_Type type, std::string text, SDL_Color color, int size, uint32 box_width, const char* path = DEFAULT_FONT, UIElement* parent = nullptr);

	void Update();
};

#endif