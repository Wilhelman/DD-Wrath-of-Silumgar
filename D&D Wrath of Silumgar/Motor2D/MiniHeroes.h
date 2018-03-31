#ifndef __MiniHeroes_H__
#define __MiniHeroes_H__

#include "ctAnimation.h"
#include "ctPoint.h"
#include "Entity.h"

class MiniHeroes : public Entity
{

public:

	//clerics's current frame (entity)
	SDL_Rect current_frame = { 0,0,0,0 };

private:


	//animations

	//animations velocity
	uint idle_vel = 0u, run_forward_vel = 0u;

private:

	void SetPlayerAnimationsSpeed(float dt);
	void SetEntitiesSpeed(float dt);
	void LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation);

public:

	MiniHeroes(int x, int y, EntityType type);
	~MiniHeroes();

	void Update(float dt);
};

#endif