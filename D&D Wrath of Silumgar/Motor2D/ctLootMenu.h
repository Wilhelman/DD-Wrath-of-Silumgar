#ifndef __LOOTMENU_H__
#define __LOOTMENU_H__

#include "UIElement.h"
#include "ctItems.h"

enum EquipType;
enum EntityType;



class LootMenu:public ctModule 
{
private:
	SDL_Texture * background_texture = nullptr;
	SDL_Texture* equip_texture = nullptr;

	UIElement* background = nullptr;
	std::vector<UIElement*> cleric_statistics;
	std::vector<UIElement*> warrior_statistics;
	std::vector<UIElement*> dwarf_statistics;
	std::vector<UIElement*> elf_statistics;

	std::vector<UIElement*> information_dropped_items;
	
	UIElement* to_cleric_label = nullptr;
	UIElement* to_dwarf_label = nullptr;
	UIElement* to_warrior_label = nullptr;
	UIElement* to_elf_label = nullptr;
	UIElement* for_no_one_label = nullptr;

	UIElement* arrow = nullptr;

	std::vector<UIElement*> main_labels;

	std::vector<Item*> inventory_items;

	std::vector<UIElement*> information_inventory_items;
	std::vector<UIElement*> information_usable_items;

	
	std::vector<UIElement*> preview_stats_items;

	int update_count = 0;
public:
	LootMenu();
	~LootMenu();

	void SavedataToXML();
	bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();
	
	void Draw();
	void DrawItems();
	void LoadClerictStats();
	void LoadWarriorStats();
	void LoadDwarfStats();
	void LoadElfStats();
	void NavigateDown(std::vector<UIElement*> &current_vector);
	void NavigateUp(std::vector<UIElement*> &current_vector);
	void ExecuteComand(std::vector<UIElement*> &current_vector);

	void SetInformationDroppedItem();

	void LoadEquipableObjects();

	void SetInformationLabels();

	void SetComparation();

	
	void SetInformationUsableItems();

	EquipType equip_type;

	std::vector<Item*> dropped_items;
	std::vector<Item*> dropped_usable_items;
};

#endif //__LOOTMENU_H__
