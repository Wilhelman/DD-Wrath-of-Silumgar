#include <iostream> 

#include "ctApp.h"
#include "ctDefs.h"
#include "ctLog.h"

#include "ctWindow.h"
#include "ctInput.h"
#include "ctRender.h"
#include "ctTextures.h"
#include "ctAudio.h"
#include "j1Map.h"
#include "ctMainMenu.h"
#include "ctSettings.h"
#include "ctWorldMap.h"
#include "ctSettings.h"
#include "ctCombat.h"
#include "ctEntities.h"
#include "ctGui.h"
#include "ctFonts.h"
#include "ctTaskManager.h"
#include "ctFadeToBlack.h"
#include "ctSkillTree.h"


// Constructor
ctApp::ctApp(int argc, char* args[]) : argc(argc), args(args)
{
	PERF_START(ptimer);

	input = new ctInput();
	win = new ctWindow();
	render = new ctRender();
	tex = new ctTextures();
	audio = new ctAudio();
	map = new j1Map();
	main_menu = new ctMainMenu();
	world_map = new ctWorldMap();
	settings = new ctSettings();
	entities = new ctEntities();
	combat = new ctCombat();
	gui = new ctGui();
	fonts = new ctFonts();
	task_manager = new ctTaskManager();
	fadeToBlack = new ctFadeToBlack();
	skill_tree = new ctSkillTree();
	
	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(main_menu);
	AddModule(world_map);
	AddModule(settings);
	AddModule(combat);
	AddModule(entities);
	AddModule(gui);
	AddModule(fonts);
	AddModule(task_manager);
	AddModule(fadeToBlack);
	AddModule(skill_tree);

	// render last to swap buffer
	AddModule(render);

	//disable modules here
	world_map->active = false;
	settings->active = false;
	combat->active = false;
	skill_tree->active = false;

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
pugi::xml_node ctApp::LoadEntities(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("entities.xml");

	if (result == NULL)
		LOG("Could not load xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("entities");
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


	static char title[256];
	sprintf_s(title, 256, "%s - FPS: %.2f Av.FPS: %.2f Last Frame Ms: %u (Cap: %s  Vsync: %s)",
		game_title.data(), framerate, avg_fps, last_frame_ms, cap_to_show.data(), vsync_to_show.data());

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