#ifndef _PLAYER_DWARF_
#define _PLAYER_DWARF_


#include "Entity.h"
#include "ctAnimation.h"



class Dwarf :public Entity
{
public:
	Dwarf();
	~Dwarf();

private:


	ctAnimation anim_attack;
	ctAnimation* current_animation = nullptr;


};



#endif