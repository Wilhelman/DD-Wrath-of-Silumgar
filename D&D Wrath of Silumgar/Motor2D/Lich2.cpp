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

#include "Lich2.h"


Lich2::Lich2(int x, int y, EntityType type) : Entity(x, y, type) {

	bool ret = true;

	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadEntities(config_file);
	node = &node->child("enemies").child("lich2nd");
	texture = App->tex->Load(App->entities->lich2_spritesheet_name.data());

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
		else if (tmp == "default_attack")
			LoadAnimation(animations, &attack);
		else if (tmp == "bidimensional_claw")
			LoadAnimation(animations, &bidimensional_claw);
		else if (tmp == "sea_of_flames")
			LoadAnimation(animations, &sea_of_flames);
		else if (tmp == "transformation")
			LoadAnimation(animations, &transformation);
	}
	LoadProperties(node->child("statistics"));
	animation = &idle;

	//prepare the actions:

	bidimensional_claw_action.name = "Bidimensional Claw";
	bidimensional_claw_action.mana_points_effect_to_himself = -35;
	bidimensional_claw_action.objective = HEROES;
	bidimensional_claw_action.type = BIDIMENSIONAL_CLAW;

	sea_of_flames_action.name = "Sea of Flames";
	sea_of_flames_action.mana_points_effect_to_himself = -40;
	sea_of_flames_action.objective = HEROES;
	sea_of_flames_action.type = SEA_OF_FLAMES;

}



// Called each loop iteration
void Lich2::Update(float dt)
{

	if (dt > 0)
	{
		if (!key_entities_speed)
			SetEntitiesSpeed(dt);

		SetPlayerAnimationsSpeed(dt);
	}

}

void Lich2::SetPlayerAnimationsSpeed(float dt)
{
	float percentage_hp = ((current_health_points * 100) / max_health_points);
	idle_animation_damaged_speed = (((100 - percentage_hp) * IDLE_SPEED_DAMAGED) / 100) + 1;
	idle.speed = idle_vel * dt * idle_animation_damaged_speed;
	run_forward.speed = run_forward_vel * dt;
}

void Lich2::SetEntitiesSpeed(float dt)
{
	idle_vel = idle.speed;
	run_forward_vel = run_forward.speed;

	key_entities_speed = true;
}

void Lich2::LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation)
{
	bool ret = true;

	for (pugi::xml_node frame = animation_node.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
}

void Lich2::Attack()
{
	//App->audio->PlayFx(App->audio->dark_warrior_attack_fx, 0);

}

void  Lich2::Death() {
	App->audio->PlayFx(App->audio->lich2_attack_fx, 0);
}
void  Lich2::Run() {
	App->audio->PlayFx(App->audio->lich2_run_fx, 0);
}
void  Lich2::Damaged() {
	App->audio->PlayFx(App->audio->lich2_damaged_fx, 0);
}
void Lich2::Ability1T1() {
	App->audio->PlayFx(App->audio->lich2_bidimensional_claw_fx, 0);
}
void Lich2::Ability2T1() {
	App->audio->PlayFx(App->audio->lich2_sea_of_flames_fx, 0);
}
void Lich2::Ability3T1() {
	//App->audio->PlayFx(App->audio->dark_warrior_insignificant_mortals_fx, 0);
}


void Lich2::PerformAction()
{
	Entity* entity_objective = nullptr;

	if (IsGoingToDoAnythingClever()) {//hacer algo cheto

		
		if (is_taunted) {
			entity_objective = (Entity*)App->entities->GetWarrior();
			is_taunted = false;
		}

		else {
			entity_objective = App->combat->GetTheWeakestHeroe();
		}

			if (GetCurrentManaPoints() >= 70) {
				App->task_manager->AddTask(new MoveToEntity(this, entity_objective, 20));
				App->task_manager->AddTask(new PerformActionToEntity(this, sea_of_flames_action, entity_objective));
				App->task_manager->AddTask(new MoveToInitialPosition(this));
			}
			else if (GetCurrentManaPoints() > 70 && GetCurrentManaPoints() >= 45) {

				App->task_manager->AddTask(new MoveToEntity(this, entity_objective, 20));
				App->task_manager->AddTask(new PerformActionToEntity(this, bidimensional_claw_action, entity_objective));
				App->task_manager->AddTask(new MoveToInitialPosition(this));
			}

			else {
				App->task_manager->AddTask(new MoveToEntity(this, entity_objective, 20));
				App->task_manager->AddTask(new PerformActionToEntity(this, this->default_attack, entity_objective));
				App->task_manager->AddTask(new MoveToInitialPosition(this));
			}
		}
	else {//hacer algo mal
		if (is_taunted) {
			entity_objective = (Entity*)App->entities->GetWarrior();
			is_taunted = false;
		}

		else {
			entity_objective = App->combat->GetRandomHeroe();
		}

		App->task_manager->AddTask(new MoveToEntity(this, entity_objective, 20));
		App->task_manager->AddTask(new PerformActionToEntity(this, this->default_attack, entity_objective));
		App->task_manager->AddTask(new MoveToInitialPosition(this));
	}

	
}


