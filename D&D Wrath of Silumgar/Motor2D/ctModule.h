// ----------------------------------------------------
// j1Module.h
// Interface for all engine modules
// ----------------------------------------------------

#ifndef __ctMODULE_H__
#define __ctMODULE_H__

#include <string>
#include "PugiXml\src\pugixml.hpp"

class ctApp;
struct Collider;
class UIElement;
enum UI_State;
enum ComboType;

class ctModule
{
public:

	ctModule() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	virtual bool Save(pugi::xml_node&) const
	{
		return true;
	}

	// Callbacks ---
	virtual void OnCollision(Collider*, Collider*) {}

	virtual void OnUITrigger(UIElement* elementTriggered, UI_State ui_state) {}

public:

	std::string		name;
	bool		active = false;

};

#endif // __ctMODULE_H__