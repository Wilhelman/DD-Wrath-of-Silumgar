#include "ctTaskManager.h"
#include "ctApp.h"
#include "ctEntities.h"
#include "ctInput.h"
#include "ctCombat.h"


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
			}
			else {
				for (int i = 0; i < App->combat->heroes.size(); i++)
				{
					entity_to_go = App->combat->heroes.at(i);
					if (entity_to_go->GetCurrentHealthPoints() != 0)
						break;
				}
			}
		}


		int x_objective = (entity_to_go->position.x + offset);

		if (((entity_to_move->position.x + 25) >= x_objective && (entity_to_move->position.x - 25) <= x_objective) && ((entity_to_move->position.y + 25) >= entity_to_go->position.y && (entity_to_move->position.y - 25) <= entity_to_go->position.y))
		{
			entity_to_move->animation = &entity_to_move->idle;
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
				}
				else {
					for (int i = 0; i < App->combat->heroes.size(); i++)
					{
						receiver_entity = App->combat->heroes.at(i);
						if (receiver_entity->GetCurrentHealthPoints() != 0)
							break;
					}
				}
			}

			actioner_entity->animation = &actioner_entity->attack;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->attack.Reset();
				actioner_entity->animation = &actioner_entity->idle;

				//TODO WE'RE NOT CALCULATING THE ATTACK AGAINST THE DEFENSE! also the action should have type of attack: fisical or magical!!!
				receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + action_to_perform.health_points_effect);

				//todo animate the receiver to hit + audio or smth
				receiver_entity->animation = &receiver_entity->hit;

				App->combat->UpdateHPBarOfEntity(receiver_entity, action_to_perform.health_points_effect);
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
	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		if (aux_task == nullptr && TaskQueue.size() != 0)
		{
			aux_task = TaskQueue.front();
			TaskQueue.pop();
		}
	}

	ret = DoTask();

	return ret;
}

bool ctTaskManager::Start()
{
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