#ifndef __ctCOMBAT_H__
#define __ctCOMBAT_H__

#include "ctModule.h"
#include "ctGui.h"
#include "UIBar.h"
#include "Entity.h"
#include <string>

using namespace std;

enum EntityType;



class ctCombat : public ctModule
{
public:

	ctCombat();

	// Destructor
	virtual ~ctCombat();

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

	void OrderPriority();
	void DrawPriority();

public:

	void SetSceneName(string new_scene_name);

	uint CalculatedDamage(Entity* attacker, Entity* defender);
	//TEST to priority attack
	std::vector<Entity*> priority_entity;
	
private:
	
	void SpawnEntities();
	void SpawnEnemies(string sceneName);
	// TEST FOR UI BAR WITH CALC DAMAGE
	UIBar* test = nullptr;
private:

	string scene_name;

	UIElement* cleric_background = nullptr;
	UIElement* warrior_background = nullptr;
	UIElement* dwarf_background = nullptr;
	UIElement* elf_background = nullptr;
	UIBar* cleric_HP_bar = nullptr;
	UIBar* warrior_HP_bar = nullptr;
	UIBar* dwarf_HP_bar = nullptr;
	UIBar* elf_HP_bar = nullptr;
	UIBar* cleric_mana_bar = nullptr;
	UIBar* warrior_mana_bar = nullptr;
	UIBar* dwarf_mana_bar = nullptr;
	UIBar* elf_mana_bar = nullptr;

};


#endif // __ctCOMBAT_H__