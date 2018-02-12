#include "ctApp.h"
#include "Entity.h"
#include "ctCollider.h"
#include "ctEntities.h"
#include "ctRender.h"
//#include "j1Scene.h"

Entity::Entity(int x, int y) : position(x, y)
{
}

Entity::~Entity()
{
	if (collider != nullptr) {
		App->collider->EraseCollider(collider);
		collider = nullptr;
	}
}

const Collider* Entity::GetCollider() const
{
	return collider;
}

void Entity::Draw(SDL_Texture* sprites)
{
	SDL_Rect r = animation->GetCurrentFrame();

	if (collider != nullptr)
		(this->type == PLAYER) ? collider->SetPos(position.x, position.y - r.h) :
		collider->SetPos(position.x, position.y);
	
	if (animation != nullptr)
		(this->type == PLAYER) ? App->render->Blit(sprites, position.x, position.y - r.h, &(animation->GetCurrentFrame())) :
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()));
}

void Entity::OnCollision(Collider* collider)
{

}