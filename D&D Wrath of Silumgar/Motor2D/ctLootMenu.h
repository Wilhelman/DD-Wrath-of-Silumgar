#ifndef __LOOTMENU_H__
#define __LOOTMENU_H__

#include "UIElement.h"
#include "ctItems.h"

enum EquipType;
enum EntityType;



class LootMenu:public ctModule 
{
private:

	SDL_Texture * equip_texture;

	UIElement* background = nullptr;
	std::vector<UIElement*> cleric_statistics;
	std::vector<UIElement*> warrior_statistics;
	std::vector<UIElement*> dwarf_statistics;
	std::vector<UIElement*> elf_statistics;
	UIElement* cleric_label = nullptr;
	UIElement* dwarf_label = nullptr;
	UIElement* warrior_label = nullptr;
	UIElement* elf_label = nullptr;
	

	std::vector<UIElement*> main_labels;
	std::vector<Item*> inventory_items;

	std::vector<UIElement*> information_inventory_items;
	UIElement* arrow = nullptr;

public:
	LootMenu();
	~LootMenu();


	bool Start();

	// Called before all Updates
	//bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	//bool PostUpdate();

	// Called before quitting
	bool CleanUp();
	
	void Draw(SDL_Texture* sprites);
	void DrawItems();
	void LoadClerictStats();
	void LoadWarriorStats();
	void LoadDwarfStats();
	void LoadElfStats();
	void NavigateDown(std::vector<UIElement*> &current_vector);
	void NavigateUp(std::vector<UIElement*> &current_vector);
	void ExecuteComand(std::vector<UIElement*> &current_vector);

	void SetUpInventory();

	void LoadEquipableObjects();

	void SetInformationLabels();


	EquipType equip_type;
};

#endif //__LOOTMENU_H__
