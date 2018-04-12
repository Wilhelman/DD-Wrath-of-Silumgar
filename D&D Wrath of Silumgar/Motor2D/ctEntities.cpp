#include "ctApp.h"
#include "ctRender.h"
#include "ctEntities.h"
#include "ctTextures.h"
#include "Entity.h"
#include "ctAudio.h"
#include "ctWindow.h"
#include "ctLog.h"
#include "ctCombat.h"
#include "ctFadeToBlack.h"
#include "j1Map.h"
#include "Cleric.h"
#include "Dwarf.h"
#include "Elf.h"
#include "Warrior.h"

#include "Kobold.h"
#include "Owlbear.h"
#include "Gnoll.h"
#include "GnollArcher.h"

#include "MiniHeroes.h"


ctEntities::ctEntities()
{
	name = "entities";
}

// Destructor
ctEntities::~ctEntities()
{
	LOG("Unloading entities spritesheet");
	
}

bool ctEntities::Awake(pugi::xml_node& config)
{
	LOG("Loading Entities from config file");
	bool ret = true;

	//cleric spritesheet
	cleric_spritesheet_name = config.child("cleric").attribute("spritesheetName").as_string();
	dwarf_spritesheet_name = config.child("dwarf").attribute("spritesheetName").as_string();
	elf_spritesheet_name = config.child("elf").attribute("spritesheetName").as_string();
	warrior_spritesheet_name = config.child("warrior").attribute("spritesheetName").as_string();

	kobold_spritesheet_name = config.child("kobold").attribute("spritesheetName").as_string();
	gnoll_spritesheet_name = config.child("gnollA").attribute("spritesheetName").as_string();
	gnollArcher_spritesheet_name = config.child("bowGnoll").attribute("spritesheetName").as_string();
	owlbear_spritesheet_name = config.child("owlbear").attribute("spritesheetName").as_string();

	miniheroes_spritesheet_name = config.child("miniheroes").attribute("spritesheetName").as_string();
	return ret;
}

bool ctEntities::Start()
{


	return true;
}

bool ctEntities::PreUpdate()
{
	for (int i = 0; i < entities.size(); i++) {
		if (entities[i]->to_destroy) {
			delete(entities[i]);
			entities[i] = nullptr;
			entities.erase(entities.cbegin() + i);
			entities.shrink_to_fit();
		}
	}

	if (entities.size() != draw_priority_entities.size())
	{
		OrderDrawEntities();
	}
	return true;
}

// Called before render is available
bool ctEntities::Update(float dt)
{

	for (int i = 0; i < entities.size(); i++)
		if (entities.at(i) != nullptr) entities[i]->Update(dt);

	for (int i = 0; i <draw_priority_entities.size(); i++)
		if (draw_priority_entities.at(i) != nullptr) draw_priority_entities[i]->Draw();


	return true;

}

// Called before quitting
bool ctEntities::CleanUp()
{
	LOG("Freeing all entities");



	for (uint i = 0; i < entities.size(); ++i)
	{
		if (entities[i] != nullptr)
		{
			delete entities[i];
			entities[i] = nullptr;
			entities.erase(entities.cbegin() + i);
			entities.shrink_to_fit();
		}
	}

	entities.clear();


	return true;
}


bool ctEntities:: SpawnEntity(int x, int y, EntityType type)
{
	// find room for the new entity
	bool ret = false;



	switch (type)
	{
	case EntityType::CLERIC: {
		Cleric* cleric = new Cleric(x, y, CLERIC);
		entities.push_back(cleric);
		App->combat->turn_priority_entity.push_back(cleric);
		ret = true;
		break;
	}
	case EntityType::DWARF: {
		Dwarf* dwarf = new Dwarf(x, y, DWARF);
		entities.push_back(dwarf);
		App->combat->turn_priority_entity.push_back(dwarf);
		ret = true;
		break;
	}
	case EntityType::ELF: {
		Elf* elf = new Elf(x, y, ELF);
		entities.push_back(elf);
		App->combat->turn_priority_entity.push_back(elf);
		ret = true;
		break;
	}
	case EntityType::WARRIOR: {
		Warrior* warrior = new Warrior(x, y, WARRIOR);
		entities.push_back(warrior);
		App->combat->turn_priority_entity.push_back(warrior);
		ret = true;
		break;
	}
	case EntityType::KOBOLD: {
		Kobold* kobold = new Kobold(x, y, KOBOLD);
		entities.push_back(kobold);
		App->combat->turn_priority_entity.push_back(kobold);
		ret = true;
		break;
	}
	case EntityType::GNOLL: {
		Gnoll* gnoll = new Gnoll(x, y, GNOLL);
		entities.push_back(gnoll);
		App->combat->turn_priority_entity.push_back(gnoll);
		ret = true;
		break;
	}
	case EntityType::GNOLL_ARCHER: {
		GnollArcher* gnollArcher = new GnollArcher(x, y, GNOLL_ARCHER);
		entities.push_back(gnollArcher);
		App->combat->turn_priority_entity.push_back(gnollArcher);
		ret = true;
		break;
	}
	case EntityType::OWLBEAR: {
		Owlbear* owlbear = new Owlbear(x, y, OWLBEAR);
		entities.push_back(owlbear);
		App->combat->turn_priority_entity.push_back(owlbear);
		ret = true;
		break;
	}
	case EntityType::MINIHEROES: {
		MiniHeroes* miniheroes = new MiniHeroes(x, y, MINIHEROES);
		entities.push_back(miniheroes);
		
		ret = true;
		break;
	}

	default:
		break;
	}


	return ret;
}


Cleric* ctEntities::GetCleric() const {

	for (uint i = 0; i < entities.size(); ++i)
	{
		if (entities.at(i) != nullptr)
		{
			if (entities[i]->type == CLERIC)
				return (Cleric*)entities[i];
		}
	}

	return nullptr;

}
Dwarf* ctEntities::GetDwarf() const {

	for (uint i = 0; i < entities.size(); ++i)
	{
		if (entities.at(i) != nullptr)
		{
			if (entities[i]->type == DWARF)
				return (Dwarf*)entities[i];
		}
	}

	return nullptr;

}
Elf* ctEntities::GetElf() const {

	for (uint i = 0; i < entities.size(); ++i)
	{
		if (entities.at(i) != nullptr)
		{
			if (entities[i]->type == ELF)
				return (Elf*)entities[i];
		}
	}

	return nullptr;

}
Warrior* ctEntities::GetWarrior() const {

	for (uint i = 0; i < entities.size(); ++i)
	{
		if (entities.at(i) != nullptr)
		{
			if (entities[i]->type == WARRIOR)
				return (Warrior*)entities[i];
		}
	}

	return nullptr;

}
Kobold* ctEntities::GetKobold() const {

	for (uint i = 0; i < entities.size(); ++i)
	{
		if (entities.at(i) != nullptr)
		{
			if (entities[i]->type == KOBOLD)
				return (Kobold*)entities[i];
		}
	}

	return nullptr;

}
Gnoll* ctEntities::GetGnoll() const {

	for (uint i = 0; i < entities.size(); ++i)
	{
		if (entities.at(i) != nullptr)
		{
			if (entities[i]->type == GNOLL)
				return (Gnoll*)entities[i];
		}
	}

	return nullptr;

}
gnollArcher* ctEntities::GetGnoll_Archer() const {

	for (uint i = 0; i < entities.size(); ++i)
	{
		if (entities.at(i) != nullptr)
		{
			if (entities[i]->type == GNOLL_ARCHER)
				return (gnollArcher*)entities[i];
		}
	}

	return nullptr;

}
Owlbear* ctEntities::GetOwlbear() const {

	for (uint i = 0; i < entities.size(); ++i)
	{
		if (entities.at(i) != nullptr)
		{
			if (entities[i]->type == OWLBEAR)
				return (Owlbear*)entities[i];
		}
	}

	return nullptr;

}

MiniHeroes* ctEntities::GetMiniheroes() const {

	for (uint i = 0; i < entities.size(); ++i)
	{
		if (entities.at(i) != nullptr)
		{
			if (entities[i]->type == MINIHEROES)
				return (MiniHeroes*)entities[i];
		}
	}

	return nullptr;

}

void ctEntities::OrderDrawEntities()
{
	bool ordered = false;

	
	
	std::vector<Entity*> order_entity = entities;

	while (!ordered)
	{
   		ordered = true;
		std::vector<Entity*>::iterator itnext = order_entity.begin();
		int count = 0;
		for (std::vector<Entity*>::iterator it = order_entity.begin(); it != order_entity.end(); ++it)
		{
			itnext++;
			count++;
			if (count != order_entity.size())
			{
				if ((*it)->initial_position.y > (*itnext)->initial_position.y)
				{
					Entity* entity_tmp = (*it);

					(*it) = (*itnext);
					it++;
					(*it) = entity_tmp;
					it--;
					ordered = false;
				}
				
			}
			else {
				break;
			}

		}

	}

	std::vector<Entity*>::iterator itnext = order_entity.begin();
	int count = 0;
	for (std::vector<Entity*>::iterator it = order_entity.begin(); it != order_entity.end(); ++it)
	{
		itnext++;
		count++;
		if (count != order_entity.size())
		{
			if (((*itnext)->type != CLERIC && (*itnext)->type != WARRIOR && (*itnext)->type != ELF && (*itnext)->type != DWARF) && ((*it)->type == CLERIC || (*it)->type == WARRIOR || (*it)->type == ELF || (*it)->type == DWARF))
			{
				Entity* entity_tmp = (*it);

				(*it) = (*itnext);
				it++;
				(*it) = entity_tmp;
				it--;
				ordered = false;
			}

		}
		else {
			break;
		}


	}
		
	



	draw_priority_entities = order_entity;

	order_entity.clear();
}

