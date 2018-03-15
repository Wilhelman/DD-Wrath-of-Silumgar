#ifndef _PLAYER_CLERIC_
#define _PLAYER_CLERIC_


#include "Entity.h"
#include "ctAnimation.h"



class Cleric :public Entity
{
public:
	Cleric();
	~Cleric();

private:


	ctAnimation anim_attack;
	


};



#endif
