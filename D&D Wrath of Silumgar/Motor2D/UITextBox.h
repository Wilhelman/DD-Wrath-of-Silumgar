#ifndef __UITEXTBOX_H__
#define __UITEXTBOX_H__

#include "UIElement.h"
#include <string>
#include "ctFonts.h"

//Se puede hacer igual que un Dialog Box

class UITextBox : public UIElement
{
private:
	std::string text;
public: //TODOR
	SDL_Rect text_rect;
public:
	UITextBox(int x, int y, UI_Type type, std::string text, SDL_Color color, int size, uint32 box_width, const char* path = DEFAULT_FONT, UIElement* parent = nullptr, ctModule* callback = nullptr);

	void Draw(SDL_Texture* sprites);

};

#endif