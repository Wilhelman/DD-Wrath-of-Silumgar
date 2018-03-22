#ifndef __ctMAINMENU_H__
#define __ctMAINMENU_H__

#include "ctModule.h"
#include "ctGui.h"
#include "UIElement.h"


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

	void OnUITrigger(UIElement* elementTriggered);

private:
	bool quit_pressed = false;
	

	UIElement* continue_label = nullptr;
	UIElement* new_game_label = nullptr;
	UIElement* settings_label = nullptr;
	UIElement* about_label = nullptr;
	UIElement* quit_label = nullptr;
	UIElement* arrow = nullptr;


};


#endif // __ctMAINMENU_H__