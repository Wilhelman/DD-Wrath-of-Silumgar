#ifndef _HELLHOUND_
#define _HELLHOUND_

#include "Entity.h"
#include "ctAnimation.h"



class Hellhound :public Entity
{
public:
	Hellhound();
	~Hellhound();

private:
	ctAnimation * current_animation = nullptr;
	ctAnimation anim_attack;



};



#endif