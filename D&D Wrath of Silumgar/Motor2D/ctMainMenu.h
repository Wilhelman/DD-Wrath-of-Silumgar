#ifndef __ctMAINMENU_H__
#define __ctMAINMENU_H__

#include "ctModule.h"
#include "ctGui.h"
#include "UIElement.h"
#include "Particle.h"
#include "ParticlePool.h"
#include "Emitter.h"

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

public:
	bool is_new_game = false;
	UIElement* pauseMenu = nullptr;

private:
	bool quit_pressed = false;

	UIElement* background = nullptr;
	UIElement* continue_label = nullptr;
	UIElement* new_game_label = nullptr;
	UIElement* settings_label = nullptr;
	UIElement* about_label = nullptr;
	UIElement* quit_label = nullptr;
	UIElement* arrow = nullptr;
	std::vector<UIElement*> labels;

	void NavigateDown(std::vector<UIElement*> &current_vector);
	void NavigateUp(std::vector<UIElement*> &current_vector);
	void ExecuteComand(std::vector<UIElement*> &current_vector);

	//test
	Emitter* eFire = nullptr;
	Emitter* eSmoke = nullptr;
	Emitter* eFlame = nullptr;
	Emitter* eWave_1 = nullptr;
	Emitter* eWave_2 = nullptr;
	Emitter* eBurst_1 = nullptr;
	Emitter* eBurst_2 = nullptr;
	Emitter* eBubbles = nullptr;
	Emitter* eSpark = nullptr;
	Emitter* ePixelSmoke = nullptr;
	SDL_Rect rect = { 0, 0, 400, 400 };
	SDL_Texture* torchTex = nullptr;
};


#endif // __ctMAINMENU_H__