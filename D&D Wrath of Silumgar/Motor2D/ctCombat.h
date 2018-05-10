#ifndef __ctCOMBAT_H__
#define __ctCOMBAT_H__

#include "ctModule.h"
#include "ctGui.h"

#include "Entity.h"
#include <string>

using namespace std;

enum EntityType;

class UICombatMenu;
class UIBar;

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

	//load all the values like health and mana from the xml
	void LoadDataFromXML();

	

public:

	void SetSceneName(string new_scene_name);

	void UpdateHPBarOfEntity(Entity* entity_to_update_bar, int quantity);
	void UpdateManaBarOfEntity(Entity* entity_to_update_bar, int quantity);

	//cuarentena
	//uint CalculatedDamage(Entity* attacker, Entity* defender);

	//TEST to priority attack
	std::vector<Entity*> turn_priority_entity;
	std::vector<Entity*> draw_turn_priority_entity;
	std::vector<EntityType> entities_to_spawn;

	//UICombat stuff
	std::vector<Entity*> enemies;
	std::vector<Entity*> heroes;

	//only gets the lowest hp
	Entity* GetTheWeakestHeroe();

	Entity* GetRandomHeroe();

	void SelectWithPreviousHeroe();
	bool pause_menu_is_open = false;
	UIElement* pauseMenu = nullptr;
	
private:

	bool heroes_are_dead = false;
	
	//spawn heroes and enemies
	void SpawnEntities();

	//load all the values like health and mana from the xml
	//void LoadDataFromXML();

	void LoadSkill(pugi::xml_node skill_node, Entity* entity);
	void LoadItem(pugi::xml_node item, Entity* entity);

	//set the base data at the start of the first combat here we write in the xml.
	void SaveDataToXML();

	//Set the values for the health and mana bar and all the things in the UI from the data of entities
	void SetDataToUI();

	//Set visible or invisible enemy bars
	void EnemiesBarShouldBeVisible();

	void OrderTurnPriority();
	void DrawTurnPriority();

	bool PerformActionWithEntity(Entity* entity_to_perform_action);

	//IA STUFF

	UIBar* GetUIBarFromEntity(Entity* entity);

	
private:

	

	string scene_name;
	bool making_decision = false;

	//UICombat stuff
	UICombatMenu* combat_menu = nullptr;
	UIElement* ready_cleric = nullptr;
	UIElement* ready_warrior = nullptr;
	UIElement* ready_dwarf = nullptr;
	UIElement* ready_elf = nullptr;

	//UI STUFF
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
	UIElement* cleric_name = nullptr;
	UIElement* warrior_name = nullptr;
	UIElement* dwarf_name = nullptr;
	UIElement* elf_name = nullptr;


	
	//END IA STUFF

	std::vector<UIBar*> enemies_bars;
public:
	bool condition_victory = true;
};


#endif // __ctCOMBAT_H__