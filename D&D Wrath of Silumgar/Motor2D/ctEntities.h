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

	MINIHEROES,

	NO_TYPE
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
	

	//getters
	Cleric* GetCleric()const;
	Dwarf* GetDwarf()const;
	Elf* GetElf()const;
	Warrior* GetWarrior()const;

	Kobold* GetKobold()const;
	Gnoll* GetGnoll()const;
	gnollArcher* GetGnoll_Archer()const;
	Owlbear* GetOwlbear()const;

	MiniHeroes* GetMiniheroes()const;

	std::vector<Entity*> entities;

private:

	

public:
	///HEROES
	//cleric_sources
	SDL_Texture* cleric_spritesheet = nullptr;
	std::string cleric_spritesheet_name;
	//dwarf_sources
	SDL_Texture* dwarf_spritesheet = nullptr;
	std::string dwarf_spritesheet_name;
	//elf_sources
	SDL_Texture* elf_spritesheet = nullptr;
	std::string elf_spritesheet_name;
	//warrior_sources
	SDL_Texture* warrior_spritesheet = nullptr;
	std::string warrior_spritesheet_name;

	///ENEMIES
	//kobold_sources
	SDL_Texture* kobold_spritesheet = nullptr;
	std::string kobold_spritesheet_name;
	//gnoll_sources
	SDL_Texture* gnoll_spritesheet = nullptr;
	std::string gnoll_spritesheet_name;
	//gnollArcher_sources
	SDL_Texture* gnollArcher_spritesheet = nullptr;
	std::string gnollArcher_spritesheet_name;
	//Owlbear_sources
	SDL_Texture* owlbear_spritesheet = nullptr;
	std::string owlbear_spritesheet_name;
	//Miniheroes source
	SDL_Texture* miniheroes_spritesheet = nullptr;
	std::string miniheroes_spritesheet_name;

	//todo: delete
	SDL_Texture* entity_sprites = nullptr;
	std::string spritesheetName;

};

#endif // __ctEnemies_H__


