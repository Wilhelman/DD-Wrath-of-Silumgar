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

	uint pressed_fx = 0u;

	int alpha = 255;


private:
	bool debug_draw = false;
	UIElement* parent = nullptr;
public:

	bool to_destroy = false;

	//Esto se podría poner en privado y poner una función que te diga el tipo con tal de que no se pueda trastear desde fuera.
	UI_Type type = UI_NOT_DEF;
	UI_State current_state = STATE_NO_DEF;
	//----------------------------------------------

	//Se puede poner en protected tocando los otros modulos sin hacer new.
	//Y en privado si lo pintamos todo en el mismo blit
	//Screen_position con local position se puede poner unicamente 1 de las dos variables, es innecesario 2.
	iPoint screen_position = iPoint(0, 0);
	iPoint local_position = iPoint(0, 0);

	//-----------------------------------------
	SDL_Texture* texture = nullptr;
	SDL_Rect current_rect = { 0,0,0,0 };
	bool non_drawable = false;
	//--------------------------------------------------
	//--------------------------------------------------
public:
	UIElement(int x, int y, UI_Type type = UI_NOT_DEF, UIElement* parent = nullptr);
	virtual ~UIElement();

	virtual void Update();
	virtual void Draw(SDL_Texture* sprites);
	//Una de las dos la podemos borrar
	iPoint GetLocalPosition() const;
	iPoint GetScreenPosition() const;
	//-------------------------------
	SDL_Rect GetRect() const;

	void SetParent(UIElement* parent);
	UIElement* GetParent() const;
};

#endif // __ENTITY_H__