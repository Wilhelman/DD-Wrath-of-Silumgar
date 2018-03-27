#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctTextures.h"
#include "ctFonts.h"

#include "SDL\include\SDL.h"
#include "SDL_TTF\include\SDL_ttf.h"
#pragma comment( lib, "SDL_ttf/libx86/SDL2_ttf.lib" )

ctFonts::ctFonts() : ctModule()
{
	//name.create("fonts");
}

// Destructor
ctFonts::~ctFonts()
{}

// Called before render is available
bool ctFonts::Awake(pugi::xml_node& conf)
{
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}
	else
	{
		path = conf.child("default_font").attribute("file").as_string(DEFAULT_FONT);
		size = conf.child("default_font").attribute("size").as_int(DEFAULT_FONT_SIZE);
		default = Load(path, size);
	}

	return ret;
}

// Called before quitting
bool ctFonts::CleanUp()
{
	LOG("Freeing True Type fonts and library");
	/*TTF_Font* item;

	for (item = fonts.front(); item != NULL; item = std::next(item,1))
	{
	TTF_CloseFont(item);
	}*/

	fonts.clear();
	TTF_Quit();
	return true;
}

// Load new texture from file path
TTF_Font* const ctFonts::Load(const char* path, int size)
{
	TTF_Font* font = TTF_OpenFont(path, size);

	if (font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
	}
	else
	{
		LOG("Successfully loaded font %s size %d", path, size);
		fonts.push_front(font);
	}

	return font;
}

// Print text using font
SDL_Texture* ctFonts::Print(const char* text, SDL_Color color, TTF_Font* font)
{
	SDL_Texture* ret = NULL;
	SDL_Surface* surface = TTF_RenderText_Blended((font) ? font : default, text, color);

	if (surface == NULL)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = App->tex->LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return ret;
}

// calculate size of a text
bool ctFonts::CalcSize(const char* text, int& width, int& height, _TTF_Font* font) const
{
	bool ret = false;

	if (TTF_SizeText((font) ? font : default, text, &width, &height) != 0)
		LOG("Unable to calc size of text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
		ret = true;

	return ret;
}


// prints text that will have multiple lines if the width of the text is larger than box_width
SDL_Texture* ctFonts::PrintTextBox(const char* text, SDL_Color color, _TTF_Font* font, Uint32 box_width, int size)
{

	SDL_Texture* ret = NULL;
	default = Load(path, size);

	SDL_Surface* surface = TTF_RenderText_Blended_Wrapped((font) ? font : default, text, color, box_width);
	SDL_SetSurfaceAlphaMod(surface, color.a);

	if (surface == NULL)
	{
		LOG("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		ret = App->tex->LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return ret;


}