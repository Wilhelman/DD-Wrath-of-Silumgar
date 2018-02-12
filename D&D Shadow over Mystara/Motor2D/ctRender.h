#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include "SDL/include/SDL.h"
#include "ctPoint.h"
#include "ctModule.h"

class ctRender : public ctModule
{
public:

	ctRender();

	// Destructor
	virtual ~ctRender();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Blit
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	//Utils
	iPoint ScreenToWorld(int x, int y) const;

public:

	SDL_Renderer*	renderer = nullptr;
	SDL_Rect		camera = { 0, 0, 0, 0 };
	SDL_Rect		last_camera = { 0, 0, 0, 0 };
	SDL_Rect		viewport = { 0, 0, 0, 0 };
	SDL_Color		background = { 0, 0, 0, 0 };
	bool			vsync_state = false;
};

#endif // __j1RENDER_H__