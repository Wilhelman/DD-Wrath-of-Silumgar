#include "ctTaskManager.h"
#include "ctApp.h"
#include "ctEntities.h"
#include "ctInput.h"



bool MoveForward::Execute()
{
	bool ret = false;

		if (actor->position.x == finalpos.x && actor->position.y == finalpos.y)
		{
			actor->animation = &actor->idle;
			ret = true;
		}
		else {
			if (actor->position.x < finalpos.x)
			{
				actor->animation = &actor->run_forward;
				actor->position.x += 5;
			}

			else if (actor->position.x > finalpos.x)
			{
				actor->animation = &actor->run_forward;
				actor->position.x -= 5;
			}

			if (actor->position.y < finalpos.y)
			{
				actor->animation = &actor->run_forward;
				actor->position.y += 5;
			}

			else if (actor->position.y > finalpos.y)
			{
				actor->animation = &actor->run_forward;
				actor->position.y -= 5;
			}
		}

	return ret;
}

bool MoveBackward::Execute()
{
	bool ret = false;


	if (actor->flip_texture != true) {
		actor->flip_texture = true;
	}

	if (actor->position.x == finalpos.x && actor->position.y == finalpos.y)
	{
		actor->animation = &actor->idle;
		actor->flip_texture = false;
		ret = true;
	}
	else {
		if (actor->position.x < finalpos.x)
		{
			actor->animation = &actor->run_forward;
			actor->position.x += 5;
		}

		else if (actor->position.x > finalpos.x)
		{
			actor->animation = &actor->run_forward;
			actor->position.x -= 5;
		}

		if (actor->position.y < finalpos.y)
		{
			actor->animation = &actor->run_forward;
			actor->position.y += 5;
		}

		else if (actor->position.y > finalpos.y)
		{
			actor->animation = &actor->run_forward;
			actor->position.y -= 5;
		}
	}

	return ret;
}



bool Attack::Execute()
{
	bool ret = false;

	actor->animation = &actor->attack;

	ret = actor->animation->Finished();

	if (ret == true) {
		actor->attack.Reset();
		actor->animation = &actor->idle;
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