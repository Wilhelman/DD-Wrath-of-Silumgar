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
		else if (tmp == "double_edged_axe")
			LoadAnimation(animations, &high_axe);
		else if (tmp == "dodge")
			LoadAnimation(animations, &dodge);
		else if (tmp == "throw")
			LoadAnimation(animations, &throw_object);
		else if (tmp == "morale_boost")
			LoadAnimation(animations, &morale_boost);
		else if (tmp == "dwarfs_wrath")
			LoadAnimation(animations, &dwarfs_wrath);
		else if (tmp == "morale_boost+")
			LoadAnimation(animations, &morale_boost_plus);
		else if (tmp == "double_edged_axe+")
			LoadAnimation(animations, &high_axe_plus);
		else if (tmp == "dwarfs_wrath+")
			LoadAnimation(animations, &dwarfs_wrath_plus);
		else if (tmp == "raging_mock")
			LoadAnimation(animations, &raging_mock);
		else if (tmp == "meteor")
			LoadAnimation(animations, &meteor);
		else if (tmp == "counter")
			LoadAnimation(animations, &counter);
		else if (tmp == "menu_animation")
			LoadAnimation(animations, &menu_animation);
		else if (tmp == "rock")
			LoadAnimation(animations, &rock);
	}
	LoadProperties(node->child("statistics"));
	animation = &idle;

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

	App->audio->PlayFx(App->audio->dwarf_attack_fx, 0);

}

void  Dwarf::Death() {
	App->audio->PlayFx(App->audio->dwarf_death_fx, 0);
}
void  Dwarf::Run() {
	App->audio->PlayFx(App->audio->dwarf_run_fx, 0);
}
void  Dwarf::Damaged() {
	App->audio->PlayFx(App->audio->dwarf_damaged_fx, 0);
}
void  Dwarf::Ability1T1() {
	App->audio->PlayFx(App->audio->dwarf_morale_boost_fx, 0);
}
void  Dwarf::Ability2T1() {
	App->audio->PlayFx(App->audio->dwarf_double_axe_fx, 0);
}
void  Dwarf::Ability3T1() {
	App->audio->PlayFx(App->audio->dwarf_dwarfs_wrath_fx, 0);
}
void  Dwarf::Ability1T2() {
	App->audio->PlayFx(App->audio->dwarf_raging_mock_fx, 0);
}
void  Dwarf::Ability2T2() {
	App->audio->PlayFx(App->audio->dwarf_meteor_fx, 0);
}
void  Dwarf::Ability3T2() {
	App->audio->PlayFx(App->audio->dwarf_counter_fx, 0);
}