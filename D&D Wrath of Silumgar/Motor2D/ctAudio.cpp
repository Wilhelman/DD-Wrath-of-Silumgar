#include "ctDefs.h"
#include "ctLog.h"
#include "ctAudio.h"
#include <list>
#include "ctApp.h"

#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"
#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

ctAudio::ctAudio() : ctModule()
{
	music = NULL;
	name = "audio";
}

// Destructor
ctAudio::~ctAudio()
{}

// Called before render is available
bool ctAudio::Awake(pugi::xml_node& audioNode)
{
	LOG("Loading Audio Mixer");

	SDL_Init(0);

	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	if (SDL_GetNumAudioDevices(0) > 0)
		device_connected = true;


	if (device_connected)
	{
		InitAudio();
	}

	return true;
}


void ctAudio::InitAudio()
{
	//Initialize SDL_mixer
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}

	Mix_AllocateChannels(16);

	//---------------------------------------Load fx-------------------------------
	mm_movement_fx = LoadFx("audio/sounds/UI and Menus/MenuMove.wav");
	mm_select_fx = LoadFx("audio/sounds/UI and Menus/MenuSelect.wav");
	wm_walk_fx = LoadFx("audio/sounds/Others/WorldMapWalk.wav");
	cm_move_fx = App->audio->LoadFx("audio/sounds/UI and Menus/CombatMenuMove.wav");
	cm_select_fx = App->audio->LoadFx("audio/sounds/UI and Menus/CombatMenuSelect.wav");
	cm_back_fx = App->audio->LoadFx("audio/sounds/UI and Menus/CombatMenuBack.wav");
	kobold_attack_fx = LoadFx("audio/sounds/Enemies/KoboldAttack.wav");
	kobold_run_fx = LoadFx("audio/sounds/Enemies/KoboldRun.wav");
	kobold_damaged_fx = LoadFx("audio/sounds/Enemies/KoboldDamaged.wav");
	kobold_death_fx = LoadFx("audio/sounds/Enemies/KoboldDeath.wav");
	gnoll_attack_fx = LoadFx("audio/sounds/Enemies/GnollAttack.wav");
	gnoll_run_fx = LoadFx("audio/sounds/Enemies/GnollRun.wav");
	gnoll_damaged_fx = LoadFx("audio/sounds/Enemies/GnollDamaged.wav");
	gnoll_death_fx = LoadFx("audio/sounds/Enemies/GnollDeath.wav");
	cleric_attack_fx = LoadFx("audio/sounds/Characters/ClericAttack.wav");
	cleric_run_fx = LoadFx("audio/sounds/Characters/ClericRun.wav");
	cleric_damaged_fx = LoadFx("audio/sounds/Characters/ClericDamaged.wav");
	cleric_death_fx = LoadFx("audio/sounds/Characters/ClericDeath.wav");
	cleric_heal_fx = LoadFx("audio/sounds/Characters/ClericHeal.wav");
	dwarf_attack_fx = LoadFx("audio/sounds/Characters/DwarfAttack.wav");
	dwarf_run_fx = LoadFx("audio/sounds/Characters/DwarfRun.wav");
	dwarf_damaged_fx = LoadFx("audio/sounds/Characters/DwarfDamaged.wav");
	dwarf_death_fx = LoadFx("audio/sounds/Characters/DwarfDeath.wav");
	dwarf_double_axe_fx = LoadFx("audio/sounds/Characters/DwarfDoubleAxe.wav");
	elf_attack_fx = LoadFx("audio/sounds/Characters/ElfAttack.wav");
	elf_run_fx = LoadFx("audio/sounds/Characters/ElfRun.wav");
	elf_damaged_fx = LoadFx("audio/sounds/Characters/ElfDamaged.wav");
	elf_death_fx = LoadFx("audio/sounds/Characters/ElfDeath.wav");
	elf_mindblown_fx = LoadFx("audio/sounds/Characters/ElfFireBall.wav");
	warrior_attack_fx = LoadFx("audio/sounds/Characters/WarriorAttack.wav");
	warrior_run_fx = LoadFx("audio/sounds/Characters/WarriorRun.wav");
	warrior_damaged_fx = LoadFx("audio/sounds/Characters/WarriorDamaged.wav");
	warrior_death_fx = LoadFx("audio/sounds/Characters/WarriorDeath.wav");
	warrior_kick_fx = LoadFx("audio/sounds/Characters/WarriorKick.wav");

	//---------------------------------------Load Music----------------------------
	
	MainMenuBSO = "audio/music/D&D Shadow Over Mystara - Song 00 Fanfare.ogg";
	MainMenuVoice = "audio/music/D&D Shadow Over Mystara - Song 02  Dungeons & Dragons.ogg";
	SettingsBSO = "audio/music/D&D Shadow Over Mystara - Song 21 What Sleeps in The Rotting Sea (Stage 5-B).ogg";
	CutsceneBSO = "audio/music/D&D Shadow Over Mystara - Song 04 Prologue (Stage 1).ogg";
	WorldMapBSO = "audio/music/D_D-Shadow-Over-Mystara-Song-39-Our-Divine-Protection-First-Child-Amidst-Death.ogg";
	CombatBSO = "audio/music/D&D Shadow Over Mystara - Song 07 Battle at Trintan Village (Stage 2).ogg";
	WinBSO = "audio/music/D_D-Shadow-Over-Mystara-Song-38-Epiloge-_Stage-10_.ogg";
	LoseBSO = "audio/music/D_D-Shadow-Over-Mystara-Song-39-Our-Divine-Protection-First-Child-Amidst-Death.ogg";

	//---------------------------------------SetVolumes----------------------------
	Mix_Volume(-1, (MIX_MAX_VOLUME * FXVolumePercent) / 100);
	Mix_VolumeMusic((MIX_MAX_VOLUME * MusicVolumePercent) / 100);
}


bool ctAudio::Update(float dt)
{
	if (device_connected == false && SDL_GetNumAudioDevices(0) > 0)
	{
		device_connected = true;
		InitAudio();
	}

	return true;
}



// Called before quitting
bool ctAudio::CleanUp()
{
	if (!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if (music != NULL)
	{
		Mix_FreeMusic(music);
	}

	for (uint i = 0; i < fx.size(); ++i)
		Mix_FreeChunk(fx[i]);
	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool ctAudio::PlayMusic(const char* path, float fade_time)
{
	bool ret = true;

	if (!device_connected)
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

bool ctAudio::PauseMusic(float fade_time)
{
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
	}
	return Mix_PlayingMusic() == 0;
}

bool ctAudio::HaltFX(int id, int fadeseconds)
{
	if (!device_connected)
		return false;

	if (id == -1)
	{
		Mix_HaltChannel(-1);
	}
	else
		for (int i = 0; i < 16; ++i)
		{
			if (Mix_GetChunk(i) == fx[id - 1])
				Mix_HaltChannel(i);
		}

	return true;
}

bool ctAudio::PauseFX(int id)
{
	if (!device_connected)
		return false;

	if (id == -1)
		Mix_Pause(-1);
	else
		for (int i = 0; i < 16; ++i)
		{
			if (Mix_GetChunk(i) == fx[id - 1])
				Mix_Pause(i);
			break;
		}
	return true;
}

bool ctAudio::ResumeFX(int id)
{
	if (!device_connected)
		return false;

	if (id == -1)
	{
		Mix_Resume(-1);
	}
	else
	{
		for (int i = 0; i < 16; ++i)
		{
			if (Mix_GetChunk(i) == fx[id - 1])
				Mix_Resume(i);
			break;
		}
	}
}

// Load WAV
unsigned int ctAudio::LoadFx(const char* path)
{
	unsigned int ret = 0;

	if (!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path, 1);

	if (chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		fx.push_back(chunk);
		ret = fx.size();
	}

	return ret;
}

// Play a previously loaded WAV
//Return the channel used or -1 if errors happened
int ctAudio::PlayFx(unsigned int id, int repeat, int channel)
{
	if (!device_connected)
		return -1;

	if (id > 0 && id <= fx.size())
	{
		return Mix_PlayChannel(channel, fx[id - 1], repeat);
	}

	return -1;
}

void ctAudio::setMusicVolume(uint percent)
{
	if (!device_connected)
		return;

	MusicVolumePercent = percent;
	Mix_VolumeMusic((MIX_MAX_VOLUME * MusicVolumePercent) / 100);
}

void ctAudio::setFXVolume(uint percent)
{
	if (!device_connected)
		return;

	FXVolumePercent = percent;
	Mix_Volume(-1, (MIX_MAX_VOLUME * FXVolumePercent) / 100);
}