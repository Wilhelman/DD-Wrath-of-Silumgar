#ifndef _ALCHEMISTGOBLIN_
#define _ALCHEMISTGOBLIN_

#include "Entity.h"
#include "ctAnimation.h"



class AlchemistGoblin :public Entity
{
public:
	AlchemistGoblin();
	~AlchemistGoblin();

private:
	ctAnimation * current_animation = nullptr;
	ctAnimation anim_attack;



};



#endif