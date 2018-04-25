#ifndef __ctITEMS_H__
#define __ctITEMS_H__

#include "ctModule.h"
#include "ctDefs.h"
#include "SDL/include/SDL.h"
#include "Entity.h"

#include <vector>
using namespace std;

struct SDL_Rect;

struct ItemStats {
	int constitution = 0;
	int focus = 0;
	int strength = 0;
	int intelligence = 0;
	int dexterity = 0;
	int agility = 0;
	int physical_defense = 0;
	int magical_defense = 0;
	int luck = 0;
	int judgement = 0;
};

enum UsableEffects {
	LOW_HEALTH_RECOVER,
	HIGH_HEALTH_RECOVER,
	LOW_MANA_RECOVER,
	HIGH_MANA_RECOVER,
	POISONED_DAGGER,
	BURN_TARGET,
	REVIVE_AN_ALLY,

	NO_USABLE_EFFECTS
};

enum EquipEffects {
	NO_EQUIP_EFFECTS
};

enum ItemType {
	USABLE,
	EQUIP,

	NO_ITEM_TYPE
};

enum EquipType {
	BOOT,
	ARM,
	CHEST,
	HELM,

	NO_EQUIP_TYPE
};

struct Item {
	string name;
	ItemType type = NO_ITEM_TYPE;
	uint tier = 0u;
	EquipType equip_type = NO_EQUIP_TYPE;
	int objective = 0;
	SDL_Rect draw_coords = { 0,0,0,0 };
	UsableEffects usable_effects = NO_USABLE_EFFECTS;
	EquipEffects equip_effects = NO_EQUIP_EFFECTS;
	ItemStats statistics;
};

class ctItems : public ctModule
{
public:

	ctItems();

	// Destructor
	virtual ~ctItems();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:

private:
	std::vector<Item> usable_items;
	std::vector<Item> tier_1_equips;
	std::vector<Item> tier_2_equips;
	std::vector<Item> tier_3_equips;

};

#endif // __ctITEMS_H__