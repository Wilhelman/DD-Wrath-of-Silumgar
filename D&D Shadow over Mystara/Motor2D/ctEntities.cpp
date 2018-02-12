#include "ctApp.h"
#include "ctRender.h"
#include "ctEntities.h"
#include "ctTextures.h"
#include "Entity.h"
#include "ctAudio.h"
#include "ctWindow.h"
//#include "j1Collider.h"
#include "ctLog.h"
//#include "ctMap.h"
#include "ctFadeToBlack.h"
//#include "j1Scene.h"

#include "Player.h"


ctEntities::ctEntities()
{
	name.create("entities");
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

	spritesheetName.create(config.child("spritesheetSource").attribute("name").as_string());

	return ret;
}

bool ctEntities::Start()
{
	bool ret = true;

	entity_sprites = App->tex->Load(spritesheetName.GetString());

	if (entity_sprites == NULL) {
		LOG("Error loading entities spritesheet!!");
		ret = false;
	}

	if (!ret)
		return false;

	return ret;
}

bool ctEntities::PreUpdate()
{
	for (int i = 0; i < entities.Count(); i++) {
		if (entities[i]->to_destroy) {
			delete(entities[i]);
			entities[i] = nullptr;
			if (!entities.RemoveAt(i)) {
				LOG("Error removing entity");
				return false;
			}
		}
	}
	return true;
}

// Called before render is available
bool ctEntities::Update(float dt)
{

	for (int i = 0; i < entities.Count() && App->fadeToBlack->FadeIsOver() /*&& !App->scene->IsGamePaused()*/; i++)
		if (entities[i] != nullptr) entities[i]->Update(dt);

	for (int i = 0; i < entities.Count(); i++)
		if (entities[i] != nullptr) entities[i]->Draw(entity_sprites);

	return true;
}

// Called before quitting
bool ctEntities::CleanUp()
{
	LOG("Freeing all enemies");

	App->tex->UnLoad(entity_sprites);

	for (int i = entities.Count() - 1; i >= 0; --i)
	{
		if (entities[i] != nullptr) {
			delete(entities[i]);
			entities[i] = nullptr;
			if (!entities.RemoveAt(i)) {
				LOG("Error removing entity");
				return false;
			}
		}
	}

	entities.Clear();

	return true;
}

bool ctEntities::SpawnEntity(int x, int y, EntityType type)
{
	// find room for the new entity
	bool ret = false;

	switch (type)
	{
	case EntityType::PLAYER: {
		Player* player = new Player(x, y, PLAYER);
		entities.PushBack(player);
		ret = true;
		break;
	}
	default:
		break;
	}


	return ret;
}

void ctEntities::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < entities.Count(); ++i)
		if (entities[i] != nullptr && entities[i]->GetCollider() == c1)
			entities[i]->OnCollision(c2);
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

Player* ctEntities::GetPlayer() const {

	for (uint i = 0; i < entities.Count(); ++i)
	{
		if (entities[i] != nullptr)
		{
			if (entities[i]->type == PLAYER)
				return (Player*)entities[i];
		}
	}

	return nullptr;

}