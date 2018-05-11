#ifndef __ctSkillTree_H__
#define __ctSkillTree_H__

#include "ctModule.h"

#include <string>
#include <vector>

class Entity;

class ctSkillTree : public ctModule
{
	struct Ability {
		uint tier = 0u;
		uint branch = 0u;
		std::string ability_name;
		SDL_Rect icon_rect;
		uint active = 0u;
		std::string description;
	};

public:

	ctSkillTree();

	// Destructor
	virtual ~ctSkillTree();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	void OnUITrigger(UIElement* elementTriggered, UI_State ui_state);

	void DrawAbilities(std::vector<Ability*> &abilities);

	void NavigateSkills(std::vector<Ability*> &abilities);
	void NavigateSkillsDown(std::vector<Ability*> &abilities);
	void NavigateSkillsUp(std::vector<Ability*> &abilities);
	void NavigateSkillsLeft(std::vector<Ability*> &abilities);
	void NavigateSkillsRight(std::vector<Ability*> &abilities);
	void ChangeTitle();

	void PrintAbilityDescription();
	void ChangeDescriptionBG();
	void LookForActiveAbilities(std::vector<Ability*> &abilities);

	void GetPreviousAbility(std::vector<Ability*> &abilities);

	void SelectAbility();

	void SavedataToXML();

	void GoToNextSkillTree();

private:

	std::string skill_tree_map_tmx;
	std::string name_spritesheet_skill_tree_map;

	SDL_Texture* spritesheet_skill_tree_map = nullptr;

private:

	std::string name_spritesheet_abilities;
	SDL_Texture* spritesheet_abilities = nullptr;


	//Ability* warrior = new Ability();
	//Ability* cleric = new Ability();
	//Ability* dwarf = new Ability();
	//Ability* elf = new Ability();
	std::vector<Ability*> warrior_abilities;
	std::vector<Ability*> cleric_abilities;
	std::vector<Ability*> dwarf_abilities;
	std::vector<Ability*> elf_abilities;
	std::vector<Ability*>::const_iterator selected_ability;
	std::vector<Ability*>::const_iterator previous_ability;
	uint current_hero = 1;
	uint cleric_upgrades = 1;
	uint warrior_upgrades = 1;
	uint dwarf_upgrades = 1;
	uint elf_upgrades = 1;
	SDL_Rect marker_pos = {0,0,44,44};

	UIElement* description = nullptr;
	UIElement* description_bg = nullptr;
	UIElement* title = nullptr;
	iPoint title_pos = { 0,25 };
	bool selecting_ability = false;

	UIElement* select_menu_bg = nullptr;
	UIElement* select_menu_text = nullptr;
	//string select_menu_mesage = "Are you sure you want to unlock ";
	UIElement* select_menu_A = nullptr;
	UIElement* select_menu_B = nullptr;
	UIElement* arrow = nullptr;
	std::vector<UIElement*> accept_decline;
	std::vector<UIElement*>::const_iterator option;
	ctPerfTimer execute_comand_time;
};


#endif // __ctSkillTree_H__