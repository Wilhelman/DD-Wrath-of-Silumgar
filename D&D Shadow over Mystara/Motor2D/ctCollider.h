#ifndef __ctCollider_H__
#define __ctCollider_H__

#define MAX_COLLIDERS 1000

#include "ctModule.h"

#include "ctRender.h"


enum ColliderType
{
	COLLIDER_NONE = -1,
	COLLIDER_PLAYER,

	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect rect = { 0, 0, 0, 0 };
	bool to_delete = false;
	ColliderType type = COLLIDER_NONE;
	ctModule* callback = nullptr;

	Collider(SDL_Rect rectangle, ColliderType type, ctModule* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
};

class ctCollider : public ctModule
{
public:

	ctCollider();
	~ctCollider();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();


	Collider* AddCollider(SDL_Rect rect, ColliderType type, ctModule* callback = nullptr);
	void DebugDraw();

	bool EraseCollider(Collider* collider);

private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
};

#endif // __ModuleCollision_H__