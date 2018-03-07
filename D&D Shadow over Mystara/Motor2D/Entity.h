#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "ctPoint.h"
#include "ctAnimation.h"

struct SDL_Texture;
struct Collider;

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

class Entity
{
private:
	SDL_Rect last_frame = { 0,0,0,0 };
protected:
	ctAnimation* animation = nullptr;
	uint currentTime = 0u;
	uint lastTime = 0u;
	bool key_entities_speed = false;

	uint health_points = 0;
	uint mana_points = 0;

public:
	iPoint position = iPoint(0, 0);
	bool to_destroy = false;

public:
	Entity(int x, int y);
	virtual ~Entity();

	virtual void Update(float dt) {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void SetEntitiesSpeed(float dt) {};

	virtual void LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation) {};

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}
};

#endif // __ENTITY_H__