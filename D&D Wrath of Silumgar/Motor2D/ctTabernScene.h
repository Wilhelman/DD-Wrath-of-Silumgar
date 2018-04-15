#ifndef __ctTabernScene_H__
#define __ctTabernScene_H__

#include "ctModule.h"
#include "Entity.h"

#include <string>
#include <vector>

class Entity;
//class MiniHeroes;
class UIDecision;


class ctTabernScene : public ctModule
{



public:

	ctTabernScene();

	// Destructor
	virtual ~ctTabernScene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	//Entity* avatar;

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	void OnUITrigger(UIElement* elementTriggered, UI_State ui_state);



private:

	UIElement* Dialog_Box = nullptr;
	UIElement* Text_continue = nullptr;
	UIElement* Background = nullptr;
	uint times_x_pressed = 0;


};


#endif // __ctWorldMap_H__
