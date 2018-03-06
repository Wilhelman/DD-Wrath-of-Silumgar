#ifndef __ctWINDOW_H__
#define __ctWINDOW_H__

#include "ctModule.h"
#include <string>

struct SDL_Window;
struct SDL_Surface;

class ctWindow : public ctModule
{
public:

	ctWindow();

	// Destructor
	virtual ~ctWindow();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* new_title);

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	uint GetScale() const;

public:
	//The window we'll be rendering to
	SDL_Window* window = nullptr;

	//The surface contained by the window
	SDL_Surface* screen_surface = nullptr;

	SDL_Surface* iconExe = nullptr;

private:
	std::string	title;
	uint		width = 0u;
	uint		height = 0u;
	uint		scale = 0u;
};

#endif // __ctWINDOW_H__