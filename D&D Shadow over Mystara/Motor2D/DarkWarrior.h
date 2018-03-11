#ifndef _DARKWARRIOR_
#define _DARKWARRIOR_

#include "Entity.h"
#include "ctAnimation.h"



class DarkWarrior :public Entity
{
public:
	DarkWarrior();
	~DarkWarrior();

private:
	ctAnimation * current_animation = nullptr;
	ctAnimation anim_attack;



};



#endif