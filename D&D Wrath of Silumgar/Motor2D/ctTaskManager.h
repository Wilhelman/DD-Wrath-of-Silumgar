#ifndef _CTTASKMANAGER_H__
#define _CTTASKMANAGER_H__

#include "ctPoint.h"
#include <queue>
#include "ctModule.h"
#include "Entity.h"
#include "SDL/include/SDL.h"

#define BASE_SPEED 3
#define BASE_DEXTERITY 75
#define BASE_AGILITY 0
#define CRITICAL_VALUE 2

class Entity;


class Task
{
public:
	Task() {};
	virtual ~Task() {};
	virtual bool Execute() { return true; };

	Entity* performed_by = nullptr;

private:

};

class ctTaskManager : public ctModule
{
public:
	ctTaskManager() {};
	~ctTaskManager() {};

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void PerformAllTheTasks();

	void OrderTasksByEntities(std::vector<Entity*> turn_priority_entity);

	void DeleteTasksFromEntity(Entity* entity);

	bool TaskOrderer();
	bool AddTask(Task* task);
	bool DoTask();

	

private:

	Task*		buttonQ = nullptr;
	Task*		buttonA = nullptr;
	Task*		buttonZ = nullptr;

	Task*		buttonW = nullptr;
	Task*		buttonS = nullptr;
	Task*		buttonX = nullptr;



public:
	std::list<Task*> TaskQueue;

	Task * aux_task = nullptr;
	iPoint posaux = { 0,0 };

};

class MoveAvatarsToPosition : public Task
{
public:
	MoveAvatarsToPosition(Entity* entity_to_move,iPoint position_to_go) {
		this->entity_to_move = entity_to_move;
		this->position_to_go = position_to_go;
	};
	~MoveAvatarsToPosition() {};

	bool Execute();
private:
	Entity* entity_to_move = nullptr;
	iPoint position_to_go = { 0,0 };
};


class MoveToEntity : public Task
{
public:
	MoveToEntity(Entity* entity_to_move, Entity* entity_to_go, int offset) {
		this->entity_to_move = entity_to_move;
		this->entity_to_go = entity_to_go;
		this->offset = offset;
		performed_by = entity_to_move;
	};
	~MoveToEntity() {};

	bool Execute();
private:
	Entity* entity_to_move = nullptr;
	Entity* entity_to_go = nullptr;
	int offset = false;
};

class MoveToInitialPosition : public Task
{
public:
	MoveToInitialPosition(Entity* entity_to_move) {
		this->entity_to_move = entity_to_move;
		performed_by = entity_to_move;
	};
	~MoveToInitialPosition() {};

	bool Execute();
private:
	Entity* entity_to_move = nullptr;
};

class PerformActionToEntity : public Task
{
public:
	PerformActionToEntity(Entity* actioner_entity, Action action_to_perform, Entity* receiver_entity) {
		this->actioner_entity = actioner_entity;
		this->action_to_perform = action_to_perform;
		this->receiver_entity = receiver_entity;
		performed_by = actioner_entity;
	};
	virtual  ~PerformActionToEntity() {};

	bool Execute();
	bool HaveObjective();
	bool HaveTeamObjective();

private:
	Entity* actioner_entity = nullptr;
	Action action_to_perform;
	Entity* receiver_entity = nullptr;
};

class MoveBackward : public Task
{
public:
public:
	MoveBackward(Entity* actor, iPoint finalpos) {
		this->actor = actor;
		this->finalpos = finalpos;
	};
	~MoveBackward() {};

	bool Execute();

private:
	Entity* actor = nullptr;
	iPoint finalpos;
};


#endif // !_TASKMANAGER_H__
