#include "ctTaskManager.h"
#include "ctApp.h"
#include "ctEntities.h"
#include "ctInput.h"
#include "ctCombat.h"

//randomize libs
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


bool MoveToEntity::Execute()
{
	bool ret = false;

	if (entity_to_move->GetCurrentHealthPoints() != 0) {
		if (entity_to_go->GetCurrentHealthPoints() == 0) {
			if (entity_to_move->type == ELF || entity_to_move->type == CLERIC || entity_to_move->type == WARRIOR || entity_to_move->type == DWARF) {
				for (int i = 0; i < App->combat->enemies.size(); i++)
				{
					entity_to_go = App->combat->enemies.at(i);
					if (entity_to_go->GetCurrentHealthPoints() != 0)
						break;
				}
				if(entity_to_go->GetCurrentHealthPoints() == 0)
					return true;
			}
			else {
				for (int i = 0; i < App->combat->heroes.size(); i++)
				{
					entity_to_go = App->combat->heroes.at(i);
					if (entity_to_go->GetCurrentHealthPoints() != 0)
						break;
				}
				if (entity_to_go->GetCurrentHealthPoints() == 0)
					return true;
			}
		}


		int x_objective = (entity_to_go->position.x + offset);

		if (((entity_to_move->position.x + 25) >= x_objective && (entity_to_move->position.x - 25) <= x_objective) && ((entity_to_move->position.y + 25) >= entity_to_go->position.y && (entity_to_move->position.y - 25) <= entity_to_go->position.y))
		{
			//entity_to_move->animation = &entity_to_move->idle;
			ret = true;
		}
		else {
			if (entity_to_move->position.x < entity_to_go->position.x)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.x += BASE_SPEED;
			}

			else if (entity_to_move->position.x > entity_to_go->position.x)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.x -= BASE_SPEED;
			}

			if (entity_to_move->position.y < entity_to_go->position.y)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.y += BASE_SPEED;
			}

			else if (entity_to_move->position.y > entity_to_go->position.y)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.y -= BASE_SPEED;
			}
		}
	}
	else
		ret = true;

	

	return ret;
}

bool MoveToInitialPosition::Execute()
{
	bool ret = false;

	//offset?

	if (entity_to_move->GetCurrentHealthPoints() != 0) {
		if (entity_to_move->flip_texture != true) {
			entity_to_move->flip_texture = true;
		}

		if (entity_to_move->position.x == entity_to_move->initial_position.x && entity_to_move->position.y == entity_to_move->initial_position.y)
		{
			entity_to_move->animation = &entity_to_move->idle;
			entity_to_move->flip_texture = false;
			ret = true;
		}
		else {
			if (entity_to_move->position.x < entity_to_move->initial_position.x)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.x += BASE_SPEED;
			}

			else if (entity_to_move->position.x > entity_to_move->initial_position.x)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.x -= BASE_SPEED;
			}

			if (entity_to_move->position.y < entity_to_move->initial_position.y)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.y += BASE_SPEED;
			}

			else if (entity_to_move->position.y > entity_to_move->initial_position.y)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.y -= BASE_SPEED;
			}
		}
	}
	else
		ret = true;
	

	return ret;
}



bool PerformActionToEntity::Execute()
{

	bool ret = false;

	if (actioner_entity->GetCurrentHealthPoints() != 0) {
		switch (action_to_perform.type)
		{
		case DEFAULT_ATTACK: {

			if (receiver_entity->GetCurrentHealthPoints() == 0) {
				if (actioner_entity->type == ELF || actioner_entity->type == CLERIC || actioner_entity->type == WARRIOR || actioner_entity->type == DWARF) {
					for (int i = 0; i < App->combat->enemies.size(); i++)
					{
						receiver_entity = App->combat->enemies.at(i);
						if (receiver_entity->GetCurrentHealthPoints() != 0)
							break;
					}
					if (receiver_entity->GetCurrentHealthPoints() == 0)
						return true;
				}
				else {
					for (int i = 0; i < App->combat->heroes.size(); i++)
					{
						receiver_entity = App->combat->heroes.at(i);
						if (receiver_entity->GetCurrentHealthPoints() != 0)
							break;
					}
					if (receiver_entity->GetCurrentHealthPoints() == 0)
						return true;
				}
			}

			actioner_entity->animation = &actioner_entity->attack;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->Attack();
				actioner_entity->attack.Reset();
				//actioner_entity->animation = &actioner_entity->idle;

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 0,255,0,255 }, 14, nullptr, nullptr);
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal =  action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;
						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if(!critical)
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
						else
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);

						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,255,0,255 }, 14, nullptr, nullptr);
				}
				//todo animate the receiver to hit + audio or smth
				
				
			}
		}
		break;
		case KICK: {

			if (receiver_entity->GetCurrentHealthPoints() == 0) {
				if (actioner_entity->type == ELF || actioner_entity->type == CLERIC || actioner_entity->type == WARRIOR || actioner_entity->type == DWARF) {
					for (int i = 0; i < App->combat->enemies.size(); i++)
					{
						receiver_entity = App->combat->enemies.at(i);
						if (receiver_entity->GetCurrentHealthPoints() != 0)
							break;
					}
					if (receiver_entity->GetCurrentHealthPoints() == 0)
						return true;
				}
				else {
					for (int i = 0; i < App->combat->heroes.size(); i++)
					{
						receiver_entity = App->combat->heroes.at(i);
						if (receiver_entity->GetCurrentHealthPoints() != 0)
							break;
					}
					if (receiver_entity->GetCurrentHealthPoints() == 0)
						return true;
				}
			}

			actioner_entity->animation = &actioner_entity->kick;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				//actioner_entity->Attack();  TODO DO THE KICK
				actioner_entity->kick.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 0,255,0,255 }, 14, nullptr, nullptr);
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;
						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical)
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
						else
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);

						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,255,0,255 }, 14, nullptr, nullptr);
				}
				//todo animate the receiver to hit + audio or smth


			}
		}
							 break;
		default:
			break;
		}
	}
	else
		ret = true;

	return ret;
}

bool ctTaskManager::Update(float dt)
{
	TaskOrderer();

	bool ret = false;

	ret = DoTask();

	return ret;
}

bool ctTaskManager::Start()
{
	/* initialize random seed: */
	srand(time(NULL));
	return true;
}

bool ctTaskManager::CleanUp()
{
	while (TaskQueue.size() != 0)
	{
		TaskQueue.pop();
	}
	return true;
}

void ctTaskManager::PerformAllTheTasks()
{
	if (aux_task == nullptr && TaskQueue.size() != 0)
	{
		aux_task = TaskQueue.front();
		TaskQueue.pop();
	}
}

bool ctTaskManager::AddTask(Task * task)
{
	//TODO 3 Add the new task to the list
	TaskQueue.push(task);

	return true;
}

bool ctTaskManager::DoTask()
{
	if (aux_task != nullptr)
	{

		if (aux_task->Execute())
		{
			if (TaskQueue.size() != 0)
			{
				aux_task = TaskQueue.front();
				TaskQueue.pop();
			}
			else 
				aux_task = nullptr;

		}
	}
	return true;
}

bool ctTaskManager::TaskOrderer()
{
	
	return true;
}