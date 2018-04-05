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
#include "ctTaskManager.h"

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
	App->tex->UnLoad(entity_sprites);
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
	bool ret = true;

	//cleric spritesheet
	cleric_spritesheet = App->tex->Load(cleric_spritesheet_name.data());
	dwarf_spritesheet = App->tex->Load(dwarf_spritesheet_name.data());
	elf_spritesheet = App->tex->Load(elf_spritesheet_name.data());
	warrior_spritesheet = App->tex->Load(warrior_spritesheet_name.data());

	kobold_spritesheet = App->tex->Load(kobold_spritesheet_name.data());
	gnoll_spritesheet = App->tex->Load(gnoll_spritesheet_name.data());
	gnollArcher_spritesheet = App->tex->Load(gnollArcher_spritesheet_name.data());
	owlbear_spritesheet = App->tex->Load(owlbear_spritesheet_name.data());

	miniheroes_spritesheet = App->tex->Load(miniheroes_spritesheet_name.data());

	if (cleric_spritesheet == NULL || dwarf_spritesheet == NULL || elf_spritesheet == NULL || warrior_spritesheet == NULL) {
		LOG("Error loading entities spritesheet!!");
		ret = false;
	}

	if (!ret)
		return false;

	return ret;
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

	return true;
}

// Called before render is available
bool ctEntities::Update(float dt)
{

	for (int i = 0; i < entities.size(); i++)
		if (entities.at(i) != nullptr) entities[i]->Update(dt);

	for (int i = 0; i < entities.size(); i++) {
		switch (entities.at(i)->type)
		{
		case CLERIC:
			if (entities.at(i) != nullptr) entities[i]->Draw(cleric_spritesheet);
			break;
		case DWARF:
			if (entities.at(i) != nullptr) entities[i]->Draw(dwarf_spritesheet);
			break;
		case ELF:
			if (entities.at(i) != nullptr) entities[i]->Draw(elf_spritesheet);
			break;
		case WARRIOR:
			if (entities.at(i) != nullptr) entities[i]->Draw(warrior_spritesheet);
			break;
		case KOBOLD:
			if (entities.at(i) != nullptr) entities[i]->Draw(kobold_spritesheet);
			break;
		case GNOLL:
			if (entities.at(i) != nullptr) entities[i]->Draw(gnoll_spritesheet);
			break;
		case GNOLL_ARCHER:
			if (entities.at(i) != nullptr) entities[i]->Draw(gnollArcher_spritesheet);
			break;
		case OWLBEAR:
			if (entities.at(i) != nullptr) entities[i]->Draw(owlbear_spritesheet);
			break;
		case MINIHEROES:
			if (entities.at(i) != nullptr) entities[i]->Draw(miniheroes_spritesheet);
			break;
		default:
			break;
		}
	}

	return true;

}

// Called before quitting
bool ctEntities::CleanUp()
{
	LOG("Freeing all entities");

	//cleric spritesheet
	App->tex->UnLoad(cleric_spritesheet);
	App->tex->UnLoad(dwarf_spritesheet);
	App->tex->UnLoad(elf_spritesheet);
	App->tex->UnLoad(warrior_spritesheet);

	App->tex->UnLoad(kobold_spritesheet);
	App->tex->UnLoad(gnoll_spritesheet);
	App->tex->UnLoad(gnollArcher_spritesheet);
	App->tex->UnLoad(owlbear_spritesheet);

	App->tex->UnLoad(miniheroes_spritesheet);

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
		App->combat->priority_entity.push_back(cleric);
		App->task_manager->Player = cleric; //guarrada: to improve (quitar include taskmanager)
		ret = true;
		break;
	}
	case EntityType::DWARF: {
		Dwarf* dwarf = new Dwarf(x, y, DWARF);
		entities.push_back(dwarf);
		App->combat->priority_entity.push_back(dwarf);
		ret = true;
		break;
	}
	case EntityType::ELF: {
		Elf* elf = new Elf(x, y, ELF);
		entities.push_back(elf);
		App->combat->priority_entity.push_back(elf);
		ret = true;
		break;
	}
	case EntityType::WARRIOR: {
		Warrior* warrior = new Warrior(x, y, WARRIOR);
		entities.push_back(warrior);
		App->combat->priority_entity.push_back(warrior);
		App->task_manager->Enemy = warrior; //guarrada: to improve (quitar include taskmanager)
		ret = true;
		break;
	}
	case EntityType::KOBOLD: {
		Kobold* kobold = new Kobold(x, y, KOBOLD);
		entities.push_back(kobold);
		App->combat->priority_entity.push_back(kobold);
		ret = true;
		break;
	}
	case EntityType::GNOLL: {
		Gnoll* gnoll = new Gnoll(x, y, GNOLL);
		entities.push_back(gnoll);
		App->combat->priority_entity.push_back(gnoll);
		ret = true;
		break;
	}
	case EntityType::GNOLL_ARCHER: {
		GnollArcher* gnollArcher = new GnollArcher(x, y, GNOLL_ARCHER);
		entities.push_back(gnollArcher);
		App->combat->priority_entity.push_back(gnollArcher);
		ret = true;
		break;
	}
	case EntityType::OWLBEAR: {
		Owlbear* owlbear = new Owlbear(x, y, OWLBEAR);
		entities.push_back(owlbear);
		App->combat->priority_entity.push_back(owlbear);
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

