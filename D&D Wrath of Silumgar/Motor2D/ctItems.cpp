#include "ctDefs.h"
#include "ctLog.h"
#include "ctItems.h"
#include "ctApp.h"


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
	node = &node->child("usables");

	for (pugi::xml_node usable = node->child("item"); usable && ret; usable = usable.next_sibling("item"))
	{
		Item item;
		item.name = usable.attribute("name").as_string();
		item.objective = usable.attribute("objective").as_int();

		std::string tmp = usable.attribute("use").as_string();
		if (tmp == "LOW_HEALTH_RECOVER")
			item.usable_effects = LOW_HEALTH_RECOVER;
		else if (tmp == "HIGH_HEALTH_RECOVER")
			item.usable_effects = HIGH_HEALTH_RECOVER;
		else if (tmp == "LOW_MANA_RECOVER")
			item.usable_effects = LOW_MANA_RECOVER;
		else if (tmp == "HIGH_MANA_RECOVER")
			item.usable_effects = HIGH_MANA_RECOVER;
		else if (tmp == "POISONED_DAGGER")
			item.usable_effects = POISONED_DAGGER;
		else if (tmp == "BURN_TARGET")
			item.usable_effects = BURN_TARGET;
		else if (tmp == "REVIVE_AN_ALLY")
			item.usable_effects = REVIVE_AN_ALLY;

		item.draw_coords = { usable.child("draw_coords").attribute("x").as_int(),usable.child("draw_coords").attribute("y").as_int(),usable.child("draw_coords").attribute("width").as_int(),usable.child("draw_coords").attribute("height").as_int() };
		usable_items.push_back(item);
	}

	return ret;
}

// Called before quitting
bool ctItems::CleanUp()
{

	return true;
}
