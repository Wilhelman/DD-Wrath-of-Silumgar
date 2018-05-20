#include "ctLog.h"
#include "j1ParticleSystem.h"
#include "ctApp.h"
#include "ctTextures.h"



j1ParticleSystem::j1ParticleSystem() : ctModule()
{
	name = "psystem";

	LOG("Loading Languages");

}

j1ParticleSystem::~j1ParticleSystem()
{

}

bool j1ParticleSystem::Awake(pugi::xml_node& config)
{
	bool ret = true;

	pugi::xml_document	psystem_config;
	pugi::xml_node* node = &App->LoadEmitters(psystem_config);
	nameParticleAtlas = node->child("particleAtlas").attribute("name").as_string();

	for (pugi::xml_node emitters = node->child("particleAtlas").child("emitter"); emitters && ret; emitters = emitters.next_sibling("emitter"))
	{
		/* TODO 1: Load emitter data into the emitter data vector:
		- We just want to load the emitter data from the fire emitter for now.
		- The for loop already parses the xml file for you. Just search for the emitter type “fire”.
		- Once you find it use LoadEmitterData() to fill the vector. Use EMITTER_TYPE_FIRE for the enum.
		*/

		std::string emitterType = emitters.attribute("type").as_string();

		//DEFAULT ATTACK
		if (emitterType == "hit_heroes")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_HIT_HEROES);
		else if (emitterType == "hit_critical_heroes")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
		else if (emitterType == "hit_enemies")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_HIT_ENEMY);
		else if (emitterType == "hit_critical_enemy")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
		//ALTERED STATS
		else if (emitterType == "poison")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_POISON);
		else if (emitterType == "bleeding")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_BLEEDING);
		else if (emitterType == "burning")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_BURNING);
		else if (emitterType == "stun")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_STUN);
		else if (emitterType == "debuff")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_DEBUFF);
		else if (emitterType == "buff")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_BUFF);
		//CLERIC
		else if (emitterType == "heal")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_HEAL);
		else if (emitterType == "heal_plus")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_HEAL_PLUS);
		else if (emitterType == "blessing")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_BLESSING);
		else if (emitterType == "clarity")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_CLARITY);
		//DWARF
		else if (emitterType == "high_axe")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_HIGH_AXE);
		else if (emitterType == "high_axe_plus")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_HIGH_AXE_PLUS);
		else if (emitterType == "morale_boost")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_MORALE_BOOST);
		else if (emitterType == "morale_boost_plus")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_MORALE_BOOST_PLUS);
		else if (emitterType == "dwarfs_wrath")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_DWARFS_WRATH);
		else if (emitterType == "dwarfs_wrath_plus")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_DWARFS_WRATH_PLUS);
		else if (emitterType == "ragging_mock")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_RAGGING_MOCK);
		//ELF
		else if (emitterType == "seed_of_life")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_SEED_OF_LIFE);
		else if (emitterType == "seed_of_life_plus")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_SEED_OF_LIFE_PLUS);
		else if (emitterType == "fireball")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_FIREBALL);
		else if (emitterType == "fireball_plus")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_FIREBALL_PLUS);
		else if (emitterType == "ligthning_bolt")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_LIGTHNING_BOLT);
		else if (emitterType == "lightning_bolt_plus")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_LIGHTNING_BOLT_PLUS);
		else if (emitterType == "revive")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_REVIVE);
		else if (emitterType == "blizzard")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_BLIZZARD);
		else if (emitterType == "fire_djin")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_FIRE_DJIN);
		//WARRIOR
		else if (emitterType == "kick")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_KICK);
		else if (emitterType == "kick_plus")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_KICK_PLUS);
		else if (emitterType == "heavy_slash")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_HEAVY_SLASH);
		else if (emitterType == "heavy_slash_plus")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_HEAVY_SLASH_PLUS);
		else if (emitterType == "charge")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_CHARGE);
		else if (emitterType == "taunt")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_TAUNT);
		else if (emitterType == "whirlwind")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_WHIRLWIND);
		//OBJECTS
		else if (emitterType == "health_potion")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_HEALTH_POTION);
		else if (emitterType == "mega_health_potion")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_MEGA_HEALTH_POTION);
		else if (emitterType == "mana_potion")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_MANA_POTION);
		else if (emitterType == "mega_mana_potion")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_MEGA_MANA_POTION);
		else if (emitterType == "poisoned_dagger")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_POISONED_DAGGER);
		else if (emitterType == "dragon_essence")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_DRAGON_ESSENCE);
		else if (emitterType == "fenix_tears")
			LoadEmitterData(emitters, EmitterType::EMITTER_TYPE_FENIX_TEARS);
	}
	return ret;
}

bool j1ParticleSystem::Start()
{
	particleAtlas = App->tex->Load(nameParticleAtlas.c_str());

	return true;
}

bool j1ParticleSystem::PreUpdate()
{
	return true;
}

bool j1ParticleSystem::Update(float dt)
{
	std::list<Emitter*>::const_iterator it;

	for (it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		if ((*it) != nullptr)
			(*it)->Update(dt);
	}

	return true;
}

bool j1ParticleSystem::PostUpdate()
{
	std::list<Emitter*>::const_iterator it;

	for (it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		if ((*it) != nullptr)
			(*it)->PostUpdate();
	}

	for (it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		if ((*it)->toDestroy)
		{
			delete (*it);
			emittersList.erase(it);
		}
	}

	return true;
}

bool j1ParticleSystem::CleanUp()
{
	LOG("Freeing emitters from the system.");

	std::list<Emitter*>::const_iterator it;

	for (it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		if ((*it) != nullptr)
			delete (*it);
	}

	emittersList.clear();
	App->tex->UnLoad(particleAtlas);

	return true;
}

Emitter* j1ParticleSystem::AddEmiter(fPoint pos, EmitterType type)
{
	Emitter* tmp_emitter = new Emitter(pos, vecEmitterData[type]);

	emittersList.push_back(tmp_emitter);

	return tmp_emitter;
}

bool j1ParticleSystem::RemoveEmitter(Emitter & emitter)
{
	std::list<Emitter*>::const_iterator it;

	for (it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		if ((*it) == &emitter)
		{
			(*it)->toDestroy = true;
			return true;
		}
	}

	return false;
}

bool j1ParticleSystem::RemoveAllEmitters()
{
	bool ret = false;

	std::list<Emitter*>::const_iterator it;

	for (it = emittersList.begin(); it != emittersList.end(); ++it)
	{
		if ((*it) != nullptr) (*it)->toDestroy = true;
		ret = true;
	}

	return ret;
}

SDL_Texture* j1ParticleSystem::GetParticleAtlas() const
{
	return particleAtlas;
}

void j1ParticleSystem::LoadEmitterData(pugi::xml_node & emitter, EmitterType type)
{
	EmitterData tmp;

	// Angle range
	tmp.angleRange.x = emitter.child("angleRange").attribute("min").as_float();
	tmp.angleRange.y = emitter.child("angleRange").attribute("max").as_float();

	// Particle speeds
	tmp.rotSpeed = emitter.child("rotSpeed").attribute("value").as_double();
	tmp.startSpeed = emitter.child("startSpeed").attribute("value").as_float();
	tmp.endSpeed = emitter.child("endSpeed").attribute("value").as_float();

	// Random controls
	tmp.startSpeedRand.x = emitter.child("startSpeed").attribute("randMin").as_float();
	tmp.startSpeedRand.y = emitter.child("startSpeed").attribute("randMax").as_float();

	tmp.endSpeedRand.x = emitter.child("endSpeed").attribute("randMin").as_float();
	tmp.endSpeedRand.y = emitter.child("endSpeed").attribute("randMax").as_float();

	tmp.rotSpeedRand.x = emitter.child("rotSpeed").attribute("randMin").as_float();
	tmp.rotSpeedRand.y = emitter.child("rotSpeed").attribute("randMax").as_float();

	tmp.emitVarianceRand.x = emitter.child("emitVariance").attribute("randMin").as_float();
	tmp.emitVarianceRand.y = emitter.child("emitVariance").attribute("randMax").as_float();

	tmp.lifeRand.x = emitter.child("maxParticleLife").attribute("randMin").as_float();
	tmp.lifeRand.y = emitter.child("maxParticleLife").attribute("randMax").as_float();

	tmp.startSizeRand.x = emitter.child("startSize").attribute("randMin").as_float();
	tmp.startSizeRand.y = emitter.child("startSize").attribute("randMax").as_float();

	tmp.endSizeRand.x = emitter.child("endSizeRand").attribute("randMin").as_float();
	tmp.endSizeRand.y = emitter.child("endSizeRand").attribute("randMax").as_float();

	// Particle size
	tmp.startSize = emitter.child("startSize").attribute("value").as_float();
	tmp.endSize = emitter.child("endSize").attribute("value").as_float();

	// Emission properties
	tmp.emitNumber = emitter.child("emitNumber").attribute("value").as_uint();
	tmp.emitVariance = emitter.child("emitVariance").attribute("value").as_uint();

	// Particle life
	tmp.maxParticleLife = emitter.child("maxParticleLife").attribute("value").as_uint();

	// Rect from particle atlas
	tmp.textureRect.x = emitter.child("textureRect").attribute("x").as_int();
	tmp.textureRect.y = emitter.child("textureRect").attribute("y").as_int();
	tmp.textureRect.w = emitter.child("textureRect").attribute("w").as_int();
	tmp.textureRect.h = emitter.child("textureRect").attribute("h").as_int();

	// Lifetime of emitter
	tmp.lifetime = emitter.child("lifetime").attribute("value").as_double();

	// Start color
	tmp.startColor.r = emitter.child("startColor").attribute("r").as_uint();
	tmp.startColor.g = emitter.child("startColor").attribute("g").as_uint();
	tmp.startColor.b = emitter.child("startColor").attribute("b").as_uint();
	tmp.startColor.a = emitter.child("startColor").attribute("a").as_uint();

	// End color
	tmp.endColor.r = emitter.child("endColor").attribute("r").as_uint();
	tmp.endColor.g = emitter.child("endColor").attribute("g").as_uint();
	tmp.endColor.b = emitter.child("endColor").attribute("b").as_uint();
	tmp.endColor.a = emitter.child("endColor").attribute("a").as_uint();

	// Blend mode
	std::string blendModeString = emitter.child("blendMode").attribute("mode").as_string();

	if (blendModeString == "add")
		tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_ADD;
	else if (blendModeString == "blend")
		tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_BLEND;
	else if (blendModeString == "mod")
		tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_MOD;
	else if (blendModeString == "none")
		tmp.blendMode = SDL_BlendMode::SDL_BLENDMODE_NONE;

	vecEmitterData[type] = tmp;
}