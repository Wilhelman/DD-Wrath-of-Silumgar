#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctRender.h"
#include "ctTextures.h"
#include "ctFadeToBlack.h"
#include "ctAudio.h"
#include "ctEntities.h"

#include "Elf.h"


Elf::Elf(int x, int y, EntityType type) : Entity(x, y, type) {

	bool ret = true;

	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadEntities(config_file);
	node = &node->child("heroes").child("elf");
	texture = App->tex->Load(App->entities->elf_spritesheet_name.data());

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
		else if (tmp == "dodge")
			LoadAnimation(animations, &dodge);
		else if (tmp == "blizzard")
			LoadAnimation(animations, &blizzard);
		else if (tmp == "throw")
			LoadAnimation(animations, &throw_object);
		else if (tmp == "seed_of_life")
			LoadAnimation(animations, &seed_of_life);
		else if (tmp == "fireball")
			LoadAnimation(animations, &fireball);
		else if (tmp == "lightning_bolt")
			LoadAnimation(animations, &lightning_bolt);
		else if (tmp == "seed_of_life+")
			LoadAnimation(animations, &seed_of_life_plus);
		else if (tmp == "fireball+")
			LoadAnimation(animations, &fireball_plus);
		else if (tmp == "lightning_bolt+")
			LoadAnimation(animations, &lightning_bol_plus);
		else if (tmp == "revive")
			LoadAnimation(animations, &revive);
		else if (tmp == "fire_djinn")
			LoadAnimation(animations, &fire_djinn);
		else if (tmp == "menu_animation")
			LoadAnimation(animations, &menu_animation);
		else if (tmp == "stun")
			LoadAnimation(animations, &stun);
		else if (tmp == "rock")
			LoadAnimation(animations, &rock);
	
	}
	LoadProperties(node->child("statistics"));
	animation = &idle;

}



// Called each loop iteration
void Elf::Update(float dt)
{

	if (dt > 0)
	{
		if (!key_entities_speed)
			SetEntitiesSpeed(dt);

		SetPlayerAnimationsSpeed(dt);
	}

}

void Elf::SetPlayerAnimationsSpeed(float dt)
{
	idle.speed = idle_vel * dt;
	run_forward.speed = run_forward_vel * dt;
}

void Elf::SetEntitiesSpeed(float dt)
{
	idle_vel = idle.speed;
	run_forward_vel = run_forward.speed;

	key_entities_speed = true;
}

void Elf::LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation)
{
	bool ret = true;

	for (pugi::xml_node frame = animation_node.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
}

void Elf::Attack()
{
	App->audio->PlayFx(App->audio->elf_attack_fx, 0);

}

void  Elf::Death() {
	App->audio->PlayFx(App->audio->elf_death_fx, 0);
}
void  Elf::Run() {
	App->audio->PlayFx(App->audio->elf_run_fx, 0);
}
void  Elf::Damaged() {
	App->audio->PlayFx(App->audio->elf_damaged_fx, 0);
}
void  Elf::Ability1T1() {
	App->audio->PlayFx(App->audio->elf_seeds_of_life_fx, 0);
}
void  Elf::Ability2T1() {
	App->audio->PlayFx(App->audio->elf_fireball_fx, 0);
}
void  Elf::Ability3T1() {
	App->audio->PlayFx(App->audio->elf_lightning_bolt_fx, 0);
}
void  Elf::Ability1T2() {
	App->audio->PlayFx(App->audio->elf_revive_fx, 0);
}
void  Elf::Ability2T2() {
	App->audio->PlayFx(App->audio->elf_blizzard_fx, 0);
}
void  Elf::Ability3T2() {
	App->audio->PlayFx(App->audio->elf_fire_djiin_fx, 0);
}
