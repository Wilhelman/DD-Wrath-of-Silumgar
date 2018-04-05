#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctWindow.h"
#include "ctRender.h"
#include "ctEntities.h"
#include "ctInput.h"
#include "j1Map.h"

#define VSYNC true

ctRender::ctRender() : ctModule()
{ 
	name = "renderer";
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
ctRender::~ctRender()
{}

// Called before render is available
bool ctRender::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	vsync_state = false;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		vsync_state = true;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if (renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = App->win->screen_surface->w;
		camera.h = App->win->screen_surface->h;
		camera.x = 0;
		camera.y = 0;
	}
	scale_factor = App->win->GetScale();
	return ret;
}

// Called before the first frame
bool ctRender::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	
	return true;
}

// Called each loop iteration
bool ctRender::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool ctRender::Update(float dt)
{

	uint winWidth, winHeight;

	App->win->GetWindowSize(winWidth, winHeight);

	
	int speed = 3;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		camera.y += speed;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		camera.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		camera.x += speed;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		camera.x -= speed;

	//LOG("Camera pos x: %i pos y: %i", camera.x, camera.y);

	return true;
}

bool ctRender::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool ctRender::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool ctRender::Load(pugi::xml_node& data)
{
	return true;
}

// Save Game State
bool ctRender::Save(pugi::xml_node& data) const
{

	return true;
}

void ctRender::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void ctRender::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void ctRender::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool ctRender::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, SDL_RendererFlip flip, int pivot_x, int pivot_y) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_Rect rect;

	if (section != NULL)
	{
		rect.w = section->w*scale_factor;
		rect.h = section->h*scale_factor;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.x = (int)((camera.x * speed) + (x  * scale_factor));
	rect.y = (int)((camera.y * speed) + (y  * scale_factor));


	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flip) != 0)
	{		
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ctRender::MapBlit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;

	uint scale = App->win->GetScale();
	SDL_Rect rect;

	if (section != NULL)
	{
		rect.w = section->w*scale_factor;
		rect.h = section->h*scale_factor;
	}

	rect.x = (int)((camera.x * speed) + ((x / section->w) * rect.w));
	rect.y = (int)((camera.y * speed) + ((y / section->h) * rect.h));



	if (section == nullptr)
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ctRender::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if (use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ctRender::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();
	scale = 1;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if (use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool ctRender::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for (uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if (result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

iPoint ctRender::ScreenToWorld(int x, int y) const
{
	iPoint ret;
	int scale = App->win->GetScale();

	ret.x = (x - camera.x / scale);
	ret.y = (y - camera.y / scale);

	return ret;
}

bool ctRender::Is_inScreen(int x, int y) {
	bool ret = false;
	if (x >= -App->render->camera.x-800 && x + App->map->data.tile_width < -App->render->camera.x + App->render->camera.w+800)
		if (y >= -App->render->camera.y-800 && y + App->map->data.tile_height < -App->render->camera.y + App->render->camera.h+800)
			ret = true;

	return ret;
}