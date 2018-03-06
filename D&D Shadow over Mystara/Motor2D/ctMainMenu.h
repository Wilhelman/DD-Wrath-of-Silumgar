#ifndef __ctMAINMENU_H__
#define __ctMAINMENU_H__

#include "ctModule.h"



class ctMainMenu : public ctModule
{
public:

	ctMainMenu();

	// Destructor
	virtual ~ctMainMenu();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	void OnUITrigger(UIElement* elementTriggered, UI_State ui_state);

private:
	bool quit_pressed = false;

};


#endif // __ctMAINMENU_H__