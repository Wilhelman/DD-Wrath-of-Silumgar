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
	//enemies
	kobold_attack_fx = LoadFx("audio/sounds/Enemies/KoboldAttack.wav");
	kobold_run_fx = LoadFx("audio/sounds/Enemies/KoboldRun.wav");
	kobold_damaged_fx = LoadFx("audio/sounds/Enemies/KoboldDamaged.wav");
	kobold_death_fx = LoadFx("audio/sounds/Enemies/KoboldDeath.wav");
	gnoll_attack_fx = LoadFx("audio/sounds/Enemies/GnollAttack.wav");
	gnoll_run_fx = LoadFx("audio/sounds/Enemies/GnollRun.wav");
	gnoll_damaged_fx = LoadFx("audio/sounds/Enemies/GnollDamaged.wav");
	gnoll_death_fx = LoadFx("audio/sounds/Enemies/GnollDeath.wav");
	gnoll_archer_attack_fx = LoadFx("audio/sounds/Enemies/GnollArcherAttack.wav");
	goblin_attack_fx = LoadFx("audio/sounds/Enemies/GoblinAttack.wav");
	goblin_death_fx = LoadFx("audio/sounds/Enemies/GoblinDeath.wav");
	goblin_damaged_fx = LoadFx("audio/sounds/Enemies/GoblinDamaged.wav");
	goblin_run_fx = kobold_run_fx;
	triton_attack_fx = LoadFx("audio/sounds/Enemies/TritonAttack.wav");
	triton_death_fx = LoadFx("audio/sounds/Enemies/TritonDeath.wav");
	triton_damaged_fx = LoadFx("audio/sounds/Enemies/TritonDamaged.wav");
	triton_run_fx = gnoll_run_fx;
	hellhound_attack_fx = LoadFx("audio/sounds/Enemies/HellhoundAttack.wav");
	hellhound_death_fx = LoadFx("audio/sounds/Enemies/HellhoundDeath.wav");
	hellhound_damaged_fx = LoadFx("audio/sounds/Enemies/HellhoundDamaged.wav");
	hellhound_run_fx = gnoll_run_fx;
	owlbear_attack_fx = LoadFx("audio/sounds/Enemies/OwlbearAttack.wav");
	owlbear_death_fx = LoadFx("audio/sounds/Enemies/OwlbearDeath.wav");
	owlbear_damaged_fx = LoadFx("audio/sounds/Enemies/OwlbearDamaged.wav");
	owlbear_run_fx = gnoll_run_fx;
	skeleton_attack_fx = LoadFx("audio/sounds/Enemies/SkeletonAttack.wav");
	skeleton_death_fx = LoadFx("audio/sounds/Enemies/SkeletonDeath.wav");
	skeleton_damaged_fx = LoadFx("audio/sounds/Enemies/SkeletonDamaged.wav");
	skeleton_run_fx = kobold_run_fx;
	dark_warrior_attack_fx = LoadFx("audio/sounds/Enemies/DarkWarriorAttack.wav");
	dark_warrior_death_fx = LoadFx("audio/sounds/Enemies/DarkWarriorDeath.wav");
	dark_warrior_damaged_fx = LoadFx("audio/sounds/Enemies/DarkWarriorDamaged.wav");
	dark_warrior_run_fx = gnoll_run_fx;
	dark_warrior_insignificant_mortals_fx= LoadFx("audio/sounds/Enemies/DarkWarriorInsignificantMortals.wav");
	dark_warrior_thunder_punch_fx= LoadFx("audio/sounds/Enemies/DarkWarriorThunderPunch.wav");
	dark_warrior_void_cannon_fx= LoadFx("audio/sounds/Enemies/DarkWarriorVoidCannon.wav");

	troll_leaders_attack_fx = LoadFx("audio/sounds/Enemies/TrollLeadersAttack.wav");
	troll_leaders_run_fx = gnoll_run_fx;
	troll_leaders_damaged_fx = LoadFx("audio/sounds/Enemies/TrollLeadersDamaged.wav");
	troll_leaders_death_fx = LoadFx("audio/sounds/Enemies/TrollLeadersDeath.wav");
	troll_leaders_block_fx = LoadFx("audio/sounds/Enemies/TrollLeadersBlock.wav");
	troll_leaders_infested_claw_fx = LoadFx("audio/sounds/Enemies/TrollLeadersInfestedClaw.wav");
	troll_leaders_brotherly_rage_fx = LoadFx("audio/sounds/Enemies/TrollLeadersBrotherlyRage.wav");


	displacer_beast_attack_fx = LoadFx("audio/sounds/Enemies/DisplacerBeastAttack.wav");
	displacer_beast_run_fx = kobold_run_fx;
	displacer_beast_damaged_fx = LoadFx("audio/sounds/Enemies/DisplacerBeastDamaged.wav");
	displacer_beast_death_fx = LoadFx("audio/sounds/Enemies/DisplacerBeastDeath.wav");
	displacer_beast_infernal_fire_fx = LoadFx("audio/sounds/Enemies/DisplacerBeastInfernalFire.wav");
	displacer_beast_shadow_jab_fx = LoadFx("audio/sounds/Enemies/DisplacerBeastShadowJab.wav");
	displacer_beast_call_of_the_dead_fx = LoadFx("audio/sounds/Enemies/DisplacerBeastCallOfTheDead.wav");

	lich1_attack_fx = LoadFx("audio/sounds/Enemies/Lich1Attack.wav");
	lich1_run_fx = kobold_run_fx;
	lich1_damaged_fx = LoadFx("audio/sounds/Enemies/Lich1Damaged.wav");
	lich1_death_fx = LoadFx("audio/sounds/Enemies/Lich1Death.wav");
	lich1_dragon_flames_fx = LoadFx("audio/sounds/Enemies/Lich1DragonFlames.wav");
	lich1_lightning_flash_fx = LoadFx("audio/sounds/Enemies/Lich1LightningFlash.wav");
	lich1_abyssal_sphere_fx = LoadFx("audio/sounds/Enemies/Lich1AbyssalSphere.wav");

	lich2_attack_fx = LoadFx("audio/sounds/Enemies/Lich2Attack.wav");
	lich2_run_fx = gnoll_run_fx;
	lich2_damaged_fx = LoadFx("audio/sounds/Enemies/Lich2Damaged.wav");
	lich2_death_fx = LoadFx("audio/sounds/Enemies/Lich2Death.wav");
	lich2_bidimensional_claw_fx = LoadFx("audio/sounds/Enemies/Lich2.wav");
	lich2_sea_of_flames_fx = LoadFx("audio/sounds/Enemies/Lich2.wav");
	

	//heroes
	cleric_attack_fx = LoadFx("audio/sounds/Characters/ClericAttack.wav");
	cleric_run_fx = LoadFx("audio/sounds/Characters/ClericRun.wav");
	cleric_damaged_fx = LoadFx("audio/sounds/Characters/ClericDamaged.wav");
	cleric_death_fx = LoadFx("audio/sounds/Characters/ClericDeath.wav");
	cleric_heal_fx = LoadFx("audio/sounds/Characters/ClericHeal.wav");
	cleric_light_strike_fx = LoadFx("audio/sounds/Characters/ClericLightStrike.wav");
	cleric_mace_throw_fx = LoadFx("audio/sounds/Characters/ClericMaceThrow.wav");
	cleric_blessing_fx = LoadFx("audio/sounds/Characters/ClericBlessing.wav");
	cleric_harden_skin_fx = LoadFx("audio/sounds/Characters/ClericHardenSkin.wav");
	cleric_clarity_fx = LoadFx("audio/sounds/Characters/ClericClarity.wav");
	dwarf_attack_fx = LoadFx("audio/sounds/Characters/DwarfAttack.wav");
	dwarf_run_fx = LoadFx("audio/sounds/Characters/DwarfRun.wav");
	dwarf_damaged_fx = LoadFx("audio/sounds/Characters/DwarfDamaged.wav");
	dwarf_death_fx = LoadFx("audio/sounds/Characters/DwarfDeath.wav");
	dwarf_double_axe_fx = LoadFx("audio/sounds/Characters/DwarfDoubleAxe.wav");
	dwarf_morale_boost_fx = LoadFx("audio/sounds/Characters/DwarfMoraleBoost.wav");
	dwarf_dwarfs_wrath_fx = LoadFx("audio/sounds/Characters/DwarfDwarfsWrath.wav");
	dwarf_raging_mock_fx = LoadFx("audio/sounds/Characters/DwarfRagingMock.wav");
	dwarf_meteor_fx = LoadFx("audio/sounds/Characters/DwarfMeteor.wav");
	dwarf_counter_fx = LoadFx("audio/sounds/Characters/DwarfCounter.wav");
	elf_attack_fx = LoadFx("audio/sounds/Characters/ElfAttack.wav");
	elf_run_fx = LoadFx("audio/sounds/Characters/ElfRun.wav");
	elf_damaged_fx = LoadFx("audio/sounds/Characters/ElfDamaged.wav");
	elf_death_fx = LoadFx("audio/sounds/Characters/ElfDeath.wav");
	elf_mindblown_fx = LoadFx("audio/sounds/Characters/ElfFireBall.wav");
	elf_fireball_fx = LoadFx("audio/sounds/Characters/ElfFireBall.wav");
	elf_seeds_of_life_fx = LoadFx("audio/sounds/Characters/ElfSeedsOfLife.wav");
	elf_lightning_bolt_fx = LoadFx("audio/sounds/Characters/ElfLightningBolt.wav");
	elf_revive_fx = LoadFx("audio/sounds/Characters/ElfRevive.wav");
	elf_blizzard_fx = LoadFx("audio/sounds/Characters/ElfBlizzard.wav");
	elf_fire_djiin_fx = LoadFx("audio/sounds/Characters/ElfFireDjiin.wav");
	warrior_attack_fx = LoadFx("audio/sounds/Characters/WarriorAttack.wav");
	warrior_run_fx = LoadFx("audio/sounds/Characters/WarriorRun.wav");
	warrior_damaged_fx = LoadFx("audio/sounds/Characters/WarriorDamaged.wav");
	warrior_death_fx = LoadFx("audio/sounds/Characters/WarriorDeath.wav");
	warrior_kick_fx = LoadFx("audio/sounds/Characters/WarriorKick.wav");
	warrior_guard_fx = LoadFx("audio/sounds/Characters/WarriorGuard.wav");
	warrior_heavy_slash_fx = LoadFx("audio/sounds/Characters/WarriorHeavySlash.wav");
	warrior_charge_fx = LoadFx("audio/sounds/Characters/WarriorCharge.wav");
	warrior_taunt_fx = LoadFx("audio/sounds/Characters/WarriorTaunt.wav");
	warrior_whirlwind_fx = LoadFx("audio/sounds/Characters/WarriorWhirlwind.wav");

	//---------------------------------------Load Music----------------------------
	
	MainMenuBSO = "audio/music/D&D Shadow Over Mystara - Song 00 Fanfare.ogg";
	MainMenuVoice = "audio/music/D&D Shadow Over Mystara - Song 02  Dungeons & Dragons.ogg";
	SettingsBSO = "audio/music/D&D Shadow Over Mystara - Song 21 What Sleeps in The Rotting Sea (Stage 5-B).ogg";
	CutsceneBSO = "audio/music/D&D Shadow Over Mystara - Song 04 Prologue (Stage 1).ogg";
	WorldMapBSO = "audio/music/D&D Shadow Over Mystara - Song 05 The Journey (Stage 1).ogg";
	CombatBSO = "audio/music/D&D Shadow Over Mystara - Song 07 Battle at Trintan Village (Stage 2).ogg";
	CombatBSO2 = "audio/music/D&D Shadow Over Mystara - Song 11 Head Down Vesuvia  River(Stage 3-B).ogg";
	CombatBSO3 = "audio/music/D&D Shadow Over Mystara - Song 15 Heavy Battle Tank Juggernaut (Stage 3-A).ogg";
	CombatBSO4 = "audio/music/D&D Shadow Over Mystara - Song 17 Shadow Elf in The Harsh Fight(Stage 4).ogg";
	CombatBSO5 = "audio/music/D&D Shadow Over Mystara - Song 23 Eyes Glinting in Darkness (Stage 10).ogg";
	CombatBSO6 = "audio/music/D_D-Shadow-Over-Mystara-Song-35-War-of-Spiral-_Stage-10_.ogg";
	WinBSO = "audio/music/D_D-Shadow-Over-Mystara-Song-38-Epiloge-_Stage-10_.ogg";
	LoseBSO = "audio/music/D_D-Shadow-Over-Mystara-Song-39-Our-Divine-Protection-First-Child-Amidst-Death.ogg";
	LootBSO = "audio/music/D_D-Shadow-Over-Mystara-Song-30-Excessive-Feast-_Stage-8-B_.ogg";

	//---------------------------------------SetVolumes----------------------------
	Mix_Volume(-1, 64);
	Mix_VolumeMusic(64);
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