#ifndef __ctAbout_H__
#define __ctAbout_H__

#include "ctModule.h"
#include "ctGui.h"
#include "UIElement.h"
#include <string>

#define BUTTON_HOVER_OFFSET 6
#define BUTTON_PUSH_OFFSET 3

class UIButton;
class UILabel;
class UIImage;
class UISlider;
class UICheckBox;
class UITextBox;

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
	

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	//void OnUITrigger(UIElement* elementTriggered, UI_State ui_state);

public:

private:
	int scroll_speed = 1;

	int y_delay =400;
	
	//std::vector<UIElement*> labels;
	//std::vector<UIElement*> buttons;

	UILabel* hold_lbl = nullptr;
	UITextBox* licence_lbl = nullptr;
	UILabel* esc_lbl = nullptr;
	UILabel* about_lbl = nullptr;
	//UIButton *git_ggs_btn = nullptr;
	//UIButton *git_vmg_btn = nullptr;

	bool credits_over = false;
};


#endif 