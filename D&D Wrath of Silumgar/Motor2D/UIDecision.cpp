#include "ctApp.h"
#include "UIDecision.h"
//#include <algorithm>
#include "UITextBox.h"
#include "UIImage.h"

//TODO change rect according to decision_number or do an enum or something
UIDecision::UIDecision(int x, int y, UI_Type type, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	background = new UIImage(x, y, IMAGE, { 843,484,264,280 }, nullptr);
	

	decision_explanation = new UITextBox(30,20, TEXTBOX, "pene", {255,255,255}, 15, 428, Second_Font, background);

	iPoint options_space = { 30,(background->GetRect().h - decision_explanation->GetRect().h) / 3 };

	options.push_back(new UITextBox(options_space.x, options_space.y, TEXTBOX, "erecto", { 255,255,255 }, 15, 428, Second_Font, background));
	options.push_back(new UITextBox(options_space.x, options_space.y*2, TEXTBOX, "flacido", { 255,255,255 }, 15, 428, Second_Font, background));

	arrow = new UIImage(-20, 0, IMAGE, { 1333, 272, 10, 14 }, nullptr, options.front());

	options.front()->current_state = STATE_FOCUSED;
}

void UIDecision::Update() {
	
	background->Update();
	arrow->Update();
	decision_explanation->Update();
	
	std::vector<UIElement*>::const_iterator it = options.begin();
	while (it != options.end()) {
		(*it)->Update();
		it++;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->gamepad.CROSS_DOWN == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		NavigateDown(options);
	}


	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->gamepad.CROSS_UP == GAMEPAD_STATE::PAD_BUTTON_DOWN) {
		NavigateUp(options);
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
		arrow->GetParent()->current_state = STATE_EXECUTED;
	}

	if (callback != nullptr)
		callback->OnUITrigger(this, current_state);
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

void UIDecision::NavigateDown(std::vector<UIElement*> &current_vector) {
	std::vector<UIElement*>::const_iterator it_vector = current_vector.begin();
	while (it_vector != current_vector.end()) {
		if ((*it_vector)->current_state == STATE_FOCUSED) {
			if ((*it_vector) != current_vector.back()) {
				(*it_vector)->current_state = STATE_NORMAL;
				it_vector++;
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
				break;
			}
			else
			{

				(*it_vector)->current_state = STATE_NORMAL;
				it_vector = current_vector.begin();
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
			}
		}
		it_vector++;
	}
}

void UIDecision::NavigateUp(std::vector<UIElement*> &current_vector) {
	std::vector<UIElement*>::const_iterator it_vector = current_vector.begin();
	while (it_vector != current_vector.end()) {
		if ((*it_vector)->current_state == STATE_FOCUSED) {
			if ((*it_vector) != current_vector.front()) {
				(*it_vector)->current_state = STATE_NORMAL;
				it_vector--;
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
				break;
			}
			else
			{
				(*it_vector)->current_state = STATE_NORMAL;
				it_vector = current_vector.end() - 1;
				(*it_vector)->current_state = STATE_FOCUSED;
				arrow->SetParent((*it_vector));
			}
		}
		it_vector++;
	}
}