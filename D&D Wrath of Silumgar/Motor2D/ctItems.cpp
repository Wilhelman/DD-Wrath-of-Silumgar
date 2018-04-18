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

	/*pugi::xml_document	language_file;
	pugi::xml_node* node = &App->LoadLanguages(language_file);
	current_language = config.attribute("current").as_string();

	for (pugi::xml_node languages = node->first_child(); languages && ret; languages = languages.next_sibling())
	{
		string tmp_language;
		tmp_language = languages.name();
		posible_languages.push_back(tmp_language);
	}

	node = &node->child(current_language.c_str());

	dictionary.MM_about_btn = (node->child("MM_about_btn").attribute("string").as_string());
	dictionary.MM_continue_btn = (node->child("MM_continue_btn").attribute("string").as_string());
	dictionary.MM_new_game_btn = (node->child("MM_new_game_btn").attribute("string").as_string());
	dictionary.MM_quit_btn = (node->child("MM_quit_btn").attribute("string").as_string());
	dictionary.MM_settings_btn = (node->child("MM_settings_btn").attribute("string").as_string());*/

	return ret;
}

// Called before quitting
bool ctItems::CleanUp()
{

	return true;
}
