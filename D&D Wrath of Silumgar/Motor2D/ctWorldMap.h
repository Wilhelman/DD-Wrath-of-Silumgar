#ifndef __ctWorldMap_H__
#define __ctWorldMap_H__

#include "ctModule.h"

#include <string>

class ctWorldMap : public ctModule
{
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

	SDL_Rect background = { 0,0,0,0 };

	std::string spritesheet_name;
	SDL_Texture* spritesheet = nullptr;

};


#endif // __ctWorldMap_H__