#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctRender.h"
#include "ctTextures.h"
#include "ctCollider.h"
#include "ctAudio.h"
#include "ctEntities.h"
#include <string>
#include "Player.h"

Player::Player(int x, int y, EntityType type) : Entity(x, y) {

	bool ret = true;
	this->type = type;

	//TODO : this looks ugly but it seems the only way :'c
	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadConfig(config_file); //todo: make a method to get the root without passing the xml_document
	node = &node->child("entities").child("player");

	//read fxs from node
	//player_jump_fx = App->audio->LoadFx(node->child("fxPlayerJump").attribute("name").as_string());

	//read animation from node
	for (pugi::xml_node animations = node->child("animations").child("animation"); animations && ret; animations = animations.next_sibling("animation"))
	{
		std::string tmp = animations.attribute("name").as_string();

		if (tmp == "down_idle")
			LoadAnimation(animations, &down_idle);
	}

	LOG("Creating player collider");
	collider = App->collider->AddCollider({ 0, 0, 16, 12 }, ColliderType::COLLIDER_PLAYER, (ctModule*)App->entities);

	if (collider == nullptr)
		LOG("Error adding player collider");


	animation = &down_idle;
}

void Player::LoadAnimation(pugi::xml_node animation_node, ctAnimation* animation)
{
	bool ret = true;

	for (pugi::xml_node frame = animation_node.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
}

Player::~Player()
{
	LOG("Freeing the player");

	LOG("Unloading player sound fx");
	//App->audio->UnLoadFx(player_jump_fx);

	LOG("Unloading player collider");
	App->collider->EraseCollider(collider);
}

// Called each loop iteration
void Player::Update(float dt)
{
	current_dt = dt;

	if (dt > 0)
	{
		if (!key_entities_speed)
			SetEntitiesSpeed(dt);

		SetPlayerAnimationsSpeed(dt);
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		position.x++;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		position.x--;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		position.y++;
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		position.y--;

	LOG("Player pos x: %i pos y: %i", position.x, position.y);

	// Draw everything --------------------------------------
	SDL_Rect r = animation->GetCurrentFrame();

	// Update player collider
	collider->SetPos(position.x, position.y - r.h);
	collider->rect.w = r.w;
	collider->rect.h = r.h;

	currentTime = SDL_GetTicks();
}

void Player::OnCollision(Collider* collider) {
}

void Player::SetPlayerAnimationsSpeed(float dt)
{
	down_idle.speed = down_idle_velocity * dt;
}

bool Player::Load(pugi::xml_node& load)
{
	bool ret = true;

	return ret;
}

bool Player::Save(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}


void Player::SetEntitiesSpeed(float dt)
{
	down_idle_velocity = down_idle.speed;

	key_entities_speed = true;
}