#ifndef _TRITON_
#define _TRITON_

#include "Entity.h"
#include "ctAnimation.h"



class Triton :public Entity
{
public:
	Triton();
	~Triton();

private:
	ctAnimation * current_animation = nullptr;
	ctAnimation anim_attack;



};



#endif