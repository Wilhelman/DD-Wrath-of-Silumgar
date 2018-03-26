#ifndef __ctWorldMap_H__
#define __ctWorldMap_H__

#include "ctModule.h"

#include <string>
#include <vector>

class Entity;

class ctWorldMap : public ctModule
{

	struct WorldMapElement {
		uint tier = 0u;
		std::vector<Entity*> entities;
		std::string scene_name;
	};

public:

	ctWorldMap();

	// Destructor
	virtual ~ctWorldMap();

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

	//used to load easily a rect from XML
	void LoadRect(pugi::xml_node rect_node, SDL_Rect* rect);

private:

	std::vector<WorldMapElement*> map_elements;

	std::string world_map_tmx;

};


#endif // __ctWorldMap_H__