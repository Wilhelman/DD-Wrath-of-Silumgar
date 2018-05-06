#ifndef __DarkWarrior_H__
#define __DarkWarrior_H__

#include "ctAnimation.h"
#include "ctPoint.h"
#include "Entity.h"

class DarkWarrior : public Entity
{

public:

	//clerics's current frame (entity)
	SDL_Rect current_frame = { 0,0,0,0 };

private:

	//animations velocity
	uint idle_vel = 0u, run_forward_vel = 0u;

	Action void_cannon_action;
	Action thunder_punch_action;
	Action insignificant_mortals_action;

private:

	bool boosted = false;
	void SetPlayerAnimationsSpeed(float dt);
	void SetEntitiesSpeed(float dt);
	void LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation);

public:

	DarkWarrior(int x, int y, EntityType type);

	void Update(float dt);
	void Attack();
	void Death();
	void Run();
	void Damaged();
	void Ability1T1();
	void Ability2T1();
	void Ability3T1();

	void PerformAction();
};

#endif