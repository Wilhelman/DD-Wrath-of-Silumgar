#ifndef __CTFONTS_H__
#define __CTFONTS_H__

#include "ctModule.h"
#include "SDL\include\SDL_pixels.h"

#define DEFAULT_FONT "Fonts/8bitlim.ttf"
#define Second_Font "Fonts/FritzQuadrata Regular.ttf"
#define DEFAULT_FONT_SIZE 15

struct SDL_Texture;
struct _TTF_Font;

class ctFonts : public ctModule
{
public:

	ctFonts();

	// Destructor
	virtual ~ctFonts();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before quitting
	bool CleanUp();

	// Load Font
	_TTF_Font* const Load(const char* path, int size = 12);

	// Create a surface from text
	SDL_Texture* Print(const char* text, SDL_Color color = { 255, 255, 255, 255 }, _TTF_Font* font = NULL);

	bool CalcSize(const char* text, int& width, int& height, _TTF_Font* font = NULL) const;

	SDL_Texture* PrintTextBox(const char* text, SDL_Color color, _TTF_Font* font, Uint32 box_width, int size);

public:

	std::list<_TTF_Font*>	fonts;
	_TTF_Font*			default;
	const char* path;
	int size = DEFAULT_FONT_SIZE;
};


#endif // __j1FONTS_H__