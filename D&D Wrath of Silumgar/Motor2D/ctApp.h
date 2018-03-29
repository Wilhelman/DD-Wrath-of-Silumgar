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
class j1Map;
class ctMainMenu;
class ctWorldMap;
class ctSettings;
class ctCombat;
class ctEntities;
class ctGui;
class ctFonts;
class ctTaskManager;
class ctFadeToBlack;
class ctSkillTree;

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

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Load entities file
	pugi::xml_node LoadEntities(pugi::xml_document&) const;

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

public:

	// Modules
	ctWindow*			win = nullptr;
	ctInput*			input = nullptr;
	ctRender*			render = nullptr;
	ctTextures*			tex = nullptr;
	ctAudio*			audio = nullptr;
	j1Map*				map = nullptr;
	ctMainMenu*			main_menu = nullptr;
	ctWorldMap*			world_map = nullptr;
	ctSettings*         settings = nullptr;
	ctCombat*			combat = nullptr;
	ctEntities*			entities = nullptr;
	ctGui*				gui = nullptr;
	ctFonts*			fonts = nullptr;
	ctTaskManager*		task_manager = nullptr;
	ctFadeToBlack*		fadeToBlack = nullptr;
	ctSkillTree*		skill_tree = nullptr;

	bool				cap_state = false;

private:

	std::list<ctModule*>	modules;
	int					argc = 0;
	char**				args = nullptr;

	std::string			game_title;
	std::string				organization;
	std::string				vsync_to_show;
	std::string				cap_to_show;

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