#include "ctApp.h"
#include "ctRender.h"
#include "ctEntities.h"
#include "ctTextures.h"
#include "Entity.h"
#include "ctAudio.h"
#include "ctWindow.h"
#include "ctLog.h"
#include "Player_Cleric.h"
#include "Player_Dwarf.h"
#include "Player_Elf.h"
#include "Player_Warrior.h"
//#include "ctMap.h"
#include "ctFadeToBlack.h"
//#include "j1Scene.h"


ctEntities::ctEntities()
{
	name = "entities";
}

// Destructor
ctEntities::~ctEntities()
{
	LOG("Unloading entities spritesheet");
	App->tex->UnLoad(entity_sprites);
}

bool ctEntities::Awake(pugi::xml_node& config)
{
	LOG("Loading Entities from config file");
	bool ret = true;

	spritesheetName = config.child("spritesheetSource").attribute("name").as_string();
	
	return ret;
}

bool ctEntities::Start()
{
	bool ret = true;

	entity_sprites = App->tex->Load(spritesheetName.data());

	if (entity_sprites == NULL) {
		LOG("Error loading entities spritesheet!!");
		//ret = false;
	}

	if (!ret)
		return false;

	return ret;
}

bool ctEntities::PreUpdate()
{

	return true;
}

// Called before render is available
bool ctEntities::Update(float dt)
{
	
	for (int i = 0; i < entities.size(); i++) {
		LOG(entities.at(i)->anim_Path.c_str());

		SDL_Texture* sprites = App->tex->Load(entities.at(i)->anim_Path.c_str());
		entities.at(i)->Draw(sprites);
	}

	return true;
}

// Called before quitting
bool ctEntities::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(entity_sprites);



	return true;
}

bool ctEntities::SpawnEntity(int x, int y, EntityType type)
{
	// find room for the new entity
	bool ret = false;

	switch (type)
	{
	case EntityType::CLERIC: {
		Cleric* cleric = new Cleric();
		cleric->battleground_position = { x,y };
		entities.push_back(cleric);
		ret = true;
		break;

	}
	case EntityType::DWARF: {
		Dwarf* dwarf = new Dwarf();
		dwarf->battleground_position = { x,y };
		entities.push_back(dwarf);
		ret = true;
		break;
	}
	case EntityType::ELF: {
		Elf* elf = new Elf();
		elf->battleground_position = { x,y };
		entities.push_back(elf);
		ret = true;
		break;
	}
	case EntityType::WARRIOR: {
		Warrior* warrior = new Warrior();
		warrior->battleground_position = { x,y };
		entities.push_back(warrior);
		ret = true;
		break;
	}
	default:
		break;
	}


	return ret;
}

bool ctEntities::Load(pugi::xml_node& load)
{
	bool ret = true;

	return ret;
}

bool ctEntities::Save(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}

/*Player* ctEntities::GetPlayer() const {

	for (uint i = 0; i < entities.capacity(); ++i)
	{
		if (entities.at(i) != nullptr)
		{
			if (entities[i]->type == PLAYER)
				return (Player*)entities[i];
		}
	}

	return nullptr;

}*/

Entity* ctEntities::AddEntity(EntityType type_entity)
{

	Entity* ret = nullptr;

	switch (type_entity)
	{
	case CLERIC: ret = new Entity(CLERIC);
		break;
	case KOBOLD: ret = new Entity(KOBOLD);
		break;
	default:
		break;
	}

	if (ret != nullptr)
	{
		entities.push_back(ret);
	}

	return ret;

}

Entity* ctEntities::GetEntity(EntityType type_entity)
{
	Entity* ret = nullptr;

	for (std::vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		if ((*it)->entity_type == type_entity)
		{
			ret = (*it);
		}
	}

	return ret;

}

