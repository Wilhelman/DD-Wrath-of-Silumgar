#ifndef __Warrior_H__
#define __Warrior_H__

#include "ctAnimation.h"
#include "ctPoint.h"
#include "Entity.h"

class Warrior : public Entity
{

public:

	//clerics's current frame (entity)
	SDL_Rect current_frame = { 0,0,0,0 };

private:

	//player state

	//animations velocity
	uint idle_vel = 0u, run_forward_vel = 0u;

private:

	void SetPlayerAnimationsSpeed(float dt);
	void SetEntitiesSpeed(float dt);
	void LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation);

public:

	Warrior(int x, int y, EntityType type);
	~Warrior();

	void Update(float dt);
};

#endif