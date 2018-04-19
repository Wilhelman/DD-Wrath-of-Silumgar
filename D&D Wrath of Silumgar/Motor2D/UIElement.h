#ifndef __UIElement_H__
#define __UIElement_H__

#include "ctPoint.h"
#include "ctGui.h"
#include "ctRender.h"
#include "ctInput.h"
#include "ctApp.h"
#include "ctTextures.h"

#include "SDL/include/SDL.h"

struct SDL_Rect;

enum UI_State {
	STATE_NORMAL,
	STATE_FOCUSED,
	STATE_EXECUTED,
	
	STATE_NO_DEF
};

class UIElement
{
protected:
	ctModule* callback = nullptr;
	iPoint local_position = iPoint(0,0);

	uint pressed_fx = 0u;

	int alpha = 255;
	
	
private:
	bool debug_draw = false;
	UIElement* parent = nullptr;
public:
	
	bool non_drawable = false;
	bool to_destroy = false;
	UI_Type type = UI_NOT_DEF;
	UI_State current_state = STATE_NO_DEF;

	//Se puede poner en protected tocando los otros modulos sin hacer new.
	//Y en privado si lo pintamos todo en el mismo blit
	iPoint screen_position = iPoint(0, 0);
	SDL_Texture* texture = nullptr;
	SDL_Rect current_rect = { 0,0,0,0 };
	//--------------------------------------------------
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