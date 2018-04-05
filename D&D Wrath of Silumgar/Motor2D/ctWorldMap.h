#ifndef __ctWorldMap_H__
#define __ctWorldMap_H__

#include "ctModule.h"
#include "Entity.h"

#include <string>
#include <vector>

class Entity;
class MiniHeroes;
class UIDecision;

class ctWorldMap : public ctModule
{

	struct WorldMapElement {
		uint tier = 0u;
		std::vector<Entity*> entities;
		std::string scene_name;
		SDL_Rect icon_rect;
		iPoint coords_in_map = { 0,0 };
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

	Entity* avatar;

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	void OnUITrigger(UIElement* elementTriggered, UI_State ui_state);



private:

	//used to load easily a rect from XML
	void LoadRect(pugi::xml_node rect_node, SDL_Rect* rect);

	void GenerateNewRandomlyMap();

	void NavigateDown(std::vector<UIElement*> &current_vector);

	void NavigateUp(std::vector<UIElement*> &current_vector);


private:

	std::vector<WorldMapElement*> all_map_elements;

	std::vector<WorldMapElement*> final_map_elements;

	std::string world_map_tmx;

	bool map_generated = false;

	std::string name_spritesheet_world_map;
	SDL_Texture* spritesheet_world_map = nullptr;
	
	//Decision
	UIDecision* decision = nullptr; 
	UIElement* arrow = nullptr;
	std::vector<UIElement*> options;
};


#endif // __ctWorldMap_H__