#ifndef _OWLBEAR_
#define _OWLBEAR_

#include "Entity.h"
#include "ctAnimation.h"



class Owlbear :public Entity
{
public:
	Owlbear();
	~Owlbear();

private:
	ctAnimation * current_animation = nullptr;
	ctAnimation anim_attack;



};



#endif