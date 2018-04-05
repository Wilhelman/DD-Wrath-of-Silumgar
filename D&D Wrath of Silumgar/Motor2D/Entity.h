#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "ctPoint.h"
#include "ctAnimation.h"
#include "ctEntities.h"

#include "SDL/include/SDL.h"

struct SDL_Texture;

struct Stats {
	uint base_constitution = 0u;
	uint base_focus = 0u;
	uint base_strength = 0u;
	uint base_intelligence = 0u;
	uint base_dexterity = 0u;
	uint base_agility = 0u;
	uint base_physical_defense = 0u;
	uint base_magical_defense = 0u;
	uint base_luck = 0u;
	uint base_judgement = 0u;
};

struct Altered_Stats {
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

struct Item {
	uint constitution = 0u;
	uint focus = 0u;
	uint strength = 0u;
	uint intelligence = 0u;
	uint dexterity = 0u;
	uint agility = 0u;
	uint physical_defense = 0u;
	uint magical_defense = 0u;
	uint luck = 0u;
	uint judgement = 0u;
};

struct Skill {
	std::string name;
	uint mana_cost = 0u;
	int health_potion_effect = 0;
	int mana_potion_effect = 0;
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

	

public:
	ctAnimation* animation = nullptr;
	ctAnimation idle = ctAnimation();
	ctAnimation run_forward = ctAnimation();
	ctAnimation run_backward = ctAnimation();
	ctAnimation attack = ctAnimation();
	ctAnimation damaged = ctAnimation();

	Stats base_stats;
	iPoint position = iPoint(0, 0);
	EntityType type = EntityType::NO_TYPE;
	bool to_destroy = false;
	bool flip_texture = false;
	

public:
	Entity(int x, int y, EntityType type);
	virtual ~Entity();

	virtual void Update(float dt) {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void SetEntitiesSpeed(float dt) {};

	virtual void LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation) {};
	bool LoadProperties(pugi::xml_node properties);
};

#endif // __ENTITY_H__