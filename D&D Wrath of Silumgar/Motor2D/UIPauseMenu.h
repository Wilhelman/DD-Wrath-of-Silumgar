#ifndef __UIPAUSEMENU_H__
#define __UIPAUSEMENU_H__

#include "UIElement.h"
#include "ctItems.h"

enum EquipType;
enum EntityType;

enum PauseMenuState
{
	Pause_Menu,
	Inventory_Menu
};

class UIPauseMenu : public UIElement
{
private:

	SDL_Texture * equip_texture;

	UIElement* background = nullptr;
	std::vector<UIElement*> cleric_statistics;
	std::vector<UIElement*> warrior_statistics;
	std::vector<UIElement*> dwarf_statistics;
	std::vector<UIElement*> elf_statistics;
	UIElement* continue_label = nullptr;
	UIElement* inventory_label = nullptr;
	UIElement* abilities_label = nullptr;
	UIElement* settings_label = nullptr;
	UIElement* quit_label = nullptr;
	std::vector<UIElement*> main_labels;
	std::vector<Item*> inventory_items;
	UIElement* arrow = nullptr;

	SDL_Rect cleric_helmet_rect = {0,137,25,24};
	SDL_Rect cleric_chest_rect = { 26,137,26,24 };
	SDL_Rect cleric_boots_rect = { 53,137,24,24 };
	SDL_Rect cleric_arms_rect = { 78,137,24,24 };
	SDL_Rect cleric_shield_rect = { 103,137,24,24 };
	SDL_Rect cleric_weapon_rect = { 128,137,26,24 };
	SDL_Rect cleric_hands_rect = { 155,137,24,24 };
	SDL_Rect cleric_accessories_rect = { 180,137,24,24 };

	SDL_Rect dwarf_helmet_rect = { 205,137,25,24 };
	SDL_Rect dwarf_chest_rect = { 231,137,26,24 };
	SDL_Rect dwarf_boots_rect = { 257,137,24,24 };
	SDL_Rect dwarf_arms_rect = { 283,137,24,24 };
	SDL_Rect dwarf_shield_rect = { 308,137,24,24 };
	SDL_Rect dwarf_weapon_rect = { 333,137,26,24 };
	SDL_Rect dwarf_hands_rect = { 360,137,24,24 };
	SDL_Rect dwarf_accessories_rect = { 385,137,24,24 };

	SDL_Rect warrior_helmet_rect = { 0,163,25,24 };
	SDL_Rect warrior_chest_rect = { 26,163,26,24 };
	SDL_Rect warrior_boots_rect = { 53,163,24,24 };
	SDL_Rect warrior_arms_rect = { 78,163,24,24 };
	SDL_Rect warrior_shield_rect = { 103,163,24,24 };
	SDL_Rect warrior_weapon_rect = { 128,163,26,24 };
	SDL_Rect warrior_hands_rect = { 155,163,24,24 };
	SDL_Rect warrior_accessories_rect = { 180,163,24,24 };

	SDL_Rect elf_helmet_rect = { 205,163,25,24 };
	SDL_Rect elf_chest_rect = { 231,163,26,24 };
	SDL_Rect elf_boots_rect = { 257,163,24,24 };
	SDL_Rect elf_arms_rect = { 283,163,24,24 };
	SDL_Rect elf_shield_rect = { 308,163,24,24 };
	SDL_Rect elf_weapon_rect = { 333,163,26,24 };
	SDL_Rect elf_hands_rect = { 360,163,24,24 };
	SDL_Rect elf_accessories_rect = { 385,163,24,24 };

	//test
	uint fake_arrow=0;
	iPoint position_fake_arrow={-100,-100};
public:
	UIPauseMenu(int x, int y, UI_Type type, ctModule* callback, UIElement* parent = nullptr);
	~UIPauseMenu();
	void Update();
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
	iPoint SetPositionFakeArrow();
	EquipType equip_type;
};

#endif //__UIPAUSEMENU_H__