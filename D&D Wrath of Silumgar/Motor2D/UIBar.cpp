#include "ctApp.h"
#include "UIBar.h"
#include "ctLog.h"
#include "ctInput.h"


UIBar::UIBar(int x, int y, int max_capacity ,UI_Type type, ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;
	

}

void UIBar::Update()
{
	

}