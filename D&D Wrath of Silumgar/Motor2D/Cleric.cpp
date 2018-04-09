#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctApp.h"
#include "ctRender.h"
#include "ctTextures.h"
#include "ctFadeToBlack.h"
#include "ctAudio.h"
#include "ctEntities.h"

#include "Cleric.h"


Cleric::Cleric(int x, int y, EntityType type) : Entity(x, y, type) {

	bool ret = true;
	texture = App->tex->Load(App->entities->cleric_spritesheet_name.data());
	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadEntities(config_file);
	node = &node->child("heroes").child("cleric");

	attack_fx = App->audio->LoadFx(node->attribute("attack_fx").as_string());

	//todo get the stats

	for (pugi::xml_node animations = node->child("animations").child("animation"); animations && ret; animations = animations.next_sibling("animation"))
	{
		std::string tmp(animations.attribute("name").as_string());

		if (tmp == "idle")
			LoadAnimation(animations, &idle);
		else if (tmp == "run_forward")
			LoadAnimation(animations, &run_forward);
		else if (tmp == "attack")
			LoadAnimation(animations, &attack);
	}
	LoadProperties(node->child("statistics"));
	animation = &idle;

}

Cleric::~Cleric()
{
	LOG("Freeing the player");

	App->audio->UnLoadFx(attack_fx);
}

// Called each loop iteration
void Cleric::Update(float dt)
{

	if (dt > 0)
	{
		if (!key_entities_speed)
			SetEntitiesSpeed(dt);

		SetPlayerAnimationsSpeed(dt);
	}
		
}

void Cleric::Attack()
{

	LOG("Attack!");
}

void Cleric::SetPlayerAnimationsSpeed(float dt)
{
	idle.speed = idle_vel * dt;
	run_forward.speed = run_forward_vel * dt;
}

void Cleric::SetEntitiesSpeed(float dt)
{
	idle_vel = idle.speed;
	run_forward_vel = run_forward.speed;

	key_entities_speed = true;
}

void Cleric::LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation)
{
	bool ret = true;

	for (pugi::xml_node frame = animation_node.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
}