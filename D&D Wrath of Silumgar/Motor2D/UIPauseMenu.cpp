#include "ctApp.h"
#include "UIPauseMenu.h"
#include "ctLog.h"
#include "ctGui.h"
#include "UIImage.h"
#include "UITextBox.h"
#include "ctRender.h"
#include "ctEntities.h"

#include "Cleric.h"
#include "Dwarf.h"
#include "Elf.h"
#include "Warrior.h"

UIPauseMenu::UIPauseMenu(int x, int y, UI_Type type, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;

	background = new UIImage(x, y, IMAGE, { 0,0,484,324 }, nullptr, this);
	LoadClerictStats();
	LoadWarriorStats();
	LoadDwarfStats();
	LoadElfStats();

}

UIPauseMenu::~UIPauseMenu() {

	App->gui->DeleteUIElement(*background);
	background = nullptr;

	for (int i = 0; i < cleric_statistics.size(); i++) {
		App->gui->DeleteUIElement(*cleric_statistics.at(i));
	}
	for (int i = 0; i < warrior_statistics.size(); i++) {
		App->gui->DeleteUIElement(*warrior_statistics.at(i));
	}
	for (int i = 0; i < dwarf_statistics.size(); i++) {
		App->gui->DeleteUIElement(*dwarf_statistics.at(i));
	}
	for (int i = 0; i < elf_statistics.size(); i++) {
		App->gui->DeleteUIElement(*elf_statistics.at(i));
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
	//App->render->Blit(Dialog_Text->texture, Dialog_Text->GetLocalPosition().x, Dialog_Text->GetLocalPosition().y, &Dialog_Text->current_rect);
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