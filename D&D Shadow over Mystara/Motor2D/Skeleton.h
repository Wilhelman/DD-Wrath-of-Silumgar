#ifndef _SKELETON_
#define _SKELETON_

#include "Entity.h"
#include "ctAnimation.h"



class Skeleton :public Entity
{
public:
	Skeleton();
	~Skeleton();

private:
	ctAnimation * current_animation = nullptr;
	ctAnimation anim_attack;



};



#endif