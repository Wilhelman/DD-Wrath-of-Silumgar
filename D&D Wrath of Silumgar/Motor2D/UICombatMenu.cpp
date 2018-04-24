#include "ctApp.h"
#include "UICombatMenu.h"
#include "ctLog.h"
#include "ctInput.h"
#include "ctAudio.h"
#include "ctCombat.h"
#include "Entity.h"
#include "ctTaskManager.h"
#include "ctPerfTimer.h"
#include "ctFadeToBlack.h"

UICombatMenu::UICombatMenu(Entity* entity, int x, int y, UI_Type type, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->entity = entity;
	this->callback = callback;
	if (entity->type == CLERIC) {
		background = App->gui->AddUIImage(x, y, { 1141, 338, 60, 90 }, callback);
		explanation_background = App->gui->AddUIImage(explanationPos.x, explanationPos.y, { 3, 622, 226, 33 }, callback);
	}
	if (entity->type == WARRIOR) {
		background = App->gui->AddUIImage(x, y, { 1261, 209, 60, 90 }, callback);
		explanation_background = App->gui->AddUIImage(explanationPos.x, explanationPos.y, { 3, 552, 226, 33 }, callback);
	}
	if (entity->type == DWARF) {
		background = App->gui->AddUIImage(x, y, { 1201, 338, 60, 90 }, callback);
		explanation_background = App->gui->AddUIImage(explanationPos.x, explanationPos.y, { 3, 762, 226, 33 }, callback);
	}
	if (entity->type == ELF) {
		background = App->gui->AddUIImage(x, y, { 1261, 299, 60, 90 }, callback);
		explanation_background = App->gui->AddUIImage(explanationPos.x, explanationPos.y, { 3, 693, 226, 33 }, callback);
	}
	//background = App->gui->AddUIImage(x, y, { 1260, 208, 60, 90 }, callback);
	attack_label = App->gui->AddUILabel(x + main_label1_pos.x, y + main_label1_pos.y, "Attack", { 255,255,255,255 }, font_size, nullptr, background);
	if (entity->abilities.size() != 0) {
		abilities_label = App->gui->AddUILabel(x + main_label2_pos.x, y + main_label2_pos.y, "Abilities", { 255,255,255,255 }, font_size, nullptr, background);
	}
	else {
		abilities_label = App->gui->AddUILabel(x + main_label2_pos.x, y + main_label2_pos.y, "Abilities", { 255,0,0,255 }, font_size, nullptr, background);
	}
	items_label = App->gui->AddUILabel(x + main_label3_pos.x, y + main_label3_pos.y, "Items", { 255,0,0,255 }, font_size, nullptr, background);
	attack_label->current_state = STATE_FOCUSED;
	explanation_label = App->gui->AddUITextBox(2, 1, 15, 224, ATTACKEXPLANATION, {255,255,255,255}, nullptr, Second_Font);
	explanation_label->SetParent(explanation_background);
	arrow = App->gui->AddUIImage(x - (main_label1_pos.x / 1.5), y, { 1333, 272, 7, 14 }, callback, background);
	main_labels.push_back(attack_label);
	if (entity->abilities.size()!=0) {
		main_labels.push_back(abilities_label);
	}
	//main_labels.push_back(items_label);
	arrow->SetParent(attack_label);
	upper_points_pos.x = x + 8;
	upper_points_pos.y = y - 20;
	lower_points_pos.x = x + 8;
	lower_points_pos.y = y + 47;
	LOG("UICombatMenu created in x:%i, y:%i", x, y);

	selected_enemy = App->combat->enemies.begin();
	selected_ally = App->combat->heroes.begin();
}

UICombatMenu::~UICombatMenu() {
	App->gui->DeleteUIElement(*enemy_select_arrow);
	enemy_select_arrow = nullptr;
	App->gui->DeleteUIElement(*arrow);
	arrow = nullptr;
	App->gui->DeleteUIElement(*attack_label);
	attack_label = nullptr;
	App->gui->DeleteUIElement(*abilities_label);
	abilities_label = nullptr;
	App->gui->DeleteUIElement(*items_label);
	items_label = nullptr;
	App->gui->DeleteUIElement(*upper_points);
	upper_points = nullptr;
	App->gui->DeleteUIElement(*lower_points);
	lower_points = nullptr;
	App->gui->DeleteUIElement(*background);
	background = nullptr;
	App->gui->DeleteUIElement(*explanation_label);
	explanation_label = nullptr;
	App->gui->DeleteUIElement(*explanation_background);
	explanation_background = nullptr;

	App->gui->DeleteUIElement(*items_label);
	items_label = nullptr;

	main_labels.clear();
	abilities.clear();
	items.clear();
	names.clear();
	entity_actions.clear();
	entity = nullptr;
}

void UICombatMenu::Update()
{
	if (App->fadeToBlack->FadeIsOver() == true) {
		//Go down
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && selecting_enemy == false || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN) {

			if (main_labels.size() != 0) {
				NavigateDown(main_labels);
			}
			else if (abilities.size() != 0) {
				NavigateDown(abilities);
			}
			else if (items.size() != 0) {
				NavigateDown(items);
			}
		}
		//Go up
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && selecting_enemy == false || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
			if (main_labels.size() != 0) {
				NavigateUp(main_labels);
			}
			else if (abilities.size() != 0) {
				NavigateUp(abilities);
			}
			else if (items.size() != 0) {
				NavigateUp(items);
			}
		}
		//Execute
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && selecting_enemy == false || App->input->gamepad.A == GAMEPAD_STATE::PAD_BUTTON_DOWN && selecting_enemy == false) {
			App->audio->PlayFx(App->audio->cm_select_fx);
			execute_comand_time.Start();
			if (main_labels.size() != 0) {
				ExecuteComand(main_labels);
			}
			else if (abilities.size() != 0) {
				ExecuteComand(abilities);
			}
			else if (items.size() != 0) {
				ExecuteComand(items);
			}
		}
		//Go back to the start combat menu
		if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN && selecting_enemy == false || App->input->gamepad.B == GAMEPAD_STATE::PAD_BUTTON_DOWN && selecting_enemy == false) {

			if (main_labels.size() != 0) {
				App->combat->SelectWithPreviousHeroe();
			}
			else {
				GoBack();
			}
		}

		if (names.size() != 0 && lower_points == nullptr && names_iterator < names.size() - 1) {
			lower_points = App->gui->AddUILabel(lower_points_pos.x, lower_points_pos.y, "...", { 255,255,255,255 }, 50, nullptr, nullptr, Second_Font);
		}
		if (names.size() != 0 && lower_points != nullptr && names_iterator == names.size() - 1) {
			App->gui->DeleteUIElement(*lower_points);
			lower_points = nullptr;
		}
		if (names.size() != 0 && upper_points == nullptr && names_iterator > 2) {
			upper_points = App->gui->AddUILabel(upper_points_pos.x, upper_points_pos.y, "...", { 255,255,255,255 }, 50, nullptr, nullptr, Second_Font);
		}
		if (names.size() != 0 && upper_points != nullptr && names_iterator < 3) {
			App->gui->DeleteUIElement(*upper_points);
			upper_points = nullptr;
		}

		//Select Enemy to attack
		if (main_labels.size() != 0 && selecting_enemy == true) {
			SelectEnemy(main_labels);
		}
		else if (abilities.size() != 0 && selecting_enemy == true) {
			if (entity->abilities.at(names_iterator).objective == ENEMIES) {
				SelectEnemy(abilities);
			}
			else {
				SelectAlly(abilities);
			}
		}

		if (executed_command == true) {
			if (abilities.size() != 0) {
				ChangeExplanation(abilities);
			}
			else if (items.size() != 0) {
				ChangeExplanation(items);
			}
		}

		executed_command = false;
	}
}


void UICombatMenu::NavigateDown(std::vector<UIElement*> &current_vector) {
	std::vector<UIElement*>::const_iterator it_vector = current_vector.begin();

	UIElement* it_focused_element = nullptr;

	bool focused_found = false;

	for (int i = 0; i < current_vector.size() && focused_found ==false; i++) {
		if (current_vector.at(i)->current_state == STATE_FOCUSED)
			it_focused_element = current_vector.at(i);
	}


	if (current_vector == main_labels) {
		while (it_vector != current_vector.end()) {
			if ((*it_vector)->current_state == STATE_FOCUSED) {
				if ((*it_vector) != current_vector.back()) {
					(*it_vector)->current_state = STATE_NORMAL;
					it_vector++;
					(*it_vector)->current_state = STATE_FOCUSED;
					arrow->SetParent((*it_vector));
					App->audio->PlayFx(App->audio->cm_move_fx);
					break;
				}
			}
			it_vector++;
		}
	}
	else {
		iPoint backgroundPos = background->GetScreenPosition();
		while (it_vector != current_vector.end()) {
			if ((*it_vector)->current_state == STATE_FOCUSED) {
				if ((*it_vector) != current_vector.back()) {
					(*it_vector)->current_state = STATE_NORMAL;
					it_vector++;
					(*it_vector)->current_state = STATE_FOCUSED;
					arrow->SetParent((*it_vector));
					App->audio->PlayFx(App->audio->cm_move_fx);
					break;
				}
			}
			it_vector++;
		}
		if (names.size() > current_vector.size() && it_vector == current_vector.end() && names_iterator < names.size() - 1) {
			for (int i = 0; i < current_vector.size(); i++) {
				App->gui->DeleteUIElement(*current_vector.at(i));
				current_vector.at(i) = nullptr;
			}
			current_vector.clear();
			if (entity->GetCurrentManaPoints() >= entity->abilities.at(names_iterator - 1).mana_points_effect_to_himself) {
				current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(names_iterator - 1), { 255,255,255,255 }, font_size, nullptr, background));
			}
			else {
				current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(names_iterator - 1), { 255,0,0,255 }, font_size, nullptr, background));
			}
			if (entity->GetCurrentManaPoints() >= entity->abilities.at(names_iterator).mana_points_effect_to_himself) {
				current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label2_pos.x, backgroundPos.y + label2_pos.y, names.at(names_iterator), { 255,255,255,255 }, font_size, nullptr, background));
			}
			else {
				current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label2_pos.x, backgroundPos.y + label2_pos.y, names.at(names_iterator), { 255,0,0,255 }, font_size, nullptr, background));
			}
			if (entity->GetCurrentManaPoints() >= entity->abilities.at(names_iterator + 1).mana_points_effect_to_himself) {
				current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label3_pos.x, backgroundPos.y + label3_pos.y, names.at(names_iterator + 1), { 255,255,255,255 }, font_size, nullptr, background));
			}
			else {
				current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label3_pos.x, backgroundPos.y + label3_pos.y, names.at(names_iterator + 1), { 255,0,0,255 }, font_size, nullptr, background));
			}
			names_iterator++;
			current_vector.back()->current_state = STATE_FOCUSED;
			arrow->SetParent(current_vector.back());
			App->audio->PlayFx(App->audio->cm_move_fx);
		}
	}

	focused_found = false;

	for (int i = 0; i < current_vector.size() && focused_found == false; i++) {
		if (current_vector.at(i)->current_state == STATE_FOCUSED)
			if(it_focused_element != current_vector.at(i))
				ChangeExplanation(current_vector);
	}
	
}


void UICombatMenu::NavigateUp(std::vector<UIElement*> &current_vector) {
	std::vector<UIElement*>::const_iterator it_vector = current_vector.begin();

	UIElement* it_focused_element = nullptr;

	bool focused_found = false;

	for (int i = 0; i < current_vector.size() && focused_found == false; i++) {
		if (current_vector.at(i)->current_state == STATE_FOCUSED)
			it_focused_element = current_vector.at(i);
	}


	if (current_vector == main_labels) {
		while (it_vector != current_vector.end()) {
			if ((*it_vector)->current_state == STATE_FOCUSED) {
				if ((*it_vector) != current_vector.front()) {
					(*it_vector)->current_state = STATE_NORMAL;
					it_vector--;
					(*it_vector)->current_state = STATE_FOCUSED;
					arrow->SetParent((*it_vector));
					App->audio->PlayFx(App->audio->cm_move_fx);
					break;
				}
			}
			it_vector++;
		}
	}
	else {
		iPoint backgroundPos = background->GetScreenPosition();
		while (it_vector != current_vector.end()) {
			if ((*it_vector)->current_state == STATE_FOCUSED) {
				if ((*it_vector) != current_vector.front()) {
					(*it_vector)->current_state = STATE_NORMAL;
					it_vector--;
					(*it_vector)->current_state = STATE_FOCUSED;
					arrow->SetParent((*it_vector));
					App->audio->PlayFx(App->audio->cm_move_fx);
					break;
				}
			}
			it_vector++;
		}
		if (names.size() > current_vector.size() && it_vector == current_vector.end() && names_iterator > 2) {
			for (int i = 0; i < current_vector.size(); i++) {
				App->gui->DeleteUIElement(*current_vector.at(i));
				current_vector.at(i) = nullptr;
			}
			current_vector.clear();
			if (entity->GetCurrentManaPoints() >= entity->abilities.at(names_iterator - 3).mana_points_effect_to_himself) {
				current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(names_iterator - 3), { 255,255,255,255 }, font_size, nullptr, background));
			}
			else {
				current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(names_iterator - 3), { 255,0,0,255 }, font_size, nullptr, background));
			}
			if (entity->GetCurrentManaPoints() >= entity->abilities.at(names_iterator - 2).mana_points_effect_to_himself) {
				current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label2_pos.x, backgroundPos.y + label2_pos.y, names.at(names_iterator - 2), { 255,255,255,255 }, font_size, nullptr, background));
			}
			else {
				current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label2_pos.x, backgroundPos.y + label2_pos.y, names.at(names_iterator - 2), { 255,0,0,255 }, font_size, nullptr, background));
			}
			if (entity->GetCurrentManaPoints() >= entity->abilities.at(names_iterator - 1).mana_points_effect_to_himself) {
				current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label3_pos.x, backgroundPos.y + label3_pos.y, names.at(names_iterator - 1), { 255,255,255,255 }, font_size, nullptr, background));
			}
			else {
				current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label3_pos.x, backgroundPos.y + label3_pos.y, names.at(names_iterator - 1), { 255,0,0,255 }, font_size, nullptr, background));
			}
			names_iterator--;
			current_vector.front()->current_state = STATE_FOCUSED;
			arrow->SetParent(current_vector.front());
			App->audio->PlayFx(App->audio->cm_move_fx);
		}
	}

	focused_found = false;

	for (int i = 0; i < current_vector.size() && focused_found == false; i++) {
		if (current_vector.at(i)->current_state == STATE_FOCUSED)
			if (it_focused_element != current_vector.at(i))
				ChangeExplanation(current_vector);
	}

}


void UICombatMenu::ExecuteComand(std::vector<UIElement*> &current_vector) {
	for (int i = 0; i < current_vector.size(); i++) {
		if (current_vector.at(i)->current_state == STATE_FOCUSED) {
			current_vector.at(i)->current_state = STATE_EXECUTED;
		}
	}

	if (current_vector == main_labels) {
		if (attack_label->current_state == STATE_EXECUTED) {
			//App->gui->DeleteUIElement(*arrow);
			//arrow = nullptr;
			//App->gui->DeleteUIElement(*background);
			//background = nullptr;
			//App->gui->DeleteUIElement(*upper_points);
			//upper_points = nullptr;
			//App->gui->DeleteUIElement(*lower_points);
			//lower_points = nullptr;
			selecting_enemy = true;
			//Call function to attack
		}
		else if (abilities_label->current_state == STATE_EXECUTED) {
			LoadAbilities();
		}
		else if (items_label->current_state == STATE_EXECUTED) {
			LoadItems();
		}
	}

	if (current_vector == abilities) {
		if (entity->GetCurrentManaPoints()>=entity->abilities.at(names_iterator).mana_points_effect_to_himself) {
			selecting_enemy = true;
		}
		/*App->gui->DeleteUIElement(*arrow);
		arrow = nullptr;
		App->gui->DeleteUIElement(*background);
		background = nullptr;
		App->gui->DeleteUIElement(*upper_points);
		upper_points = nullptr;
		App->gui->DeleteUIElement(*lower_points);
		lower_points = nullptr;*/
		//Use Abilitie
	}

	if (current_vector == items) {
		App->gui->DeleteUIElement(*arrow);
		arrow = nullptr;
		App->gui->DeleteUIElement(*background);
		background = nullptr;
		App->gui->DeleteUIElement(*upper_points);
		upper_points = nullptr;
		App->gui->DeleteUIElement(*lower_points);
		lower_points = nullptr;
		for (int i = 0; i < items.size(); i++) {
			App->gui->DeleteUIElement(*items.at(i));
		}

		items.clear();
		//Use Item
	}

	if (abilities.size()>0 || items.size()>0) {
		for (int i = 0; i < main_labels.size(); i++) {
			App->gui->DeleteUIElement(*main_labels.at(i));
		}

		main_labels.clear();
		//DELETE WHEN ADDING ITEMS OPTION BACK
		App->gui->DeleteUIElement(*items_label);
		items_label = nullptr;
	}

	executed_command = true;

}


void UICombatMenu::LoadAbilities() {
	iPoint backgroundPos = background->GetScreenPosition();
	names_iterator = 0;

	if (entity->abilities.size() != 0) {
		std::vector<Action>::const_iterator it_vector = entity->abilities.begin();
		while (it_vector != entity->abilities.end()) {
			names.push_back((*it_vector).name);
			entity_actions.push_back(*it_vector);
			it_vector++;
		}
	}
	else {
		names.push_back("0 Abilities");
	}

	//names.push_back("Ability1");
	//names.push_back("Ability2");
	//names.push_back("Ability3");
	//names.push_back("Ability4");
	//names.push_back("Ability5");
	//names.push_back("Ability6");
	//names.push_back("Ability7");
	//names.push_back("Ability8");
	//names.push_back("Ability9");

	if (names.size() > 3) {
		if (entity->GetCurrentManaPoints() >= entity->abilities.at(0).mana_points_effect_to_himself) {
			abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(0), { 255,255,255,255 }, font_size, nullptr, background));
		}
		else {
			abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(0), { 255,0,0,255 }, font_size, nullptr, background));
		}
		if (entity->GetCurrentManaPoints() >= entity->abilities.at(1).mana_points_effect_to_himself) {
			abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label2_pos.x, backgroundPos.y + label2_pos.y, names.at(1), { 255,255,255,255 }, font_size, nullptr, background));
		}
		else {
			abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label2_pos.x, backgroundPos.y + label2_pos.y, names.at(1), { 255,0,0,0 }, font_size, nullptr, background));
		}
		if (entity->GetCurrentManaPoints() >= entity->abilities.at(2).mana_points_effect_to_himself) {
			abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label3_pos.x, backgroundPos.y + label3_pos.y, names.at(2), { 255,255,255,255 }, font_size, nullptr, background));
		}
		else {
			abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label3_pos.x, backgroundPos.y + label3_pos.y, names.at(2), { 255,255,255,255 }, font_size, nullptr, background));
		}
		names_iterator = 2;
	}
	else if (names.size() == 2) {
		if (entity->GetCurrentManaPoints() >= entity->abilities.at(0).mana_points_effect_to_himself) {
			abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(0), { 255,255,255,255 }, font_size, nullptr, background));
		}
		else {
			abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(0), { 255,0,0,255 }, font_size, nullptr, background));
		}
		if (entity->GetCurrentManaPoints() >= entity->abilities.at(1).mana_points_effect_to_himself) {
			abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label2_pos.x, backgroundPos.y + label2_pos.y, names.at(1), { 255,255,255,255 }, font_size, nullptr, background));
		}
		else {
			abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label2_pos.x, backgroundPos.y + label2_pos.y, names.at(1), { 255,0,0,0 }, font_size, nullptr, background));
		}
		names_iterator = 1;
	}
	else if (names.size() == 1) {
		if (entity->GetCurrentManaPoints() >= entity->abilities.at(0).mana_points_effect_to_himself) {
			abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(0), { 255,255,255,255 }, font_size, nullptr, background));
		}
		else {
			abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(0), { 255,0,0,255 }, font_size, nullptr, background));
		}
		names_iterator = 0;
	}

	if (abilities.size() > 0) {
		abilities.front()->current_state = STATE_FOCUSED;
		arrow->SetParent(abilities.front());
	}

}


void UICombatMenu::LoadItems() {
	iPoint backgroundPos = background->GetScreenPosition();
	names_iterator = 0;
	names.push_back("Item1");
	names.push_back("Item2");
	names.push_back("Item3");
	names.push_back("Item4");
	names.push_back("Item5");

	if (names.size() > 3) {
		items.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(0), { 255,255,255,255 }, font_size, nullptr, background));
		items.push_back(App->gui->AddUILabel(backgroundPos.x + label2_pos.x, backgroundPos.y + label2_pos.y, names.at(1), { 255,255,255,255 }, font_size, nullptr, background));
		items.push_back(App->gui->AddUILabel(backgroundPos.x + label3_pos.x, backgroundPos.y + label3_pos.y, names.at(2), { 255,255,255,255 }, font_size, nullptr, background));
		names_iterator = 2;
	}
	else if (names.size() == 2) {
		items.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(0), { 255,255,255,255 }, font_size, nullptr, background));
		items.push_back(App->gui->AddUILabel(backgroundPos.x + label2_pos.x, backgroundPos.y + label2_pos.y, names.at(1), { 255,255,255,255 }, font_size, nullptr, background));
		names_iterator = 1;
	}
	else if (names.size() == 1) {
		items.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(0), { 255,255,255,255 }, font_size, nullptr, background));
		names_iterator = 0;
	}

	if (items.size() > 0) {
		items.front()->current_state = STATE_FOCUSED;
		arrow->SetParent(items.front());
	}

}


void UICombatMenu::GoBack() {
	if (abilities.size()>0) {
		for (int i = 0; i < abilities.size(); i++) {
			App->gui->DeleteUIElement(*abilities.at(i));
		}
		abilities.clear();
		names.clear();

		App->audio->PlayFx(App->audio->cm_back_fx);
	}
	else if (items.size()>0) {
		for (int i = 0; i < items.size(); i++) {
			App->gui->DeleteUIElement(*items.at(i));
		}
		items.clear();
		names.clear();

		App->audio->PlayFx(App->audio->cm_back_fx);
	}

	if (main_labels.size() == 0) {
		iPoint backgroundPos = background->GetScreenPosition();

		attack_label = App->gui->AddUILabel(backgroundPos.x + main_label1_pos.x, backgroundPos.y + main_label1_pos.y, "Attack", { 255,255,255,255 }, font_size, nullptr, background);
		abilities_label = App->gui->AddUILabel(backgroundPos.x + main_label2_pos.x, backgroundPos.y + main_label2_pos.y, "Abilities", { 255,255,255,255 }, font_size, nullptr, background);
		items_label = App->gui->AddUILabel(backgroundPos.x + main_label3_pos.x, backgroundPos.y + main_label3_pos.y, "Items", { 255,0,0,255 }, font_size, nullptr, background);
		attack_label->current_state = STATE_FOCUSED;
		arrow->SetParent(attack_label);
		main_labels.push_back(attack_label);
		main_labels.push_back(abilities_label);
		//main_labels.push_back(items_label);

		App->audio->PlayFx(App->audio->cm_back_fx);
	}

	if (upper_points != nullptr) {
		App->gui->DeleteUIElement(*upper_points);
		upper_points = nullptr;
	}


	if (lower_points != nullptr) {
		App->gui->DeleteUIElement(*lower_points);
		lower_points = nullptr;
	}

	ChangeExplanation(main_labels);

}

void UICombatMenu::SelectEnemy(std::vector<UIElement*> &current_vector) {
	if (enemy_select_arrow == nullptr) {
		while ((*selected_enemy)->GetCurrentHealthPoints() <= 0) {
			if (selected_enemy != App->combat->enemies.end()) {
				selected_enemy++;
			}
			else {
				selected_enemy = App->combat->enemies.begin();
			}
		}
		enemy_select_arrow = App->gui->AddUIImage((*selected_enemy)->position.x + ((*selected_enemy)->idle.GetCurrentFrame().w / 2), (*selected_enemy)->position.y - (*selected_enemy)->idle.GetCurrentFrame().h - 5, { 1328, 289, 14, 7 }, callback, nullptr);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		if (selected_enemy != App->combat->enemies.end()) {
			selected_enemy++;
		}
		if (selected_enemy == App->combat->enemies.end()) {
			selected_enemy = App->combat->enemies.begin();
		}
		if (enemy_select_arrow != nullptr) {
			App->gui->DeleteUIElement(*enemy_select_arrow);
			enemy_select_arrow = nullptr;
		}
		while ((*selected_enemy)->GetCurrentHealthPoints()<=0){
			if (selected_enemy != App->combat->enemies.end()) {
				selected_enemy++;
			}
			else {
				selected_enemy = App->combat->enemies.begin();
			}
		}
		enemy_select_arrow = App->gui->AddUIImage((*selected_enemy)->position.x + ((*selected_enemy)->idle.GetCurrentFrame().w / 2), (*selected_enemy)->position.y - (*selected_enemy)->idle.GetCurrentFrame().h - 5, { 1328, 289, 14, 7 }, callback, nullptr);
		App->audio->PlayFx(App->audio->cm_move_fx);
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		if (selected_enemy != App->combat->enemies.begin()-1) {
			selected_enemy--;
		}
		if (selected_enemy == App->combat->enemies.begin()-1) {
			while (selected_enemy != App->combat->enemies.end()-1) {
				selected_enemy++;
			}
		}
		while ((*selected_enemy)->GetCurrentHealthPoints() <= 0) {
			if (selected_enemy != App->combat->enemies.begin()) {
				selected_enemy--;
			}
			else {
				while (selected_enemy != App->combat->enemies.end() - 1) {
					selected_enemy++;
				}
			}
		}
		if (enemy_select_arrow != nullptr) {
			App->gui->DeleteUIElement(*enemy_select_arrow);
			enemy_select_arrow = nullptr;
		}
		enemy_select_arrow = App->gui->AddUIImage((*selected_enemy)->position.x + ((*selected_enemy)->idle.GetCurrentFrame().w / 2), (*selected_enemy)->position.y - (*selected_enemy)->idle.GetCurrentFrame().h - 5, { 1328, 289, 14, 7 }, callback, nullptr);
		App->audio->PlayFx(App->audio->cm_move_fx);
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && execute_comand_time.ReadMs() >= 500 || App->input->gamepad.A == GAMEPAD_STATE::PAD_BUTTON_DOWN && execute_comand_time.ReadMs() >= 500) {
		App->gui->DeleteUIElement(*arrow);
		arrow = nullptr;
		App->gui->DeleteUIElement(*background);
		background = nullptr;
		App->gui->DeleteUIElement(*upper_points);
		upper_points = nullptr;
		App->gui->DeleteUIElement(*lower_points);
		lower_points = nullptr;

		for (int i = 0; i < current_vector.size(); i++) {
			App->gui->DeleteUIElement(*current_vector.at(i));
		}

		//App->audio->PlayFx(combat_menu_select_fx);
		current_vector.clear();

		selecting_enemy = false;

		//perform tasks!
		//(*selected_enemy)
		if (attack_label->current_state == STATE_EXECUTED) {
			App->task_manager->AddTask(new MoveToEntity(entity, (*selected_enemy), -20));
			App->task_manager->AddTask(new PerformActionToEntity(entity, entity->default_attack, (*selected_enemy)));
			App->task_manager->AddTask(new MoveToInitialPosition(entity));
		}
		else if(abilities_label->current_state == STATE_EXECUTED && entity->abilities.size() != 0){
			if (entity->GetCurrentManaPoints() >= entity->abilities.at(names_iterator).mana_points_effect_to_himself) {
				if (entity->abilities.at(names_iterator).name == "Mindblown") {
					App->task_manager->AddTask(new PerformActionToEntity(entity, entity->abilities.at(names_iterator), (*selected_enemy)));
					App->task_manager->AddTask(new MoveToInitialPosition(entity));
				}
				else {
					App->task_manager->AddTask(new MoveToEntity(entity, (*selected_enemy), -20));
					App->task_manager->AddTask(new PerformActionToEntity(entity, entity->abilities.at(names_iterator), (*selected_enemy)));
					App->task_manager->AddTask(new MoveToInitialPosition(entity));
				}
			}
			else{
				selected_enemy = App->combat->enemies.begin();
				App->gui->DeleteUIElement(*enemy_select_arrow);
				enemy_select_arrow = nullptr;
				selecting_enemy = false;
				for (int i = 0; i < current_vector.size(); i++) {
					if (current_vector.at(i)->current_state == STATE_EXECUTED) {
						current_vector.at(i)->current_state = STATE_FOCUSED;
					}
				}
			}
		}
	}


	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN || App->input->gamepad.B == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		
		selected_enemy = App->combat->enemies.begin();
		App->gui->DeleteUIElement(*enemy_select_arrow);
		enemy_select_arrow = nullptr;
		selecting_enemy = false;
		for (int i = 0; i < current_vector.size(); i++) {
			if (current_vector.at(i)->current_state == STATE_EXECUTED) {
				current_vector.at(i)->current_state = STATE_FOCUSED;
			}
		}
	}
	
}

void UICombatMenu::SelectAlly(std::vector<UIElement*> &current_vector) {
	if (enemy_select_arrow == nullptr) {
		while ((*selected_ally)->GetCurrentHealthPoints() <= 0) {
			if (selected_ally != App->combat->heroes.end()) {
				selected_ally++;
			}
			else {
				selected_ally = App->combat->heroes.begin();
			}
		}
		enemy_select_arrow = App->gui->AddUIImage((*selected_ally)->position.x + ((*selected_ally)->idle.GetCurrentFrame().w / 2), (*selected_ally)->position.y - (*selected_ally)->idle.GetCurrentFrame().h - 5, { 1328, 289, 14, 7 }, callback, nullptr);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		if (selected_ally != App->combat->heroes.end()) {
			selected_ally++;
		}
		if (selected_ally == App->combat->heroes.end()) {
			selected_ally = App->combat->heroes.begin();
		}
		if (enemy_select_arrow != nullptr) {
			App->gui->DeleteUIElement(*enemy_select_arrow);
			enemy_select_arrow = nullptr;
		}
		while ((*selected_ally)->GetCurrentHealthPoints() <= 0) {
			if (selected_ally != App->combat->heroes.end()) {
				selected_ally++;
			}
			else {
				selected_ally = App->combat->heroes.begin();
			}
		}
		enemy_select_arrow = App->gui->AddUIImage((*selected_ally)->position.x + ((*selected_ally)->idle.GetCurrentFrame().w / 2), (*selected_ally)->position.y - (*selected_ally)->idle.GetCurrentFrame().h - 5, { 1328, 289, 14, 7 }, callback, nullptr);
		App->audio->PlayFx(App->audio->cm_move_fx);
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		if (selected_ally != App->combat->heroes.begin()-1) {
			selected_ally--;
		}
		if (selected_ally == App->combat->heroes.begin()-1) {
			while (selected_ally != App->combat->heroes.end()-1) {
				selected_ally++;
			}
		}
		while ((*selected_ally)->GetCurrentHealthPoints() <= 0) {
			if (selected_ally != App->combat->heroes.begin()) {
				selected_ally--;
			}
			else {
				while (selected_ally != App->combat->heroes.end() - 1) {
					selected_ally++;
				}
			}
		}
		if (enemy_select_arrow != nullptr) {
			App->gui->DeleteUIElement(*enemy_select_arrow);
			enemy_select_arrow = nullptr;
		}
		enemy_select_arrow = App->gui->AddUIImage((*selected_ally)->position.x + ((*selected_ally)->idle.GetCurrentFrame().w / 2), (*selected_ally)->position.y - (*selected_ally)->idle.GetCurrentFrame().h - 5, { 1328, 289, 14, 7 }, callback, nullptr);
		App->audio->PlayFx(App->audio->cm_move_fx);
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && execute_comand_time.ReadMs() >= 500 || App->input->gamepad.A == GAMEPAD_STATE::PAD_BUTTON_DOWN && execute_comand_time.ReadMs() >= 500) {
		App->gui->DeleteUIElement(*arrow);
		arrow = nullptr;
		App->gui->DeleteUIElement(*background);
		background = nullptr;
		App->gui->DeleteUIElement(*upper_points);
		upper_points = nullptr;
		App->gui->DeleteUIElement(*lower_points);
		lower_points = nullptr;

		for (int i = 0; i < current_vector.size(); i++) {
			App->gui->DeleteUIElement(*current_vector.at(i));
		}

		//App->audio->PlayFx(combat_menu_select_fx);
		current_vector.clear();

		selecting_enemy = false;

		//perform tasks!
		//(*selected_ally)
		if (attack_label->current_state == STATE_EXECUTED) {
			App->task_manager->AddTask(new MoveToEntity(entity, (*selected_ally), -20));
			App->task_manager->AddTask(new PerformActionToEntity(entity, entity->default_attack, (*selected_ally)));
			App->task_manager->AddTask(new MoveToInitialPosition(entity));
		}
		else if (abilities_label->current_state == STATE_EXECUTED && entity->abilities.size() != 0) {
			if (entity->GetCurrentManaPoints() >= entity->abilities.at(names_iterator).mana_points_effect_to_himself) {
				if (entity->abilities.at(names_iterator).name == "Heal") {
					App->task_manager->AddTask(new PerformActionToEntity(entity, entity->abilities.at(names_iterator), (*selected_ally)));
					App->task_manager->AddTask(new MoveToInitialPosition(entity));
				}
				else {
					App->task_manager->AddTask(new MoveToEntity(entity, (*selected_ally), -20));
					App->task_manager->AddTask(new PerformActionToEntity(entity, entity->abilities.at(names_iterator), (*selected_ally)));
					App->task_manager->AddTask(new MoveToInitialPosition(entity));
				}
			}
			else {
				selected_ally = App->combat->heroes.begin();
				App->gui->DeleteUIElement(*enemy_select_arrow);
				enemy_select_arrow = nullptr;
				selecting_enemy = false;
				for (int i = 0; i < current_vector.size(); i++) {
					if (current_vector.at(i)->current_state == STATE_EXECUTED) {
						current_vector.at(i)->current_state = STATE_FOCUSED;
					}
				}
			}
		}
	}


	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN || App->input->gamepad.B == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		selected_ally = App->combat->heroes.begin();
		App->gui->DeleteUIElement(*enemy_select_arrow);
		enemy_select_arrow = nullptr;
		selecting_enemy = false;
		for (int i = 0; i < current_vector.size(); i++) {
			if (current_vector.at(i)->current_state == STATE_EXECUTED) {
				current_vector.at(i)->current_state = STATE_FOCUSED;
			}
		}
	}

}

void UICombatMenu::ChangeExplanation(std::vector<UIElement*> &current_vector) {
	std::vector<UIElement*>::const_iterator it_vector = current_vector.begin();
	if (current_vector == main_labels) {
		App->gui->DeleteUIElement(*explanation_label);
		while (it_vector != current_vector.end()) {
			if ((*it_vector)->current_state == STATE_FOCUSED) {
				if ((*it_vector) == attack_label) {
					explanation_label = App->gui->AddUITextBox(2, 1, 15, 224, ATTACKEXPLANATION, { 255,255,255,255 }, nullptr, Second_Font);
					explanation_label->SetParent(explanation_background);
					break;
				}
				else if ((*it_vector) == abilities_label) {
					explanation_label = App->gui->AddUITextBox(2, 1, 15, 224, ABILITIESEXPLANATION, { 255,255,255,255 }, nullptr, Second_Font);
					explanation_label->SetParent(explanation_background);
					break;
				}
				else if ((*it_vector) == items_label) {
					explanation_label = App->gui->AddUITextBox(2, 1, 15, 224, ITEMSEXPLANATION, { 255,255,255,255 }, nullptr, Second_Font);
					explanation_label->SetParent(explanation_background);
					break;
				}
			}
			it_vector++;
		}
	}
	else{
		if (explanation_label != nullptr) {
			App->gui->DeleteUIElement(*explanation_label);
		}
		if (entity->abilities.size() != 0) {
			string description = entity->abilities.at(names_iterator).description;
			explanation_label = App->gui->AddUITextBox(2, 1, 15, 224, description, { 255,255,255,255 }, nullptr, Second_Font);
			explanation_label->SetParent(explanation_background);
		}
		else {
			explanation_label = App->gui->AddUITextBox(2, 1, 15, 224, "You have non abilities in this moment", { 255,255,255,255 }, nullptr, Second_Font);
			explanation_label->SetParent(explanation_background);
		}
	}



}