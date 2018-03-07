#ifndef __ctEntities_H__
#define __ctEntities_H__

#include "ctModule.h"
#include "ctAnimation.h"
#include <vector>
#include <string>

enum EntityType
{
	FIGHTER,
	KOBOLD,

	NO_TYPE
};

class Entity;
class Player;

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

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	bool CleanUp();

	Player* GetPlayer()const;

	bool SpawnEntity(int x, int y, EntityType type);

private:

	std::vector<Entity*> entities;

	SDL_Texture* entity_sprites = nullptr;

	std::string spritesheetName;

};

#endif // __ctEnemies_H__