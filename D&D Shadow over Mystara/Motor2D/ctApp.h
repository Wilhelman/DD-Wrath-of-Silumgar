#ifndef __ctAPP_H__
#define __ctAPP_H__

#include <list>
#include <string>
#include "ctModule.h"
#include "ctPerfTimer.h"
#include "ctTimer.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class ctWindow;
class ctInput;
class ctRender;
class ctTextures;
class ctAudio;
class ctMainMenu;
class ctEntities;
class ctGui;
class ctFadeToBlack;
/*class j1Collider;
class j1Scene;
class j1MainMenu;
class j1Credits;
class j1Entities;
class j1Particles;
class j1Map;
class j1Language;
class j1PathFinding;
class j1Fonts;
class j1UI;
class j1FadeToBlack;*/

class ctApp
{
public:

	// Constructor
	ctApp(int argc, char* args[]);

	// Destructor
	virtual ~ctApp();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(ctModule* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	void GetSaveGames(/*ctList<ctSString>& list_to_fill*/) const;

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Load save file
	bool LoadSave(pugi::xml_document&) const;

	// Load languages file
	pugi::xml_node LoadLanguages(pugi::xml_document&) const;

private:

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	ctWindow*			win = nullptr;
	ctInput*			input = nullptr;
	ctRender*			render = nullptr;
	ctTextures*			tex = nullptr;
	ctAudio*			audio = nullptr;
	ctMainMenu*			main_menu = nullptr;
	ctEntities*			entities = nullptr;
	ctGui*				gui = nullptr;
	ctFadeToBlack*		fadeToBlack = nullptr;
	/*j1Collider*			collider = nullptr;
	j1Scene*			scene = nullptr;
	j1MainMenu*			main_menu = nullptr;
	j1Credits*			credits = nullptr;
	j1Map*				map = nullptr;
	j1PathFinding*		pathfinding = nullptr;
	j1Entities*			entities = nullptr;
	j1Language*			languages = nullptr;
	j1Particles*		particles = nullptr;
	j1Fonts*			font = nullptr;
	j1UI*				ui = nullptr;
	j1FadeToBlack*		fadeToBlack = nullptr;*/

	bool				cap_state = false;

private:

	std::list<ctModule*>	modules;
	int					argc = 0;
	char**				args = nullptr;

	std::string			game_title;
	std::string				organization;
	std::string				vsync_to_show;
	std::string				cap_to_show;

	mutable bool		want_to_save = false;
	bool				want_to_load = false;

	mutable std::string	save_game;
	std::string				load_game;

	ctPerfTimer			perf_timer;
	ctTimer				simple_timer;

	ctPerfTimer			ptimer;
	uint64				frame_count = 0;
	uint				cap = 0u;
	ctTimer				startup_time;
	float				dt = 0.0f;
	int					capped_ms = -1;
	bool				all_modules_loaded = false;

};

extern ctApp* App;

#endif