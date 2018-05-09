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
		else if (tmp == "throw")
			LoadAnimation(animations, &throw_object);
		else if (tmp == "heal")
			LoadAnimation(animations, &heal);
		else if (tmp == "dodge")
			LoadAnimation(animations, &dodge);
		else if (tmp == "mace_throw")
			LoadAnimation(animations, &mace_throw);
		else if (tmp == "heal+")
			LoadAnimation(animations, &heal_plus);
		else if (tmp == "mace_throw+")
			LoadAnimation(animations, &mace_throw_plus);
		else if (tmp == "blessing")
			LoadAnimation(animations, &blessing);
		else if (tmp == "harden_skin")
			LoadAnimation(animations, &harder_skin);
		else if (tmp == "clarity")
			LoadAnimation(animations, &clarity);
		else if (tmp == "light_strike")
			LoadAnimation(animations, &light_strike);
		else if (tmp == "light_strike+")
			LoadAnimation(animations, &light_strike_plus);
		else if (tmp == "menu_animation")
			LoadAnimation(animations, &menu_animation);
		else if (tmp == "stun")
			LoadAnimation(animations, &stun);

	}
	LoadProperties(node->child("statistics"));


	//LoadActions()

	animation = &idle;

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

void Cleric::Attack()
{

	App->audio->PlayFx(App->audio->cleric_attack_fx, 0);

}

void  Cleric::Death() {
	App->audio->PlayFx(App->audio->cleric_death_fx, 0);
}
void  Cleric::Run() {
	App->audio->PlayFx(App->audio->cleric_run_fx, 0);
}
void  Cleric::Damaged() {
	App->audio->PlayFx(App->audio->cleric_damaged_fx, 0);
}
void  Cleric::Ability1T1() {
	App->audio->PlayFx(App->audio->cleric_heal_fx, 0);
}
void  Cleric::Ability2T1() {
	App->audio->PlayFx(App->audio->cleric_light_strike_fx, 0);
}
void  Cleric::Ability3T1() {
	App->audio->PlayFx(App->audio->cleric_mace_throw_fx, 0);
}
void  Cleric::Ability1T2() {
	App->audio->PlayFx(App->audio->cleric_blessing_fx, 0);
}
void  Cleric::Ability2T2() {
	App->audio->PlayFx(App->audio->cleric_harden_skin_fx, 0);
}
void  Cleric::Ability3T2() {
	App->audio->PlayFx(App->audio->cleric_clarity_fx, 0);
}