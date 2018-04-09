#include "ctApp.h"
#include "UICombatMenu.h"
#include "ctLog.h"
#include "ctInput.h"
#include "ctAudio.h"
#include "ctCombat.h"

UICombatMenu::UICombatMenu(int x, int y, UI_Type type, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;

	background = App->gui->AddUIImage(x, y, { 1260, 208, 60, 90 }, callback);
	attack_label = App->gui->AddUILabel(x + main_label1_pos.x, y + main_label1_pos.y, "Attack", { 255,255,255,255 }, font_size, nullptr, background);
	abilities_label = App->gui->AddUILabel(x + main_label2_pos.x, y + main_label2_pos.y, "Abilities", { 255,255,255,255 }, font_size, nullptr, background);
	items_label = App->gui->AddUILabel(x + main_label3_pos.x, y + main_label3_pos.y, "Items", { 255,255,255,255 }, font_size, nullptr, background);
	attack_label->current_state = STATE_FOCUSED;
	arrow = App->gui->AddUIImage(x - (main_label1_pos.x / 1.5), y, { 1333, 272, 7, 14 }, callback, background);
	main_labels.push_back(attack_label);
	main_labels.push_back(abilities_label);
	main_labels.push_back(items_label);
	arrow->SetParent(attack_label);
	upper_points_pos.x = x + 8;
	upper_points_pos.y = y - 20;
	lower_points_pos.x = x + 8;
	lower_points_pos.y = y + 47;
	LOG("UICombatMenu created in x:%i, y:%i", x, y);
	menu_move_fx = App->audio->LoadFx("audio/sounds/UI and Menus/MenuMove.wav");
	menu_select_fx = App->audio->LoadFx("audio/sounds/UI and Menus/MenuSelect.wav");
	menu_back_fx = App->audio->LoadFx("audio/sounds/UI and Menus/MenuBack.wav"); //TODO change this sound if you find any better

	selected_enemy = App->combat->enemies.begin();
}

UICombatMenu::~UICombatMenu() {
	App->audio->UnLoadFx(menu_move_fx);
	App->audio->UnLoadFx(menu_select_fx);
	App->audio->UnLoadFx(menu_back_fx);
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
}

void UICombatMenu::Update()
{
	//Go down
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && selecting_enemy == false) {

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
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && selecting_enemy == false) {
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
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && selecting_enemy == false) {
		App->audio->PlayFx(menu_select_fx);
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
	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN && selecting_enemy == false) {

		GoBack();
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
		SelectEnemy(abilities);
	}

}


void UICombatMenu::NavigateDown(std::vector<UIElement*> &current_vector) {
	std::vector<UIElement*>::const_iterator it_vector = current_vector.begin();
	if (current_vector == main_labels) {
		while (it_vector != current_vector.end()) {
			if ((*it_vector)->current_state == STATE_FOCUSED) {
				if ((*it_vector) != current_vector.back()) {
					(*it_vector)->current_state = STATE_NORMAL;
					it_vector++;
					(*it_vector)->current_state = STATE_FOCUSED;
					arrow->SetParent((*it_vector));
					App->audio->PlayFx(menu_move_fx);
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
					App->audio->PlayFx(menu_move_fx);
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
			current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(names_iterator - 1), { 255,255,255,255 }, font_size, nullptr, background));
			current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label2_pos.x, backgroundPos.y + label2_pos.y, names.at(names_iterator), { 255,255,255,255 }, font_size, nullptr, background));
			current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label3_pos.x, backgroundPos.y + label3_pos.y, names.at(names_iterator + 1), { 255,255,255,255 }, font_size, nullptr, background));
			names_iterator++;
			current_vector.back()->current_state = STATE_FOCUSED;
			arrow->SetParent(current_vector.back());
			App->audio->PlayFx(menu_move_fx);
		}
	}
}


void UICombatMenu::NavigateUp(std::vector<UIElement*> &current_vector) {
	std::vector<UIElement*>::const_iterator it_vector = current_vector.begin();
	if (current_vector == main_labels) {
		while (it_vector != current_vector.end()) {
			if ((*it_vector)->current_state == STATE_FOCUSED) {
				if ((*it_vector) != current_vector.front()) {
					(*it_vector)->current_state = STATE_NORMAL;
					it_vector--;
					(*it_vector)->current_state = STATE_FOCUSED;
					arrow->SetParent((*it_vector));
					App->audio->PlayFx(menu_move_fx);
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
					App->audio->PlayFx(menu_move_fx);
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
			current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(names_iterator - 3), { 255,255,255,255 }, font_size, nullptr, background));
			current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label2_pos.x, backgroundPos.y + label2_pos.y, names.at(names_iterator - 2), { 255,255,255,255 }, font_size, nullptr, background));
			current_vector.push_back(App->gui->AddUILabel(backgroundPos.x + label3_pos.x, backgroundPos.y + label3_pos.y, names.at(names_iterator - 1), { 255,255,255,255 }, font_size, nullptr, background));
			names_iterator--;
			current_vector.front()->current_state = STATE_FOCUSED;
			arrow->SetParent(current_vector.front());
			App->audio->PlayFx(menu_move_fx);
		}
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
		selecting_enemy = true;
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
	}

}


void UICombatMenu::LoadAbilities() {
	iPoint backgroundPos = background->GetScreenPosition();
	names_iterator = 0;
	names.push_back("Ability1");
	names.push_back("Ability2");
	names.push_back("Ability3");
	names.push_back("Ability4");
	names.push_back("Ability5");
	names.push_back("Ability6");
	names.push_back("Ability7");
	names.push_back("Ability8");
	names.push_back("Ability9");

	if (names.size() > 3) {
		abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(0), { 255,255,255,255 }, font_size, nullptr, background));
		abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label2_pos.x, backgroundPos.y + label2_pos.y, names.at(1), { 255,255,255,255 }, font_size, nullptr, background));
		abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label3_pos.x, backgroundPos.y + label3_pos.y, names.at(2), { 255,255,255,255 }, font_size, nullptr, background));
		names_iterator = 2;
	}
	else if (names.size() == 2) {
		abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(0), { 255,255,255,255 }, font_size, nullptr, background));
		abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label2_pos.x, backgroundPos.y + label2_pos.y, names.at(1), { 255,255,255,255 }, font_size, nullptr, background));
		names_iterator = 1;
	}
	else if (names.size() == 1) {
		abilities.push_back(App->gui->AddUILabel(backgroundPos.x + label1_pos.x, backgroundPos.y + label1_pos.y, names.at(0), { 255,255,255,255 }, font_size, nullptr, background));
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

		App->audio->PlayFx(menu_back_fx);
	}
	else if (items.size()>0) {
		for (int i = 0; i < items.size(); i++) {
			App->gui->DeleteUIElement(*items.at(i));
		}
		items.clear();
		names.clear();

		App->audio->PlayFx(menu_back_fx);
	}

	if (main_labels.size() == 0) {
		iPoint backgroundPos = background->GetScreenPosition();

		attack_label = App->gui->AddUILabel(backgroundPos.x + main_label1_pos.x, backgroundPos.y + main_label1_pos.y, "Attack", { 255,255,255,255 }, font_size, nullptr, background);
		abilities_label = App->gui->AddUILabel(backgroundPos.x + main_label2_pos.x, backgroundPos.y + main_label2_pos.y, "Abilities", { 255,255,255,255 }, font_size, nullptr, background);
		items_label = App->gui->AddUILabel(backgroundPos.x + main_label3_pos.x, backgroundPos.y + main_label3_pos.y, "Items", { 255,255,255,255 }, font_size, nullptr, background);
		attack_label->current_state = STATE_FOCUSED;
		arrow->SetParent(attack_label);
		main_labels.push_back(attack_label);
		main_labels.push_back(abilities_label);
		main_labels.push_back(items_label);

		App->audio->PlayFx(menu_back_fx);
	}

	if (upper_points != nullptr) {
		App->gui->DeleteUIElement(*upper_points);
		upper_points = nullptr;
	}


	if (lower_points != nullptr) {
		App->gui->DeleteUIElement(*lower_points);
		lower_points = nullptr;
	}

}

void UICombatMenu::SelectEnemy(std::vector<UIElement*> &current_vector) {
	if (enemy_select_arrow == nullptr) {
		enemy_select_arrow = App->gui->AddUIImage((*selected_enemy)->position.x + ((*selected_enemy)->idle.GetCurrentFrame().w / 2), (*selected_enemy)->position.y - (*selected_enemy)->idle.GetCurrentFrame().h - 5, { 1328, 289, 14, 7 }, callback, nullptr);
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN) {
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
		enemy_select_arrow = App->gui->AddUIImage((*selected_enemy)->position.x + ((*selected_enemy)->idle.GetCurrentFrame().w / 2), (*selected_enemy)->position.y - (*selected_enemy)->idle.GetCurrentFrame().h - 5, { 1328, 289, 14, 7 }, callback, nullptr);
	}
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN) {
		if (selected_enemy != App->combat->enemies.begin()) {
			selected_enemy--;
		}
		//if (selected_enemy == App->combat->enemies.begin()) {
		//	selected_enemy = App->combat->enemies.end();
		//}
		if (enemy_select_arrow != nullptr) {
			App->gui->DeleteUIElement(*enemy_select_arrow);
			enemy_select_arrow = nullptr;
		}
		enemy_select_arrow = App->gui->AddUIImage((*selected_enemy)->position.x + ((*selected_enemy)->idle.GetCurrentFrame().w / 2), (*selected_enemy)->position.y - (*selected_enemy)->idle.GetCurrentFrame().h - 5, { 1328, 289, 14, 7 }, callback, nullptr);
	}

	if (App->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN) {
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

		current_vector.clear();
		selecting_enemy = false;
	}


	if (App->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_DOWN) {
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