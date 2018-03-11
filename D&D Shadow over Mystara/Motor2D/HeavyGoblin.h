#ifndef _HEAVYGOBLIN_
#define _HEAVYGOBLIN_

#include "Entity.h"
#include "ctAnimation.h"



class HeavyGoblin :public Entity
{
public:
	HeavyGoblin();
	~HeavyGoblin();

private:
	ctAnimation * current_animation = nullptr;
	ctAnimation anim_attack;



};



#endif