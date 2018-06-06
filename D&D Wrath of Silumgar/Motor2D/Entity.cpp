#include "ctApp.h"
#include "Entity.h"
#include "ctRender.h"
#include "ctAudio.h"
#include "ctItems.h"
#include "j1ParticleSystem.h"
#include "ctCombat.h"
#include "ctLog.h"
#include "ctLootMenu.h"

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
	int random_number = 0;
	switch (this->type)
	{
	case GNOLL:
		random_number = (rand() % App->items->tier_2_equips.size());
		App->loot_menu->dropped_items.push_back(&App->items->tier_2_equips.at(random_number));
		App->loot_menu->dropped_usable_items.push_back(&App->items->usable_items.at(3));
		break;
	case KOBOLD:
		random_number = (rand() % App->items->tier_1_equips.size());
		App->loot_menu->dropped_items.push_back(&App->items->tier_1_equips.at(random_number));
		App->loot_menu->dropped_usable_items.push_back(&App->items->usable_items.at(0));
		break;
	case GOBLIN:
		App->loot_menu->dropped_usable_items.push_back(&App->items->usable_items.at(0));
		random_number = (rand() % App->items->tier_1_equips.size());
		App->loot_menu->dropped_items.push_back(&App->items->tier_1_equips.at(random_number));
		break;
	case ALCHEMIST_GOBLIN:
		random_number = (rand() % App->items->tier_1_equips.size());
		App->loot_menu->dropped_items.push_back(&App->items->tier_1_equips.at(random_number));
		App->loot_menu->dropped_usable_items.push_back(&App->items->usable_items.at(5));
		break;
	case HEAVY_GOBLIN:
		random_number = (rand() % App->items->tier_1_equips.size());
		App->loot_menu->dropped_items.push_back(&App->items->tier_1_equips.at(random_number));
		random_number = (rand() % App->items->usable_items.size());
		App->loot_menu->dropped_usable_items.push_back(&App->items->usable_items.at(random_number));
		break;
	case SKELETON:
		random_number = (rand() % App->items->tier_2_equips.size());
		App->loot_menu->dropped_items.push_back(&App->items->tier_2_equips.at(random_number));
		random_number = (rand() % App->items->usable_items.size());
		App->loot_menu->dropped_usable_items.push_back(&App->items->usable_items.at(random_number));
		break;
	case OWLBEAR:
		random_number = (rand() % App->items->tier_3_equips.size());
		App->loot_menu->dropped_items.push_back(&App->items->tier_3_equips.at(random_number));
		random_number = (rand() % App->items->usable_items.size());
		App->loot_menu->dropped_usable_items.push_back(&App->items->usable_items.at(random_number));
		break;
	case TRITON:
		random_number = (rand() % App->items->tier_2_equips.size());
		App->loot_menu->dropped_items.push_back(&App->items->tier_2_equips.at(random_number));
		random_number = (rand() % App->items->usable_items.size());
		App->loot_menu->dropped_usable_items.push_back(&App->items->usable_items.at(random_number));
		break;
	case HELLHOUND:
		random_number = (rand() % App->items->tier_3_equips.size());
		App->loot_menu->dropped_items.push_back(&App->items->tier_3_equips.at(random_number));
		random_number = (rand() % App->items->usable_items.size());
		App->loot_menu->dropped_usable_items.push_back(&App->items->usable_items.at(random_number));
		break;
	case DARK_WARRIOR:
	case TROLL_LEADERS:
	case DISPLACER_BEAST:
		//case DISPLACER
		random_number = (rand() % App->items->tier_2_equips.size());
		App->loot_menu->dropped_items.push_back(&App->items->tier_2_equips.at(random_number));
		random_number = (rand() % App->items->tier_3_equips.size());
		App->loot_menu->dropped_items.push_back(&App->items->tier_3_equips.at(random_number));
		random_number = (rand() % App->items->usable_items.size());
		App->loot_menu->dropped_usable_items.push_back(&App->items->usable_items.at(random_number));
		random_number = (rand() % App->items->usable_items.size());
		App->loot_menu->dropped_usable_items.push_back(&App->items->usable_items.at(random_number));
		break;

	default:
		break;
	}
	
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

	if (this->GetCurrentHealthPoints() <= 0 && !dead && this->type!=MINIHEROES) {
		this->animation = &death;
		dead = true;
	}

	if (dead && this->GetCurrentHealthPoints() > 0) {
		this->animation = &idle;
		dead = false;
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

	

	//todo calculate current_stats

	CalculateAllStats(); //PreCombat!

						 /* ADDING BASIC ACTION FOR ALL THE ENTITIES THIS SHOULD BE CHANGED */
	default_attack.name = "Attack";
	default_attack.type = DEFAULT_ATTACK;
	default_attack.objective = ENEMIES;
	default_attack.health_points_effect = -3;

	countered.name = "Countered";
	countered.type = COUNTERED;
	countered.objective = HEROES;
	countered.health_points_effect = -3;
	

	return ret;
}

void Entity::NewTurn()
{
	if (current_health_points > 0)
		this->SetCurrentManaPoints(current_mana_points + 8);

	int altered_info_y = this->position.y - this->animation->GetCurrentFrame().h;

	for (int i = 0; i < altered_stats.size(); i++)
	{
		if (altered_stats.at(i).bleeding && !dead) {
			App->combat->UpdateHPBarOfEntity(this, BLEEDING_DAMAGE * altered_stats.at(i).turn_left);

			int total_dmg = (BLEEDING_DAMAGE * altered_stats.at(i).turn_left);

			this->SetCurrentHealthPoints(this->GetCurrentHealthPoints() + total_dmg);
			this->animation = &this->hit;

			std::string tmp_string = std::to_string(total_dmg);
			std::string turn_ = "Turns Left ";
			std::string turns_left = std::to_string(altered_stats.at(i).turn_left - 1);
			std::string tmp_string2 = turn_ + turns_left;
			App->gui->AddUIFloatingValue(this->position.x + (this->animation->GetCurrentFrame().w / 2), altered_info_y - 10, tmp_string, { 127,0,8,255 }, 14, nullptr, nullptr);
			App->gui->AddUIFloatingValue(this->position.x + (this->animation->GetCurrentFrame().w / 2 - 24), altered_info_y + 6, tmp_string2, { 127,0,8,255 }, 14, nullptr, nullptr);


			fPoint  posP = { (float)(this->position.x + (this->animation->GetCurrentFrame().w / 2)), (float)(this->position.y - this->animation->GetCurrentFrame().h / 2) };
			App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_BLEEDING);
			altered_info_y += 28;
		}

		if (altered_stats.at(i).poison && !dead) {
			App->combat->UpdateHPBarOfEntity(this, POISON_DAMAGE / altered_stats.at(i).turn_left);

			int total_dmg = (POISON_DAMAGE / altered_stats.at(i).turn_left);

			this->SetCurrentHealthPoints(this->GetCurrentHealthPoints() + total_dmg);
			this->animation = &this->hit;

			std::string tmp_string = std::to_string(total_dmg);
			std::string turn_ = "Turns Left ";
			std::string turns_left = std::to_string(altered_stats.at(i).turn_left - 1);
			std::string tmp_string2 = turn_ + turns_left;
			App->gui->AddUIFloatingValue(this->position.x + (this->animation->GetCurrentFrame().w / 2), altered_info_y - 10, tmp_string, { 127,0,85,255 }, 14, nullptr, nullptr);
			App->gui->AddUIFloatingValue(this->position.x + (this->animation->GetCurrentFrame().w / 2 - 24), altered_info_y + 6, tmp_string2, { 127,0,85,255 }, 14, nullptr, nullptr);


			fPoint  posP = { (float)(this->position.x + (this->animation->GetCurrentFrame().w / 2)), (float)(this->position.y - this->animation->GetCurrentFrame().h / 2) };
			App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_POISON);
			altered_info_y += 28;
		}

		

		if (altered_stats.at(i).burn && !dead) {
			App->combat->UpdateHPBarOfEntity(this, BURN_DAMAGE);

			this->SetCurrentHealthPoints(this->GetCurrentHealthPoints() + BURN_DAMAGE);
			this->animation = &this->hit;

			std::string tmp_string = std::to_string(BURN_DAMAGE);
			std::string turn_ = "Turns Left ";
			std::string turns_left = std::to_string(altered_stats.at(i).turn_left - 1);
			std::string tmp_string2 = turn_ + turns_left;
			App->gui->AddUIFloatingValue(this->position.x + (this->animation->GetCurrentFrame().w / 2), altered_info_y - 10, tmp_string, { 255,80,80,255 }, 14, nullptr, nullptr);
			App->gui->AddUIFloatingValue(this->position.x + (this->animation->GetCurrentFrame().w / 2 - 24), altered_info_y +6, tmp_string2, { 255,80,80,255 }, 14, nullptr, nullptr);


			fPoint  posP = { (float)(this->position.x + (this->animation->GetCurrentFrame().w / 2)), (float)(this->position.y - this->animation->GetCurrentFrame().h / 2) };
			App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_BURNING);
		}

		altered_stats.at(i).turn_left--;
		if (altered_stats.at(i).turn_left == 0) {
			if (altered_stats.at(i).stun)
				stun_animation = nullptr;
			altered_stats.erase(altered_stats.cbegin() + i);
			altered_stats.shrink_to_fit();
		}
	}
}

void Entity::Recover()
{
	int health_to_recover = (int)(0.15 * current_health_points);
	App->combat->UpdateHPBarOfEntity(this, health_to_recover);
	this->SetCurrentHealthPoints(this->GetCurrentHealthPoints() + health_to_recover);

	int mana_to_recover = (int)(0.15 * current_mana_points);
	App->combat->UpdateManaBarOfEntity(this, mana_to_recover);
	this->SetCurrentManaPoints(this->GetCurrentHealthPoints() + mana_to_recover);
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

bool Entity::IsBurning() const
{
	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).burn) {
			return true;
		}
	}
	return false;
}

bool Entity::IsPoisoned() const
{
	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).poison) {
			return true;
		}
	}
	return false;
}

bool Entity::IsBleeding() const
{
	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).bleeding) {
			return true;
		}
	}
	return false;
}

int Entity::GetCurrentHealthPoints()
{

	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).stat_effect_constitution != 0) {
			switch (this->altered_stats.at(i).stat_effect_constitution)
			{
			case 0:
				return current_health_points;
				break;
			case 1:
				return (int)((0.25 * current_health_points) + current_health_points);
				break;
			case 2:
				return (int)((0.50 * current_health_points) + current_health_points);
				break;
			case -1:
				return (int)((0.25 * current_health_points) - current_health_points);
				break;
			case -2:
				return (int)((0.50 * current_health_points) - current_health_points);
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
	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).stat_effect_focus != 0) {
			switch (this->altered_stats.at(i).stat_effect_focus)
			{
			case 0:
				return current_mana_points;
				break;
			case 1:
				return (int)((0.25 * current_mana_points) + current_mana_points);
				break;
			case 2:
				return (int)((0.50 * current_mana_points) + current_mana_points);
				break;
			case -1:
				return (int)((0.25 * current_mana_points) - current_mana_points);
				break;
			case -2:
				return (int)((0.50 * current_mana_points) - current_mana_points);
				break;
			default:
				break;
			}
		}
	}
	return current_mana_points;
}

int Entity::GetCurrentStrengthPoints()
{
	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).stat_effect_strength != 0) {
			switch (this->altered_stats.at(i).stat_effect_strength)
			{
			case 0:
				return current_strength;
				break;
			case 1:
				return (int)((0.25 * current_strength) + current_strength);
				break;
			case 2:
				return (int)((0.50 * current_strength) + current_strength);
				break;
			case -1:
				if ((int)((0.25 * current_strength) - current_strength) <= 0) {
					return 3;
				}else
				return (int)((0.25 * current_strength) - current_strength);
				break;
			case -2:
				if ((int)((0.50 * current_strength) - current_strength) <= 0) {
					return 3;
				}
				else
					return (int)((0.50* current_strength) - current_strength);
				break;
			default:
				break;
			}
		}
	}

	return current_strength;
}

int Entity::GetCurrentIntelligencePoints()
{
	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).stat_effect_intelligence != 0) {
			switch (this->altered_stats.at(i).stat_effect_intelligence)
			{
			case 0:
				return current_intelligence;
				break;
			case 1:
				return (int)((0.25 * current_intelligence) + current_intelligence);
				break;
			case 2:
				return (int)((0.50 * current_intelligence) + current_intelligence);
				break;
			case -1:
				return (int)((0.25 * current_intelligence) - current_intelligence);
				break;
			case -2:
				return (int)((0.50 * current_intelligence) - current_intelligence);
				break;
			default:
				break;
			}
		}
	}
	return current_intelligence;
}

int Entity::GetCurrentAgilityPoints()
{
	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).stat_effect_agility != 0) {
			switch (this->altered_stats.at(i).stat_effect_agility)
			{
			case 0:
				return current_agility_points;
				break;
			case 1:
				return (int)((0.25 * current_agility_points) + current_agility_points);
				break;
			case 2:
				return (int)((0.50 * current_agility_points) + current_agility_points);
				break;
			case -1:
				return (int)((0.25 * current_agility_points) - current_agility_points);
				break;
			case -2:
				return (int)((0.50 * current_agility_points) - current_agility_points);
				break;
			default:
				break;
			}
		}
	}
	return current_agility_points;
}

int Entity::GetCurrentDexterityPoints()
{
	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).stat_effect_agility != 0) {
			switch (this->altered_stats.at(i).stat_effect_dexterity)
			{
			case 0:
				return current_dexterity_points;
				break;
			case 1:
				return (int)((0.25 * current_dexterity_points) + current_dexterity_points);
				break;
			case 2:
				return (int)((0.50 * current_dexterity_points) + current_dexterity_points);
				break;
			case -1:
				return (int)((0.25 * current_dexterity_points) - current_dexterity_points);
				break;
			case -2:
				return (int)((0.50 * current_dexterity_points) - current_dexterity_points);
				break;
			default:
				break;
			}
		}
	}
	return current_dexterity_points;
}

int Entity::GetCurrentPhysicalDefensePoints()
{
	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).stat_effect_physical_defense != 0) {
			switch (this->altered_stats.at(i).stat_effect_physical_defense)
			{
			case 0:
				return current_physical_defense_points;
				break;
			case 1:
				return (int)((0.25 * current_physical_defense_points) + current_physical_defense_points);
				break;
			case 2:
				return (int)((0.50 * current_physical_defense_points) + current_physical_defense_points);
				break;
			case -1:
				return (int)((0.25 * current_physical_defense_points) - current_physical_defense_points);
				break;
			case -2:
				return (int)((0.50 * current_physical_defense_points) - current_physical_defense_points);
				break;
			default:
				break;
			}
		}
	}
	return current_physical_defense_points;
}

int Entity::GetCurrentMagicalDefensePoints()
{
	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).stat_effect_magical_defense != 0) {
			switch (this->altered_stats.at(i).stat_effect_magical_defense)
			{
			case 0:
				return current_magical_defense_points;
				break;
			case 1:
				return (int)((0.25 * current_magical_defense_points) + current_magical_defense_points);
				break;
			case 2:
				return (int)((0.50 * current_magical_defense_points) + current_magical_defense_points);
				break;
			case -1:
				return (int)((0.25 * current_magical_defense_points) - current_magical_defense_points);
				break;
			case -2:
				return (int)((0.50 * current_magical_defense_points) - current_magical_defense_points);
				break;
			default:
				break;
			}
		}
	}
	return current_magical_defense_points;
}

int Entity::GetCurrentLuckPoints()
{
	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).stat_effect_luck != 0) {
			switch (this->altered_stats.at(i).stat_effect_luck)
			{
			case 0:
				return current_luck;
				break;
			case 1:
				return (int)((0.25 * current_luck) + current_luck);
				break;
			case 2:
				return (int)((0.50 * current_luck) + current_luck);
				break;
			case -1:
				return (int)((0.25 * current_luck) - current_luck);
				break;
			case -2:
				return (int)((0.50 * current_luck) - current_luck);
				break;
			default:
				break;
			}
		}
	}
	return current_luck;
}

int Entity::GetCurrentJudgement()
{
	for (int i = 0; i < this->altered_stats.size(); i++)
	{
		if (this->altered_stats.at(i).stat_effect_judgement != 0) {
			switch (this->altered_stats.at(i).stat_effect_judgement)
			{
			case 0:
				return current_judgement;
				break;
			case 1:
				return (int)((0.25 * current_judgement) + current_judgement);
				break;
			case 2:
				return (int)((0.50 * current_judgement) + current_judgement);
				break;
			case -1:
				return (int)((0.25 * current_judgement) - current_judgement);
				break;
			case -2:
				return (int)((0.50 * current_judgement) - current_judgement);
				break;
			default:
				break;
			}
		}
	}
	return current_judgement;
}

void Entity::SetCurrentHealthPoints(int new_health_points)
{
	if (new_health_points >= max_health_points)
		current_health_points = max_health_points;
	else if (new_health_points <= 0) {
		current_health_points = 0;
	}
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
		if (usable_items.at(i).usable_effects == new_item.usable_effects) {
			usable_items.at(i).quantity++;
			have_to_push_back = false;
			break;
		}
	}

	if (have_to_push_back)
		usable_items.push_back(new_item);
	
}

void Entity::AddEquipItem(Item new_item)
{
	new_item.equipped_by = this->type;
	switch (new_item.equip_type)
	{
	case HELM:
		this->helmet = new_item;
		break;
	case CHEST:
		this->chest = new_item;
		break;
	case GUANTLET:
		this->guantlet = new_item;
		break;
	case BOOT:
		this->boot = new_item;
		break;
	case RING:
		this->ring = new_item;
		break;
	case ACCESORY:
		this->accessory = new_item;
		break;
	case WEAPON:
		this->weapon= new_item;
		break;
	case SHIELD:
		this->shield = new_item;
		break;
	case NO_EQUIP_TYPE:
	default:
		LOG("this should not happen");
		break;
	}
	CalculateAllStats();
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

	tmp_constitution += helmet.statistics.constitution;
	tmp_focus += helmet.statistics.focus;
	tmp_strength += helmet.statistics.strength;
	tmp_intelligence += helmet.statistics.intelligence;
	tmp_dexterity += helmet.statistics.dexterity;
	tmp_agility += helmet.statistics.agility;
	tmp_physical_defense += helmet.statistics.physical_defense;
	tmp_magical_defense += helmet.statistics.magical_defense;
	tmp_luck += helmet.statistics.luck;
	tmp_judgement += helmet.statistics.judgement;
	
	
	tmp_constitution += chest.statistics.constitution;
	tmp_focus += chest.statistics.focus;
	tmp_strength += chest.statistics.strength;
	tmp_intelligence += chest.statistics.intelligence;
	tmp_dexterity += chest.statistics.dexterity;
	tmp_agility += chest.statistics.agility;
	tmp_physical_defense += chest.statistics.physical_defense;
	tmp_magical_defense += chest.statistics.magical_defense;
	tmp_luck += chest.statistics.luck;
	tmp_judgement += chest.statistics.judgement;
	
	tmp_constitution += guantlet.statistics.constitution;
	tmp_focus += guantlet.statistics.focus;
	tmp_strength += guantlet.statistics.strength;
	tmp_intelligence += guantlet.statistics.intelligence;
	tmp_dexterity += guantlet.statistics.dexterity;
	tmp_agility += guantlet.statistics.agility;
	tmp_physical_defense += guantlet.statistics.physical_defense;
	tmp_magical_defense += guantlet.statistics.magical_defense;
	tmp_luck += guantlet.statistics.luck;
	tmp_judgement += guantlet.statistics.judgement;

	tmp_constitution += boot.statistics.constitution;
	tmp_focus += boot.statistics.focus;
	tmp_strength += boot.statistics.strength;
	tmp_intelligence += boot.statistics.intelligence;
	tmp_dexterity += boot.statistics.dexterity;
	tmp_agility += boot.statistics.agility;
	tmp_physical_defense += boot.statistics.physical_defense;
	tmp_magical_defense += boot.statistics.magical_defense;
	tmp_luck += boot.statistics.luck;
	tmp_judgement += boot.statistics.judgement;
	
	tmp_constitution += weapon.statistics.constitution;
	tmp_focus += weapon.statistics.focus;
	tmp_strength += weapon.statistics.strength;
	tmp_intelligence += weapon.statistics.intelligence;
	tmp_dexterity += weapon.statistics.dexterity;
	tmp_agility += weapon.statistics.agility;
	tmp_physical_defense += weapon.statistics.physical_defense;
	tmp_magical_defense += weapon.statistics.magical_defense;
	tmp_luck += weapon.statistics.luck;
	tmp_judgement += weapon.statistics.judgement;
	
	tmp_constitution += shield.statistics.constitution;
	tmp_focus += shield.statistics.focus;
	tmp_strength += shield.statistics.strength;
	tmp_intelligence += shield.statistics.intelligence;
	tmp_dexterity += shield.statistics.dexterity;
	tmp_agility += shield.statistics.agility;
	tmp_physical_defense += shield.statistics.physical_defense;
	tmp_magical_defense += shield.statistics.magical_defense;
	tmp_luck += shield.statistics.luck;
	tmp_judgement += shield.statistics.judgement;

	tmp_constitution += ring.statistics.constitution;
	tmp_focus += ring.statistics.focus;
	tmp_strength += ring.statistics.strength;
	tmp_intelligence += ring.statistics.intelligence;
	tmp_dexterity += ring.statistics.dexterity;
	tmp_agility += ring.statistics.agility;
	tmp_physical_defense += ring.statistics.physical_defense;
	tmp_magical_defense += ring.statistics.magical_defense;
	tmp_luck += ring.statistics.luck;
	tmp_judgement += ring.statistics.judgement;

	tmp_constitution += accessory.statistics.constitution;
	tmp_focus += accessory.statistics.focus;
	tmp_strength += accessory.statistics.strength;
	tmp_intelligence += accessory.statistics.intelligence;
	tmp_dexterity += accessory.statistics.dexterity;
	tmp_agility += accessory.statistics.agility;
	tmp_physical_defense += accessory.statistics.physical_defense;
	tmp_magical_defense += accessory.statistics.magical_defense;
	tmp_luck += accessory.statistics.luck;
	tmp_judgement += accessory.statistics.judgement;

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

void Entity::CalculateAllStatsNoMultiply()
{
	int tmp_constitution = base_stats.constitution;
	int tmp_focus = base_stats.focus;
	int tmp_strength = base_stats.strength;
	int tmp_intelligence = base_stats.intelligence;
	int tmp_dexterity = base_stats.dexterity;
	int tmp_agility = base_stats.agility;
	int tmp_physical_defense = base_stats.physical_defense;
	int tmp_magical_defense = base_stats.magical_defense;
	int tmp_luck = base_stats.luck;
	int tmp_judgement = base_stats.judgement;

	tmp_constitution += helmet.statistics.constitution;
	tmp_focus += helmet.statistics.focus;
	tmp_strength += helmet.statistics.strength;
	tmp_intelligence += helmet.statistics.intelligence;
	tmp_dexterity += helmet.statistics.dexterity;
	tmp_agility += helmet.statistics.agility;
	tmp_physical_defense += helmet.statistics.physical_defense;
	tmp_magical_defense += helmet.statistics.magical_defense;
	tmp_luck += helmet.statistics.luck;
	tmp_judgement += helmet.statistics.judgement;


	tmp_constitution += chest.statistics.constitution;
	tmp_focus += chest.statistics.focus;
	tmp_strength += chest.statistics.strength;
	tmp_intelligence += chest.statistics.intelligence;
	tmp_dexterity += chest.statistics.dexterity;
	tmp_agility += chest.statistics.agility;
	tmp_physical_defense += chest.statistics.physical_defense;
	tmp_magical_defense += chest.statistics.magical_defense;
	tmp_luck += chest.statistics.luck;
	tmp_judgement += chest.statistics.judgement;

	tmp_constitution += guantlet.statistics.constitution;
	tmp_focus += guantlet.statistics.focus;
	tmp_strength += guantlet.statistics.strength;
	tmp_intelligence += guantlet.statistics.intelligence;
	tmp_dexterity += guantlet.statistics.dexterity;
	tmp_agility += guantlet.statistics.agility;
	tmp_physical_defense += guantlet.statistics.physical_defense;
	tmp_magical_defense += guantlet.statistics.magical_defense;
	tmp_luck += guantlet.statistics.luck;
	tmp_judgement += guantlet.statistics.judgement;

	tmp_constitution += boot.statistics.constitution;
	tmp_focus += boot.statistics.focus;
	tmp_strength += boot.statistics.strength;
	tmp_intelligence += boot.statistics.intelligence;
	tmp_dexterity += boot.statistics.dexterity;
	tmp_agility += boot.statistics.agility;
	tmp_physical_defense += boot.statistics.physical_defense;
	tmp_magical_defense += boot.statistics.magical_defense;
	tmp_luck += boot.statistics.luck;
	tmp_judgement += boot.statistics.judgement;

	tmp_constitution += weapon.statistics.constitution;
	tmp_focus += weapon.statistics.focus;
	tmp_strength += weapon.statistics.strength;
	tmp_intelligence += weapon.statistics.intelligence;
	tmp_dexterity += weapon.statistics.dexterity;
	tmp_agility += weapon.statistics.agility;
	tmp_physical_defense += weapon.statistics.physical_defense;
	tmp_magical_defense += weapon.statistics.magical_defense;
	tmp_luck += weapon.statistics.luck;
	tmp_judgement += weapon.statistics.judgement;

	tmp_constitution += shield.statistics.constitution;
	tmp_focus += shield.statistics.focus;
	tmp_strength += shield.statistics.strength;
	tmp_intelligence += shield.statistics.intelligence;
	tmp_dexterity += shield.statistics.dexterity;
	tmp_agility += shield.statistics.agility;
	tmp_physical_defense += shield.statistics.physical_defense;
	tmp_magical_defense += shield.statistics.magical_defense;
	tmp_luck += shield.statistics.luck;
	tmp_judgement += shield.statistics.judgement;

	tmp_constitution += ring.statistics.constitution;
	tmp_focus += ring.statistics.focus;
	tmp_strength += ring.statistics.strength;
	tmp_intelligence += ring.statistics.intelligence;
	tmp_dexterity += ring.statistics.dexterity;
	tmp_agility += ring.statistics.agility;
	tmp_physical_defense += ring.statistics.physical_defense;
	tmp_magical_defense += ring.statistics.magical_defense;
	tmp_luck += ring.statistics.luck;
	tmp_judgement += ring.statistics.judgement;

	tmp_constitution += accessory.statistics.constitution;
	tmp_focus += accessory.statistics.focus;
	tmp_strength += accessory.statistics.strength;
	tmp_intelligence += accessory.statistics.intelligence;
	tmp_dexterity += accessory.statistics.dexterity;
	tmp_agility += accessory.statistics.agility;
	tmp_physical_defense += accessory.statistics.physical_defense;
	tmp_magical_defense += accessory.statistics.magical_defense;
	tmp_luck += accessory.statistics.luck;
	tmp_judgement += accessory.statistics.judgement;

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
