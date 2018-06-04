#ifndef __CTENTITIES_INFO_H__
#define __CTENTITIES_INFO_H__


#include "SDL/include/SDL.h"
#include "ctDefs.h"
#include <vector>
#include <string>


struct Stats {
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

struct Altered_Stat {
	int turn_left = 0;

	bool stun = false;
	bool bleeding = false;
	bool poison = false;
	bool burn = false;

	int stat_effect_constitution = 0;
	int stat_effect_focus = 0;
	int stat_effect_strength = 0;
	int stat_effect_intelligence = 0;
	int stat_effect_dexterity = 0;
	int stat_effect_agility = 0;
	int stat_effect_physical_defense = 0;
	int stat_effect_magical_defense = 0;
	int stat_effect_luck = 0;
	int stat_effect_judgement = 0;
};

enum ActionType {
	DEFAULT_ATTACK,
	//FIGHTER
	KICK,
	GUARD,
	HEAVY_SLASH,
	KICK_PLUS,
	GUARD_PLUS,
	HEAVY_SLASH_PLUS,
	CHARGE,
	TAUNT,
	WHIRLWIND,
	//DWARF
	HIGH_AXE,
	MORALE_BOOST,
	DWARFS_WRATH,
	HIGH_AXE_PLUS,
	MORALE_BOOST_PLUS,
	DWARFS_WRATH_PLUS,
	RAGING_MOCK,
	METEOR,
	COUNTER,
	COUNTERED,
	//CLERIC
	HEAL,
	LIGHT_STRIKE,
	MACE_THROW,
	HEAL_PLUS,
	LIGHT_STRIKE_PLUS,
	MACE_THROW_PLUS,
	BLESSING,
	HARDEN_SKIN,
	CLARITY,
	//ELF
	BLIZZARD,
	SEED_OF_LIFE,
	FIREBALL,
	LIGHTNING_BOLT,
	SEED_OF_LIFE_PLUS,
	FIREBALL_PLUS,
	LIGHTNING_BOLT_PLUS,
	REVIVE,
	FIRE_DJINN,
	//DARK WARRIOR
	THUNDER_PUNCH,
	VOID_CANNON,
	INSIGNIFICANT_MORTALS,
	//Troll Leaders
	CLAW_ATTACK,
	BLOCK,
	INFESTED_CLAW,
	BROTHERLY_RAGE,
	//Displacer beast
	RUSH,
	INFERNAL_FIRE,
	SHADOW_JAB,
	CALL_OF_THE_DEAD,
	//Lich 2
	SEA_OF_FLAMES,
	BIDIMENSIONAL_CLAW,
	//LICH 1
	DRAGON_FLAMES,
	LIGHTNING_FLASH,
	ABYSSAL_SPHERE,
	
	//OBJECTS
	LOW_HEALTH_RECOVER_ACTION,
	HIGH_HEALTH_RECOVER_ACTION,
	LOW_MANA_RECOVER_ACTION,
	HIGH_MANA_RECOVER_ACTION,
	POISONED_DAGGER_ACTION,
	BURN_TARGET_ACTION,
	REVIVE_AN_ALLY_ACTION,


	ACTION_NOT_DEFINED
};

enum ActionObjectiveType {
	HEROES,
	ENEMIES,
	DEAD_HEROES,
	OBJECTIVE_NOT_DEFINED
};

struct Action {

	std::string name;
	std::string description;
	ActionType type = ACTION_NOT_DEFINED;
	ActionObjectiveType objective = OBJECTIVE_NOT_DEFINED;

	uint mana_cost = 0u;

	int health_points_effect_to_himself = 0;
	int mana_points_effect_to_himself = 0;
	int health_points_effect = 0;
	int mana_points_effect = 0;

	uint stun_chance = 0u;
	uint bleeding_chance = 0u;
	uint position_chance = 0u;
	uint burn_chance = 0u;

	uint constitution_variation = 0u;
	uint focus_variation = 0u;
	uint strength_variation = 0u;
	uint intelligence_variation = 0u;
	uint dexterity_variation = 0u;
	uint agility_variation = 0u;
	uint physical_defense_variation = 0u;
	uint magical_defense_variation = 0u;
	uint luck_variation = 0u;
	uint judgement_variation = 0u;

	bool have_to_move = false;

	bool owned = false;
};


#endif