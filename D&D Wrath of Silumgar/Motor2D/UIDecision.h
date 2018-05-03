#ifndef __UIDECISION_H__
#define __UIDECISION_H__

//#include "j1Fonts.h"
#include "ctGui.h"
#include "UIElement.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "ctTextures.h"

struct WorldMapElement;

class UIDecision : public UIElement
{
public:

	UIElement* text_border = nullptr;
	UIElement* text_decision = nullptr;
	UIElement* arrow = nullptr;

	UIElement* option_1 = nullptr;
	UIElement* option_2 = nullptr;

	WorldMapElement* choice_01 = nullptr;
	WorldMapElement* choice_02 = nullptr;

public:
	UIDecision(int x, int y, UI_Type type, UIElement* &arrow, std::string decision, WorldMapElement &option_1, WorldMapElement &option_2, ctModule* callback, UIElement* parent = nullptr);
	UIElement* aux_element = nullptr;

	~UIDecision();
};

#endif