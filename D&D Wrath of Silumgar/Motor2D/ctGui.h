#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "ctModule.h"
#include <vector>
#include <string>
#include "ctFonts.h"
#include "ctEntities.h"

#define STUN_COORDS {128,1204,32,32}
#define BLEED_COORDS {32,1204,32,32}
#define POSION_COORDS {96,1204,32,32}
#define BURN_COORDS {64,1204,32,32}
#define DEX_1_COORDS {96,1140,32,32}
#define DEX_2_COORDS {128,1140,32,32}

struct SDL_Texture;
struct SDL_Rect;
class UIElement;
struct SDL_Color;
class Entity;
struct WorldMapElement;

enum UI_Type
{
	LABEL,
	BUTTON,
	IMAGE,
	COMBATMENU,
	TEXTBOX,
	DIALOGBOX,
	LIFEBAR,
	MANABAR,
	ENEMYLIFEBAR,
	DECISION,
	FLOATING_VALUE,
	LEVELUPINFO,
	VERTICALSLICEINFO,
	PAUSEMENU,

	UI_NOT_DEF
};

// ---------------------------------------------------
class ctGui : public ctModule
{

public:

	ctGui();

	// Destructor
	virtual ~ctGui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Gui creation functions

	const SDL_Texture* GetAtlas() const;

	bool DeleteUIElement(UIElement &element);
	bool DeleteAllUIElements();

	UIElement* AddUIImage(int position_x, int position_y, SDL_Rect rect, ctModule* callback = nullptr, UIElement* parent = nullptr);
	UIElement* AddUIButton(int position_x, int position_y, SDL_Rect normal_rect, SDL_Rect focused_rect, SDL_Rect pressed_rect, ctModule* callback = nullptr, UIElement* parent = nullptr);
	UIElement* AddUILabel(int position_x, int position_y, std::string text, SDL_Color color, int size, ctModule* callback = nullptr, UIElement* parent = nullptr, const char* path = DEFAULT_FONT, bool UIblit = false);
	UIElement* AddUICombatMenu(Entity* entity, int position_x, int position_y, ctModule* callback = nullptr, UIElement* parent = nullptr);
	UIElement* AddUITextBox(int position_x, int position_y, int size, int box_width, std::string text, SDL_Color color, UIElement* parent = nullptr, const char* path = DEFAULT_FONT);
	UIElement* AddUIDialogBox(int position_x, int position_y, int size, int box_width, std::string text, SDL_Color color, UIElement* parent = nullptr, const char* path = DEFAULT_FONT);
	UIElement* AddUIBar(int position_x, int position_y, int max_capacity, UI_Type barType, Entity* entity, ctModule* callback = nullptr, UIElement* parent = nullptr);
	UIElement* AddUIDecision(int position_x, int position_y, UI_Type type, UIElement* &arrow, std::string decision, WorldMapElement &option_1, WorldMapElement &option_2, ctModule* callback, UIElement* parent = nullptr);
	UIElement* AddUIFloatingValue(int position_x, int position_y, std::string text, SDL_Color color, int size, ctModule* callback = nullptr, UIElement* parent = nullptr, const char* path = DEFAULT_FONT);
	UIElement* AddUILevelUpInfo(int position_x, int position_y, EntityType hero, ctModule* callback = nullptr, UIElement* parent = nullptr);
	UIElement* AddUIVerticalSliceInfo(int position_x, int position_y, bool overcome , ctModule* callback = nullptr, UIElement* parent = nullptr);
	UIElement* AddUIPauseMenu(int position_x, int position_y, ctModule* callback = nullptr, UIElement* parent = nullptr);
	UIElement* GetElementUnderMouse(int x, int y);

private:


	std::vector<UIElement*> ui_elements;

	//SDL_Texture* atlas;
	//SDL_Texture* pause_menu_image;
	std::string atlas_file_name;
	std::string pause_menu_image_file_name;
public:
	SDL_Texture* atlas;
	SDL_Texture* pause_menu_image;
};

#endif // __j1GUI_H__