#ifndef __ctINPUT_H__
#define __ctINPUT_H__

#include "ctModule.h"

#include "SDL/include/SDL_gamecontroller.h"

//#define NUM_KEYS 352
#define NUM_MOUSE_BUTTONS 5
//#define LAST_KEYS_PRESSED_BUFFER 50

struct SDL_Rect;

enum ctEventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum ctKeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

enum GAMEPAD_STATE
{
	PAD_BUTTON_IDLE = 0,
	PAD_BUTTON_DOWN,
	PAD_BUTTON_REPEAT,
	PAD_BUTTON_KEY_UP
};

struct Gamepad {

	GAMEPAD_STATE A = PAD_BUTTON_IDLE;
	GAMEPAD_STATE B;
	GAMEPAD_STATE Y;
	GAMEPAD_STATE X;

	GAMEPAD_STATE CROSS_UP;
	GAMEPAD_STATE CROSS_DOWN;
	GAMEPAD_STATE CROSS_LEFT;
	GAMEPAD_STATE CROSS_RIGHT;

};

class ctInput : public ctModule
{

public:

	ctInput();

	// Destructor
	virtual ~ctInput();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	// Called before quitting
	bool CleanUp();

	// Gather relevant win events
	bool GetWindowEvent(ctEventWindow ev);

	// Check key states (includes mouse and joy buttons)
	ctKeyState GetKey(int id) const
	{
		return keyboard[id];
	}

	ctKeyState GetMouseButtonDown(int id) const
	{
		return mouse_buttons[id - 1];
	}

	// Get mouse / axis position
	void GetMousePosition(int &x, int &y);
	void GetWorldMousePosition(int &x, int &y);
	void GetMouseMotion(int& x, int& y);

	Gamepad gamepad;
	void buttonForGamepad();

private:
	bool		windowEvents[WE_COUNT];
	ctKeyState*	keyboard = nullptr;
	ctKeyState	mouse_buttons[NUM_MOUSE_BUTTONS];
	int			mouse_motion_x = 0;
	int			mouse_motion_y = 0;
	int			mouse_x = 0;
	int			mouse_y = 0;

	SDL_GameController *controller;

};

#endif // __ctINPUT_H__