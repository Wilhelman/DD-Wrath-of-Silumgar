#include "ctApp.h"
#include "Entity.h"
#include "ctRender.h"
#include "ctAudio.h"

//randomize libs
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

Entity::Entity(int x, int y, EntityType type) : position(x, y), type(type)
{
	initial_position = iPoint(x, y);

	/* initialize random seed: */
	srand(time(NULL));
}

Entity::~Entity()
{
	App->tex->UnLoad(texture);
	App->audio->UnLoadFx(attack_fx);
	App->audio->UnLoadFx(run_fx);
	App->audio->UnLoadFx(death_fx);
	App->audio->UnLoadFx(damaged_fx);
	App->audio->UnLoadFx(ability_1_fx);

	abilities.clear();
	altered_stats.clear();
	
}

void Entity::Draw()
{
	SDL_Rect r = animation->GetCurrentFrame();
	

	if (animation != nullptr) {
		if (flip_texture == false)
		App->render->Blit(texture, position.x, position.y - r.h, &(animation->GetCurrentFrame()));
		else {
		App->render->Blit(texture, position.x, position.y - r.h, &(animation->GetCurrentFrame()), NULL,NULL,255, SDL_FLIP_HORIZONTAL);
		}
	}

	/*stun todo better*/

	if (stun_animation != nullptr && current_health_points > 0) {
		SDL_Rect a = stun_animation->GetCurrentFrame();
		App->render->Blit(texture, (position.x + r.w / 2) - a.w/2, position.y - r.h, &(stun_animation->GetCurrentFrame()));
	}

	for (int i = 0; i < altered_stats.size(); i++)
	{
		if (altered_stats.at(i).stun) {
			stun_animation = &stun;
		}
		else {
			stun_animation = nullptr;
		}
	}


	/*end stun*/

	if (hit.Finished()) {
		this->animation = &idle;
		hit.Reset();
	}

	if (dodge.Finished()) {
		this->animation = &idle;
		dodge.Reset();
	}

	if (this->GetCurrentHealthPoints() == 0 && !dead && this->type!=MINIHEROES) {
		this->animation = &death;
		dead = true;
	}


	if (this->animation == &run_forward) {
		if (steps_timer == 0) {
			steps_timer = current_timer;
		}

		if (steps_timer + 310 < current_timer) {
			steps_timer = 0;
			this->Run();
		}
	}
	current_timer = SDL_GetTicks();

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
	default_attack.objective = ENEMIES;
	default_attack.health_points_effect = -(int)base_stats.base_strength * StatisticsValues::STRENGTH;

	//todo change this
	
	max_health_points = current_health_points = base_stats.base_constitution * StatisticsValues::CONSTITUTION;
	max_mana_points = current_mana_points = base_stats.base_focus * StatisticsValues::FOCUS;
	current_agility_points = base_stats.base_agility * StatisticsValues::AGILITY;
	current_dexterity_points = base_stats.base_dexterity * StatisticsValues::DEXTERITY;
	current_physical_defense_points = base_stats.base_physical_defense * StatisticsValues::PHYSICAL_DEFENSE;
	current_magical_defense_points = base_stats.base_magical_defense * StatisticsValues::MAGICAL_DEFENSE;
	current_judgement = base_stats.base_judgement * StatisticsValues::JUDGEMENT;

	return ret;
}

void Entity::NewTurn()
{
	if (current_health_points > 0)
		this->SetCurrentManaPoints(current_mana_points + 8);

	for (int i = 0; i < altered_stats.size(); i++)
	{
		altered_stats.at(i).turn_left--;
		if (altered_stats.at(i).turn_left == 0) {
			if (altered_stats.at(i).stun)
				stun_animation = nullptr;
			altered_stats.erase(altered_stats.cbegin() + i);
			altered_stats.shrink_to_fit();
		}
	}
}

void Entity::AddAlteredStat(Altered_Stat new_altered_stat)
{
	altered_stats.push_back(new_altered_stat);
}

bool Entity::IsStunned() const
{
	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).stun) {
			return true;
		}
	}
	return false;
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

int Entity::GetCurrentMagicalDefensePoints()
{
	return current_magical_defense_points;
}

int Entity::GetCurrentJudgement()
{
	return current_judgement;
}

void Entity::SetCurrentHealthPoints(int new_health_points)
{
	if (new_health_points >= max_health_points)
		current_health_points = max_health_points;
	else if (new_health_points <= 0)
		current_health_points = 0;
	else
		current_health_points = new_health_points;
}

void Entity::SetCurrentManaPoints(int new_mana_points)
{
	if (new_mana_points >= max_mana_points)
		current_mana_points = max_mana_points;
	else if (new_mana_points <= 0)
		current_mana_points = 0;
	else
		current_mana_points = new_mana_points;
}

void Entity::AddAction(Action new_action)
{
	abilities.push_back(new_action);
}

bool Entity::IsGoingToDoAnythingClever()
{
	int random_number = (rand() % 100) + 1; //random del 1-100
	return this->GetCurrentJudgement() >= random_number;
}
