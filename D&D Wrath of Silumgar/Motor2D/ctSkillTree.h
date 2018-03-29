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
		uint mana = 0u;
		uint branch = 0u;
		Entity* player;
		std::string ability_name;
		SDL_Rect icon_rect;
		iPoint coords_in_map = { 0,0 };
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
	
	/*void DrawAbilities(vector<Ability*> vec);*/

private:

	std::string skill_tree_map_tmx;
	std::string name_spritesheet_skill_tree_map;

	SDL_Texture* spritesheet_skill_tree_map = nullptr;

private:

	std::string name_spritesheet_abilities;
	SDL_Texture* spritesheet_abilities = nullptr;

	//vector<Ability*> warrior_vec;
	//vector<Ability*> cleric_vec;
	//vector<Ability*> dwarf_vec;
	//vector<Ability*> elf_vec;

	uint current_hero = 1;

};


#endif // __ctSkillTree_H__