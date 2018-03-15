#include "Owlbear.h"



Owlbear::Owlbear() :Entity(OWLBEAR)
{

	pugi::xml_document data;

	if (pugi::xml_parse_result result = data.load_file("Entities.xml"))
	{
		pugi::xml_node anim = data.child("Entities").child("Owlbear").child("Animations").child("Idle").child("frame");

		while (anim != nullptr)
		{
			anim_attack.PushBack({ anim.attribute("x").as_int(), anim.attribute("y").as_int(), anim.attribute("w").as_int(), anim.attribute("h").as_int() });
			anim = anim.next_sibling();
		}

		LoadProperties(data.child("Entities").child("Owlbear").child("Statistics"));

		anim_attack.speed = anim.child("speed").attribute("speed").as_int();
	}


}

Owlbear::~Owlbear()
{

}