#ifndef __UILEVELUPINFO_H__
#define __UILEVELUPINFO_H__

//#include "j1Fonts.h"
#include "ctGui.h"
#include "UIElement.h"

class UILevelUpInfo : public UIElement
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


	UIElement* ability_unlock = nullptr;
	UIElement* level_number = nullptr;
	UIElement* name_ability = nullptr;

	

public:
	UILevelUpInfo(int x, int y, UI_Type type, EntityType hero, ctModule* callback, UIElement* parent = nullptr);
	virtual ~UILevelUpInfo();


	void Update();

	
};

#endif // UILevelUpInfo