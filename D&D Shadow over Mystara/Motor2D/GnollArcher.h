#ifndef _GNOLLARCHER_
#define _GNOLLARCHER_

#include "Entity.h"
#include "ctAnimation.h"



class GnollArcher :public Entity
{
public:
	GnollArcher();
	~GnollArcher();

private:
	ctAnimation * current_animation = nullptr;
	ctAnimation anim_attack;



};



#endif