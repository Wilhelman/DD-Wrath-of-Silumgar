#include "Player_Cleric.h"
#include "ctLog.h"



Cleric::Cleric() : Entity(CLERIC)
{
	pugi::xml_document data;

	if (pugi::xml_parse_result result = data.load_file("Entities.xml"))
	{
		pugi::xml_node anim = data.child("Entities").child("Cleric").child("Animations").child("Attack").child("frame");
		pugi::xml_node path = data.child("Entities").child("Cleric").child("spritesheetSource");

		anim_Path.assign(path.attribute("name").as_string());
		LOG(anim_Path.c_str());

		while (anim != nullptr)
		{
			anim_attack.PushBack({ anim.attribute("x").as_int(), anim.attribute("y").as_int(), anim.attribute("w").as_int(), anim.attribute("h").as_int() });
			anim = anim.next_sibling();
		}

		LoadProperties(data.child("Entities").child("Cleric").child("Statistics"));

		//anim_attack.speed = anim.child("speed").attribute("speed").as_float();
		LOG("%d", anim_attack.speed);
		
		animation = &anim_attack;
	}
}

Cleric::~Cleric()
{

}