#ifndef __ctAUDIO_H__
#define __ctAUDIO_H__

#include "ctModule.h"
#include "SDL_mixer\include\SDL_mixer.h"


#define DEFAULT_MUSIC_FADE_TIME 2.0f
#define MAX_FX 200

struct _Mix_Music;
struct Mix_Chunk;

class ctAudio : public ctModule
{
public:

	ctAudio();

	// Destructor
	virtual ~ctAudio();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);
	bool StopMusic();

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

	// UnLoad WAV
	bool UnLoadFx(uint id);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:

	_Mix_Music*			music = nullptr;
	Mix_Chunk*			fx[MAX_FX];

	uint				last_fx = 1;
};

#endif // __ctAUDIO_H__