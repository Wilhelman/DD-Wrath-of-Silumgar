#ifndef __Gnoll_H__
#define __Gnoll_H__

#include "ctAnimation.h"
#include "ctPoint.h"
#include "Entity.h"

class Gnoll : public Entity
{

public:

	//clerics's current frame (entity)
	SDL_Rect current_frame = { 0,0,0,0 };

private:

	//player state

	//animations velocity
	uint idle_vel = 0u, run_forward_vel = 0u;
	float IDLE_SPEED_DAMAGED = 1.5f;
private:

	void SetPlayerAnimationsSpeed(float dt);
	void SetEntitiesSpeed(float dt);
	void LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation);

public:

	Gnoll(int x, int y, EntityType type);

	void Update(float dt);
	void Attack();
	void Death();
	void Run();
	void Damaged();

	void PerformAction();
};

#endif