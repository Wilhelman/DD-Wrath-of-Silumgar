#include "ctApp.h"
#include "UIVerticalSliceInfo.h"
#include "ctLog.h"
#include "ctInput.h"
#include "ctAudio.h"

UIVerticalSliceInfo:: UIVerticalSliceInfo(int x, int y,bool overcome,ctModule* callback, UIElement* parent) : UIElement(x, y, type, parent)
{
	this->callback = callback;

	
}

UIVerticalSliceInfo::~UIVerticalSliceInfo() {

	
}

void UIVerticalSliceInfo::Update()
{


}