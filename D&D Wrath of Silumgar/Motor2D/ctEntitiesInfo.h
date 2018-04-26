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
	uint turn_left = 0u;

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
	KICK,
	HIGH_AXE,
	HEAL,
	MINDBLOWN,

	LOW_HEAL_POTION,
	HIGH_HEAL_POTION,
	POISONED_DAGGER,

	ACTION_NOT_DEFINED
};

enum ActionObjectiveType {
	HEROES,
	ENEMIES,
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

};


#endif