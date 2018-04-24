#ifndef __UIIMAGE_H__
#define __UIIMAGE_H__

#include "UIElement.h"

class UIImage : public UIElement
{
private:

public:
	UIImage(int x, int y, UI_Type type, SDL_Rect rect, ctModule* callback, UIElement* parent = nullptr);

	void UpdateImageWithCoords(SDL_Rect rect);
};

#endif //__UIIMAGE_H__