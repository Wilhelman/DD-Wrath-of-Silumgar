#ifndef __UIDIALOGBOX_H__
#define __UIDIALOGBOX_H__

#include "UIElement.h"
#include <string>
#include "ctFonts.h"



class UIDialogBox : public UIElement
{
private:
	UIElement* Dialog_Image;
	UIElement* Dialog_Text;

public:
	UIDialogBox(int x, int y, UI_Type type, std::string text, SDL_Color color, int size, uint32 box_width, const char* path, UIElement* parent);
	~UIDialogBox();
	void Draw(SDL_Texture* sprite);
};

#endif

