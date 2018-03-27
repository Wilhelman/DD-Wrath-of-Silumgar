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
	virtual bool Execute(Entity* actor) { return true; };

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

	Entity* Player = nullptr;
	Entity* Enemy = nullptr;
	Entity* Actor = nullptr;

	

private:
	//TODO 1 Create a queue of tasks
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
	Move() {};
	virtual  ~Move() {};

	bool Execute(Entity* actor);
};

class Attack : public Task
{
public:
	Attack() {};
	virtual  ~Attack() {};

	bool Execute(Entity* actor);
};

class Back : public Task
{
public:
	Back() {};
	virtual  ~Back() {};

	bool Execute(Entity* actor);
};


#endif // !_TASKMANAGER_H__
