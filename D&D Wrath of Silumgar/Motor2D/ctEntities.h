#ifndef __ctEntities_H__
#define __ctEntities_H__

#include "ctModule.h"
#include "ctAnimation.h"
#include <vector>
#include <string>
#include <queue>
struct Stats;
enum ComboType;

class Entity;
class Cleric;
class Dwarf;
class Elf;
class Warrior;

class Kobold;
class Gnoll;
class gnollArcher;
class Owlbear;
class Goblin;
class Goblin_Heavy;
class Goblin_Alchemist;
class Hellhound;
class Triton;
class Skeleton;
class DarkWarrior;
class DisplacerBeast;
class TrollLeaders;

class MiniHeroes;

struct SDL_Texture;

enum EntityType
{
	CLERIC,
	DWARF,
	ELF,
	WARRIOR,

	KOBOLD,
	GNOLL,
	GNOLL_ARCHER,
	OWLBEAR,
	GOBLIN,
	HEAVY_GOBLIN,
	ALCHEMIST_GOBLIN,
	HELLHOUND,
	TRITON,
	SKELETON,
	DARK_WARRIOR,
	TROLL_LEADERS,
	DISPLACER_BEAST,
	LICH_1,
	LICH_2,

	MINIHEROES,

	NO_TYPE
};

//todo read this from xml.
enum StatisticsValues
{
	CONSTITUTION = 26,
	FOCUS = 13,
	STRENGTH = 3,
	INTELLIGENCE = 3,
	DEXTERITY = 2,
	AGILITY = 2,
	PHYSICAL_DEFENSE = 2,
	MAGICAL_DEFENSE = 2,
	LUCK = 1,
	JUDGEMENT = 1
};

class ctEntities : public ctModule
{
public:

	ctEntities();
	~ctEntities();

	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool CleanUp();

	bool SpawnEntity(int x, int y, EntityType type);
	
	void OrderDrawEntities();

	//getters
	Cleric* GetCleric()const;
	Dwarf* GetDwarf()const;
	Elf* GetElf()const;
	Warrior* GetWarrior()const;

	Kobold* GetKobold()const;
	Gnoll* GetGnoll()const;
	gnollArcher* GetGnoll_Archer()const;
	Owlbear* GetOwlbear()const;
	Goblin* GetGoblin()const;
	Goblin_Heavy* GetHeavyGoblin()const;
	Goblin_Alchemist* GetAlchemistGoblin()const;
	Triton* GetTriton()const;
	Hellhound* GetHellhound()const;
	Skeleton* GetSkeleton()const;
	DarkWarrior* GetDarkWarrior()const;
	TrollLeaders* GetTrollLeader()const;
	DisplacerBeast* GetDisplacerBeast()const;

	MiniHeroes* GetMiniheroes()const;

	std::vector<Entity*> entities;
	std::vector<Entity*> draw_priority_entities;

private:

	

public:
	///HEROES
	//cleric_sources
	std::string cleric_spritesheet_name;
	//dwarf_sources
	std::string dwarf_spritesheet_name;
	//elf_sources
	std::string elf_spritesheet_name;
	//warrior_sources
	std::string warrior_spritesheet_name;

	///ENEMIES
	//kobold_sources
	std::string kobold_spritesheet_name;
	//gnoll_sources
	std::string gnoll_spritesheet_name;
	//gnollArcher_sources
	std::string gnollArcher_spritesheet_name;
	//Owlbear_sources
	std::string owlbear_spritesheet_name;
	//Goblin_sources
	std::string goblins_spritesheet_name;
	//Goblin_sources
	std::string goblin_heavy_spritesheet_name;
	//Triton sources
	std::string triton_spritesheet_name;
	//Skeleton sources
	std::string skeleton_spritesheet_name;
	//Hellhound sources
	std::string hellhound_spritesheet_name;
	//DarkWarrior sources
	std::string dark_warrior_spritesheet_name;
	//TrollLeaders sources
	std::string trollLeader_spritesheet_name;
	//Displacer beast sources
	std::string displacerBeast_spritesheet_name;

	std::string lich2_spritesheet_name;

	//Miniheroes source
	std::string miniheroes_spritesheet_name;



};

#endif // __ctEnemies_H__


