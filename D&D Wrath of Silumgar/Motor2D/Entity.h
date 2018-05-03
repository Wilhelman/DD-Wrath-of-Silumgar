#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "ctPoint.h"
#include "ctAnimation.h"
#include "ctEntities.h"
#include "ctTextures.h"
#include "SDL/include/SDL.h"
#include "ctItems.h"
#include "ctEntitiesInfo.h"

#include <vector>

struct SDL_Texture;

class Entity
{
protected:
	
	bool key_entities_speed = false;

	int current_health_points, current_mana_points, current_strength, current_intelligence, current_luck, current_agility_points, current_dexterity_points, current_physical_defense_points, current_magical_defense_points, current_judgement = 0;

	int max_health_points, max_mana_points = 0;

	bool dead = false;

public:
	ctAnimation* animation = nullptr;
	ctAnimation* stun_animation = nullptr;

	ctAnimation idle = ctAnimation();
	ctAnimation run_forward = ctAnimation();
	ctAnimation run_backward = ctAnimation();
	ctAnimation attack = ctAnimation();
	ctAnimation hit = ctAnimation();
	ctAnimation death = ctAnimation();
	ctAnimation stun = ctAnimation();
	ctAnimation dodge = ctAnimation();
	

	//custom animations
		//FIGHTER
	ctAnimation kick = ctAnimation();
	ctAnimation guard = ctAnimation();
	ctAnimation heavy_slash = ctAnimation();
	ctAnimation kick_plus = ctAnimation();
	ctAnimation guard_plus= ctAnimation();
	ctAnimation heavy_slash_plus = ctAnimation();
	ctAnimation charge = ctAnimation();
	ctAnimation taunt = ctAnimation();
	ctAnimation whirlwind = ctAnimation();

		//DWARF
	ctAnimation high_axe = ctAnimation();
	ctAnimation morale_boost = ctAnimation();
	ctAnimation dwarfs_wrath = ctAnimation();
	ctAnimation high_axe_plus = ctAnimation();
	ctAnimation morale_boost_plus = ctAnimation();
	ctAnimation dwarfs_wrath_plus = ctAnimation();
	ctAnimation raging_mock = ctAnimation();
	ctAnimation meteor = ctAnimation();
	ctAnimation counter = ctAnimation();
		//CLERIC
	ctAnimation heal = ctAnimation();
	ctAnimation light_strike = ctAnimation();
	ctAnimation mace_throw = ctAnimation();
	ctAnimation heal_plus = ctAnimation();
	ctAnimation light_strike_plus = ctAnimation();
	ctAnimation mace_throw_plus = ctAnimation();
	ctAnimation blessing = ctAnimation();
	ctAnimation harder_skin = ctAnimation();
	ctAnimation clarity = ctAnimation();
		//ELF
	ctAnimation blizzard = ctAnimation();
	ctAnimation seed_of_life = ctAnimation();
	ctAnimation fireball = ctAnimation();
	ctAnimation lightning_bolt = ctAnimation();
	ctAnimation seed_of_life_plus = ctAnimation();
	ctAnimation fireball_plus = ctAnimation();
	ctAnimation lightning_bol_plus = ctAnimation();
	ctAnimation revive = ctAnimation();
	ctAnimation fire_djinn = ctAnimation();

		//OBJECT
	ctAnimation throw_object = ctAnimation();
	ctAnimation use_potion = ctAnimation();

	//dark_warrior
	ctAnimation void_cannon = ctAnimation();
	ctAnimation thunder_punch = ctAnimation();
	ctAnimation insignificant_mortals = ctAnimation();

	Stats base_stats;
	iPoint position = iPoint(0, 0);
	iPoint initial_position = iPoint(0, 0);
	EntityType type = EntityType::NO_TYPE;
	bool to_destroy = false;
	bool flip_texture = false;
	
	SDL_Texture* texture = nullptr;

	//Priority Draw Order
	uint priority_draw_order = 0u;

	//Combat stuff
	Action default_attack;

	std::vector<Action> abilities;
	std::vector<Altered_Stat> altered_stats;

	std::vector<Item> usable_items;
	//Armor stuff
	Item ring;
	Item accessory;

	Item helmet;
	Item chest;
	Item guantlet;
	Item boot;

	Item shield;
	Item weapon;


	//Sounds stuff
	uint steps_timer = 0u;
	uint current_timer = 0u;

public:

	Entity(int x, int y, EntityType type);
	virtual ~Entity();

	virtual void Update(float dt) {};
	virtual void Draw();
	virtual void SetEntitiesSpeed(float dt) {};

	virtual void LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation) {};
	bool LoadProperties(pugi::xml_node properties);

	//for sounds
	virtual void Attack() {};
	virtual void Run() {};
	virtual void Death() {};
	virtual void Damaged() {};
	virtual void Ability1() {};

	//Combat stuff

	virtual void NewTurn();

	virtual void PerformAction() {};

	void AddAlteredStat(Altered_Stat new_altered_stat);

	bool IsStunned()const;

	int GetCurrentHealthPoints();
	int GetCurrentManaPoints();
	int GetCurrentAgilityPoints();
	int GetCurrentDexterityPoints();
	int GetCurrentPhysicalDefensePoints();
	int GetCurrentMagicalDefensePoints();

	int GetCurrentJudgement();

	void SetCurrentHealthPoints(int new_health_points);
	void SetCurrentManaPoints(int new_mana_points);

	bool IsGoingToDoAnythingClever();

	void AddAction(Action new_action);

	void AddUsableItem(Item new_item);

	private:
	void CalculateAllStats();
};

#endif // __ENTITY_H__