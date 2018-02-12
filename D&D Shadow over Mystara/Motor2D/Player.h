#ifndef __Player_H__
#define __Player_H__

#include "Entity.h"
#include "ctEntities.h"

struct SDL_Texture;
struct Collider;

class Player : public Entity
{
	enum PlayerState
	{
		ST_IDLE_RIGHT,
		ST_IDLE_LEFT,

		ST_UNKNOWN
	};

	enum PlayerLastState
	{
		LAST_ST_IDLE_RIGHT,
		LAST_ST_IDLE_LEFT,

		LAST_ST_UNKNOWN
	};

	enum Direction
	{
		RIGHT,
		LEFT,
		UP,
		DOWN,

		NO_DIR
	};

public:

	SDL_Rect current_frame = { 0,0,0,0 };

private:

	float current_dt = 0.0f;

	PlayerState current_state = PlayerState::ST_UNKNOWN;
	PlayerLastState last_state = PlayerLastState::LAST_ST_UNKNOWN;

	//anims
	ctAnimation down_idle = ctAnimation();

	//anims speed
	uint down_idle_velocity = 0u;

public:

	Player(int x, int y, EntityType type);
	~Player();

	void Update(float dt);

	void SetEntitiesSpeed(float dt);

	void LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation);

	void OnCollision(Collider* collider);

	bool Load(pugi::xml_node&);

	bool Save(pugi::xml_node&) const;

	void SetPlayerAnimationsSpeed(float dt);
};

#endif