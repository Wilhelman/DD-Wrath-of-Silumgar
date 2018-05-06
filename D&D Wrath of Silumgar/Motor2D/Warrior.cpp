#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctRender.h"
#include "ctTextures.h"
#include "ctFadeToBlack.h"
#include "ctAudio.h"
#include "ctEntities.h"

#include "Warrior.h"


Warrior::Warrior(int x, int y, EntityType type) : Entity(x, y, type) {

	bool ret = true;

	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadEntities(config_file);
	node = &node->child("heroes").child("warrior");
	texture = App->tex->Load(App->entities->warrior_spritesheet_name.data());

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
		else if (tmp == "throw")
			LoadAnimation(animations, &throw_object);
		else if (tmp == "death")
			LoadAnimation(animations, &death);
		else if (tmp == "kick")
			LoadAnimation(animations, &kick);
		else if (tmp == "dodge")
			LoadAnimation(animations, &dodge);
		else if (tmp == "heavy_slash")
			LoadAnimation(animations, &heavy_slash);
		else if (tmp == "guard")
			LoadAnimation(animations, &guard);
		else if (tmp == "kick+")
			LoadAnimation(animations, &kick_plus);
		else if (tmp == "guard+")
			LoadAnimation(animations, &guard_plus);
		else if (tmp == "heavy_slash+")
			LoadAnimation(animations, &heavy_slash_plus);
		else if (tmp == "charge")
			LoadAnimation(animations, &charge);
		else if (tmp == "taunt")
			LoadAnimation(animations, &taunt);
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
void Warrior::Update(float dt)
{

	if (dt > 0)
	{
		if (!key_entities_speed)
			SetEntitiesSpeed(dt);

		SetPlayerAnimationsSpeed(dt);
	}

}

void Warrior::SetPlayerAnimationsSpeed(float dt)
{
	idle.speed = idle_vel * dt;
	run_forward.speed = run_forward_vel * dt;
}

void Warrior::SetEntitiesSpeed(float dt)
{
	idle_vel = idle.speed;
	run_forward_vel = run_forward.speed;

	key_entities_speed = true;
}

void Warrior::LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation)
{
	bool ret = true;

	for (pugi::xml_node frame = animation_node.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
}


void Warrior::Attack()
{
	App->audio->PlayFx(App->audio->warrior_attack_fx, 0);

}

void  Warrior::Death() {
	App->audio->PlayFx(App->audio->warrior_death_fx, 0);
}
void  Warrior::Run() {
	App->audio->PlayFx(App->audio->warrior_run_fx, 0);
}
void  Warrior::Damaged() {
	App->audio->PlayFx(App->audio->warrior_damaged_fx, 0);
}
void  Warrior::Ability1T1() {
	App->audio->PlayFx(App->audio->warrior_kick_fx, 0);
}
void  Warrior::Ability2T1() {
	App->audio->PlayFx(App->audio->warrior_guard_fx, 0);
}
void  Warrior::Ability3T1() {
	App->audio->PlayFx(App->audio->warrior_heavy_slash_fx, 0);
}
void  Warrior::Ability1T2() {
	App->audio->PlayFx(App->audio->warrior_charge_fx, 0);
}
void  Warrior::Ability2T2() {
	App->audio->PlayFx(App->audio->warrior_taunt_fx, 0);
}
void  Warrior::Ability3T2() {
	App->audio->PlayFx(App->audio->warrior_whirlwind_fx, 0);
}