#include "ctApp.h"
#include "UIDecision.h"
//#include <algorithm>
#include "UITextBox.h"
#include "UIImage.h"

//TODO change rect according to decision_number or do an enum or something
UIDecision::UIDecision(int x, int y, UI_Type type, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	background = new UIImage(x, y, IMAGE, { 843,484,264,280 }, nullptr);
	

	decision_explanation = new UITextBox(50, 53, TEXTBOX, "pene", {255,255,255}, 15, 428, Second_Font, nullptr);
	options.push_back(new UITextBox(50, 73, TEXTBOX, "erecto", { 255,255,255 }, 15, 428, Second_Font, nullptr));
	options.push_back(new UITextBox(50, 93, TEXTBOX, "flacido", { 255,255,255 }, 15, 428, Second_Font, nullptr));

	arrow = new UIImage(-20, 0, IMAGE, { 1333, 272, 7, 14 }, nullptr, options.front());

	decision_explanation->screen_position.x = background->screen_position.x - decision_explanation->local_position.x;

	arrow->screen_position.x = background->screen_position.x - arrow->local_position.x;


	std::vector<UIElement*>::const_iterator it = options.begin();
	while (it != options.end()) {
		(*it)->screen_position.x = background->screen_position.x - (*it)->local_position.x;
		it++;
	}


}

void UIDecision::Draw(SDL_Texture* sprites)
{
	background->Draw(sprites);
	
	std::vector<UIElement*>::const_iterator it = options.begin();
	while (it != options.end()) {
		(*it)->Draw(sprites);
		it++;
	}

	decision_explanation->Draw(sprites);

	arrow->Draw(sprites);


}

UIDecision::~UIDecision() {
	App->gui->DeleteUIElement(*background);
	background = nullptr;

	std::vector<UIElement*>::const_iterator it = options.begin();
	while (it != options.end()) {
		(*it)->to_destroy=true;
		it++;
	}

	options.clear();

	App->gui->DeleteUIElement(*decision_explanation);
	decision_explanation = nullptr;
	
	App->gui->DeleteUIElement(*arrow);
	arrow = nullptr;
}