#ifndef _GNOLL_
#define _GNOLL_

#include "Entity.h"
#include "ctAnimation.h"



class Gnoll :public Entity
{
public:
	Gnoll();
	~Gnoll();

private:
	//ctAnimation * current_animation = nullptr;
	ctAnimation anim_attack;



};



#endif