#ifndef __UIPAUSEMENU_H__
#define __UIPAUSEMENU_H__

#include "UIElement.h"

class UIPauseMenu : public UIElement
{
private:

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
	UIElement* arrow = nullptr;


public:
	UIPauseMenu(int x, int y, UI_Type type, ctModule* callback, UIElement* parent = nullptr);
	~UIPauseMenu();
	void Update();
	void Draw(SDL_Texture* sprites);
	void LoadClerictStats();
	void LoadWarriorStats();
	void LoadDwarfStats();
	void LoadElfStats();
	void NavigateDown(std::vector<UIElement*> &current_vector);
	void NavigateUp(std::vector<UIElement*> &current_vector);
};

#endif //__UIPAUSEMENU_H__