#include "ctTaskManager.h"
#include "ctApp.h"
#include "ctEntities.h"
#include "ctInput.h"



bool Move::Execute(Entity* actor)
{
	bool ret = false;
	if (actor->type == WARRIOR) {

		if (actor->position.x == App->task_manager->posaux.x - 500)
		{
			//actor->animation = &actor->Idle_anim;
			App->task_manager->posaux.x = actor->position.x;

			ret = true;
		}
		else
		{
			//actor->animation = &actor->Move_anim;
			actor->position.x -= 5;
		}
	}

	else if (actor->type == CLERIC) {

		if (actor->position.x == App->task_manager->posaux.x + 500)
		{
			//actor->animation = &actor->Idle_anim;
			App->task_manager->posaux.x = actor->position.x;

			ret = true;
		}
		else
		{
			//actor->animation = &actor->Move_anim;
			actor->position.x += 5;
		}
	}
	return ret;
}


bool Back::Execute(Entity* actor)
{
	bool ret = false;
	if (actor->type == CLERIC) {

		if (actor->position.x == App->task_manager->posaux.x - 500)
		{
			//actor->animation = &actor->Idle_anim;
			App->task_manager->posaux.x = actor->position.x;

			ret = true;
		}
		else
		{
			//actor->animation = &actor->Move_anim;
			actor->position.x -= 5;
		}
	}

	else if (actor->type == WARRIOR) {

		if (actor->position.x == App->task_manager->posaux.x + 500)
		{
			//actor->animation = &actor->Idle_anim;
			App->task_manager->posaux.x = actor->position.x;

			ret = true;
		}
		else
		{
			//actor->animation = &actor->Move_anim;
			actor->position.x -= 5;
		}
	}
	return ret;
}

bool Attack::Execute(Entity* actor)
{
/*	bool ret = false;

	actor->animation = &actor->Attack_anim;

	ret = actor->animation->Finished();

	if (ret == true) {
		actor->Attack_anim.Reset();
		actor->animation = &actor->Idle_anim;
	}
	*/
	return true;
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
	buttonQ = new Move;
	buttonW = new Move;
	buttonA = new Attack;
	buttonS = new Attack;
	buttonZ = new Back;
	buttonX = new Back;
	return true;
}

bool ctTaskManager::CleanUp()
{
	while (TaskQueue.size() != 0)
	{
		TaskQueue.pop();
	}

	delete buttonQ;
	delete buttonW;

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
		if (aux_task->Execute(Actor))
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
	//TODO 4 Add the task to the queque 
	if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		Actor = Player;
		posaux = Actor->position;
		AddTask(buttonQ);
	}
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		Actor = Enemy;
		posaux = Actor->position;
		AddTask(buttonW);

	}

	
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
	{
		Actor = Player;
		posaux = Actor->position;
		AddTask(buttonA);
	}
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
	{
		Actor = Enemy;
		posaux = Actor->position;
		AddTask(buttonS);

	}


	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		Actor = Player;
		posaux = Actor->position;
		AddTask(buttonZ);
	}
	if (App->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		Actor = Enemy;
		posaux = Actor->position;
		AddTask(buttonX);

	}

	return true;
}