#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "ctPoint.h"
#include "ctAnimation.h"
#include "ctEntities.h"
#include "ctTextures.h"
#include "SDL/include/SDL.h"
#include "ctItems.h"

#include <vector>

struct SDL_Texture;
struct Item;

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

class Entity
{
protected:
	
	bool key_entities_speed = false;

	int current_health_points, current_mana_points, current_strength, current_intelligence, current_luck, current_agility_points, current_dexterity_points, current_physical_defense_points, current_magical_defense_points, current_judgement = 0;

	int max_health_points, max_mana_points = 0;

	bool dead = false;

public:
	ctAnimation* animation = nullptr;
	ctAnimation* stun_animation = nullptr;

	ctAnimation idle = ctAnimation();
	ctAnimation run_forward = ctAnimation();
	ctAnimation run_backward = ctAnimation();
	ctAnimation attack = ctAnimation();
	ctAnimation hit = ctAnimation();
	ctAnimation death = ctAnimation();
	ctAnimation stun = ctAnimation();
	ctAnimation dodge = ctAnimation();

	//custom animations
	ctAnimation kick = ctAnimation();
	ctAnimation high_axe = ctAnimation();
	ctAnimation heal = ctAnimation();
	ctAnimation mind_blown = ctAnimation();
	ctAnimation throw_object = ctAnimation();
	ctAnimation use_potion = ctAnimation();

	Stats base_stats;
	iPoint position = iPoint(0, 0);
	iPoint initial_position = iPoint(0, 0);
	EntityType type = EntityType::NO_TYPE;
	bool to_destroy = false;
	bool flip_texture = false;
	
	SDL_Texture* texture = nullptr;

	//Priority Draw Order
	uint priority_draw_order = 0u;

	//Combat stuff
	Action default_attack;

	std::vector<Action> abilities;
	std::vector<Altered_Stat> altered_stats;

	std::vector<Item*> usable_items;

	//Armor stuff
	Item* ring = nullptr;
	Item* accessory = nullptr;

	Item* helmet = nullptr;
	Item* chest = nullptr;
	Item* guantlet = nullptr;
	Item* boot = nullptr;

	Item* shield = nullptr;
	Item* weapon = nullptr;


	//Sounds stuff
	uint steps_timer = 0u;
	uint current_timer = 0u;

public:

	Entity(int x, int y, EntityType type);
	virtual ~Entity();

	virtual void Update(float dt) {};
	virtual void Draw();
	virtual void SetEntitiesSpeed(float dt) {};

	virtual void LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation) {};
	bool LoadProperties(pugi::xml_node properties);

	//for sounds
	virtual void Attack() {};
	virtual void Run() {};
	virtual void Death() {};
	virtual void Damaged() {};
	virtual void Ability1() {};

	//Combat stuff

	virtual void NewTurn();

	virtual void PerformAction() {};

	void AddAlteredStat(Altered_Stat new_altered_stat);

	bool IsStunned()const;

	int GetCurrentHealthPoints();
	int GetCurrentManaPoints();
	int GetCurrentAgilityPoints();
	int GetCurrentDexterityPoints();
	int GetCurrentPhysicalDefensePoints();
	int GetCurrentMagicalDefensePoints();

	int GetCurrentJudgement();

	void SetCurrentHealthPoints(int new_health_points);
	void SetCurrentManaPoints(int new_mana_points);

	bool IsGoingToDoAnythingClever();

	void AddAction(Action new_action);

	private:
	void CalculateAllStats();
};

#endif // __ENTITY_H__