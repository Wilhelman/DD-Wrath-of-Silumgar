#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctRender.h"
#include "ctTextures.h"
//#include "ctFonts.h"
#include "ctInput.h"
#include "ctGui.h"

#include "UILabel.h"
#include "UIImage.h"
#include "UIButton.h"

#include "UIElement.h"

ctGui::ctGui() : ctModule()
{
	name = "gui";
}

// Destructor
ctGui::~ctGui()
{}

// Called before render is available
bool ctGui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool ctGui::Start()
{
	atlas = App->tex->Load(atlas_file_name.c_str());


	return true;
}

// Update all guis
bool ctGui::PreUpdate()
{
	for (int i = 0; i < ui_elements.capacity(); i++)
		if (ui_elements.at(i) != nullptr) ui_elements[i]->Update();

	return true;
}

// Called after all Updates
bool ctGui::PostUpdate()
{

	for (int i = 0; i < ui_elements.capacity(); i++)
		if (ui_elements.at(i) != nullptr) ui_elements[i]->Draw(atlas);

	for (int i = 0; i < ui_elements.capacity(); i++) {
		if (ui_elements[i]->to_destroy) {
			delete(ui_elements[i]);
			ui_elements[i] = nullptr;
			ui_elements.erase(ui_elements.cbegin() + i);
			ui_elements.shrink_to_fit();
		}
	}

	//LOG("NUM ELEM: %i", ui_elements.size());

	return true;
}

// Called before quitting
bool ctGui::CleanUp()
{
	LOG("Freeing GUI");
	//TODO unload tex

	for (uint i = 0; i < ui_elements.capacity(); ++i)
	{
		if (ui_elements[i] != nullptr)
		{
			delete ui_elements[i];
			ui_elements[i] = nullptr;
			ui_elements.erase(ui_elements.cbegin() + i);
			ui_elements.shrink_to_fit();
		}
	}

	ui_elements.clear();

	return true;
}

// const getter for atlas
const SDL_Texture* ctGui::GetAtlas() const
{
	return atlas;
}

bool ctGui::DeleteUIElement(UIElement &element) {

	for (int i = 0; i < ui_elements.capacity(); i++) {
		if (ui_elements.at(i) == &element) {
			ui_elements[i]->to_destroy = true;
			return true;
		}
	}

	return false;
}

bool ctGui::DeleteAllUIElements() {
	bool ret = false;

	for (int i = 0; i < ui_elements.capacity(); i++) {
		if (ui_elements.at(i) != nullptr) ui_elements[i]->to_destroy = true;
		ret = true;
	}

	return ret;
}

UIElement* ctGui::AddUIImage(int position_x, int position_y, SDL_Rect rect, ctModule* callback, UIElement* parent) {

	UIElement* tmp_img = new UIImage(position_x, position_y, IMAGE, rect, callback, parent);
	ui_elements.push_back(tmp_img);
	return tmp_img;

	LOG("Error: Cant add the UIImage");
	return nullptr;
}

UIElement* ctGui::AddUIButton(int position_x, int position_y, SDL_Rect normal_rect, SDL_Rect focused_rect, SDL_Rect pressed_rect, ctModule* callback, UIElement* parent) {

	UIElement* tmpBtn = new UIButton(position_x, position_y, BUTTON, normal_rect, focused_rect, pressed_rect, callback, parent);
	ui_elements.push_back(tmpBtn);
	return tmpBtn;

	LOG("Error: Cant add the UIButton");
	return nullptr;
}

UIElement* ctGui::AddUILabel(int position_x, int position_y, std::string text, Color color, UIElement* parent) {

	UIElement* tmp_lbl = new UILabel(position_x, position_y, LABEL, text, color, parent);
	ui_elements.push_back(tmp_lbl);
	return tmp_lbl;

	LOG("Error: Cant add the UILabel");
	return nullptr;
}

UIElement* ctGui::GetElementUnderMouse(int x, int y)
{
	for (int i = ui_elements.capacity() - 1; i >= 0; i--) {
		if (ui_elements[i] != nullptr)
		{
			if ((x > ui_elements[i]->GetScreenPosition().x && x < ui_elements[i]->GetScreenPosition().x + ui_elements[i]->GetRect().w) && (y > ui_elements[i]->GetScreenPosition().y && y < ui_elements[i]->GetScreenPosition().y + ui_elements[i]->GetRect().h))
			{
				UIElement* tmp_element_to_return = ui_elements[i];
				return tmp_element_to_return;
			}
		}
	}
	return nullptr;
}

// class Gui ---------------------------------------------------

