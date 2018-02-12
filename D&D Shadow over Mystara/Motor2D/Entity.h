#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "ctPoint.h"
#include "ctAnimation.h"

struct SDL_Texture;
struct Collider;

class Entity
{
private:
	SDL_Rect last_frame = { 0,0,0,0 };
protected:
	ctAnimation* animation = nullptr;
	Collider* collider = nullptr;
	int lives = 0;
	uint currentTime = 0u;
	uint lastTime = 0u;
	bool key_entities_speed = false;

public:
	iPoint position = iPoint(0, 0);
	int type = 0;
	bool to_destroy = false;

public:
	Entity(int x, int y);
	virtual ~Entity();

	const Collider* GetCollider() const;

	virtual void Update(float dt) {};
	virtual void Shoot(float dt) {};
	virtual void Draw(SDL_Texture* sprites);
	virtual void OnCollision(Collider* collider);
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