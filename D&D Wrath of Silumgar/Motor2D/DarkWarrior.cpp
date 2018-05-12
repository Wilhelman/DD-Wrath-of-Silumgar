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

#include "DarkWarrior.h"


DarkWarrior::DarkWarrior(int x, int y, EntityType type) : Entity(x, y, type) {

	bool ret = true;

	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadEntities(config_file);
	node = &node->child("enemies").child("darkWarrior");
	texture = App->tex->Load(App->entities->dark_warrior_spritesheet_name.data());


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
		else if (tmp == "void_cannon")
			LoadAnimation(animations, &void_cannon);
		else if (tmp == "thunder_punch")
			LoadAnimation(animations, &thunder_punch);
		else if (tmp == "insignificant_mortals")
			LoadAnimation(animations, &insignificant_mortals);
	}
	LoadProperties(node->child("statistics"));
	animation = &idle;

	//prepare the actions:

	void_cannon_action.name = "Void Cannon";
	void_cannon_action.health_points_effect = -40;
	void_cannon_action.mana_points_effect_to_himself = 70;
	void_cannon_action.objective = HEROES;
	void_cannon_action.type = VOID_CANNON;

	thunder_punch_action.name = "Thunder Punch";
	thunder_punch_action.health_points_effect = -25;
	thunder_punch_action.mana_points_effect_to_himself = 45;
	thunder_punch_action.objective = HEROES;
	thunder_punch_action.type = THUNDER_PUNCH;

	insignificant_mortals_action.name = "Insignificant Mortals";
	insignificant_mortals_action.mana_points_effect_to_himself = 45;
	insignificant_mortals_action.objective = ENEMIES;
	insignificant_mortals_action.type = INSIGNIFICANT_MORTALS;
}



// Called each loop iteration
void DarkWarrior::Update(float dt)
{

	if (dt > 0)
	{
		if (!key_entities_speed)
			SetEntitiesSpeed(dt);

		SetPlayerAnimationsSpeed(dt);
	}

}

void DarkWarrior::SetPlayerAnimationsSpeed(float dt)
{
	idle.speed = idle_vel * dt;
	run_forward.speed = run_forward_vel * dt;
}

void DarkWarrior::SetEntitiesSpeed(float dt)
{
	idle_vel = idle.speed;
	run_forward_vel = run_forward.speed;

	key_entities_speed = true;
}

void DarkWarrior::LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation)
{
	bool ret = true;

	for (pugi::xml_node frame = animation_node.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
}

void DarkWarrior::Attack()
{
	App->audio->PlayFx(App->audio->dark_warrior_attack_fx, 0);

}

void  DarkWarrior::Death() {
	App->audio->PlayFx(App->audio->dark_warrior_death_fx, 0);
}
void  DarkWarrior::Run() {
	App->audio->PlayFx(App->audio->dark_warrior_run_fx, 0);
}
void  DarkWarrior::Damaged() {
	App->audio->PlayFx(App->audio->dark_warrior_damaged_fx, 0);
}
void DarkWarrior::Ability1T1() {
	App->audio->PlayFx(App->audio->dark_warrior_thunder_punch_fx, 0);
}
void DarkWarrior::Ability2T1() {
	App->audio->PlayFx(App->audio->dark_warrior_void_cannon_fx, 0);
}
void DarkWarrior::Ability3T1() {
	App->audio->PlayFx(App->audio->dark_warrior_insignificant_mortals_fx, 0);
}


void DarkWarrior::PerformAction()
{
	Entity* entity_objective = nullptr;

	if (IsGoingToDoAnythingClever()) {//hacer algo cheto
		if (!boosted) {
			
			App->task_manager->AddTask(new PerformActionToEntity(this, insignificant_mortals_action, App->combat->GetRandomHeroe()));
			boosted = true;
		
		}

		else {
			entity_objective = App->combat->GetTheWeakestHeroe();

			if (GetCurrentManaPoints() >= 70) {
				App->task_manager->AddTask(new PerformActionToEntity(this, void_cannon_action, entity_objective));
			}

			else if (GetCurrentManaPoints() > 70 && GetCurrentManaPoints() >= 45) {

				App->task_manager->AddTask(new MoveToEntity(this, entity_objective, 20));
				App->task_manager->AddTask(new PerformActionToEntity(this, thunder_punch_action, entity_objective));
				App->task_manager->AddTask(new MoveToInitialPosition(this));
			}

			else {
				App->task_manager->AddTask(new MoveToEntity(this, entity_objective, 20));
				App->task_manager->AddTask(new PerformActionToEntity(this, this->default_attack, entity_objective));
				App->task_manager->AddTask(new MoveToInitialPosition(this));
			}
		}
	}
	else {//hacer algo mal
		entity_objective = App->combat->GetRandomHeroe();

		App->task_manager->AddTask(new MoveToEntity(this, entity_objective, 20));
		App->task_manager->AddTask(new PerformActionToEntity(this, this->default_attack, entity_objective));
		App->task_manager->AddTask(new MoveToInitialPosition(this));
	}
	

}


