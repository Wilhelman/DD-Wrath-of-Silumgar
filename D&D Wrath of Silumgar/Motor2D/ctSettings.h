#ifndef __ctSETTINGS_H__
#define __ctSETTINGS_H__

#include "ctModule.h"
#include "ctGui.h"
#include "UIElement.h"

struct UIBar;

class ctSettings : public ctModule
{
public:

	ctSettings();

	// Destructor
	virtual ~ctSettings();

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

	uint music_volume_value = 64;
	uint fx_volume_value = 64;

private:
	bool quit_pressed = false;
	

	UIElement* background = nullptr;
	UIElement* music_volume_label = nullptr;
	UIElement* fx_volume_label = nullptr;
	UIElement* back_label = nullptr;
	UIElement* music_volume = nullptr;
	UIElement* fx_volume = nullptr;
	UIElement* arrow = nullptr;
	std::vector<UIElement*> labels;
	uint max_volume = 128;

	void NavigateDown(std::vector<UIElement*> &current_vector);
	void NavigateUp(std::vector<UIElement*> &current_vector);
	void ExecuteComand(std::vector<UIElement*> &current_vector);
	void TurnUp(std::vector<UIElement*> &current_vector);
	void TurnDown(std::vector<UIElement*> &current_vector);
	uint NumberToPercentage(uint num, uint max_num);

	UIBar* HPbar = nullptr;
	UIBar* ManaBar = nullptr;
	UIBar* EnemyBar = nullptr;
};


#endif // __ctSETTINGS_H__