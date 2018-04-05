#include "ctApp.h"
#include "Entity.h"
#include "ctRender.h"

Entity::Entity(int x, int y, EntityType type) : position(x, y), type(type)
{
}

Entity::~Entity()
{
}

void Entity::Draw(SDL_Texture* sprites)
{
	SDL_Rect r = animation->GetCurrentFrame();

	if (animation != nullptr) {
		if (flip_texture == false)
		App->render->Blit(sprites, position.x, position.y - r.h, &(animation->GetCurrentFrame()));
		else {
		App->render->Blit(sprites, position.x, position.y - r.h, &(animation->GetCurrentFrame()), NULL,NULL, SDL_FLIP_HORIZONTAL);
		}
	}
}

bool Entity::LoadProperties(pugi::xml_node properties) {
	bool ret = true;

	base_stats.base_constitution = properties.attribute("constitution").as_uint();
	base_stats.base_focus = properties.attribute("focus").as_uint();
	base_stats.base_strength = properties.attribute("strength").as_uint();
	base_stats.base_agility = properties.attribute("agility").as_uint();
	base_stats.base_dexterity = properties.attribute("dexterity").as_uint();
	base_stats.base_intelligence = properties.attribute("intelligence").as_uint();
	base_stats.base_physical_defense = properties.attribute("physicalDefense").as_uint();
	base_stats.base_magical_defense = properties.attribute("magicalDefense").as_uint();
	base_stats.base_luck = properties.attribute("luck").as_uint();
	base_stats.base_judgement = properties.attribute("judgement").as_uint();

	return ret;
}