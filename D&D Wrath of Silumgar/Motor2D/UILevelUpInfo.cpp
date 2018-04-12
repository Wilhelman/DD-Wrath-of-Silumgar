#include "ctApp.h"
#include "UILevelUpInfo.h"
#include "ctLog.h"
#include "ctInput.h"
#include "ctAudio.h"

UILevelUpInfo::UILevelUpInfo(int x, int y, UI_Type type, EntityType hero, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;

	background = App->gui->AddUIImage(x, y, { 1141,484,340,178 }, callback);
	if (hero == WARRIOR)
	{
		image_hero = App->gui->AddUIImage(x+30, y+27, {316,152,52,29}, callback);
		text = App->gui->AddUITextBox(x + 30, y+70, 18, 250, "Warrior Level Up!", { 255,255,255,255 }, this, Second_Font);
		level_number = App->gui->AddUITextBox(x + 30, y +90, 15,130,"Level 1 to 2", { 255,255,255,255 }, this, Second_Font);

		constitution = App->gui->AddUITextBox(x + 190, y + 25, 12, 130, "Constitution: 9 -> 11  ", { 255,255,255,255 }, this, Second_Font);
		focus = App->gui->AddUITextBox(x + 190, y + 40, 12, 150, "Focus: 4 -> 5 ", { 255,255,255,255 }, this, Second_Font);
		force = App->gui->AddUITextBox(x + 190, y + 55, 12, 170, "Force: 5 -> 7 ", { 255,255,255,255 }, this, Second_Font);
		agility = App->gui->AddUITextBox(x + 190, y + 70, 12, 130, "Agility: 4 -> 5 ", { 255,255,255,255 }, this, Second_Font);
		skill = App->gui->AddUITextBox(x + 190, y + 85, 12, 130, "Skill: 6 -> 8 ", { 255,255,255,255 }, this, Second_Font);
		intelligence = App->gui->AddUITextBox(x + 190, y + 100, 12, 130, "Intelligence: 2 -> 3 ", { 255,255,255,255 }, this, Second_Font);
		physical_defense = App->gui->AddUITextBox(x + 190, y + 115, 12, 200, "Physical Defense: 5 -> 7 ", { 255,255,255,255 }, this, Second_Font);
		magic_defense = App->gui->AddUITextBox(x + 190, y + 130, 12, 200, "Magic Defense: 4 -> 6 ", { 255,255,255,255 }, this, Second_Font);
		luck = App->gui->AddUITextBox(x + 190, y + 145, 12, 130, "Luck: 1 -> 2 ", { 255,255,255,255 }, this, Second_Font);
		ability_unlock = App->gui->AddUITextBox(x+ 30, y + 110, 15,100, "New ability:", { 255,255,255,255 }, this, Second_Font);
		name_ability = App->gui->AddUITextBox(x + 30, y + 125, 15, 100, "KICK", { 255,0,0,255 },this,Second_Font);
	}
	if (hero == ELF)
	{
		image_hero = App->gui->AddUIImage(x + 30, y + 27, { 946,152,52,29 }, callback);
		text = App->gui->AddUITextBox(x + 30, y + 70, 18, 250, "ELf Level Up!", { 255,255,255,255 }, this, Second_Font);
		level_number = App->gui->AddUITextBox(x + 30, y + 90, 15, 130, "Level 1 to 2", { 255,255,255,255 }, this, Second_Font);

		constitution = App->gui->AddUITextBox(x + 190, y + 25, 12, 130, "Constitution: 5 -> 6  ", { 255,255,255,255 }, this, Second_Font);
		focus = App->gui->AddUITextBox(x + 190, y + 40, 12, 150, "Focus: 7 -> 9 ", { 255,255,255,255 }, this, Second_Font);
		force = App->gui->AddUITextBox(x + 190, y + 55, 12, 170, "Force: 2 -> 3 ", { 255,255,255,255 }, this, Second_Font);
		agility = App->gui->AddUITextBox(x + 190, y + 70, 12, 130, "Agility: 6 -> 8 ", { 255,255,255,255 }, this, Second_Font);
		skill = App->gui->AddUITextBox(x + 190, y + 85, 12, 130, "Skill: 3 -> 5 ", { 255,255,255,255 }, this, Second_Font);
		intelligence = App->gui->AddUITextBox(x + 190, y + 100, 12, 130, "Intelligence: 9 -> 11 ", { 255,255,255,255 }, this, Second_Font);
		physical_defense = App->gui->AddUITextBox(x + 190, y + 115, 12, 200, "Physical Defense: 2 -> 3 ", { 255,255,255,255 }, this, Second_Font);
		magic_defense = App->gui->AddUITextBox(x + 190, y + 130, 12, 200, "Magic Defense: 3 -> 5 ", { 255,255,255,255 }, this, Second_Font);
		luck = App->gui->AddUITextBox(x + 190, y + 145, 12, 130, "Luck: 3 -> 4 ", { 255,255,255,255 }, this, Second_Font);
		ability_unlock = App->gui->AddUITextBox(x + 30, y + 110, 15, 100, "New ability:", { 255,255,255,255 }, this, Second_Font);
		name_ability = App->gui->AddUITextBox(x + 30, y + 125, 15, 100, "FIREBALL", { 255,0,0,255 }, this, Second_Font);
	}
	if (hero == DWARF)
	{
		image_hero = App->gui->AddUIImage(x + 30, y + 27, { 631,152,52,29 }, callback);
		text = App->gui->AddUITextBox(x + 30, y + 70, 18, 250, "Dwarf Level Up!", { 255,255,255,255 }, this, Second_Font);
		level_number = App->gui->AddUITextBox(x + 30, y + 90, 15, 130, "Level 1 to 2", { 255,255,255,255 }, this, Second_Font);

		constitution = App->gui->AddUITextBox(x + 190, y + 25, 12, 130, "Constitution: 7 -> 9  ", { 255,255,255,255 }, this, Second_Font);
		focus = App->gui->AddUITextBox(x + 190, y + 40, 12, 150, "Focus: 4 -> 5 ", { 255,255,255,255 }, this, Second_Font);
		force = App->gui->AddUITextBox(x + 190, y + 55, 12, 170, "Force: 9 -> 11 ", { 255,255,255,255 }, this, Second_Font);
		agility = App->gui->AddUITextBox(x + 190, y + 70, 12, 130, "Agility: 3 -> 4 ", { 255,255,255,255 }, this, Second_Font);
		skill = App->gui->AddUITextBox(x + 190, y + 85, 12, 130, "Skill: 4 -> 6 ", { 255,255,255,255 }, this, Second_Font);
		intelligence = App->gui->AddUITextBox(x + 190, y + 100, 12, 130, "Intelligence: 1 -> 2 ", { 255,255,255,255 }, this, Second_Font);
		physical_defense = App->gui->AddUITextBox(x + 190, y + 115, 12, 200, "Physical Defense: 4 -> 6 ", { 255,255,255,255 }, this, Second_Font);
		magic_defense = App->gui->AddUITextBox(x + 190, y + 130, 12, 200, "Magic Defense: 3 -> 4 ", { 255,255,255,255 }, this, Second_Font);
		luck = App->gui->AddUITextBox(x + 190, y + 145, 12, 130, "Luck: 5 -> 7 ", { 255,255,255,255 }, this, Second_Font);
		ability_unlock = App->gui->AddUITextBox(x + 30, y + 110, 15, 100, "New ability:", { 255,255,255,255 }, this, Second_Font);
		name_ability = App->gui->AddUITextBox(x + 30, y + 125, 15, 100, "ALGO", { 255,0,0,255 }, this, Second_Font);
	}
	if (hero == CLERIC)
	{
		image_hero = App->gui->AddUIImage(x + 30, y + 27, { 1,152,52,29 }, callback);
		text = App->gui->AddUITextBox(x + 30, y + 70, 18, 250, "Cleric Level Up!", { 255,255,255,255 }, this, Second_Font);
		level_number = App->gui->AddUITextBox(x + 30, y + 90, 15, 130, "Level 1 to 2", { 255,255,255,255 }, this, Second_Font);

		constitution = App->gui->AddUITextBox(x + 190, y + 25, 12, 130, "Constitution: 6 -> 7 ", { 255,255,255,255 }, this, Second_Font);
		focus = App->gui->AddUITextBox(x + 190, y + 40, 12, 150, "Focus: 5 -> 7 ", { 255,255,255,255 }, this, Second_Font);
		force = App->gui->AddUITextBox(x + 190, y + 55, 12, 170, "Force: 6 ->7 ", { 255,255,255,255 }, this, Second_Font);
		agility = App->gui->AddUITextBox(x + 190, y + 70, 12, 130, "Agility: 1 -> 2 ", { 255,255,255,255 }, this, Second_Font);
		skill = App->gui->AddUITextBox(x + 190, y + 85, 12, 130, "Skill: 4 -> 6 ", { 255,255,255,255 }, this, Second_Font);
		intelligence = App->gui->AddUITextBox(x + 190, y + 100, 12, 130, "Intelligence: 7 -> 9 ", { 255,255,255,255 }, this, Second_Font);
		physical_defense = App->gui->AddUITextBox(x + 190, y + 115, 12, 200, "Physical Defense: 3 -> 4 ", { 255,255,255,255 }, this, Second_Font);
		magic_defense = App->gui->AddUITextBox(x + 190, y + 130, 12, 200, "Magic Defense: 4 -> 6 ", { 255,255,255,255 }, this, Second_Font);
		luck = App->gui->AddUITextBox(x + 190, y + 145, 12, 130, "Luck: 4 -> 6 ", { 255,255,255,255 }, this, Second_Font);
		ability_unlock = App->gui->AddUITextBox(x + 30, y + 110, 15, 100, "New ability:", { 255,255,255,255 }, this, Second_Font);
		name_ability = App->gui->AddUITextBox(x + 30, y + 125, 15, 100, "HEAL", { 255,0,0,255 }, this, Second_Font);
	}
}

UILevelUpInfo::~UILevelUpInfo() {

	background->to_destroy = true;
	image_hero->to_destroy = true;
	text->to_destroy = true;
	level_number->to_destroy = true;
	constitution->to_destroy = true;
	focus->to_destroy = true;
	force->to_destroy = true;
	skill->to_destroy = true;
	luck->to_destroy = true;
	agility->to_destroy = true;
	intelligence->to_destroy = true;
	physical_defense->to_destroy = true;
	magic_defense->to_destroy = true;
	ability_unlock->to_destroy = true;
	name_ability->to_destroy = true;
}

void UILevelUpInfo::Update()
{
	

}