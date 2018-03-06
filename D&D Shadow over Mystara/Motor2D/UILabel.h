#ifndef __UILABEL_H__
#define __UILABEL_H__

#include "UIElement.h"
#include <string>
//#include "ctFonts.h"



class UILabel : public UIElement
{
private:
	//ctFonts normal_font;
	std::string text;
	Color text_color = COLOR_NOT_DEF;
public:
	UILabel(int x, int y, UI_Type type, std::string text, Color color, UIElement* parent = nullptr);

	void Update();
};

#endif //__UILABEL_H__