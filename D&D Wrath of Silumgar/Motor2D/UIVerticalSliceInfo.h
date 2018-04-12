#ifndef __UIVERTICALSLICEINFO_H__
#define __UIVERTICALSLICEINFO_H__

//#include "j1Fonts.h"
#include "ctGui.h"
#include "UIElement.h"

class UIVerticalSliceInfo : public UIElement
{
public:
	UIElement* background = nullptr;
	UIElement* image_hero = nullptr;
	UIElement* text = nullptr;
	UIElement* constitution = nullptr;
	UIElement* focus = nullptr;
	UIElement* force = nullptr;
	UIElement* agility = nullptr;
	UIElement* skill = nullptr;
	UIElement* intelligence = nullptr;
	UIElement* physical_defense = nullptr;
	UIElement* magic_defense = nullptr;
	UIElement* luck = nullptr;






public:
	UIVerticalSliceInfo(int x, int y,bool overcome, ctModule* callback, UIElement* parent = nullptr);
	virtual ~UIVerticalSliceInfo();


	void Update();


};

#endif // __UIVERTICALSLICEINFO_H__