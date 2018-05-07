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
#include "ctLootMenu.h"

LootMenu::LootMenu() 
{
	
	
}

LootMenu::~LootMenu() {

	
}

bool LootMenu::Start()
{
	background_texture = App->tex->Load("textures/GUI/PauseMenu.png");
	equip_texture = App->tex->Load("textures/ObjectsWithBG.png");


	background = new UIImage(0, 0, IMAGE, { 0,0,484,324 }, nullptr);

	//DELETE LATE
	App->entities->SpawnEntity(30, 125, CLERIC);
	App->entities->SpawnEntity(30, 275, WARRIOR);
	App->entities->SpawnEntity(250, 125, DWARF);
	App->entities->SpawnEntity(250, 275, ELF);

	App->entities->GetCleric()->animation = &App->entities->GetCleric()->menu_animation;
	App->entities->GetWarrior()->animation = &App->entities->GetWarrior()->menu_animation;
	App->entities->GetDwarf()->animation = &App->entities->GetDwarf()->menu_animation;
	App->entities->GetElf()->animation = &App->entities->GetElf()->menu_animation;
	LoadClerictStats();
	//*******************************************************
	//when implement in combat
	/*App->entities->GetCleric()->position = { 30,125 };
		App->entities->GetWarrior()->position = { 30,275 };
		App->entities->GetDwarf()->position = { 250,125 };
		App->entities->GetElf()->position = { 250,275 };*/





	//------------------------------- TO DELETEEEEEEEEE
	//App->entities->GetElf()->AddEquipItem(App->items->tier_2_equips.at(3));
	//App->entities->GetElf()->AddEquipItem(App->items->tier_2_equips.at(1));
	//App->entities->GetElf()->AddEquipItem(App->items->tier_2_equips.at(2));
	//App->entities->GetElf()->AddEquipItem(App->items->tier_2_equips.at(4));
	//App->entities->GetElf()->AddEquipItem(App->items->tier_2_equips.at(6));

	App->entities->GetCleric()->AddEquipItem(App->items->tier_1_equips.at(3));
	App->entities->GetCleric()->AddEquipItem(App->items->tier_1_equips.at(1));
	App->entities->GetCleric()->AddEquipItem(App->items->tier_1_equips.at(2));
	App->entities->GetCleric()->AddEquipItem(App->items->tier_1_equips.at(4));
	App->entities->GetCleric()->AddEquipItem(App->items->tier_1_equips.at(6));

	//App->entities->GetDwarf()->AddEquipItem(App->items->tier_1_equips.at(3));
	//App->entities->GetDwarf()->AddEquipItem(App->items->tier_1_equips.at(1));
	//App->entities->GetDwarf()->AddEquipItem(App->items->tier_1_equips.at(2));
	App->entities->GetDwarf()->AddEquipItem(App->items->tier_2_equips.at(4));
	//App->entities->GetDwarf()->AddEquipItem(App->items->tier_1_equips.at(6));



	dropped_items.push_back(&App->items->tier_3_equips.at(3));
	dropped_items.push_back(&App->items->tier_3_equips.at(2));
	dropped_items.push_back(&App->items->tier_3_equips.at(1));
	//-------------------------------

	to_cleric_label = new UITextBox(430, 200, TEXTBOX, "To Cleric", { 255,255,255,255 }, 17,100);
	to_dwarf_label = new UITextBox(430, 220, TEXTBOX, "To Dwarf", { 255,255,255,255 }, 17,100);
	to_warrior_label = new  UITextBox(430, 240, TEXTBOX, "To Warrior", { 255,255,255,255 }, 17,100);
	to_elf_label = new UITextBox(430, 260, TEXTBOX, "To Elf", { 255,255,255,255 }, 17,100);
	for_no_one_label = new UITextBox(430, 280, TEXTBOX, "Do not want this object", { 255,255,255,255 }, 17,100);
	to_cleric_label->current_state = STATE_FOCUSED;
	main_labels.push_back(to_cleric_label);
	main_labels.push_back(to_dwarf_label);
	main_labels.push_back(to_warrior_label);
	main_labels.push_back(to_elf_label);
	main_labels.push_back(for_no_one_label);

	arrow = new UIImage(-10, 0, IMAGE, { 1333, 272, 7, 14 }, nullptr, nullptr);
	arrow->SetParent(to_cleric_label);
	arrow->Update();

	LoadEquipableObjects();

	SetInformationDroppedItem();

	LoadClerictStats();
	LoadWarriorStats();
	LoadDwarfStats();
	LoadElfStats();

	SetComparation();

	return true;
}

bool LootMenu::Update(float dt) 
{
	if (dropped_items.size() != 0)
	{
		arrow->Update();

		//Go down
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
			App->audio->PlayFx(App->audio->mm_movement_fx);

			NavigateDown(main_labels);
			SetComparation();
		}
		//Go up
		if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
			App->audio->PlayFx(App->audio->mm_movement_fx);
			NavigateUp(main_labels);
			SetComparation();
		}
		//ExecuteCommand
		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || App->input->gamepad.A == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
			ExecuteComand(main_labels);
		}
	}
	return true;
}

bool LootMenu::PostUpdate()
{



	Draw();
	return true;
}


bool LootMenu::CleanUp()
{
	App->tex->UnLoad(background_texture);
	App->tex->UnLoad(equip_texture);

	for (int i = 0; i < App->entities->entities.size(); i++)
	{
		if (App->entities->entities.at(i) != (Entity*)App->entities->GetMiniheroes())
			App->entities->entities.at(i)->to_destroy = true;
	}

	if (preview_stats_items.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = preview_stats_items.begin(); it != preview_stats_items.end(); it++)
		{
			(*it)->~UIElement();
		}

		preview_stats_items.clear();
	}

	if (dropped_items.size() != 0)
	{
		preview_stats_items.clear();
	}

	if (information_dropped_items.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = information_dropped_items.begin(); it != information_dropped_items.end(); it++)
		{
			(*it)->~UIElement();

		}
		information_dropped_items.clear();
	}

	if (cleric_statistics.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = cleric_statistics.begin(); it != cleric_statistics.end(); it++)
		{
			(*it)->~UIElement();
		}
		cleric_statistics.clear();
	}
	if (warrior_statistics.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = warrior_statistics.begin(); it != warrior_statistics.end(); it++)
		{
			(*it)->~UIElement();
		}
		warrior_statistics.clear();
	}
	if (dwarf_statistics.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = dwarf_statistics.begin(); it != dwarf_statistics.end(); it++)
		{
			(*it)->~UIElement();
		}
		dwarf_statistics.clear();
	}
	if (elf_statistics.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = elf_statistics.begin(); it != elf_statistics.end(); it++)
		{
			(*it)->~UIElement();
		}
		elf_statistics.clear();
	}

	if (inventory_items.size() != 0)
	{
		inventory_items.clear();
	}

	if (information_inventory_items.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = information_inventory_items.begin(); it != information_inventory_items.end(); it++)
		{
			(*it)->~UIElement();
		}

		information_inventory_items.clear();
	}

	if(to_cleric_label != nullptr)
		to_cleric_label->~UIElement();

	if (to_dwarf_label != nullptr)
		to_dwarf_label->~UIElement();

	if (to_warrior_label != nullptr)
		to_warrior_label->~UIElement();

	if (to_elf_label != nullptr)
		to_elf_label->~UIElement();

	if (for_no_one_label != nullptr)
		for_no_one_label->~UIElement();

	if (main_labels.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = main_labels.begin(); it != main_labels.end(); it++)
		{
			(*it)->~UIElement();
		}
		main_labels.clear();
	}

	if (arrow != nullptr)
	{
		arrow->~UIElement();
		arrow = nullptr;
	}
	return true;
}

void LootMenu::Draw()
{
	App->render->Blit(background_texture, background->screen_position.x, background->screen_position.y, &background->current_rect);

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
		App->render->UIBlit(main_labels.at(i)->texture, main_labels.at(i)->GetLocalPosition().x, main_labels.at(i)->GetLocalPosition().y, &main_labels.at(i)->current_rect);
	}

	for (int i = 0; i < preview_stats_items.size(); i++) {
		App->render->UIBlit(preview_stats_items.at(i)->texture, preview_stats_items.at(i)->GetLocalPosition().x, preview_stats_items.at(i)->GetLocalPosition().y, &preview_stats_items.at(i)->current_rect);
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

void LootMenu::DrawItems() {


	for (std::vector<Item*>::iterator it = inventory_items.begin(); it != inventory_items.end(); it++)
	{
		switch ((*it)->equip_type)
		{
		case HELM:

			switch ((*it)->equipped_by)
			{
			case CLERIC:
				App->render->UIBlit(equip_texture, 0, 137, &(*it)->draw_coords);
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
				App->render->UIBlit(equip_texture, 26, 137, &(*it)->draw_coords);
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


	for (int i = 0; i < information_dropped_items.size(); i++)
	{
		UIElement* it = information_dropped_items[i];

		if (it->type == TEXTBOX)
			information_dropped_items[i]->Draw(information_dropped_items[i]->texture);
		else
			App->render->UIBlit(equip_texture, it->screen_position.x, it->screen_position.y, &it->current_rect);
	}

}

void LootMenu::LoadClerictStats() {

	if (cleric_statistics.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = cleric_statistics.begin(); it != cleric_statistics.end(); it++)
		{
			(*it)->~UIElement();
		}
		cleric_statistics.clear();
	}

	Entity* current_entity = App->entities->GetCleric();
	current_entity->CalculateAllStats();

	std::string entity_stat = "Con  " + std::to_string(current_entity->max_health_points);
	cleric_statistics.push_back(new UITextBox(135, 38, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Foc  " + std::to_string(current_entity->max_mana_points);
	cleric_statistics.push_back(new UITextBox(135, 48, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Str  " + std::to_string(current_entity->current_strength);
	cleric_statistics.push_back(new UITextBox(135, 58, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Agi   " + std::to_string(current_entity->current_agility_points);
	cleric_statistics.push_back(new UITextBox(135, 68, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));


	entity_stat = "Dex  " + std::to_string( current_entity->current_dexterity_points);
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

void LootMenu::LoadWarriorStats() {


	if (warrior_statistics.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = warrior_statistics.begin(); it != warrior_statistics.end(); it++)
		{
			(*it)->~UIElement();
		}
		warrior_statistics.clear();
	}


	Entity* current_entity = App->entities->GetWarrior();
	current_entity->CalculateAllStats();

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


void LootMenu::LoadDwarfStats() {

	if (dwarf_statistics.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = dwarf_statistics.begin(); it != dwarf_statistics.end(); it++)
		{
			(*it)->~UIElement();
		}
		dwarf_statistics.clear();
	}

	Entity* current_entity = App->entities->GetDwarf();
	current_entity->CalculateAllStats();

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


void LootMenu::LoadElfStats() {

	if (elf_statistics.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = elf_statistics.begin(); it != elf_statistics.end(); it++)
		{
			(*it)->~UIElement();
		}
		elf_statistics.clear();
	}

	Entity* current_entity = App->entities->GetElf();
	current_entity->CalculateAllStats();

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


void LootMenu::NavigateDown(std::vector<UIElement*> &current_vector) {
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

void LootMenu::NavigateUp(std::vector<UIElement*> &current_vector) {
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

void LootMenu::ExecuteComand(std::vector<UIElement*> &current_vector) {
	for (int i = 0; i < current_vector.size(); i++) {
		if (current_vector.at(i)->current_state == STATE_FOCUSED) {
			current_vector.at(i)->current_state = STATE_EXECUTED;
		}
	}

	if (current_vector[0]->current_state == STATE_EXECUTED)
	{
		App->entities->GetCleric()->AddEquipItem(*dropped_items[0]);
	}
	else if (current_vector[1]->current_state == STATE_EXECUTED)
	{
		App->entities->GetDwarf()->AddEquipItem(*dropped_items[0]);
	}
	else if (current_vector[2]->current_state == STATE_EXECUTED)
	{
		App->entities->GetWarrior()->AddEquipItem(*dropped_items[0]);
	}
	else if (current_vector[3]->current_state == STATE_EXECUTED)
	{
		App->entities->GetElf()->AddEquipItem(*dropped_items[0]);
	}

	dropped_items.erase(dropped_items.begin());

	for (int i = 0; i < current_vector.size(); i++) {
		if (current_vector.at(i)->current_state == STATE_EXECUTED) {
			current_vector.at(i)->current_state = STATE_FOCUSED;
		}
	}


	LoadEquipableObjects();

	SetInformationDroppedItem();

	LoadClerictStats();
	LoadWarriorStats();
	LoadDwarfStats();
	LoadElfStats();

	SetComparation();


}

void LootMenu::SetInformationDroppedItem()
{
	if (information_dropped_items.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = information_dropped_items.begin(); it != information_dropped_items.end(); it++)
		{
			(*it)->~UIElement();
		}
		information_dropped_items.clear();
	}


	information_dropped_items.push_back(new UITextBox(420, 20, TEXTBOX, dropped_items[0]->name, {255,255,255},17,100));
	information_dropped_items.push_back(new UIImage(420, 50, IMAGE, dropped_items[0]->draw_coords, nullptr));

	uint parent_dropped_position = 20;

	if (dropped_items[0]->statistics.constitution != 0)
	{
		
		information_dropped_items.push_back(new UITextBox(420, 60 + parent_dropped_position, TEXTBOX, "Con  "+std::to_string(dropped_items[0]->statistics.constitution), { 255,255,255 }, 17, 200));
		parent_dropped_position += 20;
	}
	if (dropped_items[0]->statistics.focus != 0)
	{
		information_dropped_items.push_back(new UITextBox(420, 60 + parent_dropped_position, TEXTBOX,"Foc  "+ std::to_string(dropped_items[0]->statistics.focus), { 255,255,255 }, 17, 200));
		parent_dropped_position += 20;
	}
	if (dropped_items[0]->statistics.strength != 0)
	{
		information_dropped_items.push_back(new UITextBox(420, 60 + parent_dropped_position, TEXTBOX,"Str  "+ std::to_string(dropped_items[0]->statistics.strength), { 255,255,255 }, 17, 200));
		parent_dropped_position += 20;
	}
	if (dropped_items[0]->statistics.intelligence != 0)
	{
		information_dropped_items.push_back(new UITextBox(420, 60 + parent_dropped_position, TEXTBOX,"Agi  "+ std::to_string(dropped_items[0]->statistics.intelligence), { 255,255,255 }, 17, 200));
		parent_dropped_position += 20;
	}
	if (dropped_items[0]->statistics.dexterity != 0)
	{
		information_dropped_items.push_back(new UITextBox(420,60 + parent_dropped_position, TEXTBOX,"Dex  "+ std::to_string(dropped_items[0]->statistics.dexterity), { 255,255,255 }, 17, 200));
		parent_dropped_position += 20;
	}
	if (dropped_items[0]->statistics.agility != 0)
	{
		information_dropped_items.push_back(new UITextBox(420, 60 + parent_dropped_position, TEXTBOX,"Int  "+ std::to_string(dropped_items[0]->statistics.agility), { 255,255,255 }, 17, 200));
		parent_dropped_position += 20;
	}
	if (dropped_items[0]->statistics.magical_defense != 0)
	{
		information_dropped_items.push_back(new UITextBox(420, 60 + parent_dropped_position, TEXTBOX, "M.Def "+ std::to_string(dropped_items[0]->statistics.magical_defense), { 255,255,255 }, 17, 200));
		parent_dropped_position += 20;
	}
	if (dropped_items[0]->statistics.physical_defense != 0)
	{
		information_dropped_items.push_back(new UITextBox(420, 60 + parent_dropped_position, TEXTBOX,"P.Def " +std::to_string(dropped_items[0]->statistics.physical_defense), { 255,255,255 }, 17, 200));
		parent_dropped_position += 20;
	}
	if (dropped_items[0]->statistics.luck != 0)
	{
		information_dropped_items.push_back(new UITextBox(420, 40 + parent_dropped_position, TEXTBOX,"Luck " + std::to_string(dropped_items[0]->statistics.luck), { 255,255,255 }, 17, 200));
		parent_dropped_position += 20;
	}

	
}

void LootMenu::LoadEquipableObjects()
{

	if (inventory_items.size() != 0)
	{
		inventory_items.clear();
	}


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



void LootMenu::SetInformationLabels()
{
	if (information_inventory_items.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = information_inventory_items.begin(); it != information_inventory_items.end(); it++)
		{
			(*it)->~UIElement();
		}

		information_inventory_items.clear();
	}

}

void LootMenu::SetComparation()
{
	if (preview_stats_items.size() != 0)
	{
		for (std::vector<UIElement*>::iterator it = preview_stats_items.begin(); it != preview_stats_items.end(); it++)
		{
			(*it)->~UIElement();
		}

		preview_stats_items.clear();
	}

	uint focus_position = 0;
	for (int i = 0; i < main_labels.size(); i++)
	{
		if (main_labels[i]->current_state == STATE_FOCUSED)
			focus_position = i;
	}

	if(focus_position != 4)
	{
	Entity* current_entity = nullptr;
	switch (focus_position)
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


	int total_value = 0;
	switch (dropped_items[0]->equip_type)
	{
	case HELM:
		switch (current_entity->type)
		{
		case CLERIC:
			if (current_entity->helmet.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x +40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), {0,255,0},17,200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x +40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x +40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x +40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x +40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x +40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x +40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x +40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x +40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}

			}
			else
			{
				if (current_entity->helmet.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->helmet.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = {0,255,0};
					
					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x +40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->helmet.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x +40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.strength != 0 || dropped_items[0]->statistics.strength!= 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->helmet.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x +40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->helmet.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x +40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->helmet.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x +40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->helmet.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x +40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->helmet.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x +40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->helmet.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x +40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->helmet.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x +40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}


			}
			break;
		case DWARF:
			if (current_entity->helmet.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x +40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x +40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x +40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x +40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x +40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x +40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x +40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x +40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x +40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->helmet.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->helmet.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x +40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->helmet.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x +40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->helmet.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x +40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->helmet.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x +40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->helmet.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x +40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->helmet.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x +40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->helmet.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x +40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->helmet.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x +40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->helmet.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x +40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case WARRIOR:
			if (current_entity->helmet.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x +40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x +40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x +40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x +40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x +40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x +40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x +40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x +40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x +40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->helmet.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->helmet.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x +40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->helmet.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x +40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->helmet.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x +40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->helmet.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x +40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->helmet.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x +40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->helmet.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x +40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->helmet.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x +40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->helmet.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x +40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->helmet.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x +40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case ELF:
			if (current_entity->helmet.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x +40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x +40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x +40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x +40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x +40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x +40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x +40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x +40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x +40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->helmet.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->helmet.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x +40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->helmet.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x +40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->helmet.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x +40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->helmet.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x +40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->helmet.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x +40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->helmet.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x +40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->helmet.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x +40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->helmet.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x +40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->helmet.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->helmet.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x +40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		}
		break;
	case CHEST:
		switch (current_entity->type)
		{
		case CLERIC:
			if (current_entity->chest.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x + 40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x + 40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x + 40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x + 40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x + 40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x + 40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x + 40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x + 40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x + 40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}

			}
			else
			{

				if (current_entity->chest.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->chest.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x +40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->chest.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x +40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->chest.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x +40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->chest.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x +40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->chest.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x +40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->chest.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x +40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->chest.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x +40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->chest.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x +40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->chest.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x +40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}


			}
			break;
		case DWARF:
			if (current_entity->chest.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x + 40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x + 40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x + 40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x + 40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x + 40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x + 40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x + 40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x + 40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x + 40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->chest.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->chest.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x +40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->chest.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x +40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->chest.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x +40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->chest.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x +40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->chest.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x +40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->chest.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x +40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->chest.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x +40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->chest.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x +40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->chest.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x +40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case WARRIOR:
			if (current_entity->chest.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x + 40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x + 40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x + 40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x + 40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x + 40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x + 40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x + 40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x + 40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x + 40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->chest.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->chest.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x +40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->chest.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x +40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->chest.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x +40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence!= 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->chest.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x +40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->chest.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x +40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->chest.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x +40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->chest.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x +40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->chest.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x +40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->chest.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x +40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case ELF:
			if (current_entity->chest.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x + 40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x + 40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x + 40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x + 40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x + 40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x + 40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x + 40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x + 40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x + 40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->chest.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->chest.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x +40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->chest.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x +40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->chest.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x +40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->chest.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x +40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->chest.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x +40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->chest.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x +40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->chest.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x +40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->chest.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x +40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->chest.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->chest.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x +40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		}
		break;
	case BOOT:
		switch (current_entity->type)
		{
		case CLERIC:
			if (current_entity->boot.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x + 40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x + 40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x + 40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x + 40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x + 40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x + 40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x + 40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x + 40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x + 40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}

			}
			else
			{

				if (current_entity->boot.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->boot.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x +40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->boot.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x +40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->boot.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x +40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->boot.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x +40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->boot.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x +40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->boot.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x +40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->boot.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x +40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->boot.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x +40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->boot.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x +40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}


			}
			break;
		case DWARF:
			if (current_entity->boot.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x + 40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x + 40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x + 40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x + 40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x + 40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x + 40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x + 40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x + 40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x + 40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->boot.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->boot.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x + 40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->boot.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x + 40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->boot.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x + 40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->boot.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x + 40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->boot.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x + 40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->boot.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x + 40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->boot.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x + 40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->boot.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x + 40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->boot.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x + 40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case WARRIOR:
			if (current_entity->boot.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x + 40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x + 40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x + 40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x + 40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x + 40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x + 40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x + 40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x + 40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x + 40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->boot.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->boot.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x + 40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->boot.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x + 40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->boot.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x + 40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->boot.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x + 40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->boot.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x + 40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->boot.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x + 40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->boot.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x + 40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->boot.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x + 40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->boot.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x + 40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case ELF:
			if (current_entity->boot.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x + 40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x + 40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x + 40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x + 40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x + 40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x + 40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x + 40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x + 40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x + 40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->boot.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->boot.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x + 40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->boot.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x + 40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->boot.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x + 40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->boot.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x + 40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->boot.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x + 40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->boot.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x + 40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->boot.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x + 40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->boot.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x + 40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->boot.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->boot.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x + 40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		}
		break;
	case GUANTLET:
		switch (current_entity->type)
		{
		case CLERIC:
			if (current_entity->guantlet.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x + 40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x + 40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x + 40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x + 40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x + 40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x + 40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x + 40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x + 40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x + 40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}

			}
			else
			{

				if (current_entity->guantlet.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->guantlet.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x + 40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->guantlet.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x + 40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->guantlet.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x + 40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->guantlet.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x + 40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->guantlet.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x + 40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->guantlet.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x + 40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->guantlet.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x + 40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->guantlet.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x + 40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->guantlet.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x + 40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}


			}
			break;
		case DWARF:
			if (current_entity->guantlet.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x + 40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x + 40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x + 40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x + 40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x + 40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x + 40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x + 40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x + 40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x + 40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->guantlet.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->guantlet.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x + 40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->guantlet.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x + 40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->guantlet.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x + 40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->guantlet.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x + 40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->guantlet.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x + 40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->guantlet.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x + 40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->guantlet.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x + 40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->guantlet.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x + 40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->guantlet.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x + 40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case WARRIOR:
			if (current_entity->guantlet.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x + 40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x + 40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x + 40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x + 40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x + 40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x + 40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x + 40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x + 40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x + 40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->guantlet.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->guantlet.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x + 40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->guantlet.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x + 40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->guantlet.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x + 40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->guantlet.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x + 40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->guantlet.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x + 40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->guantlet.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x + 40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->guantlet.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x + 40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->guantlet.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x + 40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->guantlet.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x + 40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case ELF:
			if (current_entity->guantlet.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x + 40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x + 40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x + 40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x + 40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x + 40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x + 40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x + 40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x + 40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x + 40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->guantlet.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->guantlet.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x + 40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->guantlet.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x + 40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->guantlet.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x + 40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->guantlet.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x + 40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->guantlet.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x + 40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->guantlet.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x + 40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->guantlet.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x + 40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->guantlet.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x + 40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->guantlet.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->guantlet.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x + 40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		}
		break;
	case SHIELD:
		switch (current_entity->type)
		{
		case CLERIC:
			if (current_entity->shield.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x + 40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x + 40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x + 40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x + 40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x + 40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x + 40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x + 40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x + 40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x + 40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}

			}
			else
			{

				if (current_entity->shield.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->shield.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x + 40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->shield.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x + 40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->shield.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x + 40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->shield.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x + 40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->shield.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x + 40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->shield.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x + 40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->shield.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x + 40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->shield.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x + 40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->shield.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x + 40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}

			}
			break;
		case DWARF:
			if (current_entity->shield.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x + 40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x + 40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x + 40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x + 40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x + 40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x + 40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x + 40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x + 40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x + 40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->shield.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->shield.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x + 40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->shield.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x + 40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->shield.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x + 40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->shield.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x + 40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->shield.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x + 40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->shield.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x + 40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->shield.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x + 40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->shield.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x + 40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->shield.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x + 40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case WARRIOR:
			if (current_entity->shield.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x + 40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x + 40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x + 40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x + 40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x + 40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x + 40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x + 40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x + 40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x + 40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->shield.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->shield.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x + 40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->shield.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x + 40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->shield.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x + 40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->shield.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x + 40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->shield.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x + 40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->shield.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x + 40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->shield.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x + 40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->shield.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x + 40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->shield.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x + 40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case ELF:
			if (current_entity->shield.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x + 40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x + 40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x + 40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x + 40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x + 40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x + 40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x + 40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x + 40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x + 40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->shield.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->shield.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x + 40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->shield.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x + 40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->shield.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x + 40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->shield.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x + 40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->shield.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x + 40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->shield.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x + 40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->shield.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x + 40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->shield.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x + 40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->shield.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->shield.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x + 40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		}
		break;
	case WEAPON:
		switch (current_entity->type)
		{
		case CLERIC:
			if (current_entity->weapon.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x + 40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x + 40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x + 40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x + 40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x + 40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x + 40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x + 40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x + 40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x + 40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}

			}
			else
			{

				if (current_entity->weapon.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->weapon.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x + 40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->weapon.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x + 40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->weapon.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x + 40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->weapon.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x + 40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->weapon.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x + 40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->weapon.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x + 40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->weapon.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x + 40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->weapon.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x + 40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->weapon.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x + 40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}

			}
			break;
		case DWARF:
			if (current_entity->weapon.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x + 40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x + 40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x + 40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x + 40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x + 40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x + 40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x + 40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x + 40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x + 40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->weapon.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->weapon.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x + 40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->weapon.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x + 40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->weapon.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x + 40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->weapon.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x + 40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->weapon.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x + 40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->weapon.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x + 40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->weapon.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x + 40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->weapon.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x + 40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->weapon.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x + 40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case WARRIOR:
			if (current_entity->weapon.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x + 40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x + 40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x + 40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x + 40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x + 40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x + 40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x + 40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x + 40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x + 40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->weapon.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->weapon.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x + 40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->weapon.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x + 40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->weapon.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x + 40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->weapon.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x + 40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->weapon.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x + 40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->weapon.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x + 40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->weapon.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x + 40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->weapon.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x + 40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->weapon.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x + 40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case ELF:
			if (current_entity->weapon.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x + 40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x + 40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x + 40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x + 40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x + 40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x + 40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x + 40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x + 40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x + 40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->weapon.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->weapon.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x + 40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->weapon.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x + 40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->weapon.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x + 40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->weapon.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x + 40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->weapon.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x + 40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->weapon.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x + 40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->weapon.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x + 40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->weapon.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x + 40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->weapon.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->weapon.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x + 40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		}
		break;
	case RING:
		switch (current_entity->type)
		{
		case CLERIC:
			if (current_entity->ring.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x + 40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x + 40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x + 40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x + 40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x + 40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x + 40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x + 40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x + 40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x + 40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}

			}
			else
			{

				if (current_entity->ring.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->ring.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x + 40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->ring.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x + 40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->ring.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x + 40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->ring.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x + 40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->ring.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x + 40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->ring.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x + 40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->ring.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x + 40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->ring.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x + 40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->ring.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x + 40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}


			}
			break;
		case DWARF:
			if (current_entity->ring.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x + 40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x + 40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x + 40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x + 40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x + 40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x + 40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x + 40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x + 40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x + 40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->ring.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->ring.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x + 40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->ring.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x + 40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->ring.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x + 40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->ring.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x + 40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->ring.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x + 40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->ring.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x + 40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->ring.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x + 40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->ring.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x + 40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->ring.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x + 40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case WARRIOR:
			if (current_entity->ring.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x + 40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x + 40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x + 40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x + 40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x + 40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x + 40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x + 40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x + 40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x + 40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->ring.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->ring.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x + 40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->ring.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x + 40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->ring.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x + 40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->ring.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x + 40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->ring.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x + 40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->ring.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x + 40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->ring.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x + 40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->ring.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x + 40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->ring.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x + 40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case ELF:
			if (current_entity->ring.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x + 40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x + 40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x + 40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x + 40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x + 40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x + 40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x + 40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x + 40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x + 40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->ring.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->ring.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x + 40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->ring.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x + 40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->ring.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x + 40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->ring.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x + 40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->ring.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x + 40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->ring.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x + 40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->ring.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x + 40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->ring.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x + 40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->ring.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->ring.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x + 40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		}
		break;
	case ACCESORY:
		switch (current_entity->type)
		{
		case CLERIC:
			if (current_entity->accessory.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x + 40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x + 40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x + 40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x + 40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x + 40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x + 40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x + 40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x + 40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x + 40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}

			}
			else
			{
				if (current_entity->accessory.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->accessory.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[0]->screen_position.x + 40, cleric_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->accessory.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(cleric_statistics[1]->screen_position.x + 40, cleric_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->accessory.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[2]->screen_position.x + 40, cleric_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->accessory.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[5]->screen_position.x + 40, cleric_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->accessory.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[4]->screen_position.x + 40, cleric_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->accessory.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[3]->screen_position.x + 40, cleric_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->accessory.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[6]->screen_position.x + 40, cleric_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->accessory.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[7]->screen_position.x + 40, cleric_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->accessory.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(cleric_statistics[8]->screen_position.x + 40, cleric_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}


			}
			break;
		case DWARF:
			if (current_entity->accessory.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x + 40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x + 40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x + 40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x + 40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x + 40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x + 40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x + 40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x + 40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x + 40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->accessory.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->accessory.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[0]->screen_position.x + 40, dwarf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->accessory.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[1]->screen_position.x + 40, dwarf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->accessory.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[2]->screen_position.x + 40, dwarf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->accessory.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[5]->screen_position.x + 40, dwarf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->accessory.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[4]->screen_position.x + 40, dwarf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->accessory.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[3]->screen_position.x + 40, dwarf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->accessory.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[6]->screen_position.x + 40, dwarf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->accessory.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[7]->screen_position.x + 40, dwarf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->accessory.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(dwarf_statistics[8]->screen_position.x + 40, dwarf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			
			break;
		case WARRIOR:
			if (current_entity->accessory.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x + 40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x + 40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x + 40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x + 40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x + 40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x + 40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x + 40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x + 40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x + 40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->accessory.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->accessory.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(warrior_statistics[0]->screen_position.x + 40, warrior_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->accessory.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[1]->screen_position.x + 40, warrior_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->accessory.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[2]->screen_position.x + 40, warrior_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->accessory.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[5]->screen_position.x + 40, warrior_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->accessory.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[4]->screen_position.x + 40, warrior_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->accessory.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[3]->screen_position.x + 40, warrior_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->accessory.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[6]->screen_position.x + 40, warrior_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->accessory.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[7]->screen_position.x + 40, warrior_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->accessory.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(warrior_statistics[8]->screen_position.x + 40, warrior_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		case ELF:
			if (current_entity->accessory.equip_type == NO_EQUIP_TYPE)
			{
				if (dropped_items[0]->statistics.constitution != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x + 40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.constitution), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.focus != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x + 40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.focus), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.strength != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x + 40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.strength), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.intelligence != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x + 40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.intelligence), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.dexterity != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x + 40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.dexterity), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.agility != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x + 40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.agility), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.magical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x + 40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.magical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.physical_defense != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x + 40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.physical_defense), { 0,255,0 }, 17, 200));
				}
				if (dropped_items[0]->statistics.luck != 0)
				{
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x + 40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(dropped_items[0]->statistics.luck), { 0,255,0 }, 17, 200));
				}
			}
			else
			{
				if (current_entity->accessory.statistics.constitution != 0 || dropped_items[0]->statistics.constitution != 0)
				{
					total_value = dropped_items[0]->statistics.constitution - current_entity->accessory.statistics.constitution;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };

					preview_stats_items.push_back(new UITextBox(elf_statistics[0]->screen_position.x + 40, elf_statistics[0]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.focus != 0 || dropped_items[0]->statistics.focus != 0)
				{
					total_value = dropped_items[0]->statistics.focus - current_entity->accessory.statistics.focus;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[1]->screen_position.x + 40, elf_statistics[1]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.strength != 0 || dropped_items[0]->statistics.strength != 0)
				{
					total_value = dropped_items[0]->statistics.strength - current_entity->accessory.statistics.strength;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[2]->screen_position.x + 40, elf_statistics[2]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.intelligence != 0 || dropped_items[0]->statistics.intelligence != 0)
				{
					total_value = dropped_items[0]->statistics.intelligence - current_entity->accessory.statistics.intelligence;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[5]->screen_position.x + 40, elf_statistics[5]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.dexterity != 0 || dropped_items[0]->statistics.dexterity != 0)
				{
					total_value = dropped_items[0]->statistics.dexterity - current_entity->accessory.statistics.dexterity;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[4]->screen_position.x + 40, elf_statistics[4]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.agility != 0 || dropped_items[0]->statistics.agility != 0)
				{
					total_value = dropped_items[0]->statistics.agility - current_entity->accessory.statistics.agility;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[3]->screen_position.x + 40, elf_statistics[3]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.magical_defense != 0 || dropped_items[0]->statistics.magical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.magical_defense - current_entity->accessory.statistics.magical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[6]->screen_position.x + 40, elf_statistics[6]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.physical_defense != 0 || dropped_items[0]->statistics.physical_defense != 0)
				{
					total_value = dropped_items[0]->statistics.physical_defense - current_entity->accessory.statistics.physical_defense;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[7]->screen_position.x + 40, elf_statistics[7]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
				if (current_entity->accessory.statistics.luck != 0 || dropped_items[0]->statistics.luck != 0)
				{
					total_value = dropped_items[0]->statistics.luck - current_entity->accessory.statistics.luck;
					SDL_Color color = { 255,255,255 };

					if (total_value < 0)
						color = { 255,0,0 };
					else if (total_value > 0)
						color = { 0,255,0 };
					preview_stats_items.push_back(new UITextBox(elf_statistics[8]->screen_position.x + 40, elf_statistics[8]->screen_position.y, TEXTBOX, std::to_string(total_value), color, 17, 200));
				}
			}
			break;
		}
		break;
	}
	}
	}



