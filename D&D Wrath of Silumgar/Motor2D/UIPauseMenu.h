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


public:
	UIPauseMenu(int x, int y, UI_Type type, ctModule* callback, UIElement* parent = nullptr);
	~UIPauseMenu();
	void Draw(SDL_Texture* sprites);
	void LoadClerictStats();
	void LoadWarriorStats();
	void LoadDwarfStats();
	void LoadElfStats();
};

#endif //__UIPAUSEMENU_H__