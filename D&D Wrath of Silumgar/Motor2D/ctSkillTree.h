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

	void DrawAbility(int x, int y, Ability* champion);

	void DrawAbilities(Ability* champion);

private:

	std::string skill_tree_map_tmx;
	std::string name_spritesheet_skill_tree_map;

	SDL_Texture* spritesheet_skill_tree_map = nullptr;

private:

	std::string name_spritesheet_abilities;
	SDL_Texture* spritesheet_abilities = nullptr;


	Ability* warrior = new Ability();
	uint current_hero = 1;

};


#endif // __ctSkillTree_H__