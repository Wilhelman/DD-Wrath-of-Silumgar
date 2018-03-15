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
	case DWARF:
		name.assign("Dwarf");
		break;
	case ELF:
		name.assign("Elf");
		break;
	case WARRIOR:
		name.assign("Warrior");
		break;
	case KOBOLD:
		name.assign("Kobold");
		break;
	case GOBLIN:
		name.assign("Goblin");
		break;
	case ALCHEMISTGOBLIN:
		name.assign("AlchemistGoblin");
		break;
	case HEAVYGOBLIN:
		name.assign("HeavyGoblin");
		break;
	case GNOLL:
		name.assign("Gnoll");
		break;
	case GNOLLARCHER:
		name.assign("GnollArcher");
		break;
	case SKELETON:
		name.assign("Skeleton");
		break;
	case OWLBEAR:
		name.assign("Owlbear");
		break;
	case TRITON:
		name.assign("Triton");
		break;
	case HELLHOUND:
		name.assign("Hellhound");
		break;
	case DARKWARRIOR:
		name.assign("DarkWarrior");
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

void Entity::Draw(SDL_Texture* sprites)
{
	SDL_Rect r = animation->GetCurrentFrame();

	if (animation != nullptr)
		App->render->Blit(sprites, battleground_position.x, battleground_position.y - r.h, &(animation->GetCurrentFrame()));
}

bool Entity::LoadProperties(pugi::xml_node properties) {
	bool ret = true;

	base_stats.base_constitution = properties.attribute("Constitution").as_uint();
	base_stats.base_focus = properties.attribute("Focus").as_uint();
	base_stats.base_strength = properties.attribute("Force").as_uint();
	base_stats.base_agility = properties.attribute("Agility").as_uint();
	base_stats.base_dexterity = properties.attribute("Skill").as_uint();
	base_stats.base_intelligence = properties.attribute("Intelligence").as_uint();
	base_stats.base_physical_defense = properties.attribute("PhysicalDefense").as_uint();
	base_stats.base_magical_defense = properties.attribute("MagicalDefense").as_uint();
	base_stats.base_luck = properties.attribute("Luck").as_uint();
	base_stats.base_judgement = properties.attribute("Judgement").as_uint();

	return ret;
}