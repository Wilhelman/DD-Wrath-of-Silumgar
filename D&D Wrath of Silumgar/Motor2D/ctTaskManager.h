#ifndef _CTTASKMANAGER_H__
#define _CTTASKMANAGER_H__

#include "ctPoint.h"
#include <queue>
#include "ctModule.h"
#include "Entity.h"
#include "SDL/include/SDL.h"

class Entity;

class Task
{
public:
	Task() {};
	virtual ~Task() {};
	virtual bool Execute() { return true; };

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

	bool TaskOrderer();
	bool AddTask(Task* task);
	bool DoTask();

	

private:

	std::queue<Task*> TaskQueue;
	Task*		buttonQ = nullptr;
	Task*		buttonA = nullptr;
	Task*		buttonZ = nullptr;

	Task*		buttonW = nullptr;
	Task*		buttonS = nullptr;
	Task*		buttonX = nullptr;



public:
	Task * aux_task = nullptr;
	iPoint posaux = { 0,0 };

};


class Move : public Task
{
public:
	Move(Entity* entity_to_move, Entity* entity_to_go) {
		this->entity_to_move = entity_to_move;
		this->entity_to_go = entity_to_go;
	};
	~Move() {};

	bool Execute();
private:
	Entity* entity_to_move = nullptr;
	Entity* entity_to_go = nullptr;
};


class MoveForward : public Task
{
public:
	MoveForward(Entity* actor, iPoint finalpos) {
		this->actor = actor;
		this->finalpos = finalpos;
	};
	~MoveForward() {};

	bool Execute();
private:
	Entity* actor = nullptr;
	iPoint finalpos;
};

class Attack : public Task
{
public:
	Attack(Entity* actor) {
		this->actor = actor;
	};
	virtual  ~Attack() {};

	bool Execute();

private:
	Entity* actor = nullptr;
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
