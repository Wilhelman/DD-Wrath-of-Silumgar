#include "ctApp.h"
#include "UIPauseMenu.h"
#include "ctInput.h"
#include "ctAudio.h"
#include "ctLog.h"
#include "ctGui.h"
#include "UIImage.h"
#include "UILabel.h"
#include "UITextBox.h"
#include "ctRender.h"
#include "ctEntities.h"
#include "ctFadeToBlack.h"
#include "ctMainMenu.h"
#include "ctAnimation.h"
#include "ctWindow.h"
#include "Cleric.h"
#include "Dwarf.h"
#include "Elf.h"
#include "Warrior.h"
#include "ctCombat.h"
#include "ctWorldMap.h"
#include "Entity.h"
#include "MiniHeroes.h"

UIPauseMenu::UIPauseMenu(int x, int y, UI_Type type, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;

	equip_texture=App->tex->Load("textures/ObjectsWithBG.png");
	//*************************************************************************
	background = new UIImage(x, y, IMAGE, { 0,0,484,324 }, nullptr, this);
	if (App->entities->GetMiniheroes() != nullptr) {
		App->entities->SpawnEntity(30, 125, CLERIC);
		App->entities->SpawnEntity(30, 275, WARRIOR);
		App->entities->SpawnEntity(250, 125, DWARF);
		App->entities->SpawnEntity(250, 275, ELF);
	}
	else {
		App->entities->GetCleric()->position = { 30,125 };
		App->entities->GetWarrior()->position = { 30,275 };
		App->entities->GetDwarf()->position = { 250,125 };
		App->entities->GetElf()->position = { 250,275 };
	}


	for (int i = 0; i < App->items->elf_equip.size(); i++)
	{
		App->entities->GetElf()->AddEquipItem(App->items->elf_equip.at(i));
	}
	

	for (int i = 0; i < App->items->warrior_equip.size(); i++)
	{
		App->entities->GetWarrior()->AddEquipItem(App->items->warrior_equip.at(i));
	}


	for (int i = 0; i < App->items->cleric_equip.size(); i++)
	{
		App->entities->GetCleric()->AddEquipItem(App->items->cleric_equip.at(i));
	}


	for (int i = 0; i < App->items->dwarf_equip.size(); i++)
	{
		App->entities->GetDwarf()->AddEquipItem(App->items->dwarf_equip.at(i));
	}


	//-------------------------------

	App->entities->GetCleric()->animation = &App->entities->GetCleric()->menu_animation;
	App->entities->GetWarrior()->animation = &App->entities->GetWarrior()->menu_animation;
	App->entities->GetDwarf()->animation = &App->entities->GetDwarf()->menu_animation;
	App->entities->GetElf()->animation = &App->entities->GetElf()->menu_animation;


	LoadEquipableObjects();
	SetUpPauseMenu();

	LoadClerictStats();
	LoadWarriorStats();
	LoadDwarfStats();
	LoadElfStats();
}

UIPauseMenu::~UIPauseMenu() {

	if (App->entities->GetMiniheroes() != nullptr) {
		for (int i = 0; i < App->entities->entities.size(); i++)
		{
			if(App->entities->entities.at(i) != (Entity*)App->entities->GetMiniheroes())
				App->entities->entities.at(i)->to_destroy = true;
		}

		App->combat->turn_priority_entity.clear();
		App->combat->draw_turn_priority_entity.clear();
		App->combat->entities_to_spawn.clear();
	}
	else {
		App->entities->GetCleric()->position = App->entities->GetCleric()->initial_position;
		App->entities->GetWarrior()->position = App->entities->GetCleric()->initial_position;
		App->entities->GetDwarf()->position = App->entities->GetCleric()->initial_position;
		App->entities->GetElf()->position = App->entities->GetCleric()->initial_position;
	}


	//current_entity->CalculateAllStatsNoMultiply();
	App->entities->GetElf()->CalculateAllStats();
	App->entities->GetCleric()->CalculateAllStats();
	App->entities->GetDwarf()->CalculateAllStats();
	App->entities->GetWarrior()->CalculateAllStats();

	background->~UIElement();
	background = nullptr;
	App->tex->UnLoad(equip_texture);
	for (int i = 0; i < cleric_statistics.size(); i++) {
	//	App->gui->DeleteUIElement(*cleric_statistics.at(i));
		cleric_statistics.at(i)->~UIElement();
	}
	cleric_statistics.clear();
	for (int i = 0; i < warrior_statistics.size(); i++) {
		//App->gui->DeleteUIElement(*warrior_statistics.at(i));
		warrior_statistics.at(i)->~UIElement();
	}
	warrior_statistics.clear();
	for (int i = 0; i < dwarf_statistics.size(); i++) {
		//App->gui->DeleteUIElement(*dwarf_statistics.at(i));
		dwarf_statistics.at(i)->~UIElement();
	}
	dwarf_statistics.clear();
	for (int i = 0; i < elf_statistics.size(); i++) {
		//App->gui->DeleteUIElement(*elf_statistics.at(i));
		elf_statistics.at(i)->~UIElement();
	}
	elf_statistics.clear();

	arrow->~UIElement();
	arrow = nullptr;

	continue_label->~UIElement();
	continue_label = nullptr;
	inventory_label->~UIElement();
	inventory_label = nullptr;
	quit_label->~UIElement();
	quit_label = nullptr;
	main_labels.clear();

	if (information_inventory_items.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = information_inventory_items.begin(); it != information_inventory_items.end(); it++)
		{
			(*it)->~UIElement();
		}

		information_inventory_items.clear();
	}
}

void UIPauseMenu::Update() {
	if (arrow != nullptr)
	{
		arrow->Update();
		//Go down
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN || App->input->gamepad.JOYSTICK_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
			App->audio->PlayFx(App->audio->mm_movement_fx);

			NavigateDown(main_labels);
		}
		//Go up
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN || App->input->gamepad.JOYSTICK_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
			App->audio->PlayFx(App->audio->mm_movement_fx);
			NavigateUp(main_labels);
		}
		//ExecuteCommand
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->GetGamepadButton(App->main_menu->key_select) == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
			ExecuteComand(main_labels);
		}
	}
	else
	{
		if (inventory_items.size() != 0)
		{
			

			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->gamepad.CROSS_LEFT == GAMEPAD_STATE::PAD_BUTTON_DOWN || App->input->gamepad.JOYSTICK_LEFT == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
				App->audio->PlayFx(App->audio->mm_movement_fx);
				ChangePositionFakeArrow(SDL_SCANCODE_LEFT);
				SetInformationLabels();
			}

			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->gamepad.CROSS_RIGHT == GAMEPAD_STATE::PAD_BUTTON_DOWN || App->input->gamepad.JOYSTICK_RIGHT == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
				App->audio->PlayFx(App->audio->mm_movement_fx);
				ChangePositionFakeArrow(SDL_SCANCODE_RIGHT);
				SetInformationLabels();
			}
			if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || App->input->gamepad.START == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
				App->audio->PlayFx(App->audio->cm_back_fx);
				SetUpPauseMenu();
			}
			if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->GetGamepadButton(App->main_menu->key_select) == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
				SetUpPauseMenu();
			}

		}
		
	}

}

void UIPauseMenu::Draw(SDL_Texture* sprites)
{
	App->render->Blit(sprites, background->screen_position.x, background->screen_position.y, &background->current_rect);

	for (int i = 0; i < cleric_statistics.size(); i++) {
		App->render->Blit(cleric_statistics.at(i)->texture, cleric_statistics.at(i)->GetLocalPosition().x, cleric_statistics.at(i)->GetLocalPosition().y, &cleric_statistics.at(i)->current_rect);
	}
	for (int i = 0; i < warrior_statistics.size(); i++) {
		App->render->Blit(warrior_statistics.at(i)->texture, warrior_statistics.at(i)->GetLocalPosition().x, warrior_statistics.at(i)->GetLocalPosition().y, &warrior_statistics.at(i)->current_rect);
	}
	for (int i = 0; i < dwarf_statistics.size(); i++) {
		App->render->Blit(dwarf_statistics.at(i)->texture, dwarf_statistics.at(i)->GetLocalPosition().x, dwarf_statistics.at(i)->GetLocalPosition().y, &dwarf_statistics.at(i)->current_rect);
	}
	for (int i = 0; i < elf_statistics.size(); i++) {
		App->render->Blit(elf_statistics.at(i)->texture, elf_statistics.at(i)->GetLocalPosition().x, elf_statistics.at(i)->GetLocalPosition().y, &elf_statistics.at(i)->current_rect);
	}

	for (int i = 0; i < main_labels.size(); i++) {
		App->render->Blit(main_labels.at(i)->texture, main_labels.at(i)->GetLocalPosition().x, main_labels.at(i)->GetLocalPosition().y, &main_labels.at(i)->current_rect);
	}
	App->entities->GetCleric()->Draw();
	App->entities->GetWarrior()->Draw();
	App->entities->GetDwarf()->Draw();
	App->entities->GetElf()->Draw();
	if (arrow != nullptr)
	{
		App->render->Blit(App->gui->atlas, arrow->screen_position.x, arrow->screen_position.y, &arrow->current_rect);
	}
	DrawItems();
}

void UIPauseMenu::DrawItems() {

	


	for (std::vector<Item*>::iterator it = inventory_items.begin(); it != inventory_items.end(); it++)
	{
		switch ((*it)->equip_type)
		{
		case HELM:
				
			switch ((*it)->equipped_by)
			{
			case CLERIC:
				App->render->UIBlit(equip_texture, 0, 137,&(*it)->draw_coords);
				break;
			case DWARF:
				App->render->UIBlit(equip_texture, 205, 137, &(*it)->draw_coords);
				break;
			case WARRIOR:
				App->render->UIBlit(equip_texture, 0, 163, &(*it)->draw_coords);
				break;
			case ELF:
				App->render->UIBlit(equip_texture, 205, 163, &(*it)->draw_coords);
				break;
			}


			break;
		case CHEST:

			switch ((*it)->equipped_by)
			{
			case CLERIC:
				App->render->UIBlit(equip_texture, 26,137, &(*it)->draw_coords);
				break;
			case DWARF:
				App->render->UIBlit(equip_texture, 231, 137, &(*it)->draw_coords);
				break;
			case WARRIOR:
				App->render->UIBlit(equip_texture, 26, 163, &(*it)->draw_coords);
				break;
			case ELF:
				App->render->UIBlit(equip_texture, 231, 163, &(*it)->draw_coords);
				break;
			}

			
			break;
		case BOOT:

			switch ((*it)->equipped_by)
			{
			case CLERIC:
				App->render->UIBlit(equip_texture, 53, 137, &(*it)->draw_coords);
				break;
			case DWARF:
				App->render->UIBlit(equip_texture, 257, 137, &(*it)->draw_coords);
				break;
			case WARRIOR:
				App->render->UIBlit(equip_texture, 53, 163, &(*it)->draw_coords);
				break;
			case ELF:
				App->render->UIBlit(equip_texture, 257, 163, &(*it)->draw_coords);
				break;
			}

			break;
		case GUANTLET:

			switch ((*it)->equipped_by)
			{
			case CLERIC:
				App->render->UIBlit(equip_texture, 78, 137, &(*it)->draw_coords);
				break;
			case DWARF:
				App->render->UIBlit(equip_texture, 283, 137, &(*it)->draw_coords);
				break;
			case WARRIOR:
				App->render->UIBlit(equip_texture, 78, 163, &(*it)->draw_coords);
				break;
			case ELF:
				App->render->UIBlit(equip_texture, 283, 163, &(*it)->draw_coords);
				break;
			}

			break;
		case SHIELD:

			switch ((*it)->equipped_by)
			{
			case CLERIC:
				App->render->UIBlit(equip_texture, 103, 137, &(*it)->draw_coords);
				break;
			case DWARF:
				App->render->UIBlit(equip_texture, 308, 137, &(*it)->draw_coords);
				break;
			case WARRIOR:
				App->render->UIBlit(equip_texture, 103, 163, &(*it)->draw_coords);
				break;
			case ELF:
				App->render->UIBlit(equip_texture, 308, 163, &(*it)->draw_coords);
				break;
			}

			
			break;
		case WEAPON:

			switch ((*it)->equipped_by)
			{
			case CLERIC:
				App->render->UIBlit(equip_texture, 128, 137, &(*it)->draw_coords);
				break;
			case DWARF:
				App->render->UIBlit(equip_texture, 333, 137, &(*it)->draw_coords);
				break;
			case WARRIOR:
				App->render->UIBlit(equip_texture, 128, 163, &(*it)->draw_coords);
				break;
			case ELF:
				App->render->UIBlit(equip_texture, 333, 163, &(*it)->draw_coords);
				break;
			}

			
			break;
		case RING:

			switch ((*it)->equipped_by)
			{
			case CLERIC:
				App->render->UIBlit(equip_texture, 155, 137, &(*it)->draw_coords);
				break;
			case DWARF:
				App->render->UIBlit(equip_texture, 360, 137, &(*it)->draw_coords);
				break;
			case WARRIOR:
				App->render->UIBlit(equip_texture, 155, 163, &(*it)->draw_coords);
				break;
			case ELF:
				App->render->UIBlit(equip_texture, 360, 163, &(*it)->draw_coords);
				break;
			}

			
			break;
		case ACCESORY:

			switch ((*it)->equipped_by)
			{
			case CLERIC:
				App->render->UIBlit(equip_texture, 180, 137, &(*it)->draw_coords);
				break;
			case DWARF:
				App->render->UIBlit(equip_texture, 385, 137, &(*it)->draw_coords);
				break;
			case WARRIOR:
				App->render->UIBlit(equip_texture, 180, 163, &(*it)->draw_coords);
				break;
			case ELF:
				App->render->UIBlit(equip_texture, 385, 163, &(*it)->draw_coords);
				break;
			}

			
			break;
		}
	}

	for (std::vector<UIElement*>::iterator it = information_inventory_items.begin(); it != information_inventory_items.end(); it++)
	{
		if ((*it)->type != IMAGE)
			App->render->UIBlit((*it)->texture, (*it)->screen_position.x, (*it)->screen_position.y, &(*it)->current_rect);
		else
			App->render->UIBlit(equip_texture, (*it)->screen_position.x, (*it)->screen_position.y, &(*it)->current_rect);
	}
	
	App->render->DrawQuad({ position_fake_arrow.x + App->render->camera.x,position_fake_arrow.y + App->render->camera.y,26,24 }, 255, 0, 0, 255,false);
}

void UIPauseMenu::LoadClerictStats() {
	Entity* current_entity = App->entities->GetCleric();
	
	current_entity->CalculateAllStatsNoMultiply();

	std::string entity_stat = "Con  " + std::to_string(current_entity->max_health_points);
	cleric_statistics.push_back(new UITextBox(135, 38, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Foc  " + std::to_string(current_entity->max_mana_points);
	cleric_statistics.push_back(new UITextBox(135, 48, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Str  " + std::to_string(current_entity->current_strength);
	cleric_statistics.push_back(new UITextBox(135, 58, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Agi   " + std::to_string(current_entity->current_agility_points);
	cleric_statistics.push_back(new UITextBox(135, 68, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Dex  " + std::to_string(current_entity->current_dexterity_points);
	cleric_statistics.push_back(new UITextBox(135, 78, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Int   " + std::to_string(current_entity->current_intelligence);
	cleric_statistics.push_back(new UITextBox(135, 88, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "M.Def " + std::to_string(current_entity->current_magical_defense_points);
	cleric_statistics.push_back(new UITextBox(135, 98, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "P.Def " + std::to_string(current_entity->current_physical_defense_points);
	cleric_statistics.push_back(new UITextBox(135, 108, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Luck " + std::to_string(current_entity->current_luck);
	cleric_statistics.push_back(new UITextBox(135, 118, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

}

void UIPauseMenu::LoadWarriorStats() {
	Entity* current_entity = App->entities->GetWarrior();
	current_entity->CalculateAllStatsNoMultiply();

	std::string entity_stat = "Con  " + std::to_string(current_entity->max_health_points);
	warrior_statistics.push_back(new UITextBox(135, 195, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Foc  " + std::to_string(current_entity->max_mana_points);
	warrior_statistics.push_back(new UITextBox(135, 205, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Str  " + std::to_string(current_entity->current_strength);
	warrior_statistics.push_back(new UITextBox(135, 215, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Agi   " + std::to_string(current_entity->current_agility_points);
	warrior_statistics.push_back(new UITextBox(135, 225, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Dex  " + std::to_string(current_entity->current_dexterity_points);
	warrior_statistics.push_back(new UITextBox(135, 235, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Int   " + std::to_string(current_entity->current_intelligence);
	warrior_statistics.push_back(new UITextBox(135, 245, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "M.Def " + std::to_string(current_entity->current_magical_defense_points);
	warrior_statistics.push_back(new UITextBox(135, 255, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "P.Def " + std::to_string(current_entity->current_physical_defense_points);
	warrior_statistics.push_back(new UITextBox(135, 265, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Luck " + std::to_string(current_entity->current_luck);
	warrior_statistics.push_back(new UITextBox(135, 275, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));
}


void UIPauseMenu::LoadDwarfStats() {
	Entity* current_entity = App->entities->GetDwarf();

	current_entity->CalculateAllStatsNoMultiply();

	std::string entity_stat = "Con  " + std::to_string(current_entity->max_health_points);
	dwarf_statistics.push_back(new UITextBox(340, 38, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Foc  " + std::to_string(current_entity->max_mana_points);
	dwarf_statistics.push_back(new UITextBox(340, 48, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Str  " + std::to_string(current_entity->current_strength);
	dwarf_statistics.push_back(new UITextBox(340, 58, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Agi   " + std::to_string(current_entity->current_agility_points);
	dwarf_statistics.push_back(new UITextBox(340, 68, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Dex  " + std::to_string(current_entity->current_dexterity_points);
	dwarf_statistics.push_back(new UITextBox(340, 78, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Int   " + std::to_string(current_entity->current_intelligence);
	dwarf_statistics.push_back(new UITextBox(340, 88, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "M.Def " + std::to_string(current_entity->current_magical_defense_points);
	dwarf_statistics.push_back(new UITextBox(340, 98, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "P.Def " + std::to_string(current_entity->current_physical_defense_points);
	dwarf_statistics.push_back(new UITextBox(340, 108, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Luck " + std::to_string(current_entity->current_luck);
	dwarf_statistics.push_back(new UITextBox(340, 118, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));
}



void UIPauseMenu::LoadElfStats() {
	Entity* current_entity = App->entities->GetElf();
 
	current_entity->CalculateAllStatsNoMultiply();

	std::string entity_stat = "Con  " + std::to_string(current_entity->max_health_points);
	elf_statistics.push_back(new UITextBox(340, 195, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Foc  " + std::to_string(current_entity->max_mana_points);
	elf_statistics.push_back(new UITextBox(340, 205, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Str  " + std::to_string(current_entity->current_strength);
	elf_statistics.push_back(new UITextBox(340, 215, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Agi   " + std::to_string(current_entity->current_agility_points);
	elf_statistics.push_back(new UITextBox(340, 225, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Dex  " + std::to_string(current_entity->current_dexterity_points);
	elf_statistics.push_back(new UITextBox(340, 235, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Int   " + std::to_string(current_entity->current_intelligence);
	elf_statistics.push_back(new UITextBox(340, 245, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "M.Def " + std::to_string(current_entity->current_magical_defense_points);
	elf_statistics.push_back(new UITextBox(340, 255, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "P.Def " + std::to_string(current_entity->current_physical_defense_points);
	elf_statistics.push_back(new UITextBox(340, 265, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Luck " + std::to_string(current_entity->current_luck);
	elf_statistics.push_back(new UITextBox(340, 275, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));
}


void UIPauseMenu::SetUpPauseMenu()
{
	position_fake_arrow = { -100,-100 };


	if (information_inventory_items.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = information_inventory_items.begin(); it != information_inventory_items.end(); it++)
		{
			(*it)->~UIElement();
		}

		information_inventory_items.clear();
	}



	continue_label = new UILabel(420, 10, LABEL, "Continue", { 255,255,255,255 }, 12);
	main_labels.push_back(continue_label);
	continue_label->current_state = STATE_FOCUSED;

	if (inventory_items.size() != 0)
	{
		inventory_label = new UILabel(420, 40, LABEL, "Inventory", { 255,255,255,255 }, 12);
	}
	else
	{
		inventory_label = new UILabel(420, 40, LABEL, "Inventory", { 255,0,0,255 }, 12);
	}
	main_labels.push_back(inventory_label);

	quit_label = new UILabel(420, 70, LABEL, "Save Quit", { 255,255,255,255 }, 12);
	main_labels.push_back(quit_label);
	arrow = new UIImage(-10, 0, IMAGE, { 1333, 272, 7, 14 }, nullptr, nullptr);
	arrow->SetParent(continue_label);
	arrow->Update();

}


void UIPauseMenu::NavigateDown(std::vector<UIElement*> &current_vector) {
	std::vector<UIElement*>::const_iterator it_vector = current_vector.begin();
	while (it_vector != current_vector.end()) {
		if ((*it_vector)->current_state == STATE_FOCUSED) {
			if ((*it_vector) != current_vector.back()) {
				(*it_vector)->current_state = STATE_NORMAL;
				it_vector++;
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
				App->audio->PlayFx(App->audio->mm_movement_fx);
				break;
			}
			else
			{
				(*it_vector)->current_state = STATE_NORMAL;
				it_vector = current_vector.begin();
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
				App->audio->PlayFx(App->audio->mm_movement_fx);
			}
		}
		it_vector++;
	}
}

void UIPauseMenu::NavigateUp(std::vector<UIElement*> &current_vector) {
	std::vector<UIElement*>::const_iterator it_vector = current_vector.begin();
	while (it_vector != current_vector.end()) {
		if ((*it_vector)->current_state == STATE_FOCUSED) {
			if ((*it_vector) != current_vector.front()) {
				(*it_vector)->current_state = STATE_NORMAL;
				it_vector--;
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
				App->audio->PlayFx(App->audio->mm_movement_fx);
				break;
			}
			else
			{
				(*it_vector)->current_state = STATE_NORMAL;
				it_vector = current_vector.end() - 1;
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
				App->audio->PlayFx(App->audio->mm_movement_fx);
			}
		}
		it_vector++;
	}
}

void UIPauseMenu::ExecuteComand(std::vector<UIElement*> &current_vector) {
	App->audio->PlayFx(App->audio->mm_select_fx);
	for (int i = 0; i < current_vector.size(); i++) {
		if (current_vector.at(i)->current_state == STATE_FOCUSED) {
			current_vector.at(i)->current_state = STATE_EXECUTED;
		}
	}

	if (current_vector == main_labels) {
		if (continue_label->current_state == STATE_EXECUTED) {
			App->gui->DeleteUIElement(*this);
			App->main_menu->pauseMenu = nullptr;
			App->combat->pauseMenu = nullptr;
			App->world_map->pauseMenu = nullptr;
			
			

			if (App->world_map->active == true)
			{
				App->world_map->pause_menu_is_open = false;
				App->input->PreUpdate();
			}
			else if (App->combat->active == true)
			{
				App->input->PreUpdate();
				Entity* curr = App->entities->GetCleric();
				curr->position = curr->initial_position;

				curr = App->entities->GetDwarf();
				curr->position = curr->initial_position;

				curr = App->entities->GetWarrior();
				curr->position =curr->initial_position;

				curr = App->entities->GetElf();
				curr->position = curr->initial_position;

				App->combat->pause_menu_is_open = false;
				App->entities->GetCleric()->animation = &App->entities->GetCleric()->idle;
				App->entities->GetDwarf()->animation = &App->entities->GetDwarf()->idle;
				App->entities->GetElf()->animation = &App->entities->GetElf()->idle;
				App->entities->GetWarrior()->animation = &App->entities->GetWarrior()->idle;
			}

		}
		else if (inventory_label->current_state == STATE_EXECUTED) {
			if (inventory_items.size() != 0)
			{
				SetUpInventory();
				SetInformationLabels();
			}
			else
			{
				inventory_label->current_state = STATE_FOCUSED;
			}
		}
		else if (quit_label->current_state == STATE_EXECUTED) {
			SaveInPauseMenu();

			this->~UIPauseMenu();
			App->combat->pauseMenuDelete = true;
			App->fadeToBlack->FadeToBlackBetweenModules(callback, App->main_menu, 1.0f);
			

			//App->world_map->pause_menu_is_open = false;
			//App->combat->pause_menu_is_open = false;
		}
	}

}

void UIPauseMenu::SetUpInventory()
{
	main_labels.clear();
	arrow->~UIElement();
	arrow = nullptr;

	fake_arrow = 0;

	if (inventory_items.size() != 0)
	{
		position_fake_arrow=SetPositionFakeArrow();
		
	}


}

void UIPauseMenu::LoadEquipableObjects()
{
	Entity* current_entity = nullptr;
	int entities_added = 0;
	

	while (entities_added != 4)
	{
		switch (entities_added)
		{
		case 0:
			current_entity = App->entities->GetCleric();
			break;
		case 1:
			current_entity = App->entities->GetDwarf();
			break;
		case 2:
			current_entity = App->entities->GetWarrior();
			break;
		case 3:
			current_entity = App->entities->GetElf();
			break;
		}


		for (uint equipable_items = 0; equipable_items < 8; equipable_items++)
		{
			switch (equipable_items)
			{
			case HELM:
				if (current_entity->helmet.type != NO_ITEM_TYPE)
					inventory_items.push_back(&current_entity->helmet);
				break;
			case CHEST:
				if (current_entity->chest.type != NO_ITEM_TYPE)
					inventory_items.push_back(&current_entity->chest);
				break;
			case BOOT:
				if (current_entity->boot.type != NO_ITEM_TYPE)
					inventory_items.push_back(&current_entity->boot);
				break;
			case GUANTLET:
				if (current_entity->guantlet.type != NO_ITEM_TYPE)
					inventory_items.push_back(&current_entity->guantlet);
				break;
			case SHIELD:
				if (current_entity->shield.type != NO_ITEM_TYPE)
					inventory_items.push_back(&current_entity->shield);
				break;
			case WEAPON:
				if (current_entity->weapon.type != NO_ITEM_TYPE)
					inventory_items.push_back(&current_entity->weapon);
				break;
			case RING:
				if (current_entity->ring.type != NO_ITEM_TYPE)
					inventory_items.push_back(&current_entity->ring);
				break;
			case ACCESORY:
				if (current_entity->accessory.type != NO_ITEM_TYPE)
					inventory_items.push_back(&current_entity->accessory);
				break;
			}
			
		}

		entities_added++;

	}

}

iPoint UIPauseMenu::SetPositionFakeArrow()
{
	iPoint ret;
	
	inventory_items.at(fake_arrow)->equipped_by;

	switch (inventory_items.at(fake_arrow)->equipped_by)
	{
		case CLERIC:
			switch (inventory_items.at(fake_arrow)->equip_type)
			{
			case HELM:
				ret = { 0,137 };
				break;
			case CHEST:
				ret = { 26,137 };
				break;
			case BOOT:
				ret = { 53,137 };
				break;
			case GUANTLET:
				ret = { 78,137 };
				break;
			case SHIELD:
				ret = { 103,137 };
				break;
			case WEAPON:
				ret = { 128,137 };
				break;
			case RING:
				ret = { 155,137 };
				break;
			case ACCESORY:
				ret = { 180,137 };
				break;
			}
			break;
		case DWARF:
			switch (inventory_items.at(fake_arrow)->equip_type)
			{
			case HELM:
				ret = { 205,137 };
				break;
			case CHEST:
				ret = { 231,137 };
				break;
			case BOOT:
				ret = { 257,137 };
				break;
			case GUANTLET:
				ret = { 283,137 };
				break;
			case SHIELD:
				ret = { 308,137 };
				break;
			case WEAPON:
				ret = { 333,137 };
				break;
			case RING:
				ret = { 360,137 };
				break;
			case ACCESORY:
				ret = { 385,137 };
				break;
			}
			break;
		case WARRIOR:
			switch (inventory_items.at(fake_arrow)->equip_type)
			{
			case HELM:
				ret = { 0,163 };
				break;
			case CHEST:
				ret = { 26,163 };
				break;
			case BOOT:
				ret = { 53,163 };
				break;
			case GUANTLET:
				ret = { 78,163 };
				break;
			case SHIELD:
				ret = { 103,163 };
				break;
			case WEAPON:
				ret = { 128,163 };
				break;
			case RING:
				ret = { 155,163 };
				break;
			case ACCESORY:
				ret = { 180,163 };
				break;
			}
			break;
		case ELF:
			switch (inventory_items.at(fake_arrow)->equip_type)
			{
			case HELM:
				ret = { 205,163 };
				break;
			case CHEST:
				ret = { 231,163 };
				break;
			case BOOT:
				ret = { 257,163 };
				break;
			case GUANTLET:
				ret = { 283,163 };
				break;
			case SHIELD:
				ret = { 308,163 };
				break;
			case WEAPON:
				ret = { 333,163 };
				break;
			case RING:
				ret = { 360,163 };
				break;
			case ACCESORY:
				ret = { 385,163 };
				break;
			}
			break;
	}

	return ret;


}

void UIPauseMenu::ChangePositionFakeArrow(const SDL_Scancode code)
{
	if (inventory_items.size() != 0)
	{

		switch (code)
		{
		case SDL_SCANCODE_RIGHT:
			if (fake_arrow + 1 >= inventory_items.size())
				fake_arrow = 0;
			else
				fake_arrow++;
				
			break;
		case SDL_SCANCODE_LEFT:
			if ((fake_arrow - 1) < 0)
				fake_arrow = inventory_items.size()-1;
			else
				fake_arrow--;
			break;
		}

		position_fake_arrow = { App->render->camera.x + SetPositionFakeArrow().x ,App->render->camera.y + SetPositionFakeArrow().y };

	}


}

void UIPauseMenu::SetInformationLabels()
{
	if (inventory_items.size() != 0)
	{
		if (information_inventory_items.size() != 0)
		{
			for (std::vector<UIElement*>::iterator it = information_inventory_items.begin(); it != information_inventory_items.end(); it++)
			{
				(*it)->~UIElement();
			}

			information_inventory_items.clear();
		}


		information_inventory_items.push_back(new UITextBox(420, 10, TEXTBOX, inventory_items.at(fake_arrow)->name.c_str(), { 255,255,255 }, 20, 100));
		information_inventory_items.push_back(new UIImage(420, 30, IMAGE, inventory_items.at(fake_arrow)->draw_coords, nullptr));

		uint parent_invetory_items = 25;
		uint helped_text_parent_position = 140;

		if (inventory_items.at(fake_arrow)->statistics.constitution != 0)
		{
			std::string text = "Con " + std::to_string(inventory_items.at(fake_arrow)->statistics.constitution);
			information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
			parent_invetory_items += 10;

			std::string txt = "Con increases\n the maximum life";
			information_inventory_items.push_back(new UITextBox(420, helped_text_parent_position, TEXTBOX, txt, { 255,255,255 }, 12, 140));
			helped_text_parent_position += 25;
		}
		if (inventory_items.at(fake_arrow)->statistics.focus != 0)
		{
			std::string text = "Foc " + std::to_string(inventory_items.at(fake_arrow)->statistics.focus);
			information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
			parent_invetory_items += 10;

			std::string txt = "Foc increases\n the maximum mana";
			information_inventory_items.push_back(new UITextBox(420, helped_text_parent_position, TEXTBOX, txt, { 255,255,255 }, 12, 140));
			helped_text_parent_position += 25;
		}
		if (inventory_items.at(fake_arrow)->statistics.strength != 0)
		{
			std::string text = "Str " + std::to_string(inventory_items.at(fake_arrow)->statistics.strength);
			information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
			parent_invetory_items += 10;

			std::string txt = "Str: increases\n the physical damage";
			information_inventory_items.push_back(new UITextBox(420, helped_text_parent_position, TEXTBOX, txt, { 255,255,255 }, 12, 140));
			helped_text_parent_position += 25;
		}
		if (inventory_items.at(fake_arrow)->statistics.intelligence != 0)
		{
			std::string text = "Int " + std::to_string(inventory_items.at(fake_arrow)->statistics.intelligence);
			information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
			parent_invetory_items += 10;

			std::string txt = "Int increases\n the magical damage";
			information_inventory_items.push_back(new UITextBox(420, helped_text_parent_position, TEXTBOX, txt, { 255,255,255 }, 12, 140));
			helped_text_parent_position += 25;
		}
		if (inventory_items.at(fake_arrow)->statistics.dexterity != 0)
		{
			std::string text = "Dex " + std::to_string(inventory_items.at(fake_arrow)->statistics.dexterity);
			information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
			parent_invetory_items += 10;

			std::string txt = "Dex increases\n the probability of critical";
			information_inventory_items.push_back(new UITextBox(420, helped_text_parent_position, TEXTBOX, txt, { 255,255,255 }, 12, 140));
			helped_text_parent_position += 25;
		}
		if (inventory_items.at(fake_arrow)->statistics.agility != 0)
		{
			std::string text = "Agi " + std::to_string(inventory_items.at(fake_arrow)->statistics.agility);
			information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
			parent_invetory_items += 10;

			std::string txt = "Agi increases the probability of dodging and hitting attacks";
			information_inventory_items.push_back(new UITextBox(420, helped_text_parent_position, TEXTBOX, txt, { 255,255,255 }, 12, 140));
			helped_text_parent_position += 25;
		}
		if (inventory_items.at(fake_arrow)->statistics.magical_defense != 0)
		{
			std::string text = "M.Def " + std::to_string(inventory_items.at(fake_arrow)->statistics.magical_defense);
			information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
			parent_invetory_items += 10;

			std::string txt = "M.def magical defense\n increases resistance to magical attacks";
			information_inventory_items.push_back(new UITextBox(420, helped_text_parent_position, TEXTBOX, txt, { 255,255,255 }, 12, 140));
			helped_text_parent_position += 25;
		}
		if (inventory_items.at(fake_arrow)->statistics.physical_defense != 0)
		{
			std::string text = "P.Def " + std::to_string(inventory_items.at(fake_arrow)->statistics.physical_defense);
			information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
			parent_invetory_items += 10;

			std::string txt = "P.def increases\n resistance to physical attacks";
			information_inventory_items.push_back(new UITextBox(420, helped_text_parent_position, TEXTBOX, txt, { 255,255,255 }, 12, 140));
			helped_text_parent_position += 25;
		}
		if (inventory_items.at(fake_arrow)->statistics.luck != 0)
		{
			std::string text = "Lck " + std::to_string(inventory_items.at(fake_arrow)->statistics.luck);
			information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
			parent_invetory_items += 10;

			std::string txt = "Luck increases\n probability of drop items";
			information_inventory_items.push_back(new UITextBox(420, helped_text_parent_position, TEXTBOX, txt, { 255,255,255 }, 12, 140));
			helped_text_parent_position += 25;
		}

		information_inventory_items.push_back(new UITextBox(420, 300, TEXTBOX, "Press ENTER\n to return", { 255,255,255 }, 17, 200));
	}
}

void UIPauseMenu::SaveInPauseMenu()
{
	//DENIED
	/*pugi::xml_document	data_file;
	pugi::xml_node* node = &App->LoadData(data_file);

	//---------------------------------------------------------
	node = &node->child("settings");
	node->attribute("select").set_value(App->main_menu->key_select);
	node->attribute("back").set_value(App->main_menu->key_back);
	
	//-----------------------------------------------------------
	node = &node->child("world_map");
	
	node->attribute("location_x").set_value(App->entities->GetMiniheroes()->position.x);
	node->attribute("location_y").set_value(App->entities->GetMiniheroes()->position.y);

	int count = 0;

	node = &node->next_sibling();
	for (pugi::xml_node tiers = node->child("tier"); tiers; tiers = tiers.next_sibling("tier"))
	{
		tiers.attribute("number").set_value(App->world_map->number_map_generated[count]);
		count++;
	}
	
	//-----------------------------------------------------------
	node = &node->child("heroes");

	for (pugi::xml_node heroe = node->child("heroe"); heroe; heroe = heroe.next_sibling("heroe"))
	{
		std::string tmp(heroe.attribute("name").as_string());

		if (tmp == "cleric") {
			for (pugi::xml_node skill = heroe.child("skills").child("skill"); skill; skill = skill.next_sibling("skill")) {
				skill.attribute("owned").set_value(0);
			}

			for (pugi::xml_node item = heroe.child("items").child("item"); item; item = item.next_sibling("item")) {
				item.attribute("quantity").set_value(0);
			}
			pugi::xml_node item = heroe.child("items");

			for (int i = 0; i < App->items->cleric_equip.size(); i++)
			{
				App->entities->GetCleric()->AddEquipItem(App->items->cleric_equip.at(i));
			}

			Entity* curr = App->entities->GetCleric();

			if (App->items->cleric_equip.size() != 0)
			{
				if (curr->helmet.type != NO_ITEM_TYPE)
				{
					item.attribute("helmet").set_value(curr->helmet.name.c_str());
				}

				if (curr->boot.type != NO_ITEM_TYPE)
				{
					item.attribute("boot").set_value(curr->boot.name.c_str());
				}

				if (curr->guantlet.type != NO_ITEM_TYPE)
				{
					item.attribute("gauntlet").set_value(curr->guantlet.name.c_str());
				}
				if (curr->ring.type != NO_ITEM_TYPE)
				{
					item.attribute("ring").set_value(curr->ring.name.c_str());
				}
				if (curr->accessory.type != NO_ITEM_TYPE)
				{
					item.attribute("accessory").set_value(curr->accessory.name.c_str());
				}
				if (curr->chest.type != NO_ITEM_TYPE)
				{
					item.attribute("chest").set_value(curr->chest.name.c_str());
				}
				if (curr->shield.type != NO_ITEM_TYPE)
				{
					item.attribute("shield").set_value(curr->shield.name.c_str());
				}
				if (curr->weapon.type != NO_ITEM_TYPE)
				{
					item.attribute("weapon").set_value(curr->weapon.name.c_str());
				}

			}


		}
		else if (tmp == "warrior") {
			for (pugi::xml_node skill = heroe.child("skills").child("skill"); skill; skill = skill.next_sibling("skill")) {
				skill.attribute("owned").set_value(0);
			}

			for (pugi::xml_node item = heroe.child("items").child("item"); item; item = item.next_sibling("item")) {
				item.attribute("quantity").set_value(0);
			}

			pugi::xml_node item = heroe.child("items");
			Entity* curr = App->entities->GetWarrior();

			if (App->items->warrior_equip.size() != 0)
			{
				if (curr->helmet.type != NO_ITEM_TYPE)
				{
					item.attribute("helmet").set_value(curr->helmet.name.c_str());
				}

				if (curr->boot.type != NO_ITEM_TYPE)
				{
					item.attribute("boot").set_value(curr->boot.name.c_str());
				}

				if (curr->guantlet.type != NO_ITEM_TYPE)
				{
					item.attribute("gauntlet").set_value(curr->guantlet.name.c_str());
				}
				if (curr->ring.type != NO_ITEM_TYPE)
				{
					item.attribute("ring").set_value(curr->ring.name.c_str());
				}
				if (curr->accessory.type != NO_ITEM_TYPE)
				{
					item.attribute("accessory").set_value(curr->accessory.name.c_str());
				}
				if (curr->chest.type != NO_ITEM_TYPE)
				{
					item.attribute("chest").set_value(curr->chest.name.c_str());
				}
				if (curr->shield.type != NO_ITEM_TYPE)
				{
					item.attribute("shield").set_value(curr->shield.name.c_str());
				}
				if (curr->weapon.type != NO_ITEM_TYPE)
				{
					item.attribute("weapon").set_value(curr->weapon.name.c_str());
				}

			}
		}
		else if (tmp == "dwarf") {
			for (pugi::xml_node skill = heroe.child("skills").child("skill"); skill; skill = skill.next_sibling("skill")) {
				skill.attribute("owned").set_value(0);

			}
			for (pugi::xml_node item = heroe.child("items").child("item"); item; item = item.next_sibling("item")) {
				item.attribute("quantity").set_value(0);
			}

			pugi::xml_node item = heroe.child("items");
			Entity* curr = App->entities->GetDwarf();

			if (App->items->warrior_equip.size() != 0)
			{
				if (curr->helmet.type != NO_ITEM_TYPE)
				{
					item.attribute("helmet").set_value(curr->helmet.name.c_str());
				}

				if (curr->boot.type != NO_ITEM_TYPE)
				{
					item.attribute("boot").set_value(curr->boot.name.c_str());
				}

				if (curr->guantlet.type != NO_ITEM_TYPE)
				{
					item.attribute("gauntlet").set_value(curr->guantlet.name.c_str());
				}
				if (curr->ring.type != NO_ITEM_TYPE)
				{
					item.attribute("ring").set_value(curr->ring.name.c_str());
				}
				if (curr->accessory.type != NO_ITEM_TYPE)
				{
					item.attribute("accessory").set_value(curr->accessory.name.c_str());
				}
				if (curr->chest.type != NO_ITEM_TYPE)
				{
					item.attribute("chest").set_value(curr->chest.name.c_str());
				}
				if (curr->shield.type != NO_ITEM_TYPE)
				{
					item.attribute("shield").set_value(curr->shield.name.c_str());
				}
				if (curr->weapon.type != NO_ITEM_TYPE)
				{
					item.attribute("weapon").set_value(curr->weapon.name.c_str());
				}

			}
		}
		else if (tmp == "elf") {
			for (pugi::xml_node skill = heroe.child("skills").child("skill"); skill; skill = skill.next_sibling("skill")) {
				skill.attribute("owned").set_value(0);
			}
			for (pugi::xml_node item = heroe.child("items").child("item"); item; item = item.next_sibling("item")) {
				item.attribute("quantity").set_value(0);
			}

			pugi::xml_node item = heroe.child("items");
			Entity* curr = App->entities->GetElf();

			if (App->items->warrior_equip.size() != 0)
			{
				if (curr->helmet.type != NO_ITEM_TYPE)
				{
					item.attribute("helmet").set_value(curr->helmet.name.c_str());
				}

				if (curr->boot.type != NO_ITEM_TYPE)
				{
					item.attribute("boot").set_value(curr->boot.name.c_str());
				}

				if (curr->guantlet.type != NO_ITEM_TYPE)
				{
					item.attribute("gauntlet").set_value(curr->guantlet.name.c_str());
				}
				if (curr->ring.type != NO_ITEM_TYPE)
				{
					item.attribute("ring").set_value(curr->ring.name.c_str());
				}
				if (curr->accessory.type != NO_ITEM_TYPE)
				{
					item.attribute("accessory").set_value(curr->accessory.name.c_str());
				}
				if (curr->chest.type != NO_ITEM_TYPE)
				{
					item.attribute("chest").set_value(curr->chest.name.c_str());
				}
				if (curr->shield.type != NO_ITEM_TYPE)
				{
					item.attribute("shield").set_value(curr->shield.name.c_str());
				}
				if (curr->weapon.type != NO_ITEM_TYPE)
				{
					item.attribute("weapon").set_value(curr->weapon.name.c_str());
				}

			}
		}

	}

	data_file.save_file("data.xml");
	data_file.reset();*/
}