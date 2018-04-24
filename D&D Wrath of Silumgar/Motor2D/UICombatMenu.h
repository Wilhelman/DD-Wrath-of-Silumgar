#ifndef __UICOMBATMENU_H__
#define __UICOMBATMENU_H__

//¿Esto no sobra?
#include "ctGui.h"
//--------------
#include "UIElement.h"

#define ATTACKEXPLANATION "Use a normal attack that has no mana cost"
#define ABILITIESEXPLANATION "Open the abilities list"
#define ITEMSEXPLANATION "Open your inventory"

struct Entity;
struct Action;
struct 	ctPerfTimer;
//Como detallito podriamos poner que se mostrara una previsualización del daño base en el ataque y la perdida de mana
//------------------------------------------------------------------------------------------------------------------
class UICombatMenu : public UIElement
{
public:
	//¿Puntero hacía una entidad? no habra manera mas fàcil?
	Entity* entity = nullptr;
	//-----------------------------------------------------

	UIElement* background = nullptr;
	UIElement* arrow = nullptr;
	UIElement* enemy_select_arrow = nullptr;
	UIElement* attack_label = nullptr;
	UIElement* abilities_label = nullptr;
	UIElement* items_label = nullptr;
	//Ni puta idea 
	UIElement* upper_points = nullptr;
	UIElement* lower_points = nullptr;
	//-------------

	//Esto y todo lo que es un puto texto y una imagen de fondo no podemos hacer un TextBox?
	//Y en vez de pasarle solo un texto pasarle un vector de opciones (como UIDecision)
	UIElement* explanation_background = nullptr;
	UIElement* explanation_label = nullptr;
	//----------------------------------------------------------------------------------

	//Mas de ni puta idea
	iPoint upper_points_pos = { 0,0 };
	iPoint lower_points_pos = { 0,0 };
	//-------------------

	//Tiene que haber otra manera porque encima aqui esta hecho el navigation cuando estaba en UIElement
	std::vector<UIElement*> main_labels;
	std::vector<UIElement*> abilities;
	std::vector<UIElement*> items;
	std::vector<std::string> names;
	std::vector<Action> entity_actions;
	//---------------------------------------------------------------------------------------------------

	int names_iterator = 0;

	//¿¿¿¿¿¿¿¿¿¿?????????
	iPoint label1_pos{ 10,20 };
	iPoint label2_pos{ 10,40 };
	iPoint label3_pos{ 10,60 };

	iPoint main_label1_pos{ 10,10 };
	iPoint main_label2_pos{ 10,40 };
	iPoint main_label3_pos{ 10,70 };
	//-------------------

	iPoint explanationPos{ 129,31 };

	int font_size = 12;

	uint combat_menu_move_fx = 0;
	uint combat_menu_select_fx = 0;
	uint combat_menu_back_fx = 0;

	//Calcular la lógica de los enemigos no se hace en la UI
	//Hay que pasarlo al combat aqui 0 lógica
	bool selecting_enemy = false;
	bool executed_command = false;
	//------------------------------------------------------

public:

	UICombatMenu(Entity* entity, int x, int y, UI_Type type, ctModule* callback, UIElement* parent = nullptr);
	virtual ~UICombatMenu();


	void Update();

	void NavigateDown(std::vector<UIElement*> &current_vector);
	void NavigateUp(std::vector<UIElement*> &current_vector);
	void ExecuteComand(std::vector<UIElement*> &current_vector);

	void LoadAbilities();
	void LoadItems();

	void GoBack();

	void SelectEnemy(std::vector<UIElement*> &current_vector);
	void SelectAlly(std::vector<UIElement*> &current_vector);

	void ChangeExplanation(std::vector<UIElement*> &current_vector);

private:
	//No se podría hacer unicamente buscando en la lista de entidades, además esto no va puto aquí.
	std::vector<Entity*>::const_iterator selected_enemy;
	std::vector<Entity*>::const_iterator selected_ally;
	//--------------------------------------------------------------------------------------------

	ctPerfTimer execute_comand_time;
};

#endif //__UICOMBATMENU_H__