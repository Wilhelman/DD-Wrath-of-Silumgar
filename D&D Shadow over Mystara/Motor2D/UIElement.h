#ifndef __UIElement_H__
#define __UIElement_H__

#include "ctPoint.h"
#include "ctGui.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
struct SDL_Rect;

enum UI_State {
	STATE_NORMAL,
	STATE_MOUSE_ENTER,
	STATE_MOUSE_LEAVE,
	STATE_LEFT_MOUSE_PRESSED,
	STATE_LEFT_MOUSE_RELEASED,
	STATE_RIGHT_MOUSE_PRESSED,
	STATE_RIGHT_MOUSE_RELEASED,
	STATE_FOCUSED,
	
	STATE_NO_DEF
};

class UIElement
{
protected:
	ctModule* callback = nullptr;
	SDL_Rect current_rect = { 0,0,0,0 };
	iPoint local_position = iPoint(0,0);
	iPoint screen_position = iPoint(0, 0);
	uint mouse_in_fx = 0u;
	int mouse_motion_x = 0, mouse_motion_y = 0;
	int tmp_mouse_x, tmp_mouse_y;
	uint pressed_fx = 0u;
	SDL_Texture* texture = nullptr;

private:
	bool debug_draw = false;

public:
	bool draggable = false;
	bool to_destroy = false;
	UIElement* parent = nullptr;
	UI_Type type = UI_NOT_DEF;
	UI_State current_state = STATE_NO_DEF;

public:
	UIElement(int x, int y, UI_Type type = UI_NOT_DEF, UIElement* parent = nullptr);
	virtual ~UIElement();

	virtual void Update();
	virtual void Draw(SDL_Texture* sprites);

	iPoint GetLocalPosition() const;
	iPoint GetScreenPosition() const;
	SDL_Rect GetRect() const;

	void SetParent(UIElement* parent);
	UIElement* GetParent() const;
};

#endif // __ENTITY_H__