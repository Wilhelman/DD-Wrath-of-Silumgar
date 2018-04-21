#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctTextures.h"
#include "ctAudio.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "ctEntities.h"
#include "j1Map.h"

#include "ctSkillTree.h"
#include "ctCombat.h"

#include "ctFadeToBlack.h"


ctSkillTree::ctSkillTree() : ctModule()
{
	name = "skill_tree";
}

// Destructor
ctSkillTree::~ctSkillTree()
{}

// Called before render is available

bool ctSkillTree::Awake(pugi::xml_node& config)
{
	LOG("Loading SkillTree Map");
	bool ret = true;

	skill_tree_map_tmx = config.child("skill_tree_map_tmx").attribute("name").as_string();
	name_spritesheet_skill_tree_map = config.child("spritesheet").attribute("name").as_string();
	name_spritesheet_abilities = config.child("spritesheet_abilities").attribute("name").as_string();

	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadAbilities(config_file);



	for (pugi::xml_node read_warrior_icons = node->child("warrior").child("ability"); read_warrior_icons && ret; read_warrior_icons = read_warrior_icons.next_sibling("ability"))
	{
		warrior->tier = read_warrior_icons.attribute("tier").as_uint();
		warrior->branch = read_warrior_icons.attribute("branch").as_uint();
		warrior->ability_name = read_warrior_icons.attribute("name").as_string();
		warrior->icon_rect = { read_warrior_icons.child("icon").attribute("x").as_int(), read_warrior_icons.child("icon").attribute("y").as_int(), read_warrior_icons.child("icon").attribute("width").as_int(),read_warrior_icons.child("icon").attribute("height").as_int() };


	}
	return ret;
}

// Called before the first frame
bool ctSkillTree::Start()
{
	bool ret = true;

	spritesheet_skill_tree_map = App->tex->Load(name_spritesheet_skill_tree_map.c_str());

	spritesheet_abilities = App->tex->Load(name_spritesheet_abilities.c_str());
	if (spritesheet_skill_tree_map == NULL) {
		LOG("Fail to load spritesheet in SkillTree!");
		ret = false;
	}

	if (spritesheet_abilities == NULL) {
		LOG("Fail to load spritesheet_abilities in SkillTree!");
		ret = false;
	}

	App->map->sceneName = skill_tree_map_tmx.c_str();
	App->map->Load(App->map->sceneName.c_str());
	App->map->LayersSetUp();
	App->map->setAllLogicForMap();
	


	return ret;
}

// Called each loop iteration
bool ctSkillTree::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ctSkillTree::Update(float dt)
{
	// Draw everything --------------------------------------
	App->map->Draw();
	//DrawAbilities(warrior);

	//App->render->Blit(spritesheet_abilities,0,0,&warrior->icon_rect);
	/*if (current_hero == 1)
	{
	DrawAbilities(warrior_vec);
	}

	if (current_hero == 2)
	{
	DrawAbilities(cleric_vec);
	}

	if (current_hero == 3)
	{
	DrawAbilities(dwarf_vec);
	}

	if (current_hero == 4)
	{
	DrawAbilities(elf_vec);
	}*/

	return true;
}

// Called each loop iteration
bool ctSkillTree::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool ctSkillTree::CleanUp()
{
	LOG("Freeing ctSkillTree");

	App->map->CleanUp();

	return true;
}

bool ctSkillTree::Load(pugi::xml_node& load)
{
	bool ret = true;

	return ret;
}

bool ctSkillTree::Save(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}

void ctSkillTree::OnUITrigger(UIElement* elementTriggered, UI_State ui_state)
{
}

void ctSkillTree::DrawAbility(int x, int y, Ability* champion)
{

}

void ctSkillTree::DrawAbilities(Ability* champion)
{
	//for (int i = 0; i < App->map->basic.size(); i++)
	//{
	//	//(App->map->basic.at(i).x, App->map->basic.at(i).y, champion);
	//}

}