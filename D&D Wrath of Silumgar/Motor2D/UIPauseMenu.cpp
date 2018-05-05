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

UIPauseMenu::UIPauseMenu(int x, int y, UI_Type type, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;

	equip_texture=App->tex->Load("textures/ObjectsWithBG.png");

	background = new UIImage(x, y, IMAGE, { 0,0,484,324 }, nullptr, this);
	App->entities->SpawnEntity(30, 125, CLERIC);
	App->entities->SpawnEntity(30, 275, WARRIOR);
	App->entities->SpawnEntity(250, 125, DWARF);
	App->entities->SpawnEntity(250, 275, ELF);
	//-------------------------------
	App->entities->GetElf()->AddEquipItem(App->items->tier_2_equips.at(3));
	App->entities->GetElf()->AddEquipItem(App->items->tier_2_equips.at(1));
	App->entities->GetElf()->AddEquipItem(App->items->tier_2_equips.at(2));
	App->entities->GetElf()->AddEquipItem(App->items->tier_2_equips.at(4));
	App->entities->GetElf()->AddEquipItem(App->items->tier_2_equips.at(6));

	App->entities->GetCleric()->AddEquipItem(App->items->tier_1_equips.at(3));
	App->entities->GetCleric()->AddEquipItem(App->items->tier_1_equips.at(1));
	App->entities->GetCleric()->AddEquipItem(App->items->tier_1_equips.at(2));
	App->entities->GetCleric()->AddEquipItem(App->items->tier_1_equips.at(4));
	App->entities->GetCleric()->AddEquipItem(App->items->tier_1_equips.at(6));

	App->entities->GetDwarf()->AddEquipItem(App->items->tier_3_equips.at(3));
	App->entities->GetDwarf()->AddEquipItem(App->items->tier_3_equips.at(1));
	App->entities->GetDwarf()->AddEquipItem(App->items->tier_3_equips.at(2));
	App->entities->GetDwarf()->AddEquipItem(App->items->tier_3_equips.at(4));
	App->entities->GetDwarf()->AddEquipItem(App->items->tier_3_equips.at(6));
	//-------------------------------

	App->entities->GetCleric()->animation = &App->entities->GetCleric()->menu_animation;
	Entity* e = App->entities->GetCleric();
	App->entities->GetWarrior()->animation = &App->entities->GetWarrior()->menu_animation;
	App->entities->GetDwarf()->animation = &App->entities->GetDwarf()->menu_animation;
	App->entities->GetElf()->animation = &App->entities->GetElf()->menu_animation;
	LoadClerictStats();
	LoadWarriorStats();
	LoadDwarfStats();
	LoadElfStats();
	continue_label = new UILabel(420, 10, LABEL, "Continue", {255,255,255,255}, 15);
	main_labels.push_back(continue_label);
	continue_label->current_state = STATE_FOCUSED;
	inventory_label = new UILabel(420, 40, LABEL, "Inventory", { 255,255,255,255 }, 15);
	main_labels.push_back(inventory_label);
	abilities_label = new UILabel(420, 70, LABEL, "Abilities", { 255,255,255,255 }, 15);
	main_labels.push_back(abilities_label);
	settings_label = new UILabel(420, 100, LABEL, "Settings", { 255,255,255,255 }, 15);
	main_labels.push_back(settings_label);
	quit_label = new UILabel(420, 130, LABEL, "Quit", { 255,255,255,255 }, 15);
	main_labels.push_back(quit_label);
	arrow = new UIImage(-10, 0, IMAGE, { 1333, 272, 7, 14 }, nullptr, nullptr);
	arrow->SetParent(continue_label);
	arrow->Update();

	LoadEquipableObjects();
}

UIPauseMenu::~UIPauseMenu() {

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
	abilities_label->~UIElement();
	abilities_label = nullptr;
	settings_label->~UIElement();
	settings_label = nullptr;
	quit_label->~UIElement();
	quit_label = nullptr;
	main_labels.clear();

	for (int i = 0; i < App->entities->entities.size(); i++)
	{
		App->entities->entities.at(i)->to_destroy = true;
	}

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
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
			App->audio->PlayFx(App->audio->mm_movement_fx);

			NavigateDown(main_labels);
		}
		//Go up
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
			App->audio->PlayFx(App->audio->mm_movement_fx);
			NavigateUp(main_labels);
		}
		//ExecuteCommand
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->gamepad.A == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
			ExecuteComand(main_labels);
		}
	}
	else
	{
		if (inventory_items.size() != 0)
		{
			

			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->gamepad.CROSS_LEFT == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
				App->audio->PlayFx(App->audio->mm_movement_fx);
				ChangePositionFakeArrow(SDL_SCANCODE_LEFT);
				SetInformationLabels();
			}

			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->gamepad.CROSS_RIGHT == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
				App->audio->PlayFx(App->audio->mm_movement_fx);
				ChangePositionFakeArrow(SDL_SCANCODE_RIGHT);
				SetInformationLabels();
			}
			//ExecuteCommand
			if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->gamepad.A == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
				ExecuteComand(main_labels);
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

	
	App->render->DrawQuad({ position_fake_arrow.x+ App->render->camera.x,position_fake_arrow.y + App->render->camera.y,26,24 }, 255, 0, 0, 255);

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
		App->render->UIBlit((*it)->texture, (*it)->screen_position.x, (*it)->screen_position.y, &(*it)->current_rect);
	}
	

}

void UIPauseMenu::LoadClerictStats() {
	Entity* current_entity = App->entities->GetCleric();
	
	std::string entity_stat =  "Con  " + std::to_string((current_entity->base_stats.constitution * 13));
	cleric_statistics.push_back(new UITextBox(135, 38, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Foc  " + std::to_string((current_entity->base_stats.focus * 17));
	cleric_statistics.push_back(new UITextBox(135, 48, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Str  " + std::to_string((current_entity->base_stats.strength * 3));
	cleric_statistics.push_back(new UITextBox(135, 58, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Agi   " + std::to_string((current_entity->base_stats.intelligence * 2));
	cleric_statistics.push_back(new UITextBox(135, 68, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Dex  " + std::to_string((50 + current_entity->base_stats.dexterity * 2));
	cleric_statistics.push_back(new UITextBox(135, 78, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Int   " + std::to_string((current_entity->base_stats.intelligence * 3));
	cleric_statistics.push_back(new UITextBox(135, 88, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "M.Def " + std::to_string((current_entity->base_stats.magical_defense * 2));
	cleric_statistics.push_back(new UITextBox(135, 98, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "P.Def " + std::to_string((current_entity->base_stats.physical_defense * 2));
	cleric_statistics.push_back(new UITextBox(135, 108, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Luck " + std::to_string((current_entity->base_stats.luck ));
	cleric_statistics.push_back(new UITextBox(135, 118, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

}

void UIPauseMenu::LoadWarriorStats() {
	Entity* current_entity = App->entities->GetWarrior();

	std::string entity_stat = "Con  " + std::to_string((current_entity->base_stats.constitution * 13));
	warrior_statistics.push_back(new UITextBox(135, 195, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Foc  " + std::to_string((current_entity->base_stats.focus * 17));
	warrior_statistics.push_back(new UITextBox(135, 205, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Str  " + std::to_string((current_entity->base_stats.strength * 3));
	warrior_statistics.push_back(new UITextBox(135, 215, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Agi   " + std::to_string((current_entity->base_stats.intelligence * 2));
	warrior_statistics.push_back(new UITextBox(135, 225, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Dex  " + std::to_string((50 + current_entity->base_stats.dexterity * 2));
	warrior_statistics.push_back(new UITextBox(135, 235, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Int   " + std::to_string((current_entity->base_stats.intelligence * 3));
	warrior_statistics.push_back(new UITextBox(135, 245, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "M.Def " + std::to_string((current_entity->base_stats.magical_defense * 2));
	warrior_statistics.push_back(new UITextBox(135, 255, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "P.Def " + std::to_string((current_entity->base_stats.physical_defense * 2));
	warrior_statistics.push_back(new UITextBox(135, 265, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Luck " + std::to_string((current_entity->base_stats.luck));
	warrior_statistics.push_back(new UITextBox(135, 275, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));
}


void UIPauseMenu::LoadDwarfStats() {
	Entity* current_entity = App->entities->GetDwarf();

	std::string entity_stat = "Con  " + std::to_string((current_entity->base_stats.constitution * 13));
	dwarf_statistics.push_back(new UITextBox(340, 38, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Foc  " + std::to_string((current_entity->base_stats.focus * 17));
	dwarf_statistics.push_back(new UITextBox(340, 48, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Str  " + std::to_string((current_entity->base_stats.strength * 3));
	dwarf_statistics.push_back(new UITextBox(340, 58, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Agi   " + std::to_string((current_entity->base_stats.intelligence * 2));
	dwarf_statistics.push_back(new UITextBox(340, 68, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Dex  " + std::to_string((50 + current_entity->base_stats.dexterity * 2));
	dwarf_statistics.push_back(new UITextBox(340, 78, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Int   " + std::to_string((current_entity->base_stats.intelligence * 3));
	dwarf_statistics.push_back(new UITextBox(340, 88, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "M.Def " + std::to_string((current_entity->base_stats.magical_defense * 2));
	dwarf_statistics.push_back(new UITextBox(340, 98, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "P.Def " + std::to_string((current_entity->base_stats.physical_defense * 2));
	dwarf_statistics.push_back(new UITextBox(340, 108, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Luck " + std::to_string((current_entity->base_stats.luck));
	dwarf_statistics.push_back(new UITextBox(340, 118, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));
}


void UIPauseMenu::LoadElfStats() {
	Entity* current_entity = App->entities->GetElf();
 
	std::string entity_stat = "Con  " + std::to_string((current_entity->base_stats.constitution * 13));
	elf_statistics.push_back(new UITextBox(340, 195, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Foc  " + std::to_string((current_entity->base_stats.focus * 17));
	elf_statistics.push_back(new UITextBox(340, 205, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Str  " + std::to_string((current_entity->base_stats.strength * 3));
	elf_statistics.push_back(new UITextBox(340, 215, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Agi   " + std::to_string((current_entity->base_stats.intelligence * 2));
	elf_statistics.push_back(new UITextBox(340, 225, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Dex  " + std::to_string((50 + current_entity->base_stats.dexterity * 2));
	elf_statistics.push_back(new UITextBox(340, 235, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Int   " + std::to_string((current_entity->base_stats.intelligence * 3));
	elf_statistics.push_back(new UITextBox(340, 245, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "M.Def " + std::to_string((current_entity->base_stats.magical_defense * 2));
	elf_statistics.push_back(new UITextBox(340, 255, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "P.Def " + std::to_string((current_entity->base_stats.physical_defense * 2));
	elf_statistics.push_back(new UITextBox(340, 265, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

	entity_stat = "Luck " + std::to_string((current_entity->base_stats.luck));
	elf_statistics.push_back(new UITextBox(340, 275, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));
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
				break;
			}
			else
			{
				(*it_vector)->current_state = STATE_NORMAL;
				it_vector = current_vector.begin();
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
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
				break;
			}
			else
			{
				(*it_vector)->current_state = STATE_NORMAL;
				it_vector = current_vector.end() - 1;
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
			}
		}
		it_vector++;
	}
}

void UIPauseMenu::ExecuteComand(std::vector<UIElement*> &current_vector) {
	for (int i = 0; i < current_vector.size(); i++) {
		if (current_vector.at(i)->current_state == STATE_FOCUSED) {
			current_vector.at(i)->current_state = STATE_EXECUTED;
		}
	}

	if (current_vector == main_labels) {
		if (continue_label->current_state == STATE_EXECUTED) {
			App->gui->DeleteUIElement(*this);
			App->main_menu->pauseMenu = nullptr;
		}
		else if (inventory_label->current_state == STATE_EXECUTED) {
				SetUpInventory();
				SetInformationLabels();
		}
		else if (abilities_label->current_state == STATE_EXECUTED) {

		}
		else if (settings_label->current_state == STATE_EXECUTED) {

		}
		else if (quit_label->current_state == STATE_EXECUTED) {
			App->fadeToBlack->FadeToBlackBetweenModules(callback, App->main_menu, 1.0f);
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
	if (information_inventory_items.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = information_inventory_items.begin(); it != information_inventory_items.end(); it++)
		{
			(*it)->~UIElement();
		}

		information_inventory_items.clear();
	}

	
	information_inventory_items.push_back(new UITextBox(420, 50, TEXTBOX, inventory_items.at(fake_arrow)->name.c_str(), { 255,255,255 },20,100));
	uint parent_invetory_items = 22;

	if (inventory_items.at(fake_arrow)->statistics.constitution != 0)
	{
		std::string text = "Con " + std::to_string(inventory_items.at(fake_arrow)->statistics.constitution);
		information_inventory_items.push_back(new UITextBox(420, 60+parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
		parent_invetory_items+= 20;
	}
	if (inventory_items.at(fake_arrow)->statistics.focus != 0)
	{
		std::string text = "Foc " + std::to_string(inventory_items.at(fake_arrow)->statistics.focus);
		information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
		parent_invetory_items += 20;
	}
	if (inventory_items.at(fake_arrow)->statistics.strength != 0)
	{
		std::string text = "Str " + std::to_string(inventory_items.at(fake_arrow)->statistics.strength);
		information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
		parent_invetory_items += 20;
	}
	if (inventory_items.at(fake_arrow)->statistics.intelligence != 0)
	{
		std::string text = "Int " + std::to_string(inventory_items.at(fake_arrow)->statistics.intelligence);
		information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
		parent_invetory_items += 20;
	}
	if (inventory_items.at(fake_arrow)->statistics.dexterity != 0)
	{
		std::string text = "Dex " + std::to_string(inventory_items.at(fake_arrow)->statistics.dexterity);
		information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
		parent_invetory_items += 20;
	}
	if (inventory_items.at(fake_arrow)->statistics.agility != 0)
	{
		std::string text = "Agi " + std::to_string(inventory_items.at(fake_arrow)->statistics.agility);
		information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
		parent_invetory_items += 20;
	}
	if (inventory_items.at(fake_arrow)->statistics.magical_defense != 0)
	{
		std::string text = "M.Def " + std::to_string(inventory_items.at(fake_arrow)->statistics.magical_defense);
		information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
		parent_invetory_items += 20;
	}
	if (inventory_items.at(fake_arrow)->statistics.physical_defense != 0)
	{
		std::string text = "P.Def " + std::to_string(inventory_items.at(fake_arrow)->statistics.physical_defense);
		information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
		parent_invetory_items += 20;
	}
	if (inventory_items.at(fake_arrow)->statistics.luck != 0)
	{
		std::string text = "Lck " + std::to_string(inventory_items.at(fake_arrow)->statistics.luck);
		information_inventory_items.push_back(new UITextBox(420, 60 + parent_invetory_items, TEXTBOX, text, { 255,255,255 }, 17, 200));
		parent_invetory_items += 20;
	}
	
	information_inventory_items.push_back(new UITextBox(420, 300, TEXTBOX, "Press SPACE\n to return", { 255,255,255 }, 17, 200));
}