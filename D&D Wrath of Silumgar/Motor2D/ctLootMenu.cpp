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




	return true;
}

bool LootMenu::Update(float dt) 
{

	return true;
}


bool LootMenu::CleanUp()
{



	return true;
}

void LootMenu::Draw(SDL_Texture* sprites)
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


}

void LootMenu::LoadClerictStats() {
	Entity* current_entity = App->entities->GetCleric();

	std::string entity_stat = "Con  " + std::to_string((current_entity->base_stats.constitution * 13));
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


	entity_stat = "Luck " + std::to_string((current_entity->base_stats.luck));
	cleric_statistics.push_back(new UITextBox(135, 118, TEXTBOX, entity_stat, { 255,255,255,255 }, 10, 428));

}

void LootMenu::LoadWarriorStats() {
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


void LootMenu::LoadDwarfStats() {
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


void LootMenu::LoadElfStats() {
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


}

void LootMenu::SetUpInventory()
{



}

void LootMenu::LoadEquipableObjects()
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