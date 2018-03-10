#include "ctApp.h"
#include "Entity.h"
#include "ctEntities.h"
#include "ctRender.h"

Entity::Entity(EntityType type)
{
	entity_type = type;


	switch (entity_type)
	{
	case CLERIC:
		name.assign("Cleric");
		break;
	case KOBOLD:
		name.assign("Kobold");
		break;
	default:
		break;
	}
	

	pugi::xml_document data;

	if (pugi::xml_parse_result result = data.load_file("Entities.xml"))
	{
		pugi::xml_node stat = data.child("Entities").child(name.c_str()).child("Stadistics");

		base_stats.base_constitution = stat.attribute("Constitution").as_int(1);
		base_stats.base_focus = stat.attribute("Focus").as_int(1);
		base_stats.base_strength = stat.attribute("Force").as_int(1);
		base_stats.base_intelligence = stat.attribute("Intelligence").as_int(1);
		base_stats.base_dexterity = stat.attribute("Dexterity").as_int(1);
		base_stats.base_agility = stat.attribute("Agility").as_int(1);
		base_stats.base_physical_defense = stat.attribute("PhysicalDefense").as_int(1);
		base_stats.base_magical_defense = stat.attribute("MagicalDefense").as_int(1);
		base_stats.base_luck = stat.attribute("Luck").as_int(1);
		base_stats.base_judgement = stat.attribute("Judgement").as_int(100);


	}
}

Entity::~Entity()
{
}
//
//void Entity::Draw(SDL_Texture* sprites)
//{
//	SDL_Rect r = animation->GetCurrentFrame();
//	
//	if (animation != nullptr)
//		App->render->Blit(sprites, position.x, position.y - r.h, &(animation->GetCurrentFrame()));
//}