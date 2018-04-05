#ifndef __GnollArcher_H__
#define __GnollArcher_H__

#include "ctAnimation.h"
#include "ctPoint.h"
#include "Entity.h"

class GnollArcher : public Entity
{

public:

	//clerics's current frame (entity)
	SDL_Rect current_frame = { 0,0,0,0 };

private:

	//player state

	//animations

	//animations velocity
	uint idle_vel = 0u, run_forward_vel = 0u;

private:

	void SetPlayerAnimationsSpeed(float dt);
	void SetEntitiesSpeed(float dt);
	void LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation);

public:

	GnollArcher(int x, int y, EntityType type);
	~GnollArcher();

	void Update(float dt);
};

#endif