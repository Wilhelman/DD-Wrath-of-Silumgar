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
#include "ctGui.h"
#include "UIElement.h"
#include "ctPerfTimer.h"
#include "ctEntities.h"

#include "ctSkillTree.h"
#include "ctCombat.h"
#include "ctWorldMap.h"

#include "ctFadeToBlack.h"

#include "Cleric.h"
#include "Warrior.h"
#include "Dwarf.h"
#include "Elf.h"


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
	//name_spritesheet_skill_tree_map = config.child("spritesheet").attribute("name").as_string();
	name_spritesheet_abilities = config.child("spritesheet_abilities").attribute("name").as_string();

	return ret;
}

// Called before the first frame
bool ctSkillTree::Start()
{
	bool ret = true;

	//spritesheet_skill_tree_map = App->tex->Load(name_spritesheet_skill_tree_map.c_str());

	spritesheet_abilities = App->tex->Load(name_spritesheet_abilities.c_str());
	//if (spritesheet_skill_tree_map == NULL) {
	//	LOG("Fail to load spritesheet in SkillTree!");
	//	ret = false;
	//}

	//App->entities->SpawnEntity(-100,-100,CLERIC);
	//App->entities->SpawnEntity(-100, -100, WARRIOR);
	//App->entities->SpawnEntity(-100, -100, DWARF);
	//App->entities->SpawnEntity(-100, -100, ELF);

	//App->combat->LoadDataFromXML();

	if (spritesheet_abilities == NULL) {
		LOG("Fail to load spritesheet_abilities in SkillTree!");
		ret = false;
	}

	current_hero = 1;
	cleric_upgrades = 1;
	warrior_upgrades = 1;
	dwarf_upgrades = 1;
	elf_upgrades = 1;

	App->map->sceneName = skill_tree_map_tmx.c_str();
	App->map->Load(App->map->sceneName.c_str());
	App->map->LayersSetUp();
	App->map->setAllLogicForMap();

	pugi::xml_document	config_file;
	pugi::xml_node* node = &App->LoadAbilities(config_file);

	for (pugi::xml_node read_warrior_icons = node->child("warrior").child("ability"); read_warrior_icons && ret; read_warrior_icons = read_warrior_icons.next_sibling("ability"))
	{
		Ability* warrior = new Ability();
		warrior->tier = read_warrior_icons.attribute("tier").as_uint();
		warrior->branch = read_warrior_icons.attribute("branch").as_uint();
		warrior->ability_name = read_warrior_icons.attribute("name").as_string();
		warrior->active = read_warrior_icons.attribute("active").as_uint();
		warrior->description = read_warrior_icons.attribute("description").as_string();
		warrior->icon_rect = { read_warrior_icons.child("frame").attribute("x").as_int(), read_warrior_icons.child("frame").attribute("y").as_int(), read_warrior_icons.child("frame").attribute("width").as_int(),read_warrior_icons.child("frame").attribute("height").as_int() };
		warrior_abilities.push_back(warrior);
	}

	for (pugi::xml_node read_cleric_icons = node->child("cleric").child("ability"); read_cleric_icons && ret; read_cleric_icons = read_cleric_icons.next_sibling("ability"))
	{
		Ability* cleric = new Ability();
		cleric->tier = read_cleric_icons.attribute("tier").as_uint();
		cleric->branch = read_cleric_icons.attribute("branch").as_uint();
		cleric->ability_name = read_cleric_icons.attribute("name").as_string();
		cleric->active = read_cleric_icons.attribute("active").as_uint();
		cleric->description = read_cleric_icons.attribute("description").as_string();
		cleric->icon_rect = { read_cleric_icons.child("frame").attribute("x").as_int(), read_cleric_icons.child("frame").attribute("y").as_int(), read_cleric_icons.child("frame").attribute("width").as_int(),read_cleric_icons.child("frame").attribute("height").as_int() };
		cleric_abilities.push_back(cleric);
	}
	for (pugi::xml_node read_dwarf_icons = node->child("dwarf").child("ability"); read_dwarf_icons && ret; read_dwarf_icons = read_dwarf_icons.next_sibling("ability"))
	{
		Ability* dwarf = new Ability();
		dwarf->tier = read_dwarf_icons.attribute("tier").as_uint();
		dwarf->branch = read_dwarf_icons.attribute("branch").as_uint();
		dwarf->ability_name = read_dwarf_icons.attribute("name").as_string();
		dwarf->active = read_dwarf_icons.attribute("active").as_uint();
		dwarf->description = read_dwarf_icons.attribute("description").as_string();
		dwarf->icon_rect = { read_dwarf_icons.child("frame").attribute("x").as_int(), read_dwarf_icons.child("frame").attribute("y").as_int(), read_dwarf_icons.child("frame").attribute("width").as_int(),read_dwarf_icons.child("frame").attribute("height").as_int() };
		dwarf_abilities.push_back(dwarf);
	}
	for (pugi::xml_node read_elf_icons = node->child("elf").child("ability"); read_elf_icons && ret; read_elf_icons = read_elf_icons.next_sibling("ability"))
	{
		Ability* elf = new Ability();
		elf->tier = read_elf_icons.attribute("tier").as_uint();
		elf->branch = read_elf_icons.attribute("branch").as_uint();
		elf->ability_name = read_elf_icons.attribute("name").as_string();
		elf->active = read_elf_icons.attribute("active").as_uint();
		elf->description = read_elf_icons.attribute("description").as_string();
		elf->icon_rect = { read_elf_icons.child("frame").attribute("x").as_int(), read_elf_icons.child("frame").attribute("y").as_int(), read_elf_icons.child("frame").attribute("width").as_int(),read_elf_icons.child("frame").attribute("height").as_int() };
		elf_abilities.push_back(elf);
	}

	selected_ability = cleric_abilities.begin();
	ChangeDescriptionBG();
	PrintAbilityDescription();
	marker_pos.x = App->map->branch_0_coords.at((*selected_ability)->tier - 1).x-2;
	marker_pos.y = App->map->branch_0_coords.at((*selected_ability)->tier - 1).y-2;
	ChangeTitle();
	LookForActiveAbilities(cleric_abilities);
	LookForActiveAbilities(warrior_abilities);
	LookForActiveAbilities(dwarf_abilities);
	LookForActiveAbilities(elf_abilities);

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

	if (App->entities->entities.size() == 0 && App->fadeToBlack->FadeIsOver() == true) {
		App->entities->SpawnEntity(-100,-100,CLERIC);
		App->entities->SpawnEntity(-100, -100, WARRIOR);
		App->entities->SpawnEntity(-100, -100, DWARF);
		App->entities->SpawnEntity(-100, -100, ELF);

		App->combat->LoadDataFromXML();
		LookForActiveAbilities(cleric_abilities);
		LookForActiveAbilities(warrior_abilities);
		LookForActiveAbilities(dwarf_abilities);
		LookForActiveAbilities(elf_abilities);
	}

	App->map->Draw();
	App->render->DrawQuad(marker_pos, 255, 0, 0, 255, true, true);
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		GoToNextSkillTree();
		//if (current_hero < 4) {
		//	current_hero++;
		//	if (current_hero == 2) { 
		//		selected_ability = warrior_abilities.begin();
		//		ChangeDescriptionBG();
		//		PrintAbilityDescription();
		//		marker_pos.x = App->map->branch_0_coords.at((*selected_ability)->tier - 1).x - 2;
		//		marker_pos.y = App->map->branch_0_coords.at((*selected_ability)->tier - 1).y - 2;
		//		ChangeTitle();
		//	}
		//	else if (current_hero == 3) { 
		//		selected_ability = dwarf_abilities.begin(); 
		//		ChangeDescriptionBG();
		//		PrintAbilityDescription();
		//		marker_pos.x = App->map->branch_0_coords.at((*selected_ability)->tier - 1).x - 2;
		//		marker_pos.y = App->map->branch_0_coords.at((*selected_ability)->tier - 1).y - 2;
		//		ChangeTitle();
		//	}
		//	else if (current_hero == 4) {
		//		selected_ability = elf_abilities.begin();
		//		ChangeDescriptionBG();
		//		PrintAbilityDescription();
		//		marker_pos.x = App->map->branch_0_coords.at((*selected_ability)->tier - 1).x - 2;
		//		marker_pos.y = App->map->branch_0_coords.at((*selected_ability)->tier - 1).y - 2;
		//		ChangeTitle();
		//	}
		//}
		//else {
		//	/*current_hero = 1;
		//	selected_ability = cleric_abilities.begin();
		//	ChangeDescriptionBG();
		//	PrintAbilityDescription();
		//	marker_pos.x = App->map->branch_0_coords.at((*selected_ability)->tier - 1).x - 2;
		//	marker_pos.y = App->map->branch_0_coords.at((*selected_ability)->tier - 1).y - 2;
		//	ChangeTitle();*/
		//	App->fadeToBlack->FadeToBlackBetweenModules(this, App->world_map, 1.0f);
		//}
	}

	if (current_hero == 1)
	{
		DrawAbilities(cleric_abilities);
		NavigateSkills(cleric_abilities);
	}

	else if (current_hero == 2)
	{
		DrawAbilities(warrior_abilities);
		NavigateSkills(warrior_abilities);
	}

	else if (current_hero == 3)
	{
		DrawAbilities(dwarf_abilities);
		NavigateSkills(dwarf_abilities);
	}

	else if (current_hero == 4)
	{
		DrawAbilities(elf_abilities);
		NavigateSkills(elf_abilities);
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && selecting_ability == false|| App->input->gamepad.A == GAMEPAD_STATE::PAD_BUTTON_DOWN && selecting_ability == false) {
		selecting_ability = true;
		execute_comand_time.Start();
		App->audio->PlayFx(App->audio->mm_select_fx);
	}

	if (selecting_ability == true) {
		if (current_hero == 1) {
			if (cleric_upgrades > 0) {
				SelectAbility();
			}
			else {
				selecting_ability = false;
			}
		}
		else if (current_hero == 2) {
			if (warrior_upgrades > 0) {
				SelectAbility();
			}
			else {
				selecting_ability = false;
			}
		}
		else if (current_hero == 3) {
			if (dwarf_upgrades > 0) {
				SelectAbility();
			}
			else {
				selecting_ability = false;
			}
		}
		else if (current_hero == 4) {
			if (elf_upgrades > 0) {
				SelectAbility();
			}
			else {
				selecting_ability = false;
			}
		}
	}

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

	if (App->entities->GetCleric() != nullptr)
		SavedataToXML();

	App->map->CleanUp();
	warrior_abilities.clear();
	cleric_abilities.clear();
	dwarf_abilities.clear();
	elf_abilities.clear();
	if (description != nullptr) {
		App->gui->DeleteUIElement(*description);
		description = nullptr;
	}
	if (description_bg != nullptr) {
		App->gui->DeleteUIElement(*description_bg);
		description_bg = nullptr;
	}
	if (title != nullptr) {
		App->gui->DeleteUIElement(*title);
		title = nullptr;
	}
	if (select_menu_bg != nullptr) {
		App->gui->DeleteUIElement(*select_menu_bg);
		select_menu_bg = nullptr;
	}
	if (select_menu_text != nullptr) {
		App->gui->DeleteUIElement(*select_menu_text);
		select_menu_text = nullptr;
	}
	if (arrow != nullptr) {
		App->gui->DeleteUIElement(*arrow);
		arrow = nullptr;
	}
	if (select_menu_A != nullptr) {
		App->gui->DeleteUIElement(*select_menu_A);
		select_menu_A = nullptr;
	}
	if (select_menu_B != nullptr) {
		App->gui->DeleteUIElement(*select_menu_B);
		select_menu_B = nullptr;
	}
	accept_decline.clear();

	App->tex->UnLoad(spritesheet_abilities);

	for (int i = 0; i < App->entities->entities.size(); i++)
	{
		App->entities->entities.at(i)->~Entity();
	}
	App->entities->entities.clear();
	for (int i = 0; i < App->entities->entities.size(); i++)
	{
		App->combat->draw_turn_priority_entity.at(i)->~Entity();
	}
	App->combat->draw_turn_priority_entity.clear();

	for (int i = 0; i < App->entities->entities.size(); i++)
	{
		App->combat->turn_priority_entity.at(i)->~Entity();
	}
	App->combat->turn_priority_entity.clear();

	

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

void ctSkillTree::DrawAbilities(std::vector<Ability*> &abilities)
{
	LOG("%d", abilities);
	std::vector<Ability*>::const_iterator ability = abilities.begin();
	while (ability != abilities.end()) {
		if ((*ability)->branch == 0) {
			App->render->Blit(spritesheet_abilities, App->map->branch_0_coords.at((*ability)->tier - 1).x, App->map->branch_0_coords.at((*ability)->tier - 1).y, &(*ability)->icon_rect);
			if ((*ability)->active == 0) {
				App->render->DrawQuad({ App->map->branch_0_coords.at((*ability)->tier - 1).x, App->map->branch_0_coords.at((*ability)->tier - 1).y ,40,40}, 0, 0, 0, 100, true, true);
			}
		}
		else if ((*ability)->branch == 1) {
			App->render->Blit(spritesheet_abilities, App->map->branch_1_coords.at((*ability)->tier - 2).x, App->map->branch_1_coords.at((*ability)->tier - 2).y, &(*ability)->icon_rect);
			if ((*ability)->active == 0) {
				App->render->DrawQuad({ App->map->branch_1_coords.at((*ability)->tier - 2).x, App->map->branch_1_coords.at((*ability)->tier - 2).y ,40,40 }, 0, 0, 0, 100, true, true);
			}
		}
		else if ((*ability)->branch == 2) {
			App->render->Blit(spritesheet_abilities, App->map->branch_2_coords.at((*ability)->tier - 2).x, App->map->branch_2_coords.at((*ability)->tier - 2).y, &(*ability)->icon_rect);
			if ((*ability)->active == 0) {
				App->render->DrawQuad({ App->map->branch_2_coords.at((*ability)->tier - 2).x, App->map->branch_2_coords.at((*ability)->tier - 2).y ,40,40 }, 0, 0, 0, 100, true, true);
			}
		}
		else if ((*ability)->branch == 3) {
			App->render->Blit(spritesheet_abilities, App->map->branch_3_coords.at((*ability)->tier - 2).x, App->map->branch_3_coords.at((*ability)->tier - 2).y, &(*ability)->icon_rect);
			if ((*ability)->active == 0) {
				App->render->DrawQuad({ App->map->branch_3_coords.at((*ability)->tier - 2).x, App->map->branch_3_coords.at((*ability)->tier - 2).y ,40,40 }, 0, 0, 0, 100, true, true);
			}
		}
		ability++;
	}
}

void ctSkillTree::NavigateSkills(std::vector<Ability*> &abilities) {
	//Navigate Down
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN && selecting_ability == false || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN && selecting_ability == false) {
		App->audio->PlayFx(App->audio->mm_movement_fx);
		NavigateSkillsDown(abilities);
	}
	//Navigate Up
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN && selecting_ability == false || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN && selecting_ability == false) {
		App->audio->PlayFx(App->audio->mm_movement_fx);
		NavigateSkillsUp(abilities);
	}
	//Navigate Left
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN && selecting_ability == false || App->input->gamepad.CROSS_LEFT == GAMEPAD_STATE::PAD_BUTTON_DOWN && selecting_ability == false) {
		App->audio->PlayFx(App->audio->mm_movement_fx);
		NavigateSkillsLeft(abilities);
	}
	//Navigate Right
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && selecting_ability == false || App->input->gamepad.CROSS_RIGHT == GAMEPAD_STATE::PAD_BUTTON_DOWN && selecting_ability == false) {
		App->audio->PlayFx(App->audio->mm_movement_fx);
		NavigateSkillsRight(abilities);
	}
}

void ctSkillTree::NavigateSkillsDown(std::vector<Ability*> &abilities) {
	if (selected_ability == abilities.end()) {
		selected_ability = abilities.begin();
	}
	else if ((*selected_ability)->tier == 4) {
		selected_ability = abilities.begin();
		marker_pos.x = App->map->branch_0_coords.at((*selected_ability)->tier - 1).x - 2;
		marker_pos.y = App->map->branch_0_coords.at((*selected_ability)->tier - 1).y - 2;
	}
	else if ((*selected_ability)->tier == 1) {
		while (selected_ability != abilities.end()) {
			if ((*selected_ability)->tier == 2 && (*selected_ability)->branch == 2) {
				marker_pos.x = App->map->branch_2_coords.at((*selected_ability)->tier - 2).x - 2;
				marker_pos.y = App->map->branch_2_coords.at((*selected_ability)->tier - 2).y - 2;
				break;
			}
			selected_ability++;
			if (selected_ability == abilities.end()) {
				selected_ability = abilities.begin();
				break;
			}
		}
	}
	else {
		std::vector<Ability*>::const_iterator previous_ability = selected_ability;
		while (selected_ability != abilities.end()) {
			if ((*selected_ability)->tier == (*previous_ability)->tier + 1 && (*selected_ability)->branch == (*previous_ability)->branch) {
				if ((*selected_ability)->branch == 1) {
					marker_pos.x = App->map->branch_1_coords.at((*selected_ability)->tier - 2).x - 2;
					marker_pos.y = App->map->branch_1_coords.at((*selected_ability)->tier - 2).y - 2;
				}
				if ((*selected_ability)->branch == 2) {
					marker_pos.x = App->map->branch_2_coords.at((*selected_ability)->tier - 2).x - 2;
					marker_pos.y = App->map->branch_2_coords.at((*selected_ability)->tier - 2).y - 2;
				}
				if ((*selected_ability)->branch == 3) {
					marker_pos.x = App->map->branch_3_coords.at((*selected_ability)->tier - 2).x - 2;
					marker_pos.y = App->map->branch_3_coords.at((*selected_ability)->tier - 2).y - 2;
				}
				break;
			}
			selected_ability++;
			if (selected_ability == abilities.end()) {
				selected_ability = abilities.begin();
				break;
			}
		}
		previous_ability._Ptr = nullptr;
	}
	PrintAbilityDescription();
}

void ctSkillTree::NavigateSkillsUp(std::vector<Ability*> &abilities) {
	if (selected_ability != abilities.begin()) {
		if ((*selected_ability)->tier == 2) {
			selected_ability = abilities.begin();
			marker_pos.x = App->map->branch_0_coords.at((*selected_ability)->tier - 1).x - 2;
			marker_pos.y = App->map->branch_0_coords.at((*selected_ability)->tier - 1).y - 2;
		}
		else {
			std::vector<Ability*>::const_iterator previous_ability = selected_ability;
			while (selected_ability != abilities.begin()) {
				if ((*selected_ability)->tier == (*previous_ability)->tier - 1 && (*selected_ability)->branch == (*previous_ability)->branch) {
					if ((*selected_ability)->branch == 1) {
						marker_pos.x = App->map->branch_1_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_1_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					if ((*selected_ability)->branch == 2) {
						marker_pos.x = App->map->branch_2_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_2_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					if ((*selected_ability)->branch == 3) {
						marker_pos.x = App->map->branch_3_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_3_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					break;
				}
				selected_ability--;
				if (selected_ability == abilities.begin()) {
					selected_ability = abilities.end();
					break;
				}
			}
			previous_ability._Ptr = nullptr;
		}
	}
	PrintAbilityDescription();
}

void ctSkillTree::NavigateSkillsLeft(std::vector<Ability*> &abilities) {
	if (selected_ability != abilities.begin()) {
		if ((*selected_ability)->branch == 1) {
			std::vector<Ability*>::const_iterator previous_ability = selected_ability;
			while (selected_ability != abilities.begin()) {
				if ((*selected_ability)->tier == (*previous_ability)->tier && (*selected_ability)->branch == 3) {
					if ((*selected_ability)->branch == 1) {
						marker_pos.x = App->map->branch_1_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_1_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					if ((*selected_ability)->branch == 2) {
						marker_pos.x = App->map->branch_2_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_2_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					if ((*selected_ability)->branch == 3) {
						marker_pos.x = App->map->branch_3_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_3_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					break;
				}
				selected_ability++;
				if (selected_ability == abilities.end()) {
					selected_ability = abilities.begin();
					selected_ability++;
				}
			}
			previous_ability._Ptr = nullptr;
		}
		else {
			std::vector<Ability*>::const_iterator previous_ability = selected_ability;
			while (selected_ability != abilities.begin()) {
				if ((*selected_ability)->tier == (*previous_ability)->tier && (*selected_ability)->branch == (*previous_ability)->branch - 1) {
					if ((*selected_ability)->branch == 1) {
						marker_pos.x = App->map->branch_1_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_1_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					if ((*selected_ability)->branch == 2) {
						marker_pos.x = App->map->branch_2_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_2_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					if ((*selected_ability)->branch == 3) {
						marker_pos.x = App->map->branch_3_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_3_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					break;
				}
				selected_ability++;
				if (selected_ability == abilities.end()) {
					selected_ability = abilities.begin();
					selected_ability++;
				}
			}
			previous_ability._Ptr = nullptr;
		}
	}
	PrintAbilityDescription();
}

void ctSkillTree::NavigateSkillsRight(std::vector<Ability*> &abilities) {
	if (selected_ability != abilities.begin()) {
		if ((*selected_ability)->branch == 3) {
			std::vector<Ability*>::const_iterator previous_ability = selected_ability;
			while (selected_ability != abilities.begin()) {
				if ((*selected_ability)->tier == (*previous_ability)->tier && (*selected_ability)->branch == 1) {
					if ((*selected_ability)->branch == 1) {
						marker_pos.x = App->map->branch_1_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_1_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					if ((*selected_ability)->branch == 2) {
						marker_pos.x = App->map->branch_2_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_2_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					if ((*selected_ability)->branch == 3) {
						marker_pos.x = App->map->branch_3_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_3_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					break;
				}
				selected_ability++;
				if (selected_ability == abilities.end()) {
					selected_ability = abilities.begin();
					selected_ability++;
				}
			}
			previous_ability._Ptr = nullptr;
		}
		else {
			std::vector<Ability*>::const_iterator previous_ability = selected_ability;
			while (selected_ability != abilities.begin()) {
				if ((*selected_ability)->tier == (*previous_ability)->tier && (*selected_ability)->branch == (*previous_ability)->branch + 1) {
					if ((*selected_ability)->branch == 1) {
						marker_pos.x = App->map->branch_1_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_1_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					if ((*selected_ability)->branch == 2) {
						marker_pos.x = App->map->branch_2_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_2_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					if ((*selected_ability)->branch == 3) {
						marker_pos.x = App->map->branch_3_coords.at((*selected_ability)->tier - 2).x - 2;
						marker_pos.y = App->map->branch_3_coords.at((*selected_ability)->tier - 2).y - 2;
					}
					break;
				}
				selected_ability++;
				if (selected_ability == abilities.end()) {
					selected_ability = abilities.begin();
					selected_ability++;
				}
			}
			previous_ability._Ptr = nullptr;
		}
	}
	PrintAbilityDescription();
}

void ctSkillTree::PrintAbilityDescription(){
	if (description != nullptr) {
		App->gui->DeleteUIElement(*description);
	}
	description = App->gui->AddUITextBox(131, 293, 15, 400, (*selected_ability)->description, {0,0,0,255});
}

void ctSkillTree::ChangeTitle() {
	if (title != nullptr) {
		App->gui->DeleteUIElement(*title);
	}
	if (current_hero == 1) {
		title = App->gui->AddUITextBox(210, title_pos.y, 50, 200, "CLERIC", { 0,0,0,255 });
	}
	else if (current_hero == 2) {
		title = App->gui->AddUITextBox(200, title_pos.y, 50, 200, "WARRIOR", { 0,0,0,255 });
	}
	else if (current_hero == 3) {
		title = App->gui->AddUITextBox(210, title_pos.y, 50, 200, "DWARF", { 0,0,0,255 });
	}
	else if (current_hero == 4) {
		title = App->gui->AddUITextBox(220, title_pos.y, 50, 200, "ELF", { 0,0,0,255 });
	}
}

void ctSkillTree::ChangeDescriptionBG() {
	if (description_bg != nullptr) {
		App->gui->DeleteUIElement(*description_bg);
	}
	if (current_hero == 1) {
		description_bg = App->gui->AddUIImage(129, 291, { 3, 622, 226, 33 }, this);
	}
	else if (current_hero == 2) {
		description_bg = App->gui->AddUIImage(129, 291, { 3, 552, 226, 33 }, this);
	}
	else if (current_hero == 3) {
		description_bg = App->gui->AddUIImage(129, 291, { 3, 762, 226, 33 }, this);
	}
	else if (current_hero == 4) {
		description_bg = App->gui->AddUIImage(129, 291, { 3, 693, 226, 33 }, this);
	}
}

void ctSkillTree::SelectAbility() {
	if ((*selected_ability)->tier != 1) {
		if (current_hero == 1) {
			GetPreviousAbility(cleric_abilities);
		}
		else if (current_hero == 2) {
			GetPreviousAbility(warrior_abilities);
		}
		else if (current_hero == 3) {
			GetPreviousAbility(dwarf_abilities);
		}
		else if (current_hero == 4) {
			GetPreviousAbility(elf_abilities);
		}
	}
	if ((*selected_ability)->active == 0) {
		if ((*previous_ability)->active == 1) {
			if (select_menu_bg == nullptr) {
				select_menu_bg = App->gui->AddUIImage(0, 0, { 1141,484,340,178 }, this);
				string new_mesage = "Are you sure you want to unlock " + (*selected_ability)->ability_name;
				select_menu_text = App->gui->AddUITextBox(20, 20, 20, 500, new_mesage, { 255,255,255,255 });
				select_menu_A = App->gui->AddUITextBox(20, 60, 20, 336, "Yes", { 255,255,255,255 });
				select_menu_B = App->gui->AddUITextBox(20, 100, 20, 336, "No", { 255,255,255,255 });
				arrow = App->gui->AddUIImage(-10, 0, { 1333, 272, 7, 14 }, this, select_menu_bg);
				arrow->SetParent(select_menu_A);
				select_menu_A->current_state = STATE_FOCUSED;
				accept_decline.push_back(select_menu_A);
				accept_decline.push_back(select_menu_B);
				option = accept_decline.begin();
			}
			if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && execute_comand_time.ReadMs() >= 500 || App->input->gamepad.A == GAMEPAD_STATE::PAD_BUTTON_DOWN && execute_comand_time.ReadMs() >= 500) {
				if (select_menu_A->current_state == STATE_FOCUSED) {
					App->audio->PlayFx(App->audio->mm_select_fx);
					//if ((*previous_ability)->active == 1) {
					
					(*selected_ability)->active = 1;
					if (current_hero == 1) {
						LOG("AbilitiesSize = %i", App->entities->GetCleric()->abilities.size());
						for (int i = 0; i < App->entities->GetCleric()->abilities.size(); i++) {
							if (App->entities->GetCleric()->abilities.at(i).name == (*selected_ability)->ability_name) {
								App->entities->GetCleric()->abilities.at(i).owned = true;
							}
						}
					}
					else if (current_hero == 2) {
						for (int i = 0; i < App->entities->GetWarrior()->abilities.size(); i++) {
							if (App->entities->GetWarrior()->abilities.at(i).name == (*selected_ability)->ability_name) {
								App->entities->GetWarrior()->abilities.at(i).owned = true;
							}
						}
					}
					else if (current_hero == 3) {
						for (int i = 0; i < App->entities->GetDwarf()->abilities.size(); i++) {
							if (App->entities->GetDwarf()->abilities.at(i).name == (*selected_ability)->ability_name) {
								App->entities->GetDwarf()->abilities.at(i).owned = true;
							}
						}
					}
					else if (current_hero == 4) {
						for (int i = 0; i < App->entities->GetElf()->abilities.size(); i++) {
							if (App->entities->GetElf()->abilities.at(i).name == (*selected_ability)->ability_name) {
								App->entities->GetElf()->abilities.at(i).owned = true;
							}
						}
					}
					if (current_hero == 1) {
						cleric_upgrades--;
					}
					if (current_hero == 2) {
						warrior_upgrades--;
					}
					if (current_hero == 3) {
						dwarf_upgrades--;
					}
					if (current_hero == 4) {
						elf_upgrades--;
					}
					GoToNextSkillTree();
				}
				else if (select_menu_B->current_state == STATE_FOCUSED) {
					App->audio->PlayFx(App->audio->cm_back_fx);
				}
				App->gui->DeleteUIElement(*select_menu_bg);
				select_menu_bg = nullptr;
				App->gui->DeleteUIElement(*select_menu_text);
				select_menu_text = nullptr;
				App->gui->DeleteUIElement(*arrow);
				arrow = nullptr;
				App->gui->DeleteUIElement(*select_menu_A);
				select_menu_A = nullptr;
				App->gui->DeleteUIElement(*select_menu_B);
				select_menu_B = nullptr;
				accept_decline.clear();
				option._Ptr = nullptr;
				selecting_ability = false;

			}
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN || App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
				if (select_menu_A->current_state == STATE_FOCUSED) {
					select_menu_A->current_state = STATE_NORMAL;
					select_menu_B->current_state = STATE_FOCUSED;
					arrow->SetParent(select_menu_B);
				}
				else {
					select_menu_A->current_state = STATE_FOCUSED;
					select_menu_B->current_state = STATE_NORMAL;
					arrow->SetParent(select_menu_A);
				}
				App->audio->PlayFx(App->audio->mm_movement_fx);
			}
		}
		else {
			selecting_ability = false;
		}
	}
	else {
		selecting_ability = false;
	}
}

void ctSkillTree::LookForActiveAbilities(std::vector<Ability*> &abilities) {
	std::vector<Ability*>::const_iterator ability = abilities.begin();
	while (ability != abilities.end()) {
		if (abilities == cleric_abilities) {
			for (int i = 0; i < App->entities->GetCleric()->abilities.size(); i++) {
				if (App->entities->GetCleric()->abilities.at(i).name == (*ability)->ability_name) {
					if (App->entities->GetCleric()->abilities.at(i).owned == true) {
						(*ability)->active = 1;
					}
					else {
						(*ability)->active = 0;
					}
				}
			}
		}
		else if (abilities == warrior_abilities) {
			for (int i = 0; i < App->entities->GetWarrior()->abilities.size(); i++) {
				if (App->entities->GetWarrior()->abilities.at(i).name == (*ability)->ability_name) {
					if (App->entities->GetWarrior()->abilities.at(i).owned == true) {
						(*ability)->active = 1;
					}
					else {
						(*ability)->active = 0;
					}
				}
			}
		}
		else if (abilities == dwarf_abilities) {
			for (int i = 0; i < App->entities->GetDwarf()->abilities.size(); i++) {
				if (App->entities->GetDwarf()->abilities.at(i).name == (*ability)->ability_name) {
					if (App->entities->GetDwarf()->abilities.at(i).owned == true) {
						(*ability)->active = 1;
					}
					else {
						(*ability)->active = 0;
					}
				}
			}
		}
		else if (abilities == elf_abilities) {
			for (int i = 0; i < App->entities->GetElf()->abilities.size(); i++) {
				if (App->entities->GetElf()->abilities.at(i).name == (*ability)->ability_name) {
					if (App->entities->GetElf()->abilities.at(i).owned == true) {
						(*ability)->active = 1;
					}
					else {
						(*ability)->active = 0;
					}
				}
			}
		}
		ability++;
	}
}

void ctSkillTree::GetPreviousAbility(std::vector<Ability*> &abilities) {
	previous_ability = abilities.begin();
	if ((*selected_ability)->tier == 2) {
		while ((*previous_ability)->tier != 1) {
			previous_ability++;
		}
	}
	else {
		while ((*previous_ability)->tier != (*selected_ability)->tier-1 || (*previous_ability)->branch != (*selected_ability)->branch) {
			previous_ability++;
		}
	}
}

void ctSkillTree::SavedataToXML() {
	pugi::xml_document	data_file;
	pugi::xml_node* node = &App->LoadData(data_file);
	node = &node->child("heroes");

	for (pugi::xml_node heroe = node->child("heroe"); heroe; heroe = heroe.next_sibling("heroe"))
	{
		std::string tmp(heroe.attribute("name").as_string());

		if (tmp == "cleric") {
			for (int i = 0; i < App->entities->GetCleric()->abilities.size(); i++)
			{
				std::string tmp = App->entities->GetCleric()->abilities.at(i).name;

				for (pugi::xml_node skill = heroe.child("skills").child("skill"); skill; skill = skill.next_sibling("skill")) {
					std::string tmp2 = skill.attribute("name").as_string();
					if (tmp2 == tmp)
						if (App->entities->GetCleric()->abilities.at(i).owned == true) {
							skill.attribute("owned").set_value(1);
						}
						else {
							skill.attribute("owned").set_value(0);
						}
				}
			}
		}
		else if (tmp == "warrior") {
			for (int i = 0; i < App->entities->GetWarrior()->abilities.size(); i++)
			{
				std::string tmp = App->entities->GetWarrior()->abilities.at(i).name;

				for (pugi::xml_node skill = heroe.child("skills").child("skill"); skill; skill = skill.next_sibling("skill")) {
					std::string tmp2 = skill.attribute("name").as_string();
					if (tmp2 == tmp)
						if (App->entities->GetWarrior()->abilities.at(i).owned == true) {
							skill.attribute("owned").set_value(1);
						}
						else {
							skill.attribute("owned").set_value(0);
						}
				}
			}

		}
		else if (tmp == "dwarf") {
			for (int i = 0; i < App->entities->GetDwarf()->abilities.size(); i++)
			{
				std::string tmp = App->entities->GetDwarf()->abilities.at(i).name;

				for (pugi::xml_node skill = heroe.child("skills").child("skill"); skill; skill = skill.next_sibling("skill")) {
					std::string tmp2 = skill.attribute("name").as_string();
					if (tmp2 == tmp)
						if (App->entities->GetDwarf()->abilities.at(i).owned == true) {
							skill.attribute("owned").set_value(1);
						}
						else {
							skill.attribute("owned").set_value(0);
						}
				}
			}
		}
		else if (tmp == "elf") {
			for (int i = 0; i < App->entities->GetElf()->abilities.size(); i++)
			{
				std::string tmp = App->entities->GetElf()->abilities.at(i).name;

				for (pugi::xml_node skill = heroe.child("skills").child("skill"); skill; skill = skill.next_sibling("skill")) {
					std::string tmp2 = skill.attribute("name").as_string();
					if (tmp2 == tmp)
						if (App->entities->GetElf()->abilities.at(i).owned == true) {
							skill.attribute("owned").set_value(1);
						}
						else {
							skill.attribute("owned").set_value(0);
						}
				}
			}
		}

	}

	data_file.save_file("data.xml");
	data_file.reset();
}

void ctSkillTree::GoToNextSkillTree(){
	if (current_hero < 4) {
		current_hero++;
		if (current_hero == 2) {
			selected_ability = warrior_abilities.begin();
			ChangeDescriptionBG();
			PrintAbilityDescription();
			marker_pos.x = App->map->branch_0_coords.at((*selected_ability)->tier - 1).x - 2;
			marker_pos.y = App->map->branch_0_coords.at((*selected_ability)->tier - 1).y - 2;
			ChangeTitle();
		}
		else if (current_hero == 3) {
			selected_ability = dwarf_abilities.begin();
			ChangeDescriptionBG();
			PrintAbilityDescription();
			marker_pos.x = App->map->branch_0_coords.at((*selected_ability)->tier - 1).x - 2;
			marker_pos.y = App->map->branch_0_coords.at((*selected_ability)->tier - 1).y - 2;
			ChangeTitle();
		}
		else if (current_hero == 4) {
			selected_ability = elf_abilities.begin();
			ChangeDescriptionBG();
			PrintAbilityDescription();
			marker_pos.x = App->map->branch_0_coords.at((*selected_ability)->tier - 1).x - 2;
			marker_pos.y = App->map->branch_0_coords.at((*selected_ability)->tier - 1).y - 2;
			ChangeTitle();
		}
	}
	else {
		/*current_hero = 1;
		selected_ability = cleric_abilities.begin();
		ChangeDescriptionBG();
		PrintAbilityDescription();
		marker_pos.x = App->map->branch_0_coords.at((*selected_ability)->tier - 1).x - 2;
		marker_pos.y = App->map->branch_0_coords.at((*selected_ability)->tier - 1).y - 2;
		ChangeTitle();*/
		App->fadeToBlack->FadeToBlackBetweenModules(this, App->world_map, 1.0f);
	}
}