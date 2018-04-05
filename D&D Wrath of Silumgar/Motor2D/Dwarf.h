#ifndef __DWARF_H__
#define __DWARF_H__

#include "ctAnimation.h"
#include "ctPoint.h"
#include "Entity.h"

class Dwarf : public Entity
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

	Dwarf(int x, int y, EntityType type);
	~Dwarf();

	void Update(float dt);
};

#endif