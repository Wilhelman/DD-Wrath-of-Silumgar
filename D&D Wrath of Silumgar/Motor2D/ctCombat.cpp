#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctTextures.h"
#include "ctAudio.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "ctEntities.h"
#include "ctTaskManager.h"
#include "ctLootMenu.h"
#include "ctCombat.h"
#include "ctWorldMap.h"
#include "j1Map.h"
#include "ctMainMenu.h"
#include "ctCutsceneManager.h"
#include "ctFinalScene.h"

#include "Cleric.h"
#include "Dwarf.h"
#include "Elf.h"
#include "Warrior.h"

#include "ctGui.h"

#include "ctFadeToBlack.h"

#include "UICombatMenu.h"
#include "UIBar.h"
#include "ctItems.h"

//randomize libs
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */



ctCombat::ctCombat() : ctModule()
{
	name = "combat";
}

// Destructor
ctCombat::~ctCombat()
{}

// Called before render is available

bool ctCombat::Awake(pugi::xml_node& config)
{
	LOG("Loading Combat");
	bool ret = true;

	/* initialize random seed: */
	srand(time(NULL));

	return ret;
}


// Called before the first frame
bool ctCombat::Start()
{
	bool ret = true;

	heroes_are_dead = false;

	making_decision = false;
	App->task_manager->Start();

	//Displaying map
	if (App->map->sceneName == "world_map.tmx")
	{
		App->map->sceneName = this->scene_name;
	}
	LOG("%s", App->map->sceneName.c_str());
	App->map->Load(App->map->sceneName.c_str());
	App->map->LayersSetUp();
	App->map->setAllLogicForMap();

	cleric_background = App->gui->AddUIImage(-1, -1, {0,0,242,32}, this, nullptr);
	warrior_background = App->gui->AddUIImage(242, -1, { 242,31,242,32 }, this, nullptr);
	elf_background = App->gui->AddUIImage(-1, 293, { 0,32,242,32 }, this, nullptr);
	dwarf_background = App->gui->AddUIImage(242, 293, { 242,0,242,32 }, this, nullptr);
	cleric_name = App->gui->AddUILabel(205, 3, "Cleric", { 255,255,255,255 }, 23, this);
	warrior_name = App->gui->AddUILabel(443, 3, "Warrior", { 255,255,255,255 }, 23, this);
	elf_name = App->gui->AddUILabel(210, 296, "Elf", { 255,255,255,255 }, 23, this);
	dwarf_name = App->gui->AddUILabel(445, 296, "Dwarf", { 255,255,255,255 }, 23, this);
	
	SpawnEntities();
	//recover hp and mana
	App->entities->GetCleric()->Recover();
	App->entities->GetWarrior()->Recover();
	App->entities->GetElf()->Recover();
	App->entities->GetDwarf()->Recover();

	/*---------------------------------------------------------- LE PETIT TESTING ZONE -------------------------------------------------------------*/

	/*App->entities->GetElf()->AddUsableItem(App->items->usable_items.at(4));
	App->entities->GetElf()->AddUsableItem(App->items->usable_items.at(5));
	App->entities->GetElf()->AddUsableItem(App->items->usable_items.at(6));
	App->entities->GetCleric()->AddUsableItem(App->items->usable_items.at(4));
	App->entities->GetCleric()->AddUsableItem(App->items->usable_items.at(5));
	App->entities->GetCleric()->AddUsableItem(App->items->usable_items.at(6));
	App->entities->GetDwarf()->AddUsableItem(App->items->usable_items.at(4));
	App->entities->GetDwarf()->AddUsableItem(App->items->usable_items.at(5));
	App->entities->GetDwarf()->AddUsableItem(App->items->usable_items.at(6));*/
	//App->entities->GetWarrior()->AddUsableItem(App->items->usable_items.at(4));
	//App->entities->GetWarrior()->AddUsableItem(App->items->usable_items.at(5));
	//App->entities->GetWarrior()->AddUsableItem(App->items->usable_items.at(6));


	/*---------------------------------------------------------- END LE PETIT TESTING ZONE -------------------------------------------------------------*/
	for (int i = 0; i < App->items->elf_equip.size(); i++)
	{
		App->entities->GetElf()->AddEquipItem(App->items->elf_equip.at(i));
	}
	App->items->elf_equip.clear();

	for (int i = 0; i < App->items->warrior_equip.size(); i++)
	{
		App->entities->GetWarrior()->AddEquipItem(App->items->warrior_equip.at(i));
	}
	App->items->warrior_equip.clear();

	for (int i = 0; i < App->items->cleric_equip.size(); i++)
	{
		App->entities->GetCleric()->AddEquipItem(App->items->cleric_equip.at(i));
	}
	App->items->cleric_equip.clear();

	for (int i = 0; i < App->items->dwarf_equip.size(); i++)
	{
		App->entities->GetDwarf()->AddEquipItem(App->items->dwarf_equip.at(i));
	}
	App->items->dwarf_equip.clear();


	LoadDataFromXML();

	SetDataToUI();

	OrderTurnPriority();

	for (std::vector<Entity *>::iterator it = turn_priority_entity.begin(); it != turn_priority_entity.end(); ++it) {
		if ((*it)->type != CLERIC && (*it)->type != WARRIOR && (*it)->type != DWARF && (*it)->type != ELF) {
			if ((*it)->GetCurrentHealthPoints()>0)
				enemies.push_back(*it);
		}
		else {
				heroes.push_back(*it);
		}
	}

	turn_priority_entity.clear();

	for (std::vector<Entity *>::iterator it_heroe = heroes.begin(); it_heroe != heroes.end(); ++it_heroe) {
		if ((*it_heroe)->GetCurrentHealthPoints()>0)
			turn_priority_entity.push_back(*it_heroe);
	}
	for (std::vector<Entity *>::iterator it_enemy = enemies.begin(); it_enemy != enemies.end(); ++it_enemy) {
		if ((*it_enemy)->GetCurrentHealthPoints()>0)
			turn_priority_entity.push_back(*it_enemy);
	}

	OrderTurnPriority();

	for (int i = 0; i < enemies.size(); i++) {
		int pos_x = enemies.at(i)->position.x + (enemies.at(i)->animation->GetCurrentFrame().w / 2) - 25;
		int pos_y = enemies.at(i)->position.y - enemies.at(i)->animation->GetCurrentFrame().h - 5;
		UIBar* bar = (UIBar*)App->gui->AddUIBar(pos_x, pos_y, enemies.at(i)->GetCurrentHealthPoints(), ENEMYLIFEBAR, enemies.at(i), this, nullptr);
		enemies_bars.push_back(bar);
	}

	int random_music = (rand() % 6);

	switch (random_music) {

	case 0:
		if (!App->audio->PlayMusic(App->audio->CombatBSO.c_str(), 1)) {

			LOG("Error playing music in ctMainMenu Start");
		}
		break;
	case 1:
		if (!App->audio->PlayMusic(App->audio->CombatBSO2.c_str(), 1)) {

			LOG("Error playing music in ctMainMenu Start");
		}
		break;
	case 2:
		if (!App->audio->PlayMusic(App->audio->CombatBSO3.c_str(), 1)) {

			LOG("Error playing music in ctMainMenu Start");
		}
		break;
	case 3:
		if (!App->audio->PlayMusic(App->audio->CombatBSO4.c_str(), 1)) {

			LOG("Error playing music in ctMainMenu Start");
		}
		break;
	case 4:
		if (!App->audio->PlayMusic(App->audio->CombatBSO5.c_str(), 1)) {

			LOG("Error playing music in ctMainMenu Start");
		}
		break;
	case 5:
		if (!App->audio->PlayMusic(App->audio->CombatBSO6.c_str(), 1)) {

			LOG("Error playing music in ctMainMenu Start");
		}
		break;
	default:
		if (!App->audio->PlayMusic(App->audio->CombatBSO.c_str(), 1)) {

			LOG("Error playing music in ctMainMenu Start");
		}
		break;

		break;
	}

	if (App->entities->GetLich() != nullptr) {
		App->cutscene_manager->ChargeCutscene(LICH_CUTSCENE);
		App->cutscene_manager->StartCutscene();
	}

	


	return ret;
}

// Called each loop iteration
bool ctCombat::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ctCombat::Update(float dt)
{
	//DELETE WHEN DONE WITH SHADOWS
	/*if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		App->entities->GetElf()->animation = &App->entities->GetElf()->dodge;
	}
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
	{
		App->entities->GetElf()->animation = &App->entities->GetElf()->idle;
	}*/

	for (int i = 0; i < altered_stats_icons.size(); i++)
	{
		altered_stats_icons.at(i)->to_destroy = true;
	}

	altered_stats_icons.clear();

	int x_pos_icon = 0;
	int y_pos_icon = 0;
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies.at(i)->GetCurrentHealthPoints() > 0 && enemies.at(i)->animation == &enemies.at(i)->idle)
		{
			for (int j = 0; j < enemies.at(i)->altered_stats.size(); j++)
			{
				y_pos_icon = enemies.at(i)->position.y - enemies.at(i)->animation->GetCurrentFrame().h - 24;
				if (j == 0) {
					x_pos_icon = enemies.at(i)->position.x + 8;
				}
				else {
					x_pos_icon = x_pos_icon + 16;
				}

				UIElement* tmp = nullptr;

				if (enemies.at(i)->altered_stats.at(j).stun == true) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, STUN_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}
				else if (enemies.at(i)->altered_stats.at(j).bleeding == true) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, BLEED_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).poison == true) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, POISON_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).burn == true) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, BURN_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_strength == 1) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, STR_1_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_strength == 2) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, STR_2_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_strength == -1) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, STR_3_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_strength == -2) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, STR_4_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_intelligence == 1) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, INT_1_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_intelligence == 2) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, INT_2_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_intelligence == -1) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, INT_3_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_intelligence == -2) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, INT_4_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_dexterity == 1) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, DEX_1_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}
				else if (enemies.at(i)->altered_stats.at(j).stat_effect_dexterity == 2) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, DEX_2_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_dexterity == -1) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, DEX_3_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}
				else if (enemies.at(i)->altered_stats.at(j).stat_effect_dexterity == -2) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, DEX_4_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_agility == 1) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, AGI_1_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}
				else if (enemies.at(i)->altered_stats.at(j).stat_effect_agility == 2) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, AGI_2_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_agility == -1) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, AGI_3_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}
				else if (enemies.at(i)->altered_stats.at(j).stat_effect_agility == -2) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, AGI_4_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_physical_defense == 1) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, PDEF_1_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}
				else if (enemies.at(i)->altered_stats.at(j).stat_effect_physical_defense == 2) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, PDEF_2_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_physical_defense == -1) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, PDEF_3_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}
				else if (enemies.at(i)->altered_stats.at(j).stat_effect_physical_defense == -2) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, PDEF_4_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_magical_defense == 1) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, MDEF_1_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}
				else if (enemies.at(i)->altered_stats.at(j).stat_effect_magical_defense == 2) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, MDEF_2_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_magical_defense == -1) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, MDEF_3_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}
				else if (enemies.at(i)->altered_stats.at(j).stat_effect_magical_defense == -2) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, MDEF_4_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				else if (enemies.at(i)->altered_stats.at(j).stat_effect_judgement == -1) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, JUD_3_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}
				else if (enemies.at(i)->altered_stats.at(j).stat_effect_judgement == -2) {
					tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, JUD_4_COORDS, this, nullptr);
					tmp->have_to_resize = false;
				}

				altered_stats_icons.push_back(tmp); //clean this every frame!
			}
		}

	}
	if (!making_decision) {

		for (int i = 0; i < heroes.size(); i++)
		{
			if (heroes.at(i)->GetCurrentHealthPoints() > 0 && heroes.at(i)->animation == &heroes.at(i)->idle)
			{
				for (int j = 0; j < heroes.at(i)->altered_stats.size(); j++)
				{
					y_pos_icon = heroes.at(i)->position.y - heroes.at(i)->animation->GetCurrentFrame().h - 16;
					if (j == 0) {
						x_pos_icon = heroes.at(i)->position.x + 8;
					}
					else {
						x_pos_icon = x_pos_icon + 16;
					}

					UIElement* tmp = nullptr;

					if (heroes.at(i)->altered_stats.at(j).stun == true) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, STUN_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}
					else if (heroes.at(i)->altered_stats.at(j).bleeding == true) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, BLEED_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).poison == true) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, POISON_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).burn == true) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, BURN_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_strength == 1) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, STR_1_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_strength == 2) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, STR_2_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_strength == -1) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, STR_3_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_strength == -2) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, STR_4_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_intelligence == 1) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, INT_1_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_intelligence == 2) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, INT_2_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_intelligence == -1) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, INT_3_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_intelligence == -2) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, INT_4_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_dexterity == 1) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, DEX_1_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}
					else if (heroes.at(i)->altered_stats.at(j).stat_effect_dexterity == 2) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, DEX_2_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_dexterity == -1) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, DEX_3_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}
					else if (heroes.at(i)->altered_stats.at(j).stat_effect_dexterity == -2) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, DEX_4_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_agility == 1) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, AGI_1_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}
					else if (heroes.at(i)->altered_stats.at(j).stat_effect_agility == 2) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, AGI_2_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_agility == -1) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, AGI_3_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}
					else if (heroes.at(i)->altered_stats.at(j).stat_effect_agility == -2) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, AGI_4_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_physical_defense == 1) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, PDEF_1_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}
					else if (heroes.at(i)->altered_stats.at(j).stat_effect_physical_defense == 2) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, PDEF_2_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_physical_defense == -1) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, PDEF_3_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}
					else if (heroes.at(i)->altered_stats.at(j).stat_effect_physical_defense == -2) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, PDEF_4_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_magical_defense == 1) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, MDEF_1_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}
					else if (heroes.at(i)->altered_stats.at(j).stat_effect_magical_defense == 2) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, MDEF_2_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_magical_defense == -1) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, MDEF_3_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}
					else if (heroes.at(i)->altered_stats.at(j).stat_effect_magical_defense == -2) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, MDEF_4_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					else if (heroes.at(i)->altered_stats.at(j).stat_effect_judgement == -1) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, JUD_3_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}
					else if (heroes.at(i)->altered_stats.at(j).stat_effect_judgement == -2) {
						tmp = App->gui->AddUIImage(x_pos_icon, y_pos_icon, JUD_4_COORDS, this, nullptr);
						tmp->have_to_resize = false;
					}

					altered_stats_icons.push_back(tmp); //clean this every frame!
				}
			}

		}
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		App->fadeToBlack->FadeToBlackBetweenModules(this, App->loot_menu);
	}

	if (!App->cutscene_manager->isActive()) {
		if (making_decision == true)
		{
			if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN || App->input->gamepad.START == GAMEPAD_STATE::PAD_BUTTON_DOWN)
			{
				if (pauseMenu == nullptr) {
					pauseMenu = App->gui->AddUIPauseMenu(0, 0, this, nullptr);
					pause_menu_is_open = true;
				}
				else {
					App->gui->DeleteUIElement(*pauseMenu);
					pauseMenu = nullptr;

					App->combat->pause_menu_is_open = false;
					App->entities->GetCleric()->animation = &App->entities->GetCleric()->idle;
					App->entities->GetDwarf()->animation = &App->entities->GetDwarf()->idle;
					App->entities->GetElf()->animation = &App->entities->GetElf()->idle;
					App->entities->GetWarrior()->animation = &App->entities->GetWarrior()->idle;
				}
			}
			if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN ) {
				if (App->fadeToBlack->FadeIsOver())
					App->fadeToBlack->FadeToBlackBetweenModules(this, App->loot_menu, 1.0f);
			}

			if (pause_menu_is_open == false)
			{
				if (App->entities->GetDwarf()->position != App->entities->GetDwarf()->initial_position)
				{
					App->entities->GetDwarf()->position = App->entities->GetDwarf()->initial_position;
					App->entities->GetElf()->position = App->entities->GetElf()->initial_position;
					App->entities->GetWarrior()->position = App->entities->GetWarrior()->initial_position;
					App->entities->GetCleric()->position = App->entities->GetCleric()->initial_position;
				}
			}

	
		}

		if (pauseMenu == nullptr || pause_menu_is_open == false)
		{
			if (turn_priority_entity.size() == 0) {
				if (ready_cleric != nullptr) {
					App->gui->DeleteUIElement(*ready_cleric);
					ready_cleric = nullptr;
				}
				if (ready_warrior != nullptr) {
					App->gui->DeleteUIElement(*ready_warrior);
					ready_warrior = nullptr;
				}
				if (ready_dwarf != nullptr) {
					App->gui->DeleteUIElement(*ready_dwarf);
					ready_dwarf = nullptr;
				}
				if (ready_elf != nullptr) {
					App->gui->DeleteUIElement(*ready_elf);
					ready_elf = nullptr;
				}
				App->task_manager->OrderTasksByEntities(draw_turn_priority_entity);
				App->task_manager->PerformAllTheTasks();

				int current_entities = 0;
				if (App->task_manager->TaskQueue.size() == 0 && App->task_manager->aux_task == nullptr) {

					

					for (int i = 0; i < App->entities->entities.size(); i++)
					{
						App->entities->entities.at(i)->NewTurn();
					}

					for (std::vector<Entity *>::iterator it_heroe = heroes.begin(); it_heroe != heroes.end(); ++it_heroe) {
						if ((*it_heroe)->GetCurrentHealthPoints() > 0)
							turn_priority_entity.push_back(*it_heroe);
					}

					if (turn_priority_entity.size() == 0) { //all heroes are dead!
						LOG("All heroes are dead!");
						condition_victory = false;
						heroes_are_dead = true;
						if (App->fadeToBlack->FadeIsOver())
							App->fadeToBlack->FadeToBlackBetweenModules(this, App->main_menu, 1.0f);
					}
					else
						current_entities = turn_priority_entity.size();

					if (App->fadeToBlack->FadeIsOver()) {
						for (std::vector<Entity *>::iterator it_enemy = enemies.begin(); it_enemy != enemies.end(); ++it_enemy) {
							if ((*it_enemy)->GetCurrentHealthPoints() > 0)
								turn_priority_entity.push_back(*it_enemy);
						}

						if (turn_priority_entity.size() == current_entities) { //all enemies are dead!
							LOG("All enemies are dead!");

							if (App->map->actual_tier == TIER_MAP_8 && App->fadeToBlack->FadeIsOver()) {
								heroes_are_dead = true;
								App->fadeToBlack->FadeToBlackBetweenModules(this, App->final_scene, 1.0f);
							}
							else {
								if (App->fadeToBlack->FadeIsOver())
									App->fadeToBlack->FadeToBlackBetweenModules(this, App->loot_menu, 1.0f);
							}
						}


						if (!(App->entities->GetCleric()->GetCurrentManaPoints() >= cleric_mana_bar->max_capacity))
							cleric_mana_bar->LowerBar(8);

						if (!(App->entities->GetElf()->GetCurrentManaPoints() >= elf_mana_bar->max_capacity))
							elf_mana_bar->LowerBar(8);

						if (!(App->entities->GetWarrior()->GetCurrentManaPoints() >= warrior_mana_bar->max_capacity))
							warrior_mana_bar->LowerBar(8);

						if (!(App->entities->GetDwarf()->GetCurrentManaPoints() >= dwarf_mana_bar->max_capacity))
							dwarf_mana_bar->LowerBar(8);

						OrderTurnPriority();

					}

				}
			}
			else {
				Entity* entity_to_perform_action = turn_priority_entity.front();

				if (PerformActionWithEntity(entity_to_perform_action)) {//Return true if the action was established.

					turn_priority_entity.erase(turn_priority_entity.cbegin());
					turn_priority_entity.shrink_to_fit();

				}
				else {
					entity_performing_action = entity_to_perform_action;
				}
				
			}

		}
		
		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->fadeToBlack->FadeIsOver())
			App->fadeToBlack->FadeToBlackBetweenModules(this, App->loot_menu, 1.0f);
		
	}

	App->map->Draw();
	DrawTurnPriority();

	EnemiesBarShouldBeVisible();
	return true;
}

// Called each loop iteration
bool ctCombat::PostUpdate()
{
	bool ret = true;
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	
	

	return ret;
}

// Called before quitting
bool ctCombat::CleanUp()
{
	LOG("Freeing combat");

	for (int i = 0; i < altered_stats_icons.size(); i++)
	{
		altered_stats_icons.at(i)->to_destroy = true;
	}

	altered_stats_icons.clear();

	App->map->CleanUp();

	if (App->main_menu->is_new_game)
		App->main_menu->is_new_game = false;

	if(App->entities->entities.size()>0)
		SaveDataToXML();

	for (int i = 0; i < App->entities->entities.size(); i++)
	{
		if (pauseMenuDelete == false)
		{
			if ((App->entities->entities.at(i)->type != DWARF && App->entities->entities.at(i)->type != ELF && App->entities->entities.at(i)->type != WARRIOR && App->entities->entities.at(i)->type != CLERIC) || heroes_are_dead)
				App->entities->entities.at(i)->to_destroy = true;
		}
		else
		{
			App->entities->entities.at(i)->to_destroy = true;
		}
	}
	
	if (pauseMenuDelete == true)
		pauseMenuDelete = false;

	enemies.clear();
	heroes.clear();
	for (int i = 0; i < enemies_bars.size(); i++)
	{
		App->gui->DeleteUIElement(*enemies_bars[i]);
		
	}
	enemies_bars.clear();
	
	turn_priority_entity.clear();
	draw_turn_priority_entity.clear();
	entities_to_spawn.clear();
	App->gui->DeleteUIElement(*combat_menu);

	App->gui->DeleteUIElement(*ready_cleric);
	ready_cleric = nullptr;
	App->gui->DeleteUIElement(*ready_warrior);
	ready_warrior = nullptr;
	App->gui->DeleteUIElement(*ready_dwarf);
	ready_dwarf = nullptr;
	App->gui->DeleteUIElement(*ready_elf);
	ready_elf = nullptr;

	App->gui->DeleteUIElement(*cleric_background);
	cleric_background = nullptr;
	App->gui->DeleteUIElement(*cleric_HP_bar);
	cleric_HP_bar = nullptr;
	App->gui->DeleteUIElement(*cleric_mana_bar);
	cleric_mana_bar = nullptr;
	App->gui->DeleteUIElement(*cleric_name);
	cleric_name = nullptr;

	App->gui->DeleteUIElement(*dwarf_background);
	dwarf_background = nullptr;
	App->gui->DeleteUIElement(*dwarf_HP_bar);
	dwarf_HP_bar = nullptr;
	App->gui->DeleteUIElement(*dwarf_mana_bar);
	dwarf_mana_bar = nullptr;
	App->gui->DeleteUIElement(*dwarf_name);
	dwarf_name = nullptr;

	App->gui->DeleteUIElement(*elf_background);
	elf_background = nullptr;
	App->gui->DeleteUIElement(*elf_HP_bar);
	elf_HP_bar = nullptr;
	App->gui->DeleteUIElement(*elf_mana_bar);
	elf_mana_bar = nullptr;
	App->gui->DeleteUIElement(*elf_name);
	elf_name = nullptr;

	App->gui->DeleteUIElement(*warrior_background);
	warrior_background = nullptr;
	App->gui->DeleteUIElement(*warrior_HP_bar);
	warrior_HP_bar = nullptr;
	App->gui->DeleteUIElement(*warrior_mana_bar);
	warrior_mana_bar = nullptr;
	App->gui->DeleteUIElement(*warrior_name);
	warrior_name = nullptr;

	App->task_manager->CleanUp();
	if (App->map->actual_tier == TIER_MAP_1)
		App->map->actual_tier = TIER_MAP_2;
	else if (App->map->actual_tier == TIER_MAP_2)
		App->map->actual_tier = TIER_MAP_3;
	else if (App->map->actual_tier == TIER_MAP_3)
		App->map->actual_tier = TIER_MAP_4;
	else if (App->map->actual_tier == TIER_MAP_4)
		App->map->actual_tier = TIER_MAP_5;
	else if (App->map->actual_tier == TIER_MAP_5)
		App->map->actual_tier = TIER_MAP_6;
	else if (App->map->actual_tier == TIER_MAP_6)
		App->map->actual_tier = TIER_MAP_7;
	else if (App->map->actual_tier == TIER_MAP_7)
		App->map->actual_tier = TIER_MAP_8;
	else if (App->map->actual_tier == TIER_MAP_8) {
		App->map->actual_tier == TIER_MAP_1;
	}

	if (heroes_are_dead)
		App->map->actual_tier = TIER_MAP_1;

	entity_performing_action = nullptr;

	
	
	return true;
}

bool ctCombat::Load(pugi::xml_node& load)
{
	bool ret = true;

	return ret;
}

bool ctCombat::Save(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}

void ctCombat::OnUITrigger(UIElement* elementTriggered, UI_State ui_state)
{ 
	int x = 0;
}

void ctCombat::SetSceneName(string new_scene_name)
{
	scene_name = new_scene_name;
}

void ctCombat::UpdateHPBarOfEntity(Entity * entity_to_update_bar, int quantity)
{
	switch (entity_to_update_bar->type)
	{
	case CLERIC:
		cleric_HP_bar->LowerBar(quantity);
		break;
	case DWARF:
		dwarf_HP_bar->LowerBar(quantity);
		break;
	case ELF:
		elf_HP_bar->LowerBar(quantity);
		break;
	case WARRIOR:
		warrior_HP_bar->LowerBar(quantity);
		break;
	case KOBOLD:
	case GNOLL:
	case GOBLIN:
	case HEAVY_GOBLIN:
	case ALCHEMIST_GOBLIN:
	case GNOLL_ARCHER:
	case OWLBEAR:
	case TRITON:
	case HELLHOUND:
	case DARK_WARRIOR:
	case DISPLACER_BEAST:
	case TROLL_LEADERS:
	case LICH_1:
	case LICH_2:
	case SKELETON:
	{
		UIBar* tmp_bar = this->GetUIBarFromEntity(entity_to_update_bar);
		tmp_bar->LowerBar(quantity);
	}
		break;
	case MINIHEROES:
	case NO_TYPE:
		break;
	default:
		break;
	}
}

void ctCombat::UpdateManaBarOfEntity(Entity * entity_to_update_bar, int quantity)
{
	switch (entity_to_update_bar->type)
	{
	case CLERIC:
		cleric_mana_bar->LowerBar(quantity);
		break;
	case DWARF:
		dwarf_mana_bar->LowerBar(quantity);
		break;
	case ELF:
		elf_mana_bar->LowerBar(quantity);
		break;
	case WARRIOR:
		warrior_mana_bar->LowerBar(quantity);
		break;
	case KOBOLD:
	case GNOLL:
	case GOBLIN:
	case HEAVY_GOBLIN:
	case ALCHEMIST_GOBLIN:
	case GNOLL_ARCHER:
	case OWLBEAR:
	case TRITON:
	case HELLHOUND:
	case DARK_WARRIOR:
	case DISPLACER_BEAST:
	case TROLL_LEADERS:
	case LICH_1:
	case LICH_2:
	case SKELETON:
	case MINIHEROES:
	case NO_TYPE:
		break;
	default:
		break;
	}
}

void ctCombat::SpawnEntities()
{
	int random_number = (rand() % 4);
	// Test assign life with lifebar

	//randomization is ugly, sorry c:
	switch (random_number)
	{
	case 0:
		for (int i = 0; i < App->map->heroes_position_coords.size(); i++)
		{
			switch (i)
			{
			case 0:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, DWARF);
				break;
			case 1:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, ELF);
				break;
			case 2:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, WARRIOR);
				break;
			case 3:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, CLERIC);
				break;
			default:
				break;
			}
		}
		break;
	case 1:
		for (int i = 0; i < App->map->heroes_position_coords.size(); i++)
		{
			switch (i)
			{
			case 0:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, ELF);
				break;
			case 1:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, DWARF);
				break;
			case 2:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, WARRIOR);
				break;
			case 3:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, CLERIC);
				break;
			default:
				break;
			}
		}
		break;
	case 2:
		for (int i = 0; i < App->map->heroes_position_coords.size(); i++)
		{
			switch (i)
			{
			case 0:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, ELF);
				
				break;
			case 1:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, WARRIOR);
				break;
			case 2:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, CLERIC);
				break;
			case 3:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, DWARF);
				break;
			default:
				break;
			}
		}
		break;
	case 3:
		for (int i = 0; i < App->map->heroes_position_coords.size(); i++)
		{
			switch (i)
			{
			case 0:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, DWARF);
				break;
			case 1:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, ELF);
				break;
			case 2:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, CLERIC);
				break;
			case 3:
				App->entities->SpawnEntity(App->map->heroes_position_coords.at(i).x, App->map->heroes_position_coords.at(i).y, WARRIOR);
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}


	for (int i = 0; i < entities_to_spawn.size(); i++)
	{
		switch (entities_to_spawn.at(i))
		{
		case KOBOLD:
			App->entities->SpawnEntity(App->map->enemies_position_coords.at(i).x, App->map->enemies_position_coords.at(i).y, KOBOLD);
			break;
		case GNOLL:
			App->entities->SpawnEntity(App->map->enemies_position_coords.at(i).x, App->map->enemies_position_coords.at(i).y, GNOLL);
			break;
		case GOBLIN:
			App->entities->SpawnEntity(App->map->enemies_position_coords.at(i).x, App->map->enemies_position_coords.at(i).y, GOBLIN);
			break;
		case HEAVY_GOBLIN:
			App->entities->SpawnEntity(App->map->enemies_position_coords.at(i).x, App->map->enemies_position_coords.at(i).y, HEAVY_GOBLIN);
			break;
		case ALCHEMIST_GOBLIN:
			App->entities->SpawnEntity(App->map->enemies_position_coords.at(i).x, App->map->enemies_position_coords.at(i).y, ALCHEMIST_GOBLIN);
			break;
		case SKELETON:
			App->entities->SpawnEntity(App->map->enemies_position_coords.at(i).x, App->map->enemies_position_coords.at(i).y, SKELETON);
			break;
		case HELLHOUND:
			App->entities->SpawnEntity(App->map->enemies_position_coords.at(i).x, App->map->enemies_position_coords.at(i).y, HELLHOUND);
			break;
		case TRITON:
			App->entities->SpawnEntity(App->map->enemies_position_coords.at(i).x, App->map->enemies_position_coords.at(i).y, TRITON);
			break;
		case OWLBEAR:
			App->entities->SpawnEntity(App->map->enemies_position_coords.at(i).x, App->map->enemies_position_coords.at(i).y, OWLBEAR);
			break;
		case DARK_WARRIOR:
			App->entities->SpawnEntity(App->map->enemies_position_coords.at(i).x, App->map->enemies_position_coords.at(i).y, DARK_WARRIOR);
			break;
		case TROLL_LEADERS:
			App->entities->SpawnEntity(App->map->enemies_position_coords.at(i).x, App->map->enemies_position_coords.at(i).y, TROLL_LEADERS);
			break;
		case LICH_1:
			App->entities->SpawnEntity(App->map->enemies_position_coords.at(1).x, App->map->enemies_position_coords.at(1).y, LICH_1);
			break;
		case LICH_2:
			App->entities->SpawnEntity(App->map->enemies_position_coords.at(1).x, App->map->enemies_position_coords.at(1).y, LICH_2);
			break;
		case DISPLACER_BEAST:
			App->entities->SpawnEntity(App->map->enemies_position_coords.at(i).x, App->map->enemies_position_coords.at(i).y, DISPLACER_BEAST);
			break;
		default:
			break;
		}
	}

}

void ctCombat::LoadDataFromXML()
{
	pugi::xml_document	data_file;
	pugi::xml_node* node = &App->LoadData(data_file);
	node = &node->child("heroes");

	for (pugi::xml_node heroe = node->child("heroe"); heroe; heroe = heroe.next_sibling("heroe"))
	{
		std::string tmp(heroe.attribute("name").as_string());

		if (tmp == "cleric") {
			if (!App->main_menu->is_new_game) {
				App->entities->GetCleric()->SetCurrentHealthPoints(heroe.child("values").attribute("health_points").as_uint());
				App->entities->GetCleric()->SetCurrentManaPoints(heroe.child("values").attribute("mana_points").as_uint());
			}
			for (pugi::xml_node skill = heroe.child("skills").child("skill"); skill; skill = skill.next_sibling("skill")) {
				if (App->main_menu->is_new_game == true) {
					skill.attribute("owned").set_value(0);
				}
				LoadSkill(skill, App->entities->GetCleric());
			}

			if (App->main_menu->is_new_game == true) {

				pugi::xml_node item = heroe.child("items");

				item.attribute("helmet").set_value("");
				item.attribute("boot").set_value("");
				item.attribute("gauntlet").set_value("");
				item.attribute("accessory").set_value("");
				item.attribute("chest").set_value("");
				item.attribute("shield").set_value("");
				item.attribute("ring").set_value("");
				item.attribute("weapon").set_value("");
			}
			else
			{
				LoadEquipableItem(heroe.child("items"), App->entities->GetCleric());
			}

			for (pugi::xml_node item = heroe.child("items").child("item"); item; item = item.next_sibling("item")) {
				if (App->main_menu->is_new_game == true) {
					item.attribute("quantity").set_value(0);
				}
				LoadItem(item, App->entities->GetCleric());
			}
		}
		else if (tmp == "warrior") {
			if (!App->main_menu->is_new_game) {
				App->entities->GetWarrior()->SetCurrentHealthPoints(heroe.child("values").attribute("health_points").as_uint());
				App->entities->GetWarrior()->SetCurrentManaPoints(heroe.child("values").attribute("mana_points").as_uint());
			}
			for (pugi::xml_node skill = heroe.child("skills").child("skill"); skill; skill = skill.next_sibling("skill")) {
				if (App->main_menu->is_new_game == true) {
					skill.attribute("owned").set_value(0);
				}
				LoadSkill(skill, App->entities->GetWarrior());
			}

			if (App->main_menu->is_new_game == true) {

				pugi::xml_node item = heroe.child("items");

				item.attribute("helmet").set_value("");
				item.attribute("boot").set_value("");
				item.attribute("gauntlet").set_value("");
				item.attribute("accessory").set_value("");
				item.attribute("chest").set_value("");
				item.attribute("shield").set_value("");
				item.attribute("ring").set_value("");
				item.attribute("weapon").set_value("");
			}
			else
			{
				LoadEquipableItem(heroe.child("items"), App->entities->GetWarrior());
			}


			for (pugi::xml_node item = heroe.child("items").child("item"); item; item = item.next_sibling("item")) {
				if (App->main_menu->is_new_game == true) {
					item.attribute("quantity").set_value(0);
				}
				LoadItem(item, App->entities->GetWarrior());
			}
		}
		else if (tmp == "dwarf") {
			if (!App->main_menu->is_new_game) {
				App->entities->GetDwarf()->SetCurrentHealthPoints(heroe.child("values").attribute("health_points").as_uint());
				App->entities->GetDwarf()->SetCurrentManaPoints(heroe.child("values").attribute("mana_points").as_uint());
			}
			for (pugi::xml_node skill = heroe.child("skills").child("skill"); skill; skill = skill.next_sibling("skill")) {
				if (App->main_menu->is_new_game == true) {
					skill.attribute("owned").set_value(0);
				}
				LoadSkill(skill, App->entities->GetDwarf());
			}

			if (App->main_menu->is_new_game == true) {

				pugi::xml_node item = heroe.child("items");

				item.attribute("helmet").set_value("");
				item.attribute("boot").set_value("");
				item.attribute("gauntlet").set_value("");
				item.attribute("accessory").set_value("");
				item.attribute("chest").set_value("");
				item.attribute("shield").set_value("");
				item.attribute("ring").set_value("");
				item.attribute("weapon").set_value("");
			}
			else
			{
				LoadEquipableItem(heroe.child("items"), App->entities->GetDwarf());
			}


			for (pugi::xml_node item = heroe.child("items").child("item"); item; item = item.next_sibling("item")) {
				if (App->main_menu->is_new_game == true) {
					item.attribute("quantity").set_value(0);
				}
				LoadItem(item, App->entities->GetDwarf());
			}
		}
		else if (tmp == "elf") {
			if (!App->main_menu->is_new_game) {
				App->entities->GetElf()->SetCurrentHealthPoints(heroe.child("values").attribute("health_points").as_uint());
				App->entities->GetElf()->SetCurrentManaPoints(heroe.child("values").attribute("mana_points").as_uint());
			}
			for (pugi::xml_node skill = heroe.child("skills").child("skill"); skill; skill = skill.next_sibling("skill")) {
				if (App->main_menu->is_new_game == true) {
					skill.attribute("owned").set_value(0);
				}
				LoadSkill(skill, App->entities->GetElf());
			}

			if (App->main_menu->is_new_game == true) {

				pugi::xml_node item = heroe.child("items");

				item.attribute("helmet").set_value("");
				item.attribute("boot").set_value("");
				item.attribute("gauntlet").set_value("");
				item.attribute("accessory").set_value("");
				item.attribute("chest").set_value("");
				item.attribute("shield").set_value("");
				item.attribute("ring").set_value("");
				item.attribute("weapon").set_value("");
			}
			else
			{
				LoadEquipableItem(heroe.child("items"), App->entities->GetElf());
			}


			for (pugi::xml_node item = heroe.child("items").child("item"); item; item = item.next_sibling("item")) {
				if (App->main_menu->is_new_game == true) {
					item.attribute("quantity").set_value(0);
				}
				LoadItem(item, App->entities->GetElf());
			}
		}

	}

	data_file.save_file("data.xml");
	data_file.reset();

}

void ctCombat::LoadSkill(pugi::xml_node skill_node, Entity * entity)
{
	Action new_action;
	std::string tmp = skill_node.attribute("name").as_string();
	new_action.name = tmp;

	new_action.owned = skill_node.attribute("owned").as_int();
	
	if (entity->type == WARRIOR)
	{
		if (tmp == "Kick")
			new_action.type = KICK;
		else if (tmp == "Guard")
			new_action.type = GUARD;
		else if (tmp == "Heavy Slash")
			new_action.type = HEAVY_SLASH;
		else if (tmp == "Kick +")
			new_action.type = KICK_PLUS;
		else if (tmp == "Guard +")
			new_action.type = GUARD_PLUS;
		else if (tmp == "Heavy Slash +")
			new_action.type = HEAVY_SLASH_PLUS;
		else if (tmp == "Charge")
			new_action.type = CHARGE;
		else if (tmp == "Taunt")
			new_action.type = TAUNT;
		else if (tmp == "Whirlwind")
			new_action.type = WHIRLWIND;
	}
	else if (entity->type == DWARF)
	{
		if (tmp == "H. Axe")
			new_action.type = HIGH_AXE;
		else if (tmp == "Morale Boost")
			new_action.type = MORALE_BOOST;
		else if (tmp == "Dwarfs Wrath")
			new_action.type = DWARFS_WRATH;
		else if (tmp == "High Axe +")
			new_action.type = HIGH_AXE_PLUS;
		else if (tmp == "Morale Boost +")
			new_action.type = MORALE_BOOST_PLUS;
		else if (tmp == "Dwarfs Wrath +")
			new_action.type = DWARFS_WRATH_PLUS;
		else if (tmp == "Raging Mock")
			new_action.type = RAGING_MOCK;
		else if (tmp == "Meteor")
			new_action.type = METEOR;
		else if (tmp == "Counter")
			new_action.type = COUNTER;
	}
	else if (entity->type == CLERIC)
	{
		if (tmp == "Heal")
			new_action.type = HEAL;
		else if (tmp == "Light Strike")
			new_action.type = LIGHT_STRIKE;
		else if (tmp == "Mace Throw")
			new_action.type = MACE_THROW;
		else if (tmp == "Heal +")
			new_action.type = HEAL_PLUS;
		else if (tmp == "Light Strike +")
			new_action.type = LIGHT_STRIKE_PLUS;
		else if (tmp == "Mace Throw +")
			new_action.type = MACE_THROW_PLUS;
		else if (tmp == "Blessing")
			new_action.type = BLESSING;
		else if (tmp == "Harden Skin")
			new_action.type = HARDEN_SKIN;
		else if (tmp == "Clarity")
			new_action.type = CLARITY;
	}
	else if (entity->type == ELF)
	{
		if (tmp == "Blizzard")
			new_action.type = BLIZZARD;
		else if (tmp == "Seed Of Life")
			new_action.type = SEED_OF_LIFE;
		else if (tmp == "Fireball")
			new_action.type = FIREBALL;
		else if (tmp == "Lightning Bolt")
			new_action.type = LIGHTNING_BOLT;
		else if (tmp == "Seed Of Life +")
			new_action.type = SEED_OF_LIFE_PLUS;
		else if (tmp == "Fireball +")
			new_action.type = FIREBALL_PLUS;
		else if (tmp == "Lightning Bolt +")
			new_action.type = LIGHTNING_BOLT_PLUS;
		else if (tmp == "Revive")
			new_action.type = REVIVE;
		else if (tmp == "Fire Djinn")
			new_action.type = FIRE_DJINN;
	}

	if (skill_node.attribute("objective").as_int() == 1) 
		new_action.objective = ENEMIES;
	else if(skill_node.attribute("objective").as_int() == 0)
		new_action.objective = HEROES;
	else if (skill_node.attribute("objective").as_int() == 2)
		new_action.objective = DEAD_HEROES;
	else if (skill_node.attribute("objective").as_int() == 3)
		new_action.objective = NO_SELECTION;

	new_action.have_to_move = skill_node.attribute("have_to_move").as_int();

	new_action.description = skill_node.attribute("description").as_string();
	new_action.mana_points_effect_to_himself = skill_node.attribute("mana_points_effect_to_himself").as_int();
	new_action.health_points_effect = skill_node.attribute("health_points_effect").as_int();
	new_action.stun_chance = skill_node.attribute("stun_chance").as_int();
	new_action.burn_chance = skill_node.attribute("burn_chance").as_int();
	new_action.magical_defense_variation = skill_node.attribute("magical_defense_variation").as_int();
	new_action.physical_defense_variation = skill_node.attribute("physical_defense_variation").as_int();
	new_action.intelligence_variation = skill_node.attribute("intelligence_variation").as_int();
	new_action.strength_variation = skill_node.attribute("strength_variation").as_int();
	new_action.bleeding_chance = skill_node.attribute("bleeding_chance").as_int();
	new_action.judgement_variation = skill_node.attribute("judgement_variation").as_int();
	new_action.dexterity_variation = skill_node.attribute("dexterity_variation").as_int();
	entity->AddAction(new_action);
}

void ctCombat::LoadItem(pugi::xml_node item, Entity * entity)
{
	std::string tmp = item.attribute("name").as_string();

	if (tmp == "Life Potion") {
		for (int i = 0; i < item.attribute("quantity").as_int(); i++)
			entity->AddUsableItem(App->items->usable_items.at(0));
	}
	else if (tmp == "Mega Life Potion") {
		for (int i = 0; i < item.attribute("quantity").as_int(); i++)
			entity->AddUsableItem(App->items->usable_items.at(1));
	}
	else if (tmp == "Mana Potion") {
		for (int i = 0; i < item.attribute("quantity").as_int(); i++)
			entity->AddUsableItem(App->items->usable_items.at(2));
	}
	else if (tmp == "Mega Mana Potion") {
		for (int i = 0; i < item.attribute("quantity").as_int(); i++)
			entity->AddUsableItem(App->items->usable_items.at(3));
	}
	else if (tmp == "Poisoned Dagger") {
		for (int i = 0; i < item.attribute("quantity").as_int(); i++)
			entity->AddUsableItem(App->items->usable_items.at(4));
	}
	else if (tmp == "Dragon Essence") {
		for (int i = 0; i < item.attribute("quantity").as_int(); i++)
			entity->AddUsableItem(App->items->usable_items.at(5));
	}
	else if (tmp == "Fenix Tail") {
		for (int i = 0; i < item.attribute("quantity").as_int(); i++)
			entity->AddUsableItem(App->items->usable_items.at(6));
	}
}

void ctCombat::LoadEquipableItem(pugi::xml_node item, Entity* entity)
{
	std::string tmp = "";

	for (int i = 0; i < 8; i++)
	{
		switch (i)
		{
		case 0:
			tmp = item.attribute("boot").as_string();
			break;
		case 1:
			tmp = item.attribute("ring").as_string();
			break;
		case 2:
			tmp = item.attribute("helmet").as_string();
			break;
		case 3:
			tmp = item.attribute("shield").as_string();
			break;
		case 4:
			tmp = item.attribute("weapon").as_string();
			break;
		case 5:
			tmp = item.attribute("gauntlet").as_string();
			break;
		case 6:
			tmp = item.attribute("chest").as_string();
			break;
		case 7:
			tmp = item.attribute("accessory").as_string();
			break;
		}

		bool finded = false;

		for (int i = 0; i < App->items->tier_1_equips.size(); i++)
		{
			if (tmp == App->items->tier_1_equips.at(i).name)
			{
				finded = true;
				entity->AddEquipItem(App->items->tier_1_equips.at(i));
				break;
			}
		}

		if (!finded)
		{
			for (int i = 0; i < App->items->tier_2_equips.size(); i++)
			{
				if (tmp == App->items->tier_2_equips.at(i).name)
				{
					finded = true;
					entity->AddEquipItem(App->items->tier_2_equips.at(i));
					break;
				}
			}
		}

		if (!finded)
		{
			for (int i = 0; i < App->items->tier_3_equips.size(); i++)
			{
				if (tmp == App->items->tier_3_equips.at(i).name)
				{
					finded = true;
					entity->AddEquipItem(App->items->tier_3_equips.at(i));
					break;
				}
			}
		}

	}

}

void ctCombat::SaveDataToXML()
{
	pugi::xml_document	data_file;
	pugi::xml_node* node = &App->LoadData(data_file);
	node = &node->child("heroes");



	for (pugi::xml_node heroe = node->child("heroe"); heroe; heroe = heroe.next_sibling("heroe"))
	{
		std::string tmp(heroe.attribute("name").as_string());

		if (tmp == "cleric") {
			heroe.child("values").attribute("health_points").set_value(App->entities->GetCleric()->GetCurrentHealthPoints());
			heroe.child("values").attribute("mana_points").set_value(App->entities->GetCleric()->GetCurrentManaPoints());
			for (int i = 0; i < App->entities->GetCleric()->usable_items.size(); i++)
			{
				std::string tmp = App->entities->GetCleric()->usable_items.at(i).name;

				for (pugi::xml_node item = heroe.child("items").child("item"); item; item = item.next_sibling("item")) {
					std::string tmp2 = item.attribute("name").as_string();
					if (tmp2 == tmp) {
						if(!heroes_are_dead || App->map->actual_tier != TierList::TIER_MAP_8)
							item.attribute("quantity").set_value(App->entities->GetCleric()->usable_items.at(i).quantity);
						else
							item.attribute("quantity").set_value(0);
					}
						
				}
			}
			App->items->cleric_equip.push_back(App->entities->GetCleric()->helmet);
			App->items->cleric_equip.push_back(App->entities->GetCleric()->chest);
			App->items->cleric_equip.push_back(App->entities->GetCleric()->guantlet);
			App->items->cleric_equip.push_back(App->entities->GetCleric()->boot);
			App->items->cleric_equip.push_back(App->entities->GetCleric()->ring);
			App->items->cleric_equip.push_back(App->entities->GetCleric()->accessory);
			App->items->cleric_equip.push_back(App->entities->GetCleric()->shield);
			App->items->cleric_equip.push_back(App->entities->GetCleric()->weapon);
		}
		else if (tmp == "warrior") {
			heroe.child("values").attribute("health_points").set_value(App->entities->GetWarrior()->GetCurrentHealthPoints());
			heroe.child("values").attribute("mana_points").set_value(App->entities->GetWarrior()->GetCurrentManaPoints());

			for (int i = 0; i < App->entities->GetWarrior()->usable_items.size(); i++)
			{
				std::string tmp = App->entities->GetWarrior()->usable_items.at(i).name;

				for (pugi::xml_node item = heroe.child("items").child("item"); item; item = item.next_sibling("item")) {
					std::string tmp2 = item.attribute("name").as_string();
					if (tmp2 == tmp) {
						if (!heroes_are_dead || App->map->actual_tier != TierList::TIER_MAP_8)
							item.attribute("quantity").set_value(App->entities->GetWarrior()->usable_items.at(i).quantity);
						else
							item.attribute("quantity").set_value(0);
					}
				}
			}
			App->items->warrior_equip.push_back(App->entities->GetWarrior()->helmet);
			App->items->warrior_equip.push_back(App->entities->GetWarrior()->chest);
			App->items->warrior_equip.push_back(App->entities->GetWarrior()->guantlet);
			App->items->warrior_equip.push_back(App->entities->GetWarrior()->boot);
			App->items->warrior_equip.push_back(App->entities->GetWarrior()->ring);
			App->items->warrior_equip.push_back(App->entities->GetWarrior()->accessory);
			App->items->warrior_equip.push_back(App->entities->GetWarrior()->shield);
			App->items->warrior_equip.push_back(App->entities->GetWarrior()->weapon);

		}
		else if (tmp == "dwarf") {
			heroe.child("values").attribute("health_points").set_value(App->entities->GetDwarf()->GetCurrentHealthPoints());
			heroe.child("values").attribute("mana_points").set_value(App->entities->GetDwarf()->GetCurrentManaPoints());
			for (int i = 0; i < App->entities->GetDwarf()->usable_items.size(); i++)
			{
				std::string tmp = App->entities->GetDwarf()->usable_items.at(i).name;

				for (pugi::xml_node item = heroe.child("items").child("item"); item; item = item.next_sibling("item")) {
					std::string tmp2 = item.attribute("name").as_string();
					if (tmp2 == tmp) {
						if (!heroes_are_dead || App->map->actual_tier != TierList::TIER_MAP_8)
							item.attribute("quantity").set_value(App->entities->GetDwarf()->usable_items.at(i).quantity);
						else
							item.attribute("quantity").set_value(0);
					}
				}
			}
			App->items->dwarf_equip.push_back(App->entities->GetDwarf()->helmet);
			App->items->dwarf_equip.push_back(App->entities->GetDwarf()->chest);
			App->items->dwarf_equip.push_back(App->entities->GetDwarf()->guantlet);
			App->items->dwarf_equip.push_back(App->entities->GetDwarf()->boot);
			App->items->dwarf_equip.push_back(App->entities->GetDwarf()->ring);
			App->items->dwarf_equip.push_back(App->entities->GetDwarf()->accessory);
			App->items->dwarf_equip.push_back(App->entities->GetDwarf()->shield);
			App->items->dwarf_equip.push_back(App->entities->GetDwarf()->weapon);
		}
		else if (tmp == "elf") {
			heroe.child("values").attribute("health_points").set_value(App->entities->GetElf()->GetCurrentHealthPoints());
			heroe.child("values").attribute("mana_points").set_value(App->entities->GetElf()->GetCurrentManaPoints());
			for (int i = 0; i < App->entities->GetElf()->usable_items.size(); i++)
			{
				std::string tmp = App->entities->GetElf()->usable_items.at(i).name;

				for (pugi::xml_node item = heroe.child("items").child("item"); item; item = item.next_sibling("item")) {
					std::string tmp2 = item.attribute("name").as_string();
					if (tmp2 == tmp) {
						if (!heroes_are_dead || App->map->actual_tier != TierList::TIER_MAP_8)
							item.attribute("quantity").set_value(App->entities->GetElf()->usable_items.at(i).quantity);
						else
							item.attribute("quantity").set_value(0);
					}
				}
			}
			App->items->elf_equip.push_back(App->entities->GetElf()->helmet);
			App->items->elf_equip.push_back(App->entities->GetElf()->chest);
			App->items->elf_equip.push_back(App->entities->GetElf()->guantlet);
			App->items->elf_equip.push_back(App->entities->GetElf()->boot);
			App->items->elf_equip.push_back(App->entities->GetElf()->ring);
			App->items->elf_equip.push_back(App->entities->GetElf()->accessory);
			App->items->elf_equip.push_back(App->entities->GetElf()->shield);
			App->items->elf_equip.push_back(App->entities->GetElf()->weapon);
		}

	}

	data_file.save_file("data.xml");
	data_file.reset();
}

void ctCombat::SetDataToUI()
{
	// TEST FOR UI BAR WITH CALC DAMAGE
	//Entity* cleric = App->entities->GetCleric();
	//test = (UIBar*)App->gui->AddUIBar(100,100,cleric->base_stats.base_constitution*13,LIFEBAR);

	Entity* cleric = App->entities->GetCleric();
	cleric_HP_bar = (UIBar*)App->gui->AddUIBar(34, -1, cleric->max_health_points, LIFEBAR, cleric);
	cleric_mana_bar = (UIBar*)App->gui->AddUIBar(34, 10, cleric->max_mana_points, MANABAR, cleric);

	Entity* warrior = App->entities->GetWarrior();
	warrior_HP_bar = (UIBar*)App->gui->AddUIBar(277, -1, warrior->max_health_points, LIFEBAR, warrior);
	warrior_mana_bar = (UIBar*)App->gui->AddUIBar(277, 10, warrior->max_mana_points, MANABAR, warrior);

	Entity* elf = App->entities->GetElf();
	elf_HP_bar = (UIBar*)App->gui->AddUIBar(34, 293, elf->max_health_points, LIFEBAR, elf);
	elf_mana_bar = (UIBar*)App->gui->AddUIBar(34, 304, elf->max_mana_points, MANABAR, elf);

	Entity* dwarf = App->entities->GetDwarf();
	dwarf_HP_bar = (UIBar*)App->gui->AddUIBar(277, 293, dwarf->max_health_points, LIFEBAR, dwarf);
	dwarf_mana_bar = (UIBar*)App->gui->AddUIBar(277, 304, dwarf->max_mana_points, MANABAR, dwarf);

	//update bar
	cleric_HP_bar->LowerBar((cleric_HP_bar->max_capacity - cleric->GetCurrentHealthPoints()) * -1);
	cleric_mana_bar->LowerBar((cleric_mana_bar->max_capacity - cleric->GetCurrentManaPoints()) * -1);
	warrior_HP_bar->LowerBar((warrior_HP_bar->max_capacity - warrior->GetCurrentHealthPoints()) * -1);
	warrior_mana_bar->LowerBar((warrior_mana_bar->max_capacity - warrior->GetCurrentManaPoints()) * -1);
	elf_HP_bar->LowerBar((elf_HP_bar->max_capacity - elf->GetCurrentHealthPoints()) * -1);
	elf_mana_bar->LowerBar((elf_mana_bar->max_capacity - elf->GetCurrentManaPoints()) * -1);
	dwarf_HP_bar->LowerBar((dwarf_HP_bar->max_capacity - dwarf->GetCurrentHealthPoints()) * -1);
	dwarf_mana_bar->LowerBar((dwarf_mana_bar->max_capacity - dwarf->GetCurrentManaPoints())* - 1);
}

void ctCombat::EnemiesBarShouldBeVisible()
{
	std::vector<Entity*>::const_iterator it_vector = enemies.begin();
	while (it_vector != enemies.end()) {
		if ((*it_vector)->animation == &(*it_vector)->idle || (*it_vector)->animation == &(*it_vector)->hit || (*it_vector)->animation == &(*it_vector)->death || (*it_vector)->animation == &(*it_vector)->dodge) {
			UIBar* current_entity_bar = GetUIBarFromEntity((*it_vector));
			current_entity_bar->MakeElementsVisible();
			it_vector++;
		}
		else {
			UIBar* current_entity_bar = GetUIBarFromEntity((*it_vector));
			current_entity_bar->MakeElementsInvisible();
			it_vector++;
		}
	}
}

void ctCombat::OrderTurnPriority()
{
	bool ordered = false;

	while (!ordered)
	{
		ordered = true;
		std::vector<Entity*>::iterator itnext = turn_priority_entity.begin();
		int count = 0;
		for (std::vector<Entity*>::iterator it = turn_priority_entity.begin(); it != turn_priority_entity.end(); ++it)
		{
			itnext++;
			count++;
			if (count != turn_priority_entity.size())
			{
				if ((*it)->current_agility_points < (*itnext)->current_agility_points)
				{
					Entity* entity_tmp = (*it);

					(*it) = (*itnext);
					it++;
					(*it) = entity_tmp;
					it--;
					ordered = false;
				}
			}
			else {
				break;
			}
				
		}

	}

	draw_turn_priority_entity = turn_priority_entity;

	for (int i = 0; i < draw_turn_priority_entity.size(); i++)
	{
		if (draw_turn_priority_entity.at(i)->IsStunned()) {
			draw_turn_priority_entity.erase(draw_turn_priority_entity.cbegin() + i);
			draw_turn_priority_entity.shrink_to_fit();
		}
	}

}

void ctCombat::DrawTurnPriority()
{
	uint x=10, y=30;
	for (std::vector<Entity*>::iterator it = draw_turn_priority_entity.begin(); it != draw_turn_priority_entity.end(); ++it)
	{

		SDL_Rect rect;
	
		switch ((*it)->type)
		{
		case CLERIC:
			rect = { 142, 2652, 26, 26 };
			break;
		case DWARF:
			rect = { 81, 333, 26, 26 };
			break;
		case ELF:
			rect = { 852, 2150, 26, 26 };
			break;
		case WARRIOR:
			rect = { 1646, 112, 26, 26 };
			break;
		case KOBOLD:
			rect = { 881,71,26,26 };
			break;
		case GOBLIN:
			rect = { 1039,610,26,26 };
			break;
		case HEAVY_GOBLIN:
			rect = { 1039,1040,26,26 };
			break;
		case ALCHEMIST_GOBLIN:
			rect = { 1039,121,26,26 };
			break;
		case GNOLL:
			rect = { 821,8,26,26 };
			break;
		case GNOLL_ARCHER:
			rect = { 993, 25, 26, 26 };
			break;
		case OWLBEAR:
			rect = { 1147, 19, 26, 26 };
			break;
		case TRITON:
			rect = { 70, 11, 26, 26 };
			break;
		case SKELETON:
			rect = { 264, 86, 26, 26 };
			break;
		case HELLHOUND:
			rect = { 858, 22, 26, 26 };
			break;
		case TROLL_LEADERS:
			rect = { 30, 726, 26, 26 };
			break;
		case DISPLACER_BEAST:
			rect = { 1078,230,26,26 };
			break;
		case DARK_WARRIOR:
			rect = { 89,251,26,26 };
			break;
		case LICH_1:
			rect = { 161,375,26,26 };
			break;
		case LICH_2:
			rect = { 305,22,26,26 };
			break;
		case MINIHEROES:
			break;
		case NO_TYPE:
			break;
		}
		App->render->Blit((*it)->texture, x, y, &rect);
		y += 26;

	}

}

bool ctCombat::PerformActionWithEntity(Entity * entity_to_perform_action)
{
	bool established_action = false;

	Entity* entity_objective = nullptr;

	if (entity_to_perform_action->GetCurrentHealthPoints() == 0)
		established_action = true;

	if (App->entities->GetCleric()->animation == &App->entities->GetCleric()->selected && App->entities->GetCleric() != entity_to_perform_action)
		App->entities->GetCleric()->animation = &App->entities->GetCleric()->idle;
	if (App->entities->GetWarrior()->animation == &App->entities->GetWarrior()->selected && App->entities->GetWarrior() != entity_to_perform_action)
		App->entities->GetWarrior()->animation = &App->entities->GetWarrior()->idle;
	if (App->entities->GetElf()->animation == &App->entities->GetElf()->selected && App->entities->GetElf() != entity_to_perform_action)
		App->entities->GetElf()->animation = &App->entities->GetElf()->idle;
	if (App->entities->GetDwarf()->animation == &App->entities->GetDwarf()->selected && App->entities->GetDwarf() != entity_to_perform_action)
		App->entities->GetDwarf()->animation = &App->entities->GetDwarf()->idle;

	if (!established_action) {
		switch (entity_to_perform_action->type)
		{
		case CLERIC:
			if (!entity_to_perform_action->IsStunned()) {
				if (!making_decision) {
					combat_menu = (UICombatMenu*)App->gui->AddUICombatMenu(entity_to_perform_action, entity_to_perform_action->position.x + entity_to_perform_action->animation->GetCurrentFrame().w + 10, entity_to_perform_action->position.y - entity_to_perform_action->animation->GetCurrentFrame().h - 10, this, nullptr);
					making_decision = true;
					App->entities->GetCleric()->animation = &App->entities->GetCleric()->selected;
				}
				else {

					if (combat_menu->background == nullptr) {
						ready_cleric = App->gui->AddUILabel(entity_to_perform_action->position.x + 20, entity_to_perform_action->position.y - entity_to_perform_action->animation->GetCurrentFrame().h - 20, "Ready", { 255,255,255,255 }, 25, this);
						combat_menu->~UICombatMenu();
						ready_cleric->to_destroy = false;
						App->gui->DeleteUIElement(*combat_menu);
						combat_menu = nullptr;
						established_action = true;
						making_decision = false;
						App->entities->GetCleric()->animation = &App->entities->GetCleric()->idle;
					}
				}
			}else
				established_action = true;
			break;
		case DWARF:
			if (!entity_to_perform_action->IsStunned()) {
				if (!making_decision) {
					combat_menu = (UICombatMenu*)App->gui->AddUICombatMenu(entity_to_perform_action, entity_to_perform_action->position.x + entity_to_perform_action->animation->GetCurrentFrame().w + 10, entity_to_perform_action->position.y - entity_to_perform_action->animation->GetCurrentFrame().h - 10, this, nullptr);
					making_decision = true;
					App->entities->GetDwarf()->animation = &App->entities->GetDwarf()->selected;
				}
				else {
					if (combat_menu->background == nullptr) {
						ready_dwarf = App->gui->AddUILabel(entity_to_perform_action->position.x + 15, entity_to_perform_action->position.y - entity_to_perform_action->animation->GetCurrentFrame().h-20, "Ready", { 255,255,255,255 }, 25, this);
						combat_menu->~UICombatMenu();
						ready_dwarf->to_destroy = false;
						App->gui->DeleteUIElement(*combat_menu);
						combat_menu = nullptr;
						established_action = true;
						making_decision = false;
						App->entities->GetDwarf()->animation = &App->entities->GetDwarf()->idle;
					}
				}
			}
			else
				established_action = true;
			break;
		case ELF:
			if (!entity_to_perform_action->IsStunned()) {
				if (!making_decision) {
					combat_menu = (UICombatMenu*)App->gui->AddUICombatMenu(entity_to_perform_action, entity_to_perform_action->position.x + entity_to_perform_action->animation->GetCurrentFrame().w + 10, entity_to_perform_action->position.y - entity_to_perform_action->animation->GetCurrentFrame().h - 10, this, nullptr);
					making_decision = true;
					App->entities->GetElf()->animation = &App->entities->GetElf()->selected;
				}
				else {
					if (combat_menu->background == nullptr) {
						ready_elf = App->gui->AddUILabel(entity_to_perform_action->position.x + 10, entity_to_perform_action->position.y - entity_to_perform_action->animation->GetCurrentFrame().h-20, "Ready", { 255,255,255,255 }, 25, this);
						if(combat_menu->abilities_label != nullptr)
							combat_menu->~UICombatMenu();
						ready_elf->to_destroy = false; 
						App->gui->DeleteUIElement(*combat_menu);
						combat_menu = nullptr;
						established_action = true;
						making_decision = false;
						App->entities->GetElf()->animation = &App->entities->GetElf()->idle;
					}
				}
			}
			else
				established_action = true;
			break;
		case WARRIOR:
			if (!entity_to_perform_action->IsStunned()) {
				if (!making_decision) {
					combat_menu = (UICombatMenu*)App->gui->AddUICombatMenu(entity_to_perform_action, entity_to_perform_action->position.x + entity_to_perform_action->animation->GetCurrentFrame().w + 10, entity_to_perform_action->position.y - entity_to_perform_action->animation->GetCurrentFrame().h - 10, this, nullptr);
					making_decision = true;
					App->entities->GetWarrior()->animation = &App->entities->GetWarrior()->selected;
				}
				else {
					if (combat_menu->background == nullptr) {
						ready_warrior = App->gui->AddUILabel(entity_to_perform_action->position.x+22, entity_to_perform_action->position.y- entity_to_perform_action->animation->GetCurrentFrame().h-20, "Ready", { 255,255,255,255 }, 25, this);
						combat_menu->~UICombatMenu();
						ready_warrior->to_destroy = false;
						App->gui->DeleteUIElement(*combat_menu);
						combat_menu = nullptr;
						established_action = true;
						making_decision = false;
						App->entities->GetWarrior()->animation = &App->entities->GetWarrior()->idle;
					}
				}
			}
			else
				established_action = true;
			break;
		case KOBOLD: 
		case GNOLL:
		case GOBLIN:
		case HEAVY_GOBLIN:
		case ALCHEMIST_GOBLIN:
		case GNOLL_ARCHER:
		case OWLBEAR:
		case HELLHOUND:
		case TRITON:
		case SKELETON:
		case DARK_WARRIOR:
		case TROLL_LEADERS:
		case LICH_1:
		case LICH_2:
		case DISPLACER_BEAST:
		{
			bool can_perform_action = true;
			//todo do an action like IsStunned()
			if(entity_to_perform_action->IsStunned())
					can_perform_action = false;

			if (can_perform_action)
				entity_to_perform_action->PerformAction();

			established_action = true;
		}
		break;
		case MINIHEROES:
		case NO_TYPE:
			LOG("this should not happen");
			break;
		default:
			break;
		}
	}
	

	return established_action;
}

Entity * ctCombat::GetTheWeakestHeroe()
{
	//now we get the lowest hp

	Entity* tmp_entity = App->entities->GetCleric();
	if(tmp_entity->GetCurrentHealthPoints() == 0)
		tmp_entity = App->entities->GetDwarf();
	if (tmp_entity->GetCurrentHealthPoints() == 0)
		tmp_entity = App->entities->GetElf();
	if (tmp_entity->GetCurrentHealthPoints() == 0)
		tmp_entity = App->entities->GetWarrior();

	if (tmp_entity->GetCurrentHealthPoints() > App->entities->GetCleric()->GetCurrentHealthPoints() && App->entities->GetCleric()->GetCurrentHealthPoints()>0)
		tmp_entity = App->entities->GetCleric();

	if (tmp_entity->GetCurrentHealthPoints() > App->entities->GetDwarf()->GetCurrentHealthPoints() && App->entities->GetDwarf()->GetCurrentHealthPoints()>0)
		tmp_entity = App->entities->GetDwarf();

	if (tmp_entity->GetCurrentHealthPoints() > App->entities->GetElf()->GetCurrentHealthPoints() && App->entities->GetElf()->GetCurrentHealthPoints()>0)
		tmp_entity = App->entities->GetElf();

	if (tmp_entity->GetCurrentHealthPoints() > App->entities->GetWarrior()->GetCurrentHealthPoints() && App->entities->GetWarrior()->GetCurrentHealthPoints()>0)
		tmp_entity = App->entities->GetWarrior();

	return tmp_entity;
}

Entity * ctCombat::GetRandomHeroe()
{
	int random_number = rand() % 4; //0-3

	Entity* tmp_entity = nullptr;

	do
	{
		int random_number = rand() % 4; //0-3

		switch (random_number)
		{
		case 0:
			tmp_entity = App->entities->GetCleric();
			break;
		case 1:
			tmp_entity = App->entities->GetWarrior();
			break;
		case 2:
			tmp_entity = App->entities->GetDwarf();
			break;
		case 3:
			tmp_entity = App->entities->GetElf();
			break;
		default:
			break;
		}

	} while (tmp_entity->GetCurrentHealthPoints()==0);

	return tmp_entity;
}

UIBar* ctCombat::GetUIBarFromEntity(Entity* entity) {
	for (int i = 0; i < enemies_bars.size(); i++) {
		if (enemies_bars.at(i)->entity == entity) {
			return enemies_bars.at(i);
		}
	}
}

void ctCombat::SelectWithPreviousHeroe() {

	std::vector<Entity*> auxiliar_vector;
	std::vector<Entity*>::const_iterator it_vector = heroes.begin();
	while (it_vector != heroes.end()) {
		if ((*it_vector) == combat_menu->entity) {
			if (it_vector != heroes.begin()) {
				making_decision = false;
				it_vector--;
				while ((*it_vector)->GetCurrentHealthPoints()<=0) {
					if (it_vector != heroes.begin()) {
						it_vector--;
					}
					else {
						break;
					}
				}
				if ((*it_vector)->type == ELF) {
					App->gui->DeleteUIElement(*ready_elf);
					ready_elf = nullptr;
				}
				else if ((*it_vector)->type == CLERIC) {
					App->gui->DeleteUIElement(*ready_cleric);
					ready_cleric = nullptr;
				}
				else if ((*it_vector)->type == DWARF) {
					App->gui->DeleteUIElement(*ready_dwarf);
					ready_dwarf = nullptr;
				}
				else if ((*it_vector)->type == WARRIOR) {
					App->gui->DeleteUIElement(*ready_warrior);
					ready_warrior = nullptr;
				}
				App->gui->DeleteUIElement(*combat_menu);
				auxiliar_vector.push_back((*it_vector));
				std::vector<Entity*>::const_iterator it_vector2 = turn_priority_entity.begin();
				while (it_vector2 != turn_priority_entity.end()) {
					auxiliar_vector.push_back((*it_vector2));
					it_vector2++;
				}
				App->task_manager->DeleteTasksFromEntity((*it_vector));
				turn_priority_entity = auxiliar_vector;
				break;
			}
			break;
		}
		else {
			it_vector++;
		}
	}
}