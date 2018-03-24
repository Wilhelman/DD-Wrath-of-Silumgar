#include "ctDefs.h"
#include "ctLog.h"
#include "ctAudio.h"

#include "ctInput.h"
#include "ctApp.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer\include\SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ctAudio::ctAudio() : ctModule()
{
	music = NULL;
	name = "audio";
}

// Destructor
ctAudio::~ctAudio()
{}

bool ctAudio::Load(pugi::xml_node& save) {
	bool ret = true;

	Mix_VolumeMusic(save.child("volume").attribute("music").as_int(50));

	return ret;
}

bool ctAudio::Save(pugi::xml_node& save)const {
	bool ret = true;

	if (save.child("volume").empty()) {
		save = save.append_child("volume");
		save.append_attribute("music").set_value(Mix_VolumeMusic(-1));
	}
	else {
		save.child("volume").attribute("music").set_value(Mix_VolumeMusic(-1));
	}

	return ret;
}

// Called before render is available
bool ctAudio::Awake(pugi::xml_node& config)
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if ((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}

	Mix_VolumeMusic(20);

	return ret;
}

bool ctAudio::Update(float dt)
{
	if (!active)
		return true;

	if (App->input->GetKey(SDL_SCANCODE_KP_PLUS) == KEY_DOWN) {
		Mix_VolumeMusic(Mix_VolumeMusic(-1) + 10);
	}

	if (App->input->GetKey(SDL_SCANCODE_KP_MINUS) == KEY_DOWN) {
		if (Mix_VolumeMusic(-1) < 10)
			Mix_VolumeMusic(0);
		else
			Mix_VolumeMusic(Mix_VolumeMusic(-1) - 10);
	}

	return true;
}

// Called before quitting
bool ctAudio::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing music, closing Mixer and Audio subsystem");

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
	return true;
}

// Play a music file
bool ctAudio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if (!active)
		return false;

	if (music != NULL)
	{
		if (fade_time > 0.0f)
		{
			Mix_FadeOutMusic(int(fade_time * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if (music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if (fade_time > 0.0f)
		{
			if (Mix_FadeInMusic(music, -1, (int)(fade_time * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if (Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

bool ctAudio::StopMusic() {
	Mix_FreeMusic(music);
	music = nullptr;
	Mix_HaltMusic();
	return true;
}

// Load WAV
unsigned int ctAudio::LoadFx(const char* path)
{
	if (!active)
		return 0;

	uint ret = 0;
	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if (chunk == nullptr)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx[last_fx] = chunk;
		ret = last_fx++;
		if (last_fx == MAX_FX) {
			last_fx = 0;
			ret = last_fx;
		}
	}

	return ret;
}

// Play WAV
bool ctAudio::PlayFx(unsigned int id, int repeat)
{

	if (!active)
		return false;

	bool ret = false;

	if (fx[id] != nullptr)
	{
		Mix_PlayChannel(-1, fx[id], repeat);
		ret = true;
	}

	return ret;
}

// UnLoad WAV
bool ctAudio::UnLoadFx(uint id)
{
	if (!active)
		return true;

	bool ret = false;

	if (fx[id] != nullptr)
	{
		Mix_FreeChunk(fx[id]);
		fx[id] = nullptr;
		ret = true;
		//last_fx--;
	}

	return ret;
}