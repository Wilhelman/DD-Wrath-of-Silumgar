#ifndef __TrollLeaders_H__
#define __TrollLeaders_H__

#include "ctAnimation.h"
#include "ctPoint.h"
#include "Entity.h"

class TrollLeaders : public Entity
{

public:

	//clerics's current frame (entity)
	SDL_Rect current_frame = { 0,0,0,0 };

private:

	//animations velocity
	uint idle_vel = 0u, run_forward_vel = 0u;

	Action claw_attack;
	Action block;
	Action infested_claw;
	Action brotherly_rage;

private:

	bool boosted = false;
	void SetPlayerAnimationsSpeed(float dt);
	void SetEntitiesSpeed(float dt);
	void LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation);

public:

	TrollLeaders(int x, int y, EntityType type);

	void Update(float dt);
	void Attack();
	void Death();
	void Run();
	void Damaged();
	/*void Ability1T1();
	void Ability2T1();
	void Ability3T1();*/

	void PerformAction();
};

#endif