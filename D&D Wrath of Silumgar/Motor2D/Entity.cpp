#include "ctApp.h"
#include "Entity.h"
#include "ctRender.h"

Entity::Entity(int x, int y, EntityType type) : position(x, y), type(type)
{
	initial_position = iPoint(x, y);
}

Entity::~Entity()
{
	App->tex->UnLoad(texture);
}

void Entity::Draw()
{
	SDL_Rect r = animation->GetCurrentFrame();

	if (animation != nullptr) {
		if (flip_texture == false)
		App->render->Blit(texture, position.x, position.y - r.h, &(animation->GetCurrentFrame()));
		else {
		App->render->Blit(texture, position.x, position.y - r.h, &(animation->GetCurrentFrame()), NULL,NULL, SDL_FLIP_HORIZONTAL);
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

	/* ADDING BASIC ACTION FOR ALL THE ENTITIES THIS SHOULD BE CHANGED */
	default_attack.name = "Attack";
	default_attack.type = DEFAULT_ATTACK;
	default_attack.health_points_effect = -(int)base_stats.base_strength * StatisticsValues::STRENGTH;

	//todo change this
	current_health_points = base_stats.base_constitution * StatisticsValues::CONSTITUTION;
	current_mana_points = base_stats.base_constitution * StatisticsValues::FOCUS;

	current_judgement = base_stats.base_judgement * StatisticsValues::JUDGEMENT;

	return ret;
}

uint Entity::GetCurrentHealthPoints()
{
	//todo calculate from items buffs or debuffs. For now only read the stat and the multiper
	return current_health_points;
}

uint Entity::GetCurrentManaPoints()
{
	//todo calculate from items buffs or debuffs. For now only read the stat and the multiper
	return current_mana_points;
}

uint Entity::GetCurrentJudgement()
{
	return current_judgement;
}

void Entity::SetCurrentHealthPoints(uint new_health_points)
{
	current_health_points = new_health_points;
}

void Entity::SetCurrentManaPoints(uint new_mana_points)
{
	current_mana_points = new_mana_points;
}
