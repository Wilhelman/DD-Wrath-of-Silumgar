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

	if (hit.Finished()) {
		this->animation = &idle;
		hit.Reset();
	}

	if (this->GetCurrentHealthPoints() == 0 && !dead && this->type!=MINIHEROES) {
		this->animation = &death;
		dead = true;
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
	current_mana_points = base_stats.base_focus * StatisticsValues::FOCUS;
	current_agility_points = base_stats.base_agility * StatisticsValues::AGILITY;
	current_dexterity_points = base_stats.base_dexterity * StatisticsValues::DEXTERITY;
	current_physical_defense_points = base_stats.base_physical_defense * StatisticsValues::PHYSICAL_DEFENSE;

	current_judgement = base_stats.base_judgement * StatisticsValues::JUDGEMENT;

	return ret;
}

int Entity::GetCurrentHealthPoints()
{
	//todo calculate from items buffs or debuffs. For now only read the stat and the multiper
	return current_health_points;
}

int Entity::GetCurrentManaPoints()
{
	//todo calculate from items buffs or debuffs. For now only read the stat and the multiper
	return current_mana_points;
}

int Entity::GetCurrentAgilityPoints()
{
	//todo calculate from items buffs or debuffs. For now only read the stat and the multiper
	return current_agility_points;
}

int Entity::GetCurrentDexterityPoints()
{
	//todo calculate from items buffs or debuffs. For now only read the stat and the multiper
	return current_dexterity_points;
}

int Entity::GetCurrentPhysicalDefensePoints()
{
	//todo calculate from items buffs or debuffs. For now only read the stat and the multiper
	return current_physical_defense_points;
}

int Entity::GetCurrentJudgement()
{
	return current_judgement;
}

void Entity::SetCurrentHealthPoints(int new_health_points)
{
	if (new_health_points <= 0)
		current_health_points = 0;
	else
		current_health_points = new_health_points;
}

void Entity::SetCurrentManaPoints(int new_mana_points)
{
	current_mana_points = new_mana_points;
}
