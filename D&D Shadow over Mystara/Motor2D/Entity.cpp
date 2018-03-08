#include "ctApp.h"
#include "Entity.h"
#include "ctEntities.h"
#include "ctRender.h"

Entity::Entity(int x, int y) : position(x, y)
{
}

Entity::~Entity()
{
}

void Entity::Draw(SDL_Texture* sprites)
{
	SDL_Rect r = animation->GetCurrentFrame();
	
	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y - r.h, &(animation->GetCurrentFrame()));
}