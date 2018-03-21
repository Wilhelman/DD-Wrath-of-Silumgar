#ifndef _PLAYER_ELF_
#define _PLAYER_ELF_


#include "Entity.h"
#include "ctAnimation.h"



class Elf :public Entity
{
public:
	Elf();
	~Elf();

private:


	ctAnimation anim_attack;
	//ctAnimation* current_animation = nullptr;


};



#endif