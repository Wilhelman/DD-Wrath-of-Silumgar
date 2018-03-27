#ifndef __UIBAR_H__
#define __UIBAR_H__

//#include "j1Fonts.h"
#include "ctGui.h"
#include "UIElement.h"

class UIBar : public UIElement
{
public:



public:
	UIBar(int x, int y, int max_capacity, UI_Type type, ctModule* callback, UIElement* parent = nullptr);

	void Update();
};

#endif //__UIBAR_H__