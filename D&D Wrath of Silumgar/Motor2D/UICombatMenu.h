#ifndef __UICOMBATMENU_H__
#define __UICOMBATMENU_H__

//#include "j1Fonts.h"
#include "ctGui.h"
#include "UIElement.h"

#define ATTACKEXPLANATION "Use a normal attack that has no mana cost"
#define ABILITIESEXPLANATION "Open the abilities list"
#define ITEMSEXPLANATION "Open your inventory"

struct Entity;
struct Action;
struct Item;
struct 	ctPerfTimer;

class UICombatMenu : public UIElement
{
public:
	Entity* entity = nullptr;

	UIElement* background = nullptr;
	UIElement* arrow = nullptr;
	UIElement* enemy_select_arrow = nullptr;
	UIElement* attack_label = nullptr;
	UIElement* abilities_label = nullptr;
	UIElement* items_label = nullptr;
	UIElement* upper_points = nullptr;
	UIElement* lower_points = nullptr;
	UIElement* explanation_background = nullptr;
	UIElement* explanation_label = nullptr;
	UIElement* mana_cost = nullptr;
	iPoint upper_points_pos = { 0,0 };
	iPoint lower_points_pos = { 0,0 };
	std::vector<UIElement*> main_labels;
	std::vector<UIElement*> abilities;
	std::vector<UIElement*> items;
	std::vector<std::string> names;
	std::vector<Action> entity_actions;
	int names_iterator = 0;
	int current_ability = 0;
	int current_item = 0;
	int dead_heroes = 0;

	iPoint label1_pos{ 10,20 };
	iPoint label2_pos{ 10,40 };
	iPoint label3_pos{ 10,60 };

	iPoint main_label1_pos{ 10,10 };
	iPoint main_label2_pos{ 10,40 };
	iPoint main_label3_pos{ 10,70 };

	iPoint explanationPos{129,31};

	int font_size = 12;

	bool selecting_enemy = false;
	bool executed_command = false;

public:

	UICombatMenu(Entity* entity, int x, int y, UI_Type type, ctModule* callback, UIElement* parent = nullptr);
	virtual ~UICombatMenu();


	void Update();

	void NavigateDown(std::vector<UIElement*> &current_vector);
	void NavigateUp(std::vector<UIElement*> &current_vector);
	void ExecuteComand(std::vector<UIElement*> &current_vector);

	void LoadAbilities();
	void LoadItems();

	void GoBack();

	void SelectEnemy(std::vector<UIElement*> &current_vector);
	void SelectAlly(std::vector<UIElement*> &current_vector);
	void SelectDeadAlly(std::vector<UIElement*> &current_vector);

	void ChangeExplanation(std::vector<UIElement*> &current_vector);

private:
	std::vector<Entity*>::const_iterator selected_enemy;
	std::vector<Entity*>::const_iterator selected_ally;
	std::vector<Action*>::const_iterator selected_ability;
	ctPerfTimer execute_comand_time;
};

#endif //__UICOMBATMENU_H__