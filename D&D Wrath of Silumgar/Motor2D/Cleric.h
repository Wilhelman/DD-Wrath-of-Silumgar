#ifndef __Cleric_H__
#define __Cleric_H__

#include "ctAnimation.h"
#include "ctPoint.h"
#include "Entity.h"

class Cleric : public Entity
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

	Cleric(int x, int y, EntityType type);
	~Cleric();

	void Update(float dt);
};

#endif