#include "ctApp.h"
#include "UIVerticalSliceInfo.h"
#include "ctLog.h"
#include "ctInput.h"
#include "ctAudio.h"

UIVerticalSliceInfo:: UIVerticalSliceInfo(int x, int y,bool overcome,ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;
	
	background = App->gui->AddUIImage(x, y, { 1141,484,340,178 },callback);
	if (overcome == true)
	{
		congratulations = App->gui->AddUITextBox(x+20, y+20, 15, 200, "CONGRATULATIONS!", {255,255,255,255},this, Second_Font);
		text = App->gui->AddUITextBox(x+20,y+50, 12,320,"You have managed to overcome the first part of this adventure.  We hope you've enjoyed this gameplay. More adventures, objects and abilities await you in the final version of the game. See you soon, hero!\n\nAtt: Triggered Squid.", { 255,255,255,255 }, this, Second_Font);
	}
	else if (overcome == false)
	{
		congratulations = App->gui->AddUITextBox(x + 20, y + 20, 15, 200, "All hope is lost ... or maybe ... not ...", { 255,255,255,255 }, this, Second_Font);
		text = App->gui->AddUITextBox(x + 20, y + 50, 12, 320, "Your efforts have not been enough to defeat Lich's troops. Try again, the kingdom of Allarad needs heroes like you!", { 255,255,255,255 }, this, Second_Font);
	}

	
}

UIVerticalSliceInfo::~UIVerticalSliceInfo() {
	App->gui->DeleteUIElement(*background);
	background = nullptr;
	App->gui->DeleteUIElement(*congratulations);
	congratulations = nullptr;
	App->gui->DeleteUIElement(*text);
	text = nullptr;
	callback = nullptr;
	
}

