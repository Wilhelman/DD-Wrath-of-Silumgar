#ifndef _GOBLIN_
#define _GOBLIN_

#include "Entity.h"
#include "ctAnimation.h"



class Goblin :public Entity
{
public:
	Goblin();
	~Goblin();

private:
	ctAnimation * current_animation = nullptr;
	ctAnimation anim_attack;



};



#endif