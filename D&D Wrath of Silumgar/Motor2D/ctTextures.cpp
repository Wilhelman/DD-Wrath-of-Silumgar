#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctRender.h"
#include "ctTextures.h"

#include "SDL_image/include/SDL_image.h"
#pragma comment( lib, "SDL_image/libx86/SDL2_image.lib" )

ctTextures::ctTextures() : ctModule()
{
	name = "textures";
}

// Destructor
ctTextures::~ctTextures()
{}

// Called before render is available
bool ctTextures::Awake(pugi::xml_node& config)
{
	LOG("Init Image library");
	bool ret = true;
	// load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool ctTextures::Start()
{
	LOG("start textures");
	bool ret = true;
	return ret;
}

// Called before quitting
bool ctTextures::CleanUp()
{
	LOG("Freeing textures and Image library");
	
	for(std::list<SDL_Texture*>::iterator it= textures.begin(); it != textures.end(); it++ )
	{
		SDL_DestroyTexture(*it);
	}

	textures.clear();
	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const ctTextures::Load(const char* path)
{
	SDL_Texture* texture = nullptr;
	SDL_Surface* surface = IMG_Load(path);

	if (surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return texture;
}

// Unload texture
bool ctTextures::UnLoad(SDL_Texture* texture)
{
	
	for (std::list<SDL_Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
	{
		if (texture == *it)
		{
			SDL_DestroyTexture(texture);
			textures.remove(texture);
			return true;
		}
	}

	return false;
}

// Translate a surface into a texture
SDL_Texture* const ctTextures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(App->render->renderer, surface);

	if (texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		textures.push_back(texture);
	}

	return texture;
}

// Retrieve size of a texture
void ctTextures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*)&width, (int*)&height);
}
