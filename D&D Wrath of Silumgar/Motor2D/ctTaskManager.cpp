#include "ctTaskManager.h"
#include "ctApp.h"
#include "ctEntities.h"
#include "ctInput.h"
#include "ctCombat.h"
#include "Particle.h"
#include "ParticlePool.h"
#include "Emitter.h"

//randomize libs
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


bool MoveToEntity::Execute()
{
	bool ret = false;

	if (entity_to_move->IsStunned())
			return true;

	if (entity_to_move->GetCurrentHealthPoints() != 0) {
		if (entity_to_go->GetCurrentHealthPoints() == 0) {
			if (entity_to_move->type == ELF || entity_to_move->type == CLERIC || entity_to_move->type == WARRIOR || entity_to_move->type == DWARF) {
				for (int i = 0; i < App->combat->enemies.size(); i++)
				{
					entity_to_go = App->combat->enemies.at(i);
					if (entity_to_go->GetCurrentHealthPoints() != 0)
						break;
				}
				if(entity_to_go->GetCurrentHealthPoints() == 0)
					return true;
			}
			else {
				for (int i = 0; i < App->combat->heroes.size(); i++)
				{
					entity_to_go = App->combat->heroes.at(i);
					if (entity_to_go->GetCurrentHealthPoints() != 0)
						break;
				}
				if (entity_to_go->GetCurrentHealthPoints() == 0)
					return true;
			}
		}


		int x_objective = (entity_to_go->position.x + offset);

		if (((entity_to_move->position.x + 25) >= x_objective && (entity_to_move->position.x - 25) <= x_objective) && ((entity_to_move->position.y + 25) >= entity_to_go->position.y && (entity_to_move->position.y - 25) <= entity_to_go->position.y))
		{
			ret = true;
		}
		else {
			if (entity_to_move->position.x < entity_to_go->position.x)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.x += BASE_SPEED;
			}

			else if (entity_to_move->position.x > entity_to_go->position.x)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.x -= BASE_SPEED;
			}

			if (entity_to_move->position.y < entity_to_go->position.y)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.y += BASE_SPEED;
			}

			else if (entity_to_move->position.y > entity_to_go->position.y)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.y -= BASE_SPEED;
			}
		}
	}
	else
		ret = true;

	

	return ret;
}

bool MoveToInitialPosition::Execute()
{
	bool ret = false;

	if(entity_to_move->IsStunned())
			return true;

	if (entity_to_move->GetCurrentHealthPoints() != 0) {
		if (entity_to_move->flip_texture != true) {
			entity_to_move->flip_texture = true;
		}

		if (entity_to_move->position.x == entity_to_move->initial_position.x && entity_to_move->position.y == entity_to_move->initial_position.y)
		{
			entity_to_move->animation = &entity_to_move->idle;
			entity_to_move->flip_texture = false;
			ret = true;
		}
		else {
			if (entity_to_move->position.x < entity_to_move->initial_position.x)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.x += BASE_SPEED;
			}

			else if (entity_to_move->position.x > entity_to_move->initial_position.x)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.x -= BASE_SPEED;
			}

			if (entity_to_move->position.y < entity_to_move->initial_position.y)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.y += BASE_SPEED;
			}

			else if (entity_to_move->position.y > entity_to_move->initial_position.y)
			{
				entity_to_move->animation = &entity_to_move->run_forward;
				entity_to_move->position.y -= BASE_SPEED;
			}
		}
	}
	else
		ret = true;

	//todo check this
	if (ret) {
		App->combat->draw_turn_priority_entity.erase(App->combat->draw_turn_priority_entity.cbegin());
		App->combat->draw_turn_priority_entity.shrink_to_fit();
	}
	

	return ret;
}



bool PerformActionToEntity::Execute()
{

	bool ret = false;

	if (actioner_entity->IsStunned())
			return true;

	if (actioner_entity->GetCurrentHealthPoints() != 0) {
		switch (action_to_perform.type)
		{
		case DEFAULT_ATTACK: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->attack;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->attack.Reset();


				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal =  action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}
						
						
						

						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case VOID_CANNON: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->void_cannon;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->void_cannon.Reset();
				actioner_entity->animation = &actioner_entity->idle;

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS


						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
					
						
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
					
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
						//TODO SITO
						fPoint posP;
						posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };

						App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_MINDBLOWN);
						App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);

						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
							 break;
		case KICK: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->kick;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->SetCurrentManaPoints(actioner_entity->GetCurrentManaPoints() - action_to_perform.mana_points_effect_to_himself);
				App->combat->UpdateManaBarOfEntity(actioner_entity, (-action_to_perform.mana_points_effect_to_himself));

				actioner_entity->kick.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity && !receiver_entity->IsStunned()) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;
						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical)
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
						else
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);

						receiver_entity->Damaged();

						Altered_Stat stun;
						stun.stun = true;
						stun.turn_left = 2;

						receiver_entity->AddAlteredStat(stun);
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				fPoint  posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };

				App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_KICK);

				actioner_entity->Ability1();

			}
		}
		break;
		case GUARD: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->guard;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->guard.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case HEAVY_SLASH: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->heavy_slash;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->heavy_slash.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case KICK_PLUS: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->kick_plus;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->kick_plus.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case GUARD_PLUS: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->guard_plus;
			
			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->guard_plus.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case HEAVY_SLASH_PLUS: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->heavy_slash_plus;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->heavy_slash_plus.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		 break;

		case CHARGE: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->charge;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->charge.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case TAUNT: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->taunt;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->taunt.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case WHIRLWIND: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->whirlwind;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->whirlwind.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
						 break;


		
		case HIGH_AXE: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->high_axe;

			

			ret = actioner_entity->animation->Finished();

			if (ret == true) {

				actioner_entity->SetCurrentManaPoints(actioner_entity->GetCurrentManaPoints() - action_to_perform.mana_points_effect_to_himself);
				App->combat->UpdateManaBarOfEntity(actioner_entity, (-action_to_perform.mana_points_effect_to_himself));

				actioner_entity->high_axe.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity || receiver_entity->IsStunned()) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;
						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						
						if (receiver_entity->IsStunned())
							damage_to_deal = damage_to_deal * 2;
						
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (critical)
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
						else if(receiver_entity->IsStunned())
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 253,103,6,255 }, 16, nullptr, nullptr);
						else
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 },14, nullptr, nullptr);

						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				fPoint  posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };

				App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIGH_AXE);
				actioner_entity->Ability1();

			}
		}
		break;

		case MORALE_BOOST: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->morale_boost;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->morale_boost.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case DWARFS_WRATH: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->dwarfs_wrath;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->dwarfs_wrath.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case HIGH_AXE_PLUS: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->high_axe_plus;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->high_axe_plus.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case MORALE_BOOST_PLUS: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->morale_boost_plus;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->morale_boost_plus.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case DWARFS_WRATH_PLUS: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->dwarfs_wrath_plus;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->dwarfs_wrath_plus.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		 break;

		case RAGING_MOCK: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->raging_mock;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->raging_mock.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case METEOR: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->meteor;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->meteor.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case COUNTER: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->counter;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->counter.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;





		case HEAL: {
			
			if (!HaveTeamObjective())
				return true;

			actioner_entity->animation = &actioner_entity->heal;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {

				actioner_entity->SetCurrentManaPoints(actioner_entity->GetCurrentManaPoints() - action_to_perform.mana_points_effect_to_himself);
				App->combat->UpdateManaBarOfEntity(actioner_entity, (-action_to_perform.mana_points_effect_to_himself));

				actioner_entity->heal.Reset();

				bool critical = false;

				int damage_to_deal = action_to_perform.health_points_effect;

				damage_to_deal = damage_to_deal;
				receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
				//receiver_entity->animation = &receiver_entity->hit;
				App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
				std::string tmp_dmg = std::to_string(damage_to_deal);

				App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 0,255,0,255 }, 14, nullptr, nullptr);
				
				
				//TODO SITO
				fPoint  posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
				
				App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_LOWER_HEALTH);
				
				actioner_entity->Ability1();

			}
		}
		break;

		case THUNDER_PUNCH: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->thunder_punch;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->SetCurrentManaPoints(actioner_entity->GetCurrentManaPoints() - action_to_perform.mana_points_effect_to_himself);
				App->combat->UpdateManaBarOfEntity(actioner_entity, (-action_to_perform.mana_points_effect_to_himself));

				actioner_entity->thunder_punch.Reset();
				actioner_entity->animation = &actioner_entity->idle;

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity && !receiver_entity->IsStunned()) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;
						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical)
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
						else
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);

						receiver_entity->Damaged();

						Altered_Stat stun;
						stun.stun = true;
						stun.turn_left = 1;

						receiver_entity->AddAlteredStat(stun);
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				fPoint  posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };

				App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_KICK);

				actioner_entity->Ability1();

			}
		}
		break;


		case INSIGNIFICANT_MORTALS: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->insignificant_mortals;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->base_stats.agility += 5;
				
				App->gui->AddUIFloatingValue(actioner_entity->position.x + (actioner_entity->animation->GetCurrentFrame().w / 2), actioner_entity->position.y - actioner_entity->animation->GetCurrentFrame().h, "Stats Up!", { 0,255,0,255 }, 14, nullptr, nullptr);
				//animate the receiver to hit + audio or smth
				//fPoint  posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };

				//App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_KICK);

				//actioner_entity->Ability1();

			}
		}
		break;


		case LIGHT_STRIKE: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->light_strike;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->light_strike.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case MACE_THROW: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->mace_throw;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->mace_throw.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case HEAL_PLUS: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->heal_plus;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->heal_plus.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case LIGHT_STRIKE_PLUS: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->light_strike_plus;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->light_strike_plus.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case MACE_THROW_PLUS: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->mace_throw_plus;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->mace_throw_plus.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case BLESSING: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->blessing;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->blessing.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case HARDEN_SKIN: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->harder_skin;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->harder_skin.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case CLARITY: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->clarity;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->clarity.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		 break;


		case BLIZZARD: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->blizzard;



			ret = actioner_entity->animation->Finished();

			if (ret == true) {

				actioner_entity->SetCurrentManaPoints(actioner_entity->GetCurrentManaPoints() - action_to_perform.mana_points_effect_to_himself);
				App->combat->UpdateManaBarOfEntity(actioner_entity, (-action_to_perform.mana_points_effect_to_himself));

				actioner_entity->blizzard.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity || receiver_entity->IsStunned()) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentMagicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;
						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}

						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical)
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
						else
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);

						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				fPoint  posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };

				App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_MINDBLOWN);
				actioner_entity->Ability1();

			}
		}
		break;

		case SEED_OF_LIFE: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->seed_of_life;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->seed_of_life.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case FIREBALL: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->fireball;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->fireball.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case LIGHTNING_BOLT: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->lightning_bolt;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->lightning_bolt.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case SEED_OF_LIFE_PLUS: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->seed_of_life_plus;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->seed_of_life_plus.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		 break;

		case FIREBALL_PLUS: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->fireball_plus;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->fireball_plus.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		 break;

		case LIGHTNING_BOLT_PLUS: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->lightning_bol_plus;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->lightning_bol_plus.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case REVIVE: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->revive;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->revive.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
		break;

		case FIRE_DJINN: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->fire_djinn;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {
				actioner_entity->fire_djinn.Reset();

				int actioner_dexterity = BASE_DEXTERITY + actioner_entity->GetCurrentDexterityPoints();

				int random_thousand_faces_die = (rand() % 100) + 1;

				if (random_thousand_faces_die <= actioner_dexterity) {// THE ACTIONER HITS THE RECEIVER
					int receiver_agility = BASE_AGILITY + receiver_entity->GetCurrentAgilityPoints();

					random_thousand_faces_die = (rand() % 100) + 1;
					if (random_thousand_faces_die <= receiver_agility && !receiver_entity->IsStunned()) {// THE RECEIVER DODGES THE ATTACK
						App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Dodge", { 204,204,0,255 }, 14, nullptr, nullptr);
						receiver_entity->animation = &receiver_entity->dodge;
					}
					else {// THE ATTACK HITS

						bool critical = false;

						int damage_to_deal = action_to_perform.health_points_effect;
						float damage_reduction = (float)receiver_entity->GetCurrentPhysicalDefensePoints() / 100 * (float)damage_to_deal;
						actioner_dexterity = actioner_dexterity / 10;

						random_thousand_faces_die = (rand() % 100) + 1;
						if (random_thousand_faces_die <= actioner_dexterity) {
							damage_to_deal = damage_to_deal * CRITICAL_VALUE;
							critical = true;
						}
						damage_to_deal = damage_to_deal - damage_reduction;
						receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
						receiver_entity->animation = &receiver_entity->hit;
						App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
						std::string tmp_dmg = std::to_string(damage_to_deal);
						if (!critical) {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 14, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_ENEMY);
							}
						}
						else {
							App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,255,255 }, 16, nullptr, nullptr);
							//TODO SITO
							fPoint posP;
							if (receiver_entity->type == CLERIC || receiver_entity->type == WARRIOR || receiver_entity->type == ELF || receiver_entity->type == DWARF)
							{
								posP = { (float)(receiver_entity->position.x), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_HEROES);
							}
							else
							{
								posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
								App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_HIT_CRITICAL_ENEMY);
							}
						}




						receiver_entity->Damaged();
					}
				}
				else {//ACTIONER MISSES!
					App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, "Miss", { 0,102,204,255 }, 14, nullptr, nullptr);
				}
				//animate the receiver to hit + audio or smth
				actioner_entity->Attack();

			}
		}
						 break;




		case LOW_HEALTH_RECOVER_ACTION: {

			if (!HaveTeamObjective())
				return true;

			actioner_entity->animation = &actioner_entity->throw_object;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {

				//todo reducir la quantity
				for (int i = 0; i < actioner_entity->usable_items.size(); i++)
				{
					if (actioner_entity->usable_items.at(i).action.type == action_to_perform.type) {
						actioner_entity->usable_items.at(i).quantity--;
						if (actioner_entity->usable_items.at(i).quantity == 0)
							actioner_entity->usable_items.erase(actioner_entity->usable_items.cbegin() + i);
						break;
					}
				}

				actioner_entity->throw_object.Reset();

				bool critical = false;

				int damage_to_deal = action_to_perform.health_points_effect;

				damage_to_deal = damage_to_deal;
				receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
				//receiver_entity->animation = &receiver_entity->hit;
				App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
				std::string tmp_dmg = std::to_string(damage_to_deal);

				App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 0,255,0,255 }, 14, nullptr, nullptr);


				//TODO SITO
				fPoint  posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
				App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_LOWER_HEALTH);
				

				//actioner_entity->Ability1();

			}
		}
		break;

		case HIGH_HEALTH_RECOVER_ACTION: { 

			if (!HaveTeamObjective())
				return true;

			actioner_entity->animation = &actioner_entity->throw_object;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {

				//todo reducir la quantity
				for (int i = 0; i < actioner_entity->usable_items.size(); i++)
				{
					if (actioner_entity->usable_items.at(i).action.type == action_to_perform.type) {
						actioner_entity->usable_items.at(i).quantity--;
						if (actioner_entity->usable_items.at(i).quantity == 0)
							actioner_entity->usable_items.erase(actioner_entity->usable_items.cbegin() + i);
						break;
					}
				}

				actioner_entity->throw_object.Reset();

				bool critical = false;

				int damage_to_deal = action_to_perform.health_points_effect;

				damage_to_deal = damage_to_deal;
				receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
				//receiver_entity->animation = &receiver_entity->hit;
				App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
				std::string tmp_dmg = std::to_string(damage_to_deal);

				App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 0,255,0,255 }, 14, nullptr, nullptr);


				fPoint  posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
				App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_LOWER_HEALTH);

			}
		}
		break;

		case LOW_MANA_RECOVER_ACTION: {

			if (!HaveTeamObjective())
				return true;

			actioner_entity->animation = &actioner_entity->throw_object;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {

				//todo reducir la quantity
				for (int i = 0; i < actioner_entity->usable_items.size(); i++)
				{
					if (actioner_entity->usable_items.at(i).action.type == action_to_perform.type) {
						actioner_entity->usable_items.at(i).quantity--;
						if (actioner_entity->usable_items.at(i).quantity == 0)
							actioner_entity->usable_items.erase(actioner_entity->usable_items.cbegin() + i);
						break;
					}
				}

				actioner_entity->throw_object.Reset();

				bool critical = false;

				int damage_to_deal = action_to_perform.mana_points_effect;
				damage_to_deal = damage_to_deal;
				receiver_entity->SetCurrentManaPoints(receiver_entity->GetCurrentManaPoints()+damage_to_deal);
				//receiver_entity->animation = &receiver_entity->hit;
				App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
				std::string tmp_dmg = std::to_string(damage_to_deal);

				App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 0,0,255,255 }, 14, nullptr, nullptr);


				fPoint  posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
				App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_MANA_POTION);

			}
		}
		break;

		case HIGH_MANA_RECOVER_ACTION: {

			if (!HaveTeamObjective())
				return true;

			actioner_entity->animation = &actioner_entity->throw_object;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {

				//todo reducir la quantity
				for (int i = 0; i < actioner_entity->usable_items.size(); i++)
				{
					if (actioner_entity->usable_items.at(i).action.type == action_to_perform.type) {
						actioner_entity->usable_items.at(i).quantity--;
						if (actioner_entity->usable_items.at(i).quantity == 0)
							actioner_entity->usable_items.erase(actioner_entity->usable_items.cbegin() + i);
						break;
					}
				}

				actioner_entity->throw_object.Reset();

				bool critical = false;

				int damage_to_deal = action_to_perform.mana_points_effect;
				damage_to_deal = damage_to_deal;
				receiver_entity->SetCurrentManaPoints(receiver_entity->GetCurrentManaPoints() + damage_to_deal);
				//receiver_entity->animation = &receiver_entity->hit;
				App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
				std::string tmp_dmg = std::to_string(damage_to_deal);

				App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 0,0,255,255 }, 14, nullptr, nullptr);


				fPoint  posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
				App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_MANA_POTION);

			}
		}
		break;

		case POISONED_DAGGER_ACTION: {

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->throw_object;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {

				//todo reducir la quantity
				for (int i = 0; i < actioner_entity->usable_items.size(); i++)
				{
					if (actioner_entity->usable_items.at(i).action.type == action_to_perform.type) {
						actioner_entity->usable_items.at(i).quantity--;
						if (actioner_entity->usable_items.at(i).quantity == 0)
							actioner_entity->usable_items.erase(actioner_entity->usable_items.cbegin() + i);
						break;
					}
				}

				actioner_entity->throw_object.Reset();

				bool critical = false;

				int damage_to_deal = action_to_perform.health_points_effect;

				damage_to_deal = damage_to_deal;
				receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
				//receiver_entity->animation = &receiver_entity->hit;
				App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
				std::string tmp_dmg = std::to_string(damage_to_deal);

				App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 16, nullptr, nullptr);


				//TODO SITO
				fPoint  posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
				App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_POISON);

				//actioner_entity->Ability1();

			}
		}
		break;

		case BURN_TARGET_ACTION:
		{

			if (!HaveObjective())
				return true;

			actioner_entity->animation = &actioner_entity->throw_object;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {

				//todo reducir la quantity
				for (int i = 0; i < actioner_entity->usable_items.size(); i++)
				{
					if (actioner_entity->usable_items.at(i).action.type == action_to_perform.type) {
						actioner_entity->usable_items.at(i).quantity--;
						if (actioner_entity->usable_items.at(i).quantity == 0)
							actioner_entity->usable_items.erase(actioner_entity->usable_items.cbegin() + i);
						break;
					}
				}

				actioner_entity->throw_object.Reset();

				bool critical = false;

				int damage_to_deal = action_to_perform.health_points_effect;

				damage_to_deal = damage_to_deal;
				receiver_entity->SetCurrentHealthPoints(receiver_entity->GetCurrentHealthPoints() + damage_to_deal);
				//receiver_entity->animation = &receiver_entity->hit;
				App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
				std::string tmp_dmg = std::to_string(damage_to_deal);

				App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 255,0,0,255 }, 16, nullptr, nullptr);


				//TODO SITO
				fPoint  posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
				App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_POISON);

				//actioner_entity->Ability1();

			}
		}
		break;
		case REVIVE_AN_ALLY_ACTION:
		{

			//if (!HaveTeamObjective())
				//return true;

			actioner_entity->animation = &actioner_entity->throw_object;

			ret = actioner_entity->animation->Finished();

			if (ret == true) {

				//todo reducir la quantity
				for (int i = 0; i < actioner_entity->usable_items.size(); i++)
				{
					if (actioner_entity->usable_items.at(i).action.type == action_to_perform.type) {
						actioner_entity->usable_items.at(i).quantity--;
						if (actioner_entity->usable_items.at(i).quantity == 0)
							actioner_entity->usable_items.erase(actioner_entity->usable_items.cbegin() + i);
						break;
					}
				}

				actioner_entity->throw_object.Reset();

				bool critical = false;

				int damage_to_deal = action_to_perform.mana_points_effect;
				damage_to_deal = damage_to_deal;
				receiver_entity->SetCurrentManaPoints(receiver_entity->GetCurrentManaPoints() + damage_to_deal);
				//receiver_entity->animation = &receiver_entity->hit;
				App->combat->UpdateHPBarOfEntity(receiver_entity, damage_to_deal);
				std::string tmp_dmg = std::to_string(damage_to_deal);

				App->gui->AddUIFloatingValue(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2), receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h - 10, tmp_dmg, { 0,255,0,255 }, 14, nullptr, nullptr);


				fPoint  posP = { (float)(receiver_entity->position.x + (receiver_entity->animation->GetCurrentFrame().w / 2)), (float)(receiver_entity->position.y - receiver_entity->animation->GetCurrentFrame().h / 2) };
				App->psystem->AddEmiter(posP, EmitterType::EMITTER_TYPE_LOWER_HEALTH);
			}
		}
		break;


		default:
			break;
		}
	}
	else {
		ret = true;
	}
		

	return ret;
}

bool PerformActionToEntity::HaveObjective()
{

	if (receiver_entity->GetCurrentHealthPoints() == 0) {
		if (actioner_entity->type == ELF || actioner_entity->type == CLERIC || actioner_entity->type == WARRIOR || actioner_entity->type == DWARF) {
			for (int i = 0; i < App->combat->enemies.size(); i++)
			{
				receiver_entity = App->combat->enemies.at(i);
				if (receiver_entity->GetCurrentHealthPoints() != 0)
					break;
			}
			if (receiver_entity->GetCurrentHealthPoints() == 0)
				return false;
		}
		else {
			for (int i = 0; i < App->combat->heroes.size(); i++)
			{
				receiver_entity = App->combat->heroes.at(i);
				if (receiver_entity->GetCurrentHealthPoints() != 0)
					break;
			}
			if (receiver_entity->GetCurrentHealthPoints() == 0)
				return false;
		}
	}

	return true;
}

bool PerformActionToEntity::HaveTeamObjective()
{
	if (receiver_entity->GetCurrentHealthPoints() == 0) {

		for (int i = 0; i < App->combat->heroes.size(); i++)
		{
			receiver_entity = App->combat->heroes.at(i);
			if (receiver_entity->GetCurrentHealthPoints() != 0)
				break;
		}
		if (receiver_entity->GetCurrentHealthPoints() == 0)
			return false;

	}
	return true;
}

bool ctTaskManager::Update(float dt)
{
	TaskOrderer();

	bool ret = false;

	ret = DoTask();

	return ret;
}

bool ctTaskManager::Start()
{
	/* initialize random seed: */
	srand(time(NULL));
	return true;
}

bool ctTaskManager::CleanUp()
{
	while (TaskQueue.size() != 0)
	{
		TaskQueue.pop_front();
	}
	return true;
}

void ctTaskManager::PerformAllTheTasks()
{
	if (aux_task == nullptr && TaskQueue.size() != 0)
	{
		aux_task = TaskQueue.front();
		TaskQueue.pop_front();
	}
}

void ctTaskManager::OrderTasksByEntities(std::vector<Entity*> turn_priority_entity)
{
	std::list<Task*> task_tmp;
	
	int i = 0;
	
	while (i < turn_priority_entity.size())
	{
		for (std::list<Task*>::const_iterator it_list = TaskQueue.begin(); it_list != TaskQueue.end(); ++it_list)
		{
			if ((*it_list)->performed_by == turn_priority_entity[i])
			{
				task_tmp.push_back((*it_list));
			}

		}

		i++;
	}
	TaskQueue = task_tmp;
}

void ctTaskManager::DeleteTasksFromEntity(Entity * entity)
{
	std::list<Task*>::const_iterator it_list = TaskQueue.begin();

	while (it_list != TaskQueue.end()) {
		if ((*it_list)->performed_by == entity) {
			TaskQueue.remove((*it_list));
		}
		it_list++;
	}
}

bool ctTaskManager::AddTask(Task * task)
{
	TaskQueue.push_back(task);
	return true;
}

bool ctTaskManager::DoTask()
{
	if (aux_task != nullptr)
	{

		if (aux_task->Execute())
		{
			if (TaskQueue.size() != 0)
			{
				aux_task = TaskQueue.front();
				TaskQueue.pop_front();
			}
			else 
				aux_task = nullptr;

		}
	}
	return true;
}

bool ctTaskManager::TaskOrderer()
{
	
	return true;
}

bool MoveAvatarsToPosition::Execute()
{
	bool ret = false;


	if (((entity_to_move->position.x + 10) >= position_to_go.x && (entity_to_move->position.x - 10) <= position_to_go.x) && ((entity_to_move->position.y + 10) >= position_to_go.y && (entity_to_move->position.y - 10) <= position_to_go.y))
	{
		//entity_to_move->animation = &entity_to_move->idle;
		ret = true;
	}
	else {
		if (entity_to_move->position.x < position_to_go.x)
		{
			entity_to_move->animation = &entity_to_move->run_forward;
			entity_to_move->position.x += 1;
		}

		else if (entity_to_move->position.x > position_to_go.x)
		{
			entity_to_move->animation = &entity_to_move->run_forward;
			entity_to_move->position.x -= 1;
		}

		if (entity_to_move->position.y < position_to_go.y)
		{
			entity_to_move->animation = &entity_to_move->run_forward;
			entity_to_move->position.y += 1;
		}

		else if (entity_to_move->position.y > position_to_go.y)
		{
			entity_to_move->animation = &entity_to_move->run_forward;
			entity_to_move->position.y -= 1;
		}
	}




	return ret;
}
