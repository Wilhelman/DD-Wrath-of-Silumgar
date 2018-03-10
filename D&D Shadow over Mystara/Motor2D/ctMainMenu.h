#ifndef __ctMAINMENU_H__
#define __ctMAINMENU_H__

#include "ctModule.h"
#include "ctGui.h"



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
	

	UIElement* continue_label;
	UIElement* new_game_label;
	UIElement* settings_label;
	UIElement* about_label;
	UIElement* quit_label;
	UIElement* arrow;

	int arrow_pos_x = 5;
	int arrow_pos_y = 10;

};


#endif // __ctMAINMENU_H__