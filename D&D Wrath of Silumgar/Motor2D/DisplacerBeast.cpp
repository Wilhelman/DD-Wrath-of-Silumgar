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

#include "DisplacerBeast.h"


DisplacerBeast::DisplacerBeast(int x, int y, EntityType type) : Entity(x, y, type) {

	bool ret = true;

	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadEntities(config_file);
	node = &node->child("enemies").child("displacerBeast");
	texture = App->tex->Load(App->entities->displacerBeast_spritesheet_name.data());


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
		else if (tmp == "stun")
			LoadAnimation(animations, &stun);
		else if (tmp == "dodge")
			LoadAnimation(animations, &dodge);
		else if (tmp == "infernal_fire")
			LoadAnimation(animations, &infernal_fire_animation);
		else if (tmp == "shadow_jab")
			LoadAnimation(animations, &shadow_jab_animation);

	}
	LoadProperties(node->child("statistics"));
	animation = &idle;

	//prepare the actions:

	rush.name = "Rush";
	rush.health_points_effect = -50;
	rush.mana_points_effect_to_himself = 70;
	rush.objective = HEROES;
	rush.type = RUSH;

	infernal_fire.name = "Infernal Fire";
	infernal_fire.health_points_effect = -25;
	infernal_fire.mana_points_effect_to_himself = 45;
	infernal_fire.objective = HEROES;
	infernal_fire.type = INFERNAL_FIRE;

	shadow_jab.name = "Shadow Jab";
	shadow_jab.health_points_effect = -25;
	shadow_jab.mana_points_effect_to_himself = 45;
	shadow_jab.objective = HEROES;
	shadow_jab.type = SHADOW_JAB;

	call_of_the_dead.name = "Call of the Dead";
	call_of_the_dead.mana_points_effect_to_himself = 45;
	call_of_the_dead.objective = ENEMIES;
	call_of_the_dead.type = CALL_OF_THE_DEAD;
}



// Called each loop iteration
void DisplacerBeast::Update(float dt)
{

	if (dt > 0)
	{
		if (!key_entities_speed)
			SetEntitiesSpeed(dt);

		SetPlayerAnimationsSpeed(dt);
	}

}

void DisplacerBeast::SetPlayerAnimationsSpeed(float dt)
{
	idle.speed = idle_vel * dt;
	run_forward.speed = run_forward_vel * dt;
}

void DisplacerBeast::SetEntitiesSpeed(float dt)
{
	idle_vel = idle.speed;
	run_forward_vel = run_forward.speed;

	key_entities_speed = true;
}

void DisplacerBeast::LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation)
{
	bool ret = true;

	for (pugi::xml_node frame = animation_node.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
}

void DisplacerBeast::Attack()
{
	App->audio->PlayFx(App->audio->displacer_beast_attack_fx, 0);

}

void  DisplacerBeast::Death() {
	App->audio->PlayFx(App->audio->displacer_beast_death_fx, 0);
}
void  DisplacerBeast::Run() {
	App->audio->PlayFx(App->audio->displacer_beast_run_fx, 0);
}
void  DisplacerBeast::Damaged() {
	App->audio->PlayFx(App->audio->displacer_beast_damaged_fx, 0);
}
void DisplacerBeast::Ability1T1() {
	App->audio->PlayFx(App->audio->displacer_beast_infernal_fire_fx, 0);
}
void DisplacerBeast::Ability2T1() {
	App->audio->PlayFx(App->audio->displacer_beast_shadow_jab_fx, 0);
}
void DisplacerBeast::Ability3T1() {
	App->audio->PlayFx(App->audio->displacer_beast_call_of_the_dead_fx, 0);
}


void DisplacerBeast::PerformAction()
{
	Entity* entity_objective = nullptr;

	entity_objective = App->combat->GetRandomHeroe();

	if (IsGoingToDoAnythingClever()) {//hacer algo cheto
		
			if (GetCurrentManaPoints() >= 70) {
				App->task_manager->AddTask(new MoveToEntity(this, entity_objective, 20));
				App->task_manager->AddTask(new PerformActionToEntity(this, shadow_jab, entity_objective));
				App->task_manager->AddTask(new MoveToInitialPosition(this));
			}

			else {
				App->task_manager->AddTask(new MoveToEntity(this, entity_objective, 20));
				App->task_manager->AddTask(new PerformActionToEntity(this, this->infernal_fire, entity_objective));
				App->task_manager->AddTask(new MoveToInitialPosition(this));
			}
		
	}
	else {//hacer algo mal
		

		App->task_manager->AddTask(new MoveToEntity(this, entity_objective, 20));
		App->task_manager->AddTask(new PerformActionToEntity(this, this->rush, entity_objective));
		App->task_manager->AddTask(new MoveToInitialPosition(this));
	}
	}
	





