#ifndef __AUDIO_H__
#define __AUDIO_H__

#include "SDL_mixer\include\SDL_mixer.h"
#include <vector>
#include "ctApp.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

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


	bool Update(float dt);


	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);

	//Pause a playing music
	bool PauseMusic(float fade_time = DEFAULT_MUSIC_FADE_TIME);

	//Stop a FX
	bool HaltFX(int id = -1, int fadeseconds = 0);

	//Pause a FX
	bool PauseFX(int id = -1);

	//Resume a previously paused FX
	bool ResumeFX(int id = -1);

	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	//Return the channel used or -1 if errors happened
	int PlayFx(unsigned int fx, int repeat = 0, int channel = -1);

	//Set Volumes
	void setMusicVolume(uint percent);
	void setFXVolume(uint percent);

	//InitAudio

	void InitAudio();

public:

	//fx sounds
	uint mm_movement_fx = 0u;
	uint mm_select_fx = 0u;
	uint wm_walk_fx = 0u;
	uint cm_move_fx = 0u;
	uint cm_select_fx = 0u;
	uint cm_back_fx = 0u;
	//fx enemies
	uint kobold_attack_fx = 0u;
	uint kobold_run_fx = 0u;
	uint kobold_damaged_fx = 0u;
	uint kobold_death_fx = 0u;

	uint gnoll_attack_fx = 0u;
	uint gnoll_run_fx = 0u;
	uint gnoll_damaged_fx = 0u;
	uint gnoll_death_fx = 0u;
	uint gnoll_archer_attack_fx = 0u;

	uint goblin_run_fx = 0u;
	uint goblin_attack_fx = 0u;
	uint goblin_damaged_fx = 0u;
	uint goblin_death_fx = 0u;

	uint triton_run_fx = 0u;
	uint triton_attack_fx = 0u;
	uint triton_damaged_fx = 0u;
	uint triton_death_fx = 0u;

	uint hellhound_run_fx = 0u;
	uint hellhound_attack_fx = 0u;
	uint hellhound_damaged_fx = 0u;
	uint hellhound_death_fx = 0u;

	uint owlbear_run_fx = 0u;
	uint owlbear_attack_fx = 0u;
	uint owlbear_damaged_fx = 0u;
	uint owlbear_death_fx = 0u;

	uint skeleton_run_fx = 0u;
	uint skeleton_attack_fx = 0u;
	uint skeleton_damaged_fx = 0u;
	uint skeleton_death_fx = 0u;

	//fx heroes
	uint cleric_attack_fx = 0u;
	uint cleric_run_fx = 0u;
	uint cleric_damaged_fx = 0u;
	uint cleric_death_fx = 0u;
	uint cleric_heal_fx = 0u;

	uint dwarf_attack_fx = 0u;
	uint dwarf_run_fx = 0u;
	uint dwarf_damaged_fx = 0u;
	uint dwarf_death_fx = 0u;
	uint dwarf_double_axe_fx = 0u;

	uint elf_attack_fx = 0u;
	uint elf_run_fx = 0u;
	uint elf_damaged_fx = 0u;
	uint elf_death_fx = 0u;
	uint elf_mindblown_fx = 0u;

	uint warrior_attack_fx = 0u;
	uint warrior_run_fx = 0u;
	uint warrior_damaged_fx = 0u;
	uint warrior_death_fx = 0u;
	uint warrior_kick_fx = 0u;

	//music sounds
	std::string MainMenuBSO;
	std::string SettingsBSO;
	std::string MainMenuVoice;
	std::string CutsceneBSO;
	std::string WorldMapBSO;
	std::string CombatBSO;
	std::string WinBSO;
	std::string LoseBSO;

	//volumes
	uint MusicVolumePercent = 10;
	uint FXVolumePercent = 10;

private:

	_Mix_Music*			music;
	std::vector <Mix_Chunk*> fx;

	bool device_connected = false;

};

#endif // __AUDIO_H__
