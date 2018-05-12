#ifndef _CTCUTSCENEMANAGER_H__
#define _CTCUTSCENEMANAGER_H__

#include "ctPoint.h"
#include <queue>
#include <list>
#include "ctModule.h"
#include "ctTimer.h"
#include "Entity.h"
#include "Lich2.h"
#include "SDL/include/SDL.h"

class Entity;

enum EntityType;

enum Cutscene_code
{
	FINAL_CUTSCENE
};


class CutsceneAction
{
public:
	CutsceneAction(uint32 start_time, uint32 end_time) {
		this->start_time = start_time;
		this->end_time = end_time;
	};
	virtual ~CutsceneAction() {};
	virtual bool Execute() { return true; };
	uint32 start_time;
	uint32 end_time;
protected:



};

class MoveX : public CutsceneAction
{
public:
	MoveX(uint32 start_time, uint32 end_time, Entity* actor, uint32 speed) : CutsceneAction(start_time, end_time) {
		this->actor = actor;
		speed = move_speed;
	};
	~MoveX() {};

	bool Execute();
private:
	Entity* actor = nullptr;
	int move_speed;
};

class MoveY : public CutsceneAction
{
public:
	MoveY(uint32 start_time, uint32 end_time, Entity* actor, uint32 speed) : CutsceneAction(start_time, end_time) {
		this->actor = actor;
		speed = move_speed;
	};
	~MoveY() {};

	bool Execute();
private:
	Entity* actor = nullptr;
	int move_speed;
};


class MoveCameraX : public CutsceneAction
{
public:
	MoveCameraX(uint32 start_time, uint32 end_time, uint32 speed) : CutsceneAction(start_time, end_time) {
		speed = move_speed;
	};
	~MoveCameraX() {};

	bool Execute();
private:
	int move_speed;
};

class MoveCameraY : public CutsceneAction
{
public:
	MoveCameraY(uint32 start_time, uint32 end_time, uint32 speed) : CutsceneAction(start_time, end_time) {
		speed = move_speed;
	};
	~MoveCameraY() {};

	bool Execute();
private:
	int move_speed;
};

class ExecuteLichAnimation : public CutsceneAction
{
public:
	ExecuteLichAnimation(uint32 start_time, uint32 end_time) : CutsceneAction(start_time, end_time) {
		//actor = App->entities->GetLich();
	};
	~ExecuteLichAnimation() {};

	bool Execute();
private:

	Lich2* actor = nullptr;
};

class CutsceneDialog : public CutsceneAction
{
public:
	CutsceneDialog(uint32 start_time, uint32 end_time, const char*text) : CutsceneAction(start_time, end_time) {};
	~CutsceneDialog();

	bool Execute();
private:
	UIElement* Text_UI = nullptr;
	UIElement* Background = nullptr;
	const char* text;
	bool first_iteration = true;
};

class ctCutsceneManager : public ctModule
{
public:
	ctCutsceneManager() {};
	~ctCutsceneManager() {};

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	bool ChargeCutscene(Cutscene_code cutscene);
	void ExecuteCutscene();
	ctTimer Cutscene_timer;
	std::list<CutsceneAction*> CutsceneActions;

	void StartCutscene();
	bool isActive();


private:

	bool Executing_Cutscene = false;

};




#endif