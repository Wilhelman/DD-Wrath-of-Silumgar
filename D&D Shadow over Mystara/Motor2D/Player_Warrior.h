#ifndef _PLAYER_WARRIOR_
#define _PLAYER_WARRIOW_


#include "Entity.h"
#include "ctAnimation.h"



class Warrior :public Entity
{
public:
	Warrior();
	~Warrior();

private:


	ctAnimation anim_attack;
	ctAnimation* current_animation = nullptr;


};



#endif