#include "ctTaskManager.h"
#include "ctApp.h"
#include "ctEntities.h"
#include "ctInput.h"
#include "ctCombat.h"


bool MoveToEntity::Execute()
{
	bool ret = false;


	int x_objective = (entity_to_go->position.x + offset);

	if (((entity_to_move->position.x + 25) >= x_objective && (entity_to_move->position.x - 25) <= x_objective ) && ((entity_to_move->position.y + 25) >= entity_to_go->position.y && (entity_to_move->position.y - 25) <= entity_to_go->position.y))
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

	return ret;
}

bool MoveToInitialPosition::Execute()
{
	bool ret = false;

	//offset?

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

	return ret;
}



bool PerformActionToEntity::Execute()
{

	bool ret = false;

	switch (action_to_perform.type)
	{
	case DEFAULT_ATTACK: {

		actioner_entity->animation = &actioner_entity->attack;

		ret = actioner_entity->animation->Finished();

		if (ret == true) {
			actioner_entity->attack.Reset();
			actioner_entity->animation = &actioner_entity->idle;

			//TODO WE'RE NOT CALCULATING THE ATTACK AGAINST THE DEFENSE! also the action should have type of attack: fisical or magical!!!
			receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + action_to_perform.health_points_effect);

			//todo animate the receiver to hit + audio or smth

			App->combat->UpdateHPBarOfEntity(receiver_entity, action_to_perform.health_points_effect);
		}
	}
		break;
	default:
		break;
	}

	
	
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
		//TODO 3 If the task is finished, pop the next task until the queue is empty
		if (aux_task->Execute())
		{
			if (TaskQueue.size() != 0)
			{
				aux_task = TaskQueue.front();
				TaskQueue.pop();
			}
			else aux_task = nullptr;

		}
	}
	return true;
}

bool ctTaskManager::TaskOrderer()
{
	
	return true;
}