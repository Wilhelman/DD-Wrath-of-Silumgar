#ifndef __ctAbilitiesMap_H__
#define __ctAbilitiesMap_H__

#include "ctModule.h"

#include <string>
#include <vector>

class Entity;

class ctAbiltiesMap : public ctModule
{

public:

	ctAbiltiesMap();

	// Destructor
	virtual ~ctAbiltiesMap();

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

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	void OnUITrigger(UIElement* elementTriggered, UI_State ui_state);

private:
	std::string abilities_map_tmx;
	std::string name_spritesheet_abilities_map;

	SDL_Texture* spritesheet_abilities_map = nullptr;

private:

	
	

};


#endif // __ctWorldMap_H__