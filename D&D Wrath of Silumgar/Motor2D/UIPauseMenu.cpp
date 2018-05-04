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

#include "Cleric.h"
#include "Dwarf.h"
#include "Elf.h"
#include "Warrior.h"

UIPauseMenu::UIPauseMenu(int x, int y, UI_Type type, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;

	background = new UIImage(x, y, IMAGE, { 0,0,484,324 }, nullptr, this);
	App->entities->SpawnEntity(30, 125, CLERIC);
	App->entities->SpawnEntity(30, 275, WARRIOR);
	App->entities->SpawnEntity(250, 125, DWARF);
	App->entities->SpawnEntity(250, 275, ELF);
	//-------------------------------
	App->entities->GetElf()->AddEquipItem(App->items->tier_2_equips.at(3));
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


}

UIPauseMenu::~UIPauseMenu() {

	background->~UIElement();
	background = nullptr;

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

}

void UIPauseMenu::Update() {
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
	App->render->Blit(App->gui->atlas, arrow->screen_position.x, arrow->screen_position.y, &arrow->current_rect);
	
	DrawItems();
}

void UIPauseMenu::DrawItems() {
	Entity* cleric = App->entities->GetCleric();
	//App->render->UIBlit(textura , cleric_helmet_rect.x, cleric_helmet_rect.y, &cleric->helmet.draw_coords);

	Entity* warrior = App->entities->GetWarrior();


	Entity* dwarf = App->entities->GetDwarf();


	Entity* elf = App->entities->GetElf();

}

void UIPauseMenu::LoadClerictStats() {
	Entity* current_entity = App->entities->GetCleric();
	//char constitution[(((sizeof current_entity->GetCurrentHealthPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(constitution, "%d", current_entity->GetCurrentHealthPoints());
	//string con = "Con    ";
	//string entity_con = con + constitution;
	string entity_con = "Con    10";
	cleric_statistics.push_back(new UITextBox(135, 38, TEXTBOX, entity_con, { 255,255,255,255 }, 10, 428));
	//char focus[(((sizeof current_entity->GetCurrentManaPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(focus, "%d", current_entity->GetCurrentManaPoints());
	//string foc = "Foc    ";
	//string entity_foc = foc + focus;
	string entity_foc = "Foc    10";
	cleric_statistics.push_back(new UITextBox(135, 48, TEXTBOX, entity_foc, { 255,255,255,255 }, 10, 428));
	//char strength[(((sizeof current_entity->GetCurrentStrength()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(strength, "%d", current_entity->GetCurrentStrength());
	//string str = "Str    ";
	//string entity_str = str + strength;
	string entity_str = "Str    10";
	cleric_statistics.push_back(new UITextBox(135, 58, TEXTBOX, entity_str, { 255,255,255,255 }, 10, 428));
	//char agility[(((sizeof current_entity->GetCurrentAgilityPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(agility, "%d", current_entity->GetCurrentAgilityPoints());
	//string agl = "Agl    ";
	//string entity_agl = agl + agility;
	string entity_agl = "Agl    10";
	cleric_statistics.push_back(new UITextBox(135, 68, TEXTBOX, entity_agl, { 255,255,255,255 }, 10, 428));
	//char dexterity[(((sizeof current_entity->GetCurrentDexterityPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(dexterity, "%d", current_entity->GetCurrentDexterityPoints());
	//string dex = "Dex    ";
	//string entity_dex = dex + dexterity;
	string entity_dex = "Dex    10";
	cleric_statistics.push_back(new UITextBox(135, 78, TEXTBOX, entity_dex, { 255,255,255,255 }, 10, 428));
	//char intelligence[(((sizeof current_entity->GetCurrentIntelligencePoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(intelligence, "%d", current_entity->GetCurrentIntelligencePoints());
	//string int = "Int    ";
	//string entity_int = int + intelligence;
	string entity_int = "Int    10";
	cleric_statistics.push_back(new UITextBox(135, 88, TEXTBOX, entity_int, { 255,255,255,255 }, 10, 428));
	//char magical_def[(((sizeof current_entity->GetCurrentMagicalDefensePoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(magical_def, "%d", current_entity->GetCurrentMagicalDefensePoints());
	//string mdef = "M.Def  ";
	//string entity_mdef = mdef + magical_def;
	string entity_mdef = "M.Def  10";
	cleric_statistics.push_back(new UITextBox(135, 98, TEXTBOX, entity_mdef, { 255,255,255,255 }, 10, 428));
	//char physical_def[(((sizeof current_entity->GetCurrentPhysicalDefensePoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(physical_def, "%d", current_entity->GetCurrentPhysicalDefensePoints());
	//string pdef = "P.Def  ";
	//string entity_pdef = pdef + physical_def;
	string entity_pdef = "P.Def  10";
	cleric_statistics.push_back(new UITextBox(135, 108, TEXTBOX, entity_pdef, { 255,255,255,255 }, 10, 428));
	//char luck[(((sizeof current_entity->GetCurrentPhysicalLuckPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(luck, "%d", current_entity->GetCurrentPhysicalLuckPoints());
	//string lck = "Lck   ";
	//string entity_lck = lck + luck;
	string entity_lck = "Lck   10";
	cleric_statistics.push_back(new UITextBox(135, 118, TEXTBOX, entity_lck, { 255,255,255,255 }, 10, 428));
}

void UIPauseMenu::LoadWarriorStats() {
	Entity* current_entity = App->entities->GetWarrior();
	//char constitution[(((sizeof current_entity->GetCurrentHealthPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(constitution, "%d", current_entity->GetCurrentHealthPoints());
	//string con = "Con    ";
	//string entity_con = con + constitution;
	string entity_con = "Con    10";
	warrior_statistics.push_back(new UITextBox(135, 195, TEXTBOX, entity_con, { 255,255,255,255 }, 10, 428));
	//char focus[(((sizeof current_entity->GetCurrentManaPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(focus, "%d", current_entity->GetCurrentManaPoints());
	//string foc = "Foc    ";
	//string entity_foc = foc + focus;
	string entity_foc = "Foc    10";
	warrior_statistics.push_back(new UITextBox(135, 205, TEXTBOX, entity_foc, { 255,255,255,255 }, 10, 428));
	//char strength[(((sizeof current_entity->GetCurrentStrength()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(strength, "%d", current_entity->GetCurrentStrength());
	//string str = "Str    ";
	//string entity_str = str + strength;
	string entity_str = "Str    10";
	warrior_statistics.push_back(new UITextBox(135, 215, TEXTBOX, entity_str, { 255,255,255,255 }, 10, 428));
	//char agility[(((sizeof current_entity->GetCurrentAgilityPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(agility, "%d", current_entity->GetCurrentAgilityPoints());
	//string agl = "Agl    ";
	//string entity_agl = agl + agility;
	string entity_agl = "Agl    10";
	warrior_statistics.push_back(new UITextBox(135, 225, TEXTBOX, entity_agl, { 255,255,255,255 }, 10, 428));
	//char dexterity[(((sizeof current_entity->GetCurrentDexterityPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(dexterity, "%d", current_entity->GetCurrentDexterityPoints());
	//string dex = "Dex    ";
	//string entity_dex = dex + dexterity;
	string entity_dex = "Dex    10";
	warrior_statistics.push_back(new UITextBox(135, 235, TEXTBOX, entity_dex, { 255,255,255,255 }, 10, 428));
	//char intelligence[(((sizeof current_entity->GetCurrentIntelligencePoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(intelligence, "%d", current_entity->GetCurrentIntelligencePoints());
	//string int = "Int    ";
	//string entity_int = int + intelligence;
	string entity_int = "Int    10";
	warrior_statistics.push_back(new UITextBox(135, 245, TEXTBOX, entity_int, { 255,255,255,255 }, 10, 428));
	//char magical_def[(((sizeof current_entity->GetCurrentMagicalDefensePoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(magical_def, "%d", current_entity->GetCurrentMagicalDefensePoints());
	//string mdef = "M.Def  ";
	//string entity_mdef = mdef + magical_def;
	string entity_mdef = "M.Def  10";
	warrior_statistics.push_back(new UITextBox(135, 255, TEXTBOX, entity_mdef, { 255,255,255,255 }, 10, 428));
	//char physical_def[(((sizeof current_entity->GetCurrentPhysicalDefensePoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(physical_def, "%d", current_entity->GetCurrentPhysicalDefensePoints());
	//string pdef = "P.Def  ";
	//string entity_pdef = pdef + physical_def;
	string entity_pdef = "P.Def  10";
	warrior_statistics.push_back(new UITextBox(135, 265, TEXTBOX, entity_pdef, { 255,255,255,255 }, 10, 428));
	//char luck[(((sizeof current_entity->GetCurrentPhysicalLuckPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(luck, "%d", current_entity->GetCurrentPhysicalLuckPoints());
	//string lck = "Lck   ";
	//string entity_lck = lck + luck;
	string entity_lck = "Lck   10";
	warrior_statistics.push_back(new UITextBox(135, 275, TEXTBOX, entity_lck, { 255,255,255,255 }, 10, 428));
}


void UIPauseMenu::LoadDwarfStats() {
	Entity* current_entity = App->entities->GetDwarf();
	//char constitution[(((sizeof current_entity->GetCurrentHealthPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(constitution, "%d", current_entity->GetCurrentHealthPoints());
	//string con = "Con    ";
	//string entity_con = con + constitution;
	string entity_con = "Con    10";
	dwarf_statistics.push_back(new UITextBox(340, 38, TEXTBOX, entity_con, { 255,255,255,255 }, 10, 428));
	//char focus[(((sizeof current_entity->GetCurrentManaPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(focus, "%d", current_entity->GetCurrentManaPoints());
	//string foc = "Foc    ";
	//string entity_foc = foc + focus;
	string entity_foc = "Foc    10";
	dwarf_statistics.push_back(new UITextBox(340, 48, TEXTBOX, entity_foc, { 255,255,255,255 }, 10, 428));
	//char strength[(((sizeof current_entity->GetCurrentStrength()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(strength, "%d", current_entity->GetCurrentStrength());
	//string str = "Str    ";
	//string entity_str = str + strength;
	string entity_str = "Str    10";
	dwarf_statistics.push_back(new UITextBox(340, 58, TEXTBOX, entity_str, { 255,255,255,255 }, 10, 428));
	//char agility[(((sizeof current_entity->GetCurrentAgilityPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(agility, "%d", current_entity->GetCurrentAgilityPoints());
	//string agl = "Agl    ";
	//string entity_agl = agl + agility;
	string entity_agl = "Agl    10";
	dwarf_statistics.push_back(new UITextBox(340, 68, TEXTBOX, entity_agl, { 255,255,255,255 }, 10, 428));
	//char dexterity[(((sizeof current_entity->GetCurrentDexterityPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(dexterity, "%d", current_entity->GetCurrentDexterityPoints());
	//string dex = "Dex    ";
	//string entity_dex = dex + dexterity;
	string entity_dex = "Dex    10";
	dwarf_statistics.push_back(new UITextBox(340, 78, TEXTBOX, entity_dex, { 255,255,255,255 }, 10, 428));
	//char intelligence[(((sizeof current_entity->GetCurrentIntelligencePoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(intelligence, "%d", current_entity->GetCurrentIntelligencePoints());
	//string int = "Int    ";
	//string entity_int = int + intelligence;
	string entity_int = "Int    10";
	dwarf_statistics.push_back(new UITextBox(340, 88, TEXTBOX, entity_int, { 255,255,255,255 }, 10, 428));
	//char magical_def[(((sizeof current_entity->GetCurrentMagicalDefensePoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(magical_def, "%d", current_entity->GetCurrentMagicalDefensePoints());
	//string mdef = "M.Def  ";
	//string entity_mdef = mdef + magical_def;
	string entity_mdef = "M.Def  10";
	dwarf_statistics.push_back(new UITextBox(340, 98, TEXTBOX, entity_mdef, { 255,255,255,255 }, 10, 428));
	//char physical_def[(((sizeof current_entity->GetCurrentPhysicalDefensePoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(physical_def, "%d", current_entity->GetCurrentPhysicalDefensePoints());
	//string pdef = "P.Def  ";
	//string entity_pdef = pdef + physical_def;
	string entity_pdef = "P.Def  10";
	dwarf_statistics.push_back(new UITextBox(340, 108, TEXTBOX, entity_pdef, { 255,255,255,255 }, 10, 428));
	//char luck[(((sizeof current_entity->GetCurrentPhysicalLuckPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(luck, "%d", current_entity->GetCurrentPhysicalLuckPoints());
	//string lck = "Lck   ";
	//string entity_lck = lck + luck;
	string entity_lck = "Lck   10";
	dwarf_statistics.push_back(new UITextBox(340, 118, TEXTBOX, entity_lck, { 255,255,255,255 }, 10, 428));
}


void UIPauseMenu::LoadElfStats() {
	Entity* current_entity = App->entities->GetElf();
	//char constitution[(((sizeof current_entity->GetCurrentHealthPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(constitution, "%d", current_entity->GetCurrentHealthPoints());
	//string con = "Con    ";
	//string entity_con = con + constitution;
	string entity_con = "Con    10";
	elf_statistics.push_back(new UITextBox(340, 195, TEXTBOX, entity_con, { 255,255,255,255 }, 10, 428));
	//char focus[(((sizeof current_entity->GetCurrentManaPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(focus, "%d", current_entity->GetCurrentManaPoints());
	//string foc = "Foc    ";
	//string entity_foc = foc + focus;
	string entity_foc = "Foc    10";
	elf_statistics.push_back(new UITextBox(340, 205, TEXTBOX, entity_foc, { 255,255,255,255 }, 10, 428));
	//char strength[(((sizeof current_entity->GetCurrentStrength()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(strength, "%d", current_entity->GetCurrentStrength());
	//string str = "Str    ";
	//string entity_str = str + strength;
	string entity_str = "Str    10";
	elf_statistics.push_back(new UITextBox(340, 215, TEXTBOX, entity_str, { 255,255,255,255 }, 10, 428));
	//char agility[(((sizeof current_entity->GetCurrentAgilityPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(agility, "%d", current_entity->GetCurrentAgilityPoints());
	//string agl = "Agl    ";
	//string entity_agl = agl + agility;
	string entity_agl = "Agl    10";
	elf_statistics.push_back(new UITextBox(340, 225, TEXTBOX, entity_agl, { 255,255,255,255 }, 10, 428));
	//char dexterity[(((sizeof current_entity->GetCurrentDexterityPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(dexterity, "%d", current_entity->GetCurrentDexterityPoints());
	//string dex = "Dex    ";
	//string entity_dex = dex + dexterity;
	string entity_dex = "Dex    10";
	elf_statistics.push_back(new UITextBox(340, 235, TEXTBOX, entity_dex, { 255,255,255,255 }, 10, 428));
	//char intelligence[(((sizeof current_entity->GetCurrentIntelligencePoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(intelligence, "%d", current_entity->GetCurrentIntelligencePoints());
	//string int = "Int    ";
	//string entity_int = int + intelligence;
	string entity_int = "Int    10";
	elf_statistics.push_back(new UITextBox(340, 245, TEXTBOX, entity_int, { 255,255,255,255 }, 10, 428));
	//char magical_def[(((sizeof current_entity->GetCurrentMagicalDefensePoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(magical_def, "%d", current_entity->GetCurrentMagicalDefensePoints());
	//string mdef = "M.Def  ";
	//string entity_mdef = mdef + magical_def;
	string entity_mdef = "M.Def  10";
	elf_statistics.push_back(new UITextBox(340, 255, TEXTBOX, entity_mdef, { 255,255,255,255 }, 10, 428));
	//char physical_def[(((sizeof current_entity->GetCurrentPhysicalDefensePoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(physical_def, "%d", current_entity->GetCurrentPhysicalDefensePoints());
	//string pdef = "P.Def  ";
	//string entity_pdef = pdef + physical_def;
	string entity_pdef = "P.Def  10";
	elf_statistics.push_back(new UITextBox(340, 265, TEXTBOX, entity_pdef, { 255,255,255,255 }, 10, 428));
	//char luck[(((sizeof current_entity->GetCurrentPhysicalLuckPoints()) * CHAR_BIT) + 2) / 3 + 2];
	//sprintf_s(luck, "%d", current_entity->GetCurrentPhysicalLuckPoints());
	//string lck = "Lck   ";
	//string entity_lck = lck + luck;
	string entity_lck = "Lck   10";
	elf_statistics.push_back(new UITextBox(340, 275, TEXTBOX, entity_lck, { 255,255,255,255 }, 10, 428));
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






}

void UIPauseMenu::LoadEquipableObjects()
{
	Entity* current_entity = nullptr;
	uint entities_added = 0;
	

	while (entities_added < 4);
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
			switch (equip_type)
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







	}




}