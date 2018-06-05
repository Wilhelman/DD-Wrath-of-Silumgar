#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctInput.h"
#include "ctTextures.h"
#include "ctAudio.h"
#include "ctRender.h"
#include "ctWindow.h"
#include "j1Map.h"
#include "ctVideoIntro.h"
#include "j1Video.h"
#include "ctFadeToBlack.h"
#include "ctMainMenu.h"
#include "SDL_mixer\include\SDL_mixer.h"

ctVideoIntro::ctVideoIntro() : ctModule()
{
	name = "video_intro";
}

// Destructor
ctVideoIntro::~ctVideoIntro()
{}

// Called before render is available
bool ctVideoIntro::Awake()
{
	LOG("Loading Intro Video");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool ctVideoIntro::Start()
{
	//App->win->SetTitle("Video Player");
	// TODO 1: Call the initialize function from the video module in the start of the scene.
	App->video_manager->Initialize("video/TriggeredSquidIntro.avi");
	// TODO 8: Play the music of the video using the audio module. 
	//App->audio->PlayMusic("video/IntroAudio.ogg", 0.0f);

	is_active = true;

	return true;
}

// Called each loop iteration
bool ctVideoIntro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ctVideoIntro::Update(float dt)
{
	App->render->DrawQuad({0,0,App->render->camera.w,App->render->camera.h},13,97,191,255,true);
	if (App->video_manager->isVideoFinished == true) {
	//	App->audio->PauseMusic();
		App->fadeToBlack->FadeToBlackBetweenModules(this, App->main_menu);
	}

	else if (App->video_manager->isVideoFinished == false)
	{
		App->video_manager->GrabAVIFrame();

	}
	//else
		//Mix_PauseMusic();

	if (App->input->GetKey(SDL_SCANCODE_1) && App->video_manager->isVideoFinished) {
		App->video_manager->Initialize("video/TriggeredSquidIntro.avi");
		//App->audio->PlayMusic("video/IntroAudio.ogg", 0.0f);
	}


	return true;
}

// Called each loop iteration
bool ctVideoIntro::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool ctVideoIntro::CleanUp()
{
	if (is_active == true) {
		App->video_manager->CloseAVI();
		is_active = false;
	}
	return true;
}
