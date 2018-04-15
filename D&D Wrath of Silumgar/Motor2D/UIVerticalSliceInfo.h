#ifndef __UIVERTICALSLICEINFO_H__
#define __UIVERTICALSLICEINFO_H__

//#include "j1Fonts.h"
#include "ctGui.h"
#include "UIElement.h"

class UIVerticalSliceInfo : public UIElement
{
public:
	UIElement* background = nullptr;
	UIElement* congratulations = nullptr;
	UIElement* text = nullptr;

public:
	UIVerticalSliceInfo(int x, int y,bool overcome, ctModule* callback, UIElement* parent = nullptr);
	virtual ~UIVerticalSliceInfo();
};

#endif // __UIVERTICALSLICEINFO_H__