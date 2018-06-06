#ifndef __J1_PARTICLE_SYSTEM_H__
#define __J1_PARTICLE_SYSTEM_H__

#include "ctPoint.h"
#include "ctModule.h"
#include "Emitter.h"
#include <list>
#include <string>
#include "SDL/include/SDL.h"

#define MAX_NUM_EMITTERS_TYPE 80

class Emitter;
struct SDL_Texture;
struct SDL_Rect;
struct SDL_Color;

enum EmitterType
{
	EMITTER_TYPE_NONE = -1,

	//DEFAULT ATTACK
	EMITTER_TYPE_HIT_HEROES,//
	EMITTER_TYPE_HIT_CRITICAL_HEROES,//
	EMITTER_TYPE_HIT_ENEMY,//
	EMITTER_TYPE_HIT_CRITICAL_ENEMY,//

	//ALTERED STATS
	EMITTER_TYPE_POISON,//
	EMITTER_TYPE_BLEEDING,//
	EMITTER_TYPE_BURNING,//
	EMITTER_TYPE_STUN,
	EMITTER_TYPE_DEBUFF,//
	EMITTER_TYPE_BUFF,//

	//CLERIC
	EMITTER_TYPE_HEAL,//
	EMITTER_TYPE_HEAL_PLUS,//
	EMITTER_TYPE_BLESSING,//
	EMITTER_TYPE_CLARITY,//

	//DWARF
	EMITTER_TYPE_HIGH_AXE,//
	EMITTER_TYPE_HIGH_AXE_PLUS,//
	EMITTER_TYPE_MORALE_BOOST,//
	EMITTER_TYPE_MORALE_BOOST_PLUS,//
	EMITTER_TYPE_DWARFS_WRATH,//
	EMITTER_TYPE_DWARFS_WRATH_PLUS,//
	EMITTER_TYPE_RAGGING_MOCK,//
	EMITTER_TYPE_METEOR,//

	//ELF
	EMITTER_TYPE_SEED_OF_LIFE,//
	EMITTER_TYPE_SEED_OF_LIFE_PLUS,//
	EMITTER_TYPE_FIREBALL,//
	EMITTER_TYPE_FIREBALL_PLUS,//
	EMITTER_TYPE_LIGTHNING_BOLT,//
	EMITTER_TYPE_LIGHTNING_BOLT_PLUS,//
	EMITTER_TYPE_REVIVE,//
	EMITTER_TYPE_BLIZZARD,//
	EMITTER_TYPE_FIRE_DJIN,//

	//WARRIOR
	EMITTER_TYPE_KICK,//
	EMITTER_TYPE_KICK_PLUS,//
	EMITTER_TYPE_HEAVY_SLASH,//
	EMITTER_TYPE_HEAVY_SLASH_PLUS,//
	EMITTER_TYPE_CHARGE,//
	EMITTER_TYPE_TAUNT,
	EMITTER_TYPE_WHIRLWIND,//

	//OBJECTS
	EMITTER_TYPE_HEALTH_POTION,//
	EMITTER_TYPE_MEGA_HEALTH_POTION,//
	EMITTER_TYPE_MANA_POTION,//
	EMITTER_TYPE_MEGA_MANA_POTION,//
	EMITTER_TYPE_POISONED_DAGGER,//
	EMITTER_TYPE_DRAGON_ESSENCE,//
	EMITTER_TYPE_FENIX_TEARS,//

	//DARK WARRIOR
	EMITTER_TYPE_VOID_CANNON,//
	EMITTER_TYPE_THUNDER_PUNCH,//
	
	//DISPLACER BEAST
	EMITTER_TYPE_INFERNAL_FIRE,//
	EMITTER_TYPE_SHADOW_JAB,//

	//TROLL LEADER
	EMITTER_TYPE_INFESTED_CLAW,//

	//NEW GAME
	EMITTER_TYPE_NEW_GAME
};

struct EmitterData
{
	fPoint angleRange = { 0.0f, 0.0f };
	float startSpeed = 0.0f, endSpeed = 0.0f;
	float startSize = 0.0f, endSize = 0.0f;
	uint emitNumber = 0u;
	uint emitVariance = 0u;
	uint maxParticleLife = 0u;
	SDL_Rect textureRect = { 0, 0 };
	double lifetime = -1.0f;
	SDL_Color startColor = { 0, 0, 0, 0 };
	SDL_Color endColor = { 0, 0, 0, 0 };
	SDL_BlendMode blendMode = SDL_BlendMode::SDL_BLENDMODE_NONE;
	double rotSpeed = 0;

	// Randoms
	fPoint rotSpeedRand = { 0.0f, 0.0f };
	fPoint startSpeedRand = { 0.0f, 0.0f };
	fPoint endSpeedRand = { 0.0f, 0.0f };
	fPoint emitVarianceRand = { 0.0f, 0.0f };
	fPoint lifeRand = { 0.0f, 0.0f };
	fPoint startSizeRand = { 0.0f, 0.0f };
	fPoint endSizeRand = { 0.0f, 0.0f };
};

class j1ParticleSystem : public ctModule
{

private:

	std::list<Emitter*> emittersList;
	SDL_Texture* particleAtlas = nullptr;
	std::string nameParticleAtlas;

	// Static array that stores all the data of emitters
	EmitterData vecEmitterData[MAX_NUM_EMITTERS_TYPE];

public:

	j1ParticleSystem();

	// Destructor
	virtual ~j1ParticleSystem();

	// Called when before render is available
	bool Awake(pugi::xml_node& config);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Emitter methods
	Emitter* AddEmiter(fPoint pos, EmitterType type);
	bool RemoveEmitter(Emitter& emitter);
	bool RemoveAllEmitters();

	SDL_Texture* GetParticleAtlas() const;
	void LoadEmitterData(pugi::xml_node& config, EmitterType type);
};

#endif
