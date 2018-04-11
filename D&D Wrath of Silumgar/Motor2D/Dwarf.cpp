#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctRender.h"
#include "ctTextures.h"
#include "ctFadeToBlack.h"
#include "ctAudio.h"
#include "ctEntities.h"

#include "Dwarf.h"


Dwarf::Dwarf(int x, int y, EntityType type) : Entity(x, y, type) {

	bool ret = true;

	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadEntities(config_file);
	node = &node->child("heroes").child("dwarf");
	texture = App->tex->Load(App->entities->dwarf_spritesheet_name.data());

	attack_fx = App->audio->LoadFx(node->child("sounds").attribute("attack_fx").as_string());
	death_fx = App->audio->LoadFx(node->child("sounds").attribute("death_fx").as_string());
	damaged_fx = App->audio->LoadFx(node->child("sounds").attribute("damaged_fx").as_string());
	run_fx = App->audio->LoadFx(node->child("sounds").attribute("run_fx").as_string());
	//todo get the stats

	for (pugi::xml_node animations = node->child("animations").child("animation"); animations && ret; animations = animations.next_sibling("animation"))
	{
		std::string tmp(animations.attribute("name").as_string());

		if (tmp == "idle")
			LoadAnimation(animations, &idle);
		else if (tmp == "run")
			LoadAnimation(animations, &run_forward);
		else if (tmp == "default_attack")
			LoadAnimation(animations, &attack);
		else if (tmp == "hit")
			LoadAnimation(animations, &hit);
		else if (tmp == "death")
			LoadAnimation(animations, &death);
	}
	LoadProperties(node->child("statistics"));
	animation = &idle;

}

Dwarf::~Dwarf()
{
	LOG("Freeing the player");

}

// Called each loop iteration
void Dwarf::Update(float dt)
{

	if (dt > 0)
	{
		if (!key_entities_speed)
			SetEntitiesSpeed(dt);

		SetPlayerAnimationsSpeed(dt);
	}
		
}

void Dwarf::SetPlayerAnimationsSpeed(float dt)
{
	idle.speed = idle_vel * dt;
	run_forward.speed = run_forward_vel * dt;
}

void Dwarf::SetEntitiesSpeed(float dt)
{
	idle_vel = idle.speed;
	run_forward_vel = run_forward.speed;

	key_entities_speed = true;
}

void Dwarf::LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation)
{
	bool ret = true;

	for (pugi::xml_node frame = animation_node.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
}


void Dwarf::Attack()
{

	App->audio->PlayFx(attack_fx, 0);

}

void  Dwarf::Death() {
	App->audio->PlayFx(death_fx, 0);
}
void  Dwarf::Run() {
	App->audio->PlayFx(run_fx, 0);
}
void  Dwarf::Damaged() {
	App->audio->PlayFx(damaged_fx, 0);
}