#ifndef __ctEntities_H__
#define __ctEntities_H__

#include "ctModule.h"
#include "ctAnimation.h"
#include <vector>
#include <string>

enum ComboType;

enum EntityType
{
	CLERIC,
	DWARF,
	ELF,
	WARRIOR,
	 

	NO_TYPE
};

class Entity;
class Cleric;
class Dwarf;
class Elf;
class Warrior;

struct SDL_Texture;

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

private:

	std::vector<Entity*> entities;

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

	//todo: delete
	SDL_Texture* entity_sprites = nullptr;
	std::string spritesheetName;

};

#endif // __ctEnemies_H__