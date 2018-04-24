#ifndef __UIDECISION_H__
#define __UIDECISION_H__

//#include "j1Fonts.h"
#include "ctGui.h"
#include "UIElement.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "ctTextures.h"



class UIDecision : public UIElement
{
public:

	UIDecision(int x, int y, UI_Type type, ctModule* callback, UIElement* parent);
	~UIDecision();
	void Draw(SDL_Texture* sprites);
	void NavigateDown(std::vector<UIElement*> &current_vector);
	void NavigateUp(std::vector<UIElement*> &current_vector);
	void Update();


private:

	UIElement* decision_explanation = nullptr;
	std::vector<UIElement*> options;


	UIElement* arrow = nullptr;
	UIElement* background = nullptr;



};

#endif //Questions to Ricardo