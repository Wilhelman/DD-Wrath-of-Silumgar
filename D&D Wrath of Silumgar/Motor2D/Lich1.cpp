#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctRender.h"
#include "ctTextures.h"
#include "ctFadeToBlack.h"
#include "ctAudio.h"
#include "ctEntities.h"
#include "ctCombat.h"
#include "ctTaskManager.h"

#include "Lich1.h"
#include "Lich2.h"


Lich1::Lich1(int x, int y, EntityType type) : Entity(x, y, type) {

	bool ret = true;

	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadEntities(config_file);
	node = &node->child("enemies").child("lich1st");
	texture = App->tex->Load(App->entities->lich1_spritesheet_name.data());


	for (pugi::xml_node animations = node->child("animations").child("animation"); animations && ret; animations = animations.next_sibling("animation"))
	{
		std::string tmp(animations.attribute("name").as_string());

		if (tmp == "idle")
			LoadAnimation(animations, &idle);
		else if (tmp == "run")
			LoadAnimation(animations, &run_forward);
		else if (tmp == "hit")
			LoadAnimation(animations, &hit);
		else if (tmp == "death")
			LoadAnimation(animations, &death);
		else if (tmp == "stun")
			LoadAnimation(animations, &stun);
		else if (tmp == "dodge")
			LoadAnimation(animations, &dodge);
		else if (tmp == "dragon_flames")
			LoadAnimation(animations, &dragon_flames);
		else if (tmp == "lightning_flash")
			LoadAnimation(animations, &lightning_flash);
		else if (tmp == "abyssal_sphere")
			LoadAnimation(animations, &abyssal_sphere);
	}
	LoadProperties(node->child("statistics"));
	animation = &idle;

	//prepare the actions:

	dragon_flames_action.name = "Dragon Flames";
	dragon_flames_action.mana_points_effect_to_himself = 0;
	dragon_flames_action.objective = HEROES;
	dragon_flames_action.type = DRAGON_FLAMES;

	lightning_flash_action.name = "Lightning Flash";
	lightning_flash_action.mana_points_effect_to_himself = 0;
	lightning_flash_action.objective = HEROES;
	lightning_flash_action.type = LIGHTNING_FLASH;

	abyssal_sphere_action.name = "Abyssal Sphere";
	abyssal_sphere_action.mana_points_effect_to_himself = 0;
	abyssal_sphere_action.objective = ENEMIES;
	abyssal_sphere_action.type = ABYSSAL_SPHERE;
}



// Called each loop iteration
void Lich1::Update(float dt)
{

	if (dt > 0)
	{
		if (!key_entities_speed)
			SetEntitiesSpeed(dt);

		SetPlayerAnimationsSpeed(dt);
	}

}

void Lich1::SetPlayerAnimationsSpeed(float dt)
{
	idle.speed = idle_vel * dt;
	run_forward.speed = run_forward_vel * dt;
}

void Lich1::SetEntitiesSpeed(float dt)
{
	idle_vel = idle.speed;
	run_forward_vel = run_forward.speed;

	key_entities_speed = true;
}

void Lich1::LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation)
{
	bool ret = true;

	for (pugi::xml_node frame = animation_node.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
}

void Lich1::Attack()
{
	//App->audio->PlayFx(App->audio->dark_warrior_attack_fx, 0);

}

void  Lich1::Death() {
	App->audio->PlayFx(App->audio->dark_warrior_death_fx, 0);
}
void  Lich1::Run() {
	App->audio->PlayFx(App->audio->dark_warrior_run_fx, 0);
}
void  Lich1::Damaged() {
	App->audio->PlayFx(App->audio->dark_warrior_damaged_fx, 0);
}
void Lich1::Ability1T1() {
	//App->audio->PlayFx(App->audio->dark_warrior_thunder_punch_fx, 0);
}
void Lich1::Ability2T1() {
	//App->audio->PlayFx(App->audio->dark_warrior_void_cannon_fx, 0);
}
void Lich1::Ability3T1() {
	//App->audio->PlayFx(App->audio->dark_warrior_insignificant_mortals_fx, 0);
}


void Lich1::PerformAction()
{
	Entity* entity_objective = nullptr;

	entity_objective = App->combat->GetRandomHeroe();

	int random_number = (rand() % 2) + 1; //random del 1-2

	if (random_number == 1)
		App->task_manager->AddTask(new PerformActionToEntity(this, this->dragon_flames_action, entity_objective));
	else if (random_number == 2)
		App->task_manager->AddTask(new PerformActionToEntity(this, this->lightning_flash_action, entity_objective));
}





