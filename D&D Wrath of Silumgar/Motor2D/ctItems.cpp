#include "ctDefs.h"
#include "ctLog.h"
#include "ctItems.h"
#include "ctApp.h"
#include "Entity.h"


ctItems::ctItems() : ctModule()
{
	name = "items";
}

// Destructor
ctItems::~ctItems()
{}

bool ctItems::Load(pugi::xml_node& save) {
	bool ret = true;

	return ret;
}

bool ctItems::Save(pugi::xml_node& save)const {
	bool ret = true;

	return ret;
}

// Called before render is available
bool ctItems::Awake(pugi::xml_node& config)
{
	LOG("Loading Items");
	bool ret = true;

	pugi::xml_document	items_file;
	pugi::xml_node* node = &App->LoadItems(items_file);

	// ------------------------------------- USABLE ITEMS --------------------------------------- //

	for (pugi::xml_node usable = node->child("usables").child("item"); usable && ret; usable = usable.next_sibling("item"))
	{
		Item item;
		item.type = USABLE;
		item.name = usable.attribute("name").as_string();
		item.objective = usable.attribute("objective").as_int();

		std::string tmp = usable.attribute("use").as_string();

		Action tmp_action;

		if (tmp == "LOW_HEALTH_RECOVER") {
			item.usable_effects = LOW_HEALTH_RECOVER;
			tmp_action.type = LOW_HEALTH_RECOVER_ACTION;
		}
		else if (tmp == "HIGH_HEALTH_RECOVER") {
			item.usable_effects = HIGH_HEALTH_RECOVER;
			tmp_action.type = HIGH_HEALTH_RECOVER_ACTION;
		}
		else if (tmp == "LOW_MANA_RECOVER") {
			item.usable_effects = LOW_MANA_RECOVER;
			tmp_action.type = LOW_MANA_RECOVER_ACTION;
		}
		else if (tmp == "HIGH_MANA_RECOVER") {
			item.usable_effects = HIGH_MANA_RECOVER;
			tmp_action.type = HIGH_MANA_RECOVER_ACTION;
		}
		else if (tmp == "POISONED_DAGGER") {
			item.usable_effects = POISONED_DAGGER;
			tmp_action.type = POISONED_DAGGER_ACTION;
		}
		else if (tmp == "BURN_TARGET") {
			item.usable_effects = BURN_TARGET;
			tmp_action.type = BURN_TARGET_ACTION;
		}
		else if (tmp == "REVIVE_AN_ALLY") {
			item.usable_effects = REVIVE_AN_ALLY;
			tmp_action.type = REVIVE_AN_ALLY_ACTION;
		}

		/*
		
		std::string name;
	std::string description;
	ActionType type = ACTION_NOT_DEFINED;
	ActionObjectiveType objective = OBJECTIVE_NOT_DEFINED;

	uint mana_cost = 0u;

	int health_points_effect_to_himself = 0;
	int mana_points_effect_to_himself = 0;
	int health_points_effect = 0;
	int mana_points_effect = 0;
		*/

		tmp_action.name = item.name;
		switch (tmp_action.type)
		{
		case LOW_HEALTH_RECOVER_ACTION: {
			tmp_action.health_points_effect = 30;
		}
		break;
		case POISONED_DAGGER_ACTION: {

		}
		break;
		default:
			break;
		}
		
		item.action = tmp_action;

		item.draw_coords = { usable.child("draw_coords").attribute("x").as_int(),usable.child("draw_coords").attribute("y").as_int(),usable.child("draw_coords").attribute("width").as_int(),usable.child("draw_coords").attribute("height").as_int() };
		usable_items.push_back(item);
	}

	// ------------------------------------- EQUIPABLE ITEMS --------------------------------------- //
	for (pugi::xml_node equip = node->child("equips").child("equip"); equip && ret; equip = equip.next_sibling("equip"))
	{
		Item item;
		item.type = EQUIP;
		item.name = equip.attribute("name").as_string();

		std::string tmp = equip.attribute("type").as_string();
		if (tmp == "BOOT")
			item.equip_type = BOOT;
		else if (tmp == "ARMS")
			item.equip_type = GUANTLET;
		else if (tmp == "HANDS")
			item.equip_type = RING;
		else if (tmp == "ACCESSORIES")
			item.equip_type = ACCESORY;
		else if (tmp == "ARMOR")
			item.equip_type = CHEST;
		else if (tmp == "HELMET")
			item.equip_type = HELM;
		else if (tmp == "SHIELDS")
			item.equip_type = SHIELD;
		else if (tmp == "WEAPONS")
			item.equip_type = WEAPON;

		item.draw_coords = { equip.child("draw_coords").attribute("x").as_int(),equip.child("draw_coords").attribute("y").as_int(),equip.child("draw_coords").attribute("width").as_int(),equip.child("draw_coords").attribute("height").as_int() };
		
		for (pugi::xml_node stat = equip.child("stats").child("stat"); stat && ret; stat = stat.next_sibling("stat"))
		{
			
			item.statistics.strength += stat.attribute("str").as_int(0);
			
			item.statistics.intelligence += stat.attribute("int").as_int(0);
		
			item.statistics.agility += stat.attribute("agi").as_int(0);
			
			item.statistics.constitution += stat.attribute("cnst").as_int(0);
			
			item.statistics.focus += stat.attribute("fcs").as_int(0);
		
			item.statistics.dexterity += stat.attribute("dext").as_int(0);
		
			item.statistics.magical_defense += stat.attribute("mr").as_int(0);
		
			item.statistics.physical_defense += stat.attribute("pr").as_int(0);
		
			item.statistics.luck += stat.attribute("lck").as_int(0);
		}

		item.tier = equip.attribute("tier").as_int();

		switch (item.tier)
		{
		case 1:
			tier_1_equips.push_back(item);
			break;
		case 2:
			tier_2_equips.push_back(item);
			break;
		case 3:
			tier_3_equips.push_back(item);
			break;
		default:
			break;
		}
		
		
	}

	return ret;
}

// Called before quitting
bool ctItems::CleanUp()
{
	//maybe we have to do news...
	//TODO CLEAN ALL ITEMS

	return true;
}
