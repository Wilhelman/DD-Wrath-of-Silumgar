#include <iostream> 

#include "ctApp.h"
#include "ctDefs.h"
#include "ctLog.h"

#include "ctWindow.h"
#include "ctInput.h"
#include "ctRender.h"
#include "ctTextures.h"
#include "ctAudio.h"
#include "ctMainMenu.h"
#include "ctEntities.h"
#include "ctGui.h"
#include "ctFadeToBlack.h"
/*
#include "j1Scene.h"
#include "j1MainMenu.h"
#include "j1Collider.h"
#include "j1Credits.h"
#include "j1Map.h"
#include "j1Entities.h"
#include "j1Particles.h"
#include "j1Fonts.h"
#include "j1Language.h"
#include "j1UI.h"
#include "j1Pathfinding.h"
#include "j1FadeToBlack.h"
#include "Player.h"
*/

// Constructor
ctApp::ctApp(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	want_to_save = want_to_load = all_modules_loaded = false;
	load_game = "save_game";
	save_game = "save_game";

	input = new ctInput();
	win = new ctWindow();
	render = new ctRender();
	tex = new ctTextures();
	audio = new ctAudio();
	main_menu = new ctMainMenu();
	entities = new ctEntities();
	gui = new ctGui();
	fadeToBlack = new ctFadeToBlack();
	/*scene = new j1Scene();
	main_menu = new j1MainMenu();
	credits = new j1Credits();
	collider = new j1Collider();
	map = new j1Map();
	languages = new j1Language();
	entities = new j1Entities();
	particles = new j1Particles();
	font = new j1Fonts();
	ui = new j1UI();
	pathfinding = new j1PathFinding();
	fadeToBlack = new j1FadeToBlack();*/

	//TODO : CHECK ALL TODO'S!!!

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(main_menu);
	AddModule(entities);
	AddModule(gui);
	AddModule(fadeToBlack);
	
	/*AddModule(map);
	AddModule(pathfinding);
	AddModule(scene);
	AddModule(languages);
	AddModule(main_menu);
	AddModule(credits);
	AddModule(entities);
	AddModule(particles);
	AddModule(font);
	AddModule(ui);
	AddModule(collider);
	AddModule(fadeToBlack);*/
	

	// render last to swap buffer
	AddModule(render);

	//disable lvl scene
	/*scene->active = false;
	credits->active = false;*/

	PERF_PEEK(ptimer);
}

// Destructor
ctApp::~ctApp()
{
	// release modules
	
	for (std::list<ctModule*>::iterator it = modules.begin(); it != modules.end(); ++it)
	{
		RELEASE(*it);
	}

	modules.clear();
}

void ctApp::AddModule(ctModule* module)
{
	module->Init();
	modules.push_back(module);
}

// Called before render is available
bool ctApp::Awake()
{
	PERF_START(ptimer);

	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;
	cap_state = true;

	config = LoadConfig(config_file);


	if (config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		game_title = app_config.child("title").child_value();
		organization =app_config.child("organization").child_value();

		cap = app_config.attribute("framerate_cap").as_uint();

		if (cap > 0)
			capped_ms = 1000 / cap;

	}

	if (ret == true)
	{

		std::list<ctModule*>::iterator it = modules.begin();

		while (it != modules.end() && ret == true)
		{
			ret = (*it)->Awake(config.child((*it)->name.data()));
			
			it++;
		}
	}

	PERF_PEEK(ptimer);

	return ret;
}

// Called before the first frame
bool ctApp::Start()
{
	PERF_START(ptimer);

	bool ret = true;

	std::list<ctModule*>::iterator it = modules.begin();
	
	while (it != modules.end() && ret == true)
	{
		if ((*it)->active == false) {
			it++;
			continue;
		}
		ret = (*it)->Start();
		it++;
	}
	startup_time.Start();

	PERF_PEEK(ptimer);

	return ret;
}

// Called each loop iteration
bool ctApp::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// ---------------------------------------------
pugi::xml_node ctApp::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if (result == NULL)
		LOG("Could not load xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");
	return ret;
}

// ---------------------------------------------
pugi::xml_node ctApp::LoadLanguages(pugi::xml_document& language_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = language_file.load_file("languages");

	if (result == NULL)
		LOG("Could not load xml file config.xml. pugi error: %s", result.description());
	else
		ret = language_file.child("languages");
	return ret;
}

// ---------------------------------------------
bool ctApp::LoadSave(pugi::xml_document& save_file) const
{
	bool ret = false;
	pugi::xml_parse_result result = save_file.load_file("save_game");

	if (result == NULL)
		LOG("Could not load xml file config.xml. pugi error: %s", result.description());
	else
		ret = true;
	return ret;
}

// ---------------------------------------------
void ctApp::PrepareUpdate()
{

	perf_timer.Start();

}

// ---------------------------------------------
void ctApp::FinishUpdate()
{

	frame_count++;

	if (want_to_save == true)
		SavegameNow();

	if (want_to_load == true)
		LoadGameNow();

	// Framerate calculations --


	if (App->render->vsync_state)
		vsync_to_show = "on";
	else
		vsync_to_show = "off";

	if (cap_state)
		cap_to_show = "on";
	else
		cap_to_show = "off";

	float avg_fps = float(frame_count) / startup_time.ReadSec();

	float seconds_since_startup = simple_timer.Read();

	uint32 current_ms_frame = perf_timer.ReadMs();
	uint32 last_frame_ms = current_ms_frame;
	uint32 frames_on_last_update = 0;


	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		cap_state = !cap_state;

	if (cap_state) {
		capped_ms = 1000 / cap;

		if (current_ms_frame < capped_ms && !App->render->vsync_state)
			SDL_Delay(capped_ms - current_ms_frame);
	}

	double framerate = 1000.0f / perf_timer.ReadMs();

	dt = 1.0f / framerate;
	
	std::string godMode;

	/*if (App->entities->GetPlayer() != nullptr) {
		(App->entities->GetPlayer()->god_mode) ?
			godMode.create("ON") :
			godMode.create("OFF");
	}
	else*/
		godMode="Player not reachable";

	static char title[256];
	sprintf_s(title, 256, "%s - FPS: %.2f Av.FPS: %.2f Last Frame Ms: %u (Cap: %s  Vsync: %s) | GOD MODE: %s",
		game_title.data(), framerate, avg_fps, last_frame_ms, cap_to_show.data(), vsync_to_show.data(), godMode.data());

	App->win->SetTitle(title);

	if (!all_modules_loaded)
		all_modules_loaded = true;
}

// Call modules before each loop iteration
bool ctApp::PreUpdate()
{
		bool ret = true;
	
	

	ctModule* pModule = NULL;

	for (std::list<ctModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		pModule = (*it);

		if (pModule->active == false) {
			continue;
		}

		ret = (*it)->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool ctApp::DoUpdate()
{
	bool ret = true;

	ctModule* pModule = NULL;

	for (std::list<ctModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		pModule = (*it);

		if (pModule->active == false) {
			continue;
		}

		(all_modules_loaded) ? ret = (*it)->Update(dt) : ret = (*it)->Update(0);
	}

	return ret;
}

// Call modules after each loop iteration
bool ctApp::PostUpdate()
{
	PERF_START(ptimer);
	bool ret = true;

	ctModule* pModule = NULL;

	for (std::list<ctModule*>::iterator it = modules.begin(); it != modules.end() && ret == true; it++)
	{
		pModule = (*it);

		if (pModule->active == false) {
			continue;
		}

		ret = (*it)->PostUpdate();
	}

	//PERF_PEEK(ptimer);
	return ret;
}

// Called before quitting
bool ctApp::CleanUp()
{
	bool ret = true;
	
	std::list<ctModule*>::reverse_iterator it = modules.rbegin();

	while (it != modules.rend() && ret == true)
	{
		ret = (*it)->CleanUp();
		it++;
	}

	return ret;
}

// ---------------------------------------
int ctApp::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* ctApp::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* ctApp::GetTitle() const
{
	return game_title.data();
}

// ---------------------------------------
const char* ctApp::GetOrganization() const
{
	return organization.data();
}

// Load / Save
void ctApp::LoadGame()
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
}

// ---------------------------------------
void ctApp::SaveGame() const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
}

// ---------------------------------------
void ctApp::GetSaveGames(/*ctList<ctSString>& list_to_fill*/) const
{
	// need to add functionality to file_system module for this to work
}

bool ctApp::LoadGameNow()
{
	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.data());

	if (result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.data());

		root = data.child("game_state");

		std::list<ctModule*>::iterator it = modules.begin();
		
		ret = true;

		while (it != modules.end() && ret == true)
		{
			ret = (*it)->Load(root.child((*it)->name.data()));
			it++;
		}

		data.reset();
		if (ret == true)
			LOG("...finished loading");
		else
			LOG("...loading process interrupted with error on module %s", (*it) ? (*it)->name.data() : "unknown");
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.data(), result.description());

	want_to_load = false;
	return ret;
}

bool ctApp::SavegameNow() const
{
	bool ret = true;

	LOG("Saving Game State to %s...", save_game.data());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;

	root = data.append_child("game_state");

	
	std::list<ctModule*>::const_iterator it = modules.begin();

	while (it != modules.end() && ret == true)
	{
		ret = (*it)->Save(root.append_child((*it)->name.data()));
		it++;
	}

	if (ret == true)
	{
		data.save_file(save_game.data());
		LOG("... finished saving", );
	}
	else
		LOG("Save process halted from an error in module %s", *(it) ? (*it)->name.data() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}