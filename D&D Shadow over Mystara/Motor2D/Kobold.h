#ifndef _KOBOLD_
#define _KOBOLD_

#include "Entity.h"
#include "ctAnimation.h"



class Kobold :public Entity
{
public: 
	Kobold();
	~Kobold();

private:
	ctAnimation * current_animation = nullptr;
	ctAnimation anim_attack;
	


};



#endif