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
	node = &node->child("usables");


	for (pugi::xml_node usable = node->child("item"); usable && ret; usable = usable.next_sibling("item"))
	{
		Item*  items = new Item();
		items->name = usable.attribute("name").as_string();
		items->objective = usable.attribute("objective").as_int();
		//items->usable_effects = usable.attribute("use").as_int();
	}
	


	

	return ret;
}

// Called before quitting
bool ctItems::CleanUp()
{

	return true;
}
