#ifndef __UICOMBATMENU_H__
#define __UICOMBATMENU_H__

//#include "j1Fonts.h"
#include "ctGui.h"
#include "UIElement.h"

class UICombatMenu : public UIElement
{
public:
	UIElement* background = nullptr;
	UIElement* arrow = nullptr;
	UIElement* attack_label = nullptr;
	UIElement* abilities_label = nullptr;
	UIElement* items_label = nullptr;
	UIElement* upper_points = nullptr;
	UIElement* lower_points = nullptr;
	iPoint upper_points_pos = { 0,0 };
	iPoint lower_points_pos = {0,0};
	std::vector<UIElement*> main_labels;
	std::vector<UIElement*> abilities;
	std::vector<UIElement*> items;
	std::vector<std::string> names;
	int names_iterator = 0;

	iPoint label1_pos {15,20};
	iPoint label2_pos{ 15,40 };
	iPoint label3_pos{ 15,60 };

	iPoint main_label1_pos{ 15,10 };
	iPoint main_label2_pos{ 15,40 };
	iPoint main_label3_pos{ 15,70 };

	int font_size = 12;

public:
	UICombatMenu(int x, int y, UI_Type type,ctModule* callback, UIElement* parent = nullptr);

	void Update();

	void NavigateDown(std::vector<UIElement*> &current_vector);
	void NavigateUp(std::vector<UIElement*> &current_vector);
	void ExecuteComand(std::vector<UIElement*> &current_vector);

	void LoadAbilities();
	void LoadItems();

	void GoBack();
};

#endif //__UICOMBATMENU_H__