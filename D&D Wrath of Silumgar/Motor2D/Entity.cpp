#include "ctApp.h"
#include "Entity.h"
#include "ctRender.h"
#include "ctAudio.h"
#include "ctItems.h"

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

	base_stats.constitution = properties.attribute("constitution").as_uint();
	base_stats.focus = properties.attribute("focus").as_uint();
	base_stats.strength = properties.attribute("strength").as_uint();
	base_stats.agility = properties.attribute("agility").as_uint();
	base_stats.dexterity = properties.attribute("dexterity").as_uint();
	base_stats.intelligence = properties.attribute("intelligence").as_uint();
	base_stats.physical_defense = properties.attribute("physicalDefense").as_uint();
	base_stats.magical_defense = properties.attribute("magicalDefense").as_uint();
	base_stats.luck = properties.attribute("luck").as_uint();
	base_stats.judgement = properties.attribute("judgement").as_uint();

	/* ADDING BASIC ACTION FOR ALL THE ENTITIES THIS SHOULD BE CHANGED */
	default_attack.name = "Attack";
	default_attack.type = DEFAULT_ATTACK;
	default_attack.objective = ENEMIES;
	default_attack.health_points_effect = -(int)base_stats.strength * StatisticsValues::STRENGTH;

	//todo calculate current_stats

	CalculateAllStats(); //PreCombat!
	

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

	//todo add the debuffs/buffs!

	//
	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).stat_effect_constitution != 0) {
			switch (this->altered_stats.at(i).stat_effect_constitution)
			{
			case 1:
				break;
			case 2:
				break;
			case -1:
				break;
			case -2:
				break;
			default:
				break;
			}
		}
	}

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

void Entity::AddUsableItem(Item new_item)
{
	bool have_to_push_back = true;

	for (int i = 0; i < usable_items.size(); i++)
	{
		if (usable_items.at(i)->usable_effects == new_item.usable_effects) {
			usable_items.at(i)->quantity++;
			have_to_push_back = false;
			break;
		}
	}

	if (have_to_push_back) {

		Item* very_new_item = new Item();

		very_new_item->draw_coords = new_item.draw_coords;
		very_new_item->name = new_item.name;
		very_new_item->objective = new_item.objective;
		very_new_item->quantity = 1;
		very_new_item->usable_effects = new_item.usable_effects;

		usable_items.push_back(very_new_item);
		App->items->items_to_delete.push_back(very_new_item);
	}
}

void Entity::CalculateAllStats()
{
	int tmp_constitution = base_stats.constitution * StatisticsValues::CONSTITUTION;
	int tmp_focus = base_stats.focus * StatisticsValues::FOCUS;
	int tmp_strength = base_stats.strength * StatisticsValues::STRENGTH;
	int tmp_intelligence = base_stats.intelligence * StatisticsValues::INTELLIGENCE;
	int tmp_dexterity = base_stats.dexterity * StatisticsValues::DEXTERITY;
	int tmp_agility = base_stats.agility * StatisticsValues::AGILITY;
	int tmp_physical_defense = base_stats.physical_defense * StatisticsValues::PHYSICAL_DEFENSE;
	int tmp_magical_defense = base_stats.magical_defense * StatisticsValues::MAGICAL_DEFENSE;
	int tmp_luck = base_stats.luck * StatisticsValues::LUCK;
	int tmp_judgement = base_stats.judgement * StatisticsValues::JUDGEMENT;

	if (helmet != nullptr) {
		tmp_constitution += helmet->statistics.constitution;
		tmp_focus += helmet->statistics.focus;
		tmp_strength += helmet->statistics.strength;
		tmp_intelligence += helmet->statistics.intelligence;
		tmp_dexterity += helmet->statistics.dexterity;
		tmp_agility += helmet->statistics.agility;
		tmp_physical_defense += helmet->statistics.physical_defense;
		tmp_magical_defense += helmet->statistics.magical_defense;
		tmp_luck += helmet->statistics.luck;
		tmp_judgement += helmet->statistics.judgement;
	}
	
	if (chest != nullptr) {
		tmp_constitution += chest->statistics.constitution;
		tmp_focus += chest->statistics.focus;
		tmp_strength += chest->statistics.strength;
		tmp_intelligence += chest->statistics.intelligence;
		tmp_dexterity += chest->statistics.dexterity;
		tmp_agility += chest->statistics.agility;
		tmp_physical_defense += chest->statistics.physical_defense;
		tmp_magical_defense += chest->statistics.magical_defense;
		tmp_luck += chest->statistics.luck;
		tmp_judgement += chest->statistics.judgement;
	}
	
	if (guantlet != nullptr) {
		tmp_constitution += guantlet->statistics.constitution;
		tmp_focus += guantlet->statistics.focus;
		tmp_strength += guantlet->statistics.strength;
		tmp_intelligence += guantlet->statistics.intelligence;
		tmp_dexterity += guantlet->statistics.dexterity;
		tmp_agility += guantlet->statistics.agility;
		tmp_physical_defense += guantlet->statistics.physical_defense;
		tmp_magical_defense += guantlet->statistics.magical_defense;
		tmp_luck += guantlet->statistics.luck;
		tmp_judgement += guantlet->statistics.judgement;
	}

	if (boot != nullptr) {
		tmp_constitution += boot->statistics.constitution;
		tmp_focus += boot->statistics.focus;
		tmp_strength += boot->statistics.strength;
		tmp_intelligence += boot->statistics.intelligence;
		tmp_dexterity += boot->statistics.dexterity;
		tmp_agility += boot->statistics.agility;
		tmp_physical_defense += boot->statistics.physical_defense;
		tmp_magical_defense += boot->statistics.magical_defense;
		tmp_luck += boot->statistics.luck;
		tmp_judgement += boot->statistics.judgement;
	}
	
	if (weapon != nullptr) {
		tmp_constitution += weapon->statistics.constitution;
		tmp_focus += weapon->statistics.focus;
		tmp_strength += weapon->statistics.strength;
		tmp_intelligence += weapon->statistics.intelligence;
		tmp_dexterity += weapon->statistics.dexterity;
		tmp_agility += weapon->statistics.agility;
		tmp_physical_defense += weapon->statistics.physical_defense;
		tmp_magical_defense += weapon->statistics.magical_defense;
		tmp_luck += weapon->statistics.luck;
		tmp_judgement += weapon->statistics.judgement;
	}

	if (shield != nullptr) {
		tmp_constitution += shield->statistics.constitution;
		tmp_focus += shield->statistics.focus;
		tmp_strength += shield->statistics.strength;
		tmp_intelligence += shield->statistics.intelligence;
		tmp_dexterity += shield->statistics.dexterity;
		tmp_agility += shield->statistics.agility;
		tmp_physical_defense += shield->statistics.physical_defense;
		tmp_magical_defense += shield->statistics.magical_defense;
		tmp_luck += shield->statistics.luck;
		tmp_judgement += shield->statistics.judgement;
	}

	if (ring != nullptr) {
		tmp_constitution += ring->statistics.constitution;
		tmp_focus += ring->statistics.focus;
		tmp_strength += ring->statistics.strength;
		tmp_intelligence += ring->statistics.intelligence;
		tmp_dexterity += ring->statistics.dexterity;
		tmp_agility += ring->statistics.agility;
		tmp_physical_defense += ring->statistics.physical_defense;
		tmp_magical_defense += ring->statistics.magical_defense;
		tmp_luck += ring->statistics.luck;
		tmp_judgement += ring->statistics.judgement;
	}

	if (accessory != nullptr) {
		tmp_constitution += accessory->statistics.constitution;
		tmp_focus += accessory->statistics.focus;
		tmp_strength += accessory->statistics.strength;
		tmp_intelligence += accessory->statistics.intelligence;
		tmp_dexterity += accessory->statistics.dexterity;
		tmp_agility += accessory->statistics.agility;
		tmp_physical_defense += accessory->statistics.physical_defense;
		tmp_magical_defense += accessory->statistics.magical_defense;
		tmp_luck += accessory->statistics.luck;
		tmp_judgement += accessory->statistics.judgement;
	}

	max_health_points = current_health_points = tmp_constitution;
	max_mana_points = current_mana_points = tmp_focus;
	current_strength = tmp_strength;
	current_intelligence = tmp_intelligence;
	current_dexterity_points = tmp_dexterity;
	current_agility_points = tmp_agility;
	current_physical_defense_points = tmp_physical_defense;
	current_magical_defense_points = tmp_magical_defense;
	current_luck = tmp_luck;
	current_judgement = tmp_judgement;
}

bool Entity::IsGoingToDoAnythingClever()
{
	int random_number = (rand() % 100) + 1; //random del 1-100
	return this->GetCurrentJudgement() >= random_number;
}
