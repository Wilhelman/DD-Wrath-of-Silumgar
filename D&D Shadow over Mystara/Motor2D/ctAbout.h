#ifndef __ctABOUT_H__
#define __ctABOUT_H__

#include "ctModule.h"
#include "ctGui.h"


class ctAbout : public ctModule
{
public:

	ctAbout();

	// Destructor
	virtual ~ctAbout();

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
	bool first_update = true;
	int arrow_x= 115;
	int arrow_y= 60;

	UIElement* back = NULL;
	UIElement* arrow = NULL;




};


#endif // __ctABOUT_H__