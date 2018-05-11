#ifndef __Lich1_H__
#define __Lich1_H__

#include "ctAnimation.h"
#include "ctPoint.h"
#include "Entity.h"

class Lich1 : public Entity
{

public:

	//clerics's current frame (entity)
	SDL_Rect current_frame = { 0,0,0,0 };

private:

	//animations velocity
	uint idle_vel = 0u, run_forward_vel = 0u;
	bool is_transformed = false;

	Action dragon_flames_action;
	Action lightning_flash_action;
	Action abyssal_sphere_action;
	Action bidimensional_claw_action;
	Action sea_of_flames_action;

	ctAnimation idle_transformed = ctAnimation();
	ctAnimation run_transformed = ctAnimation();
	ctAnimation hit_transformed = ctAnimation();
	ctAnimation death_transformed = ctAnimation();
	ctAnimation stun_transformed = ctAnimation();
	ctAnimation dodge_transformed = ctAnimation();


private:

	bool second_form = false;
	void SetPlayerAnimationsSpeed(float dt);
	void SetEntitiesSpeed(float dt);
	void LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation);

public:

	Lich1(int x, int y, EntityType type);

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
