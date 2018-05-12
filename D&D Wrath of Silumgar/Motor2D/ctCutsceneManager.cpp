#include "ctCutsceneManager.h"
#include "ctApp.h"
#include "ctEntities.h"
#include "ctRender.h"
#include "ctInput.h"
#include "ctGui.h"



bool MoveX::Execute()
{
	bool ret = false;

	if (App->cutscene_manager->Cutscene_timer.Read() <= end_time) {
		actor->position.x += 5;
	}

	else {
		ret = true;
	}
	return ret;
}

bool MoveY::Execute()
{
	bool ret = false;

	if (App->cutscene_manager->Cutscene_timer.Read() <= end_time) {
		actor->position.y += 5;
	}

	else {
		ret = true;
	}
	return ret;
}

bool MoveCameraX::Execute()
{
	bool ret = false;

	if (App->cutscene_manager->Cutscene_timer.Read() <= end_time) {
		App->render->camera.x += 5;
	}

	else {
		ret = true;
	}
	return ret;
}

bool MoveCameraY::Execute()
{
	bool ret = false;

	if (App->cutscene_manager->Cutscene_timer.Read() <= end_time) {
		App->render->camera.y += 5;
	}

	else {
		ret = true;
	}
	return ret;
}

bool CutsceneDialog::Execute()
{
	bool ret = false;

	if (first_iteration) {
		Text_UI = App->gui->AddUIDialogBox(20, 158, 15, 224, "The heroes defeated Lich and returned sorez to the gods! The war end in Allarad, long life our heroes!", { 255,255,255,255 }, nullptr, Second_Font);
		first_iteration = false;
	}

	if (App->cutscene_manager->Cutscene_timer.Read() > end_time) {
		ret = true;
		App->gui->DeleteUIElement(*Text_UI);
		App->gui->DeleteUIElement(*Background);
	}

	return ret;
}

bool ExecuteAnimation::Execute()
{
	bool ret = false;

	if (animation == "lich_transformation") {

		actor->animation = &actor->transformation;
	}

	ret = actor->transformation.Finished();

	if (ret == true){
		
		actor->transformation.Reset();
		actor->animation = &actor->idle;
	}
	

	return ret;
}

CutsceneDialog::~CutsceneDialog() {

	
}


bool ctCutsceneManager::Update(float dt)
{

	ExecuteCutscene();

	return true;
}

void ctCutsceneManager::StartCutscene() {
	if (Executing_Cutscene == false) {
		Executing_Cutscene = true;
		Cutscene_timer.Start();
	}
}

bool ctCutsceneManager::Start()
{
	return true;
}

bool ctCutsceneManager::CleanUp()
{
	CutsceneActions.clear();
	return true;
}

bool ctCutsceneManager::isActive()
{
	return Executing_Cutscene;
}


bool ctCutsceneManager::ChargeCutscene(Cutscene_code cutscene)
{
	pugi::xml_document track_file;
	pugi::xml_parse_result result = track_file.load_file("cutscene.xml");
	pugi::xml_node cutscene_node;
	switch (cutscene) {
	case FINAL_CUTSCENE:
		cutscene_node = track_file.child("cutscenes").child("final_cutscene").child("cutscene_action");
		break;
	case LICH_CUTSCENE:
		cutscene_node = track_file.child("cutscenes").child("lich_cutscene").child("cutscene_action");
		break;
	default:
		break;
	}

	while (cutscene_node != nullptr) {
		int action = cutscene_node.attribute("cutscene_action").as_int();

		if (action == 0) {
			CutsceneActions.push_back(new MoveX(cutscene_node.attribute("start_time").as_int(), cutscene_node.attribute("end_time").as_int(), App->entities->GetActor(cutscene_node.attribute("actor").as_int()), cutscene_node.attribute("speed").as_int()));
		}

		if (action == 1) {
			CutsceneActions.push_back(new MoveY(cutscene_node.attribute("start_time").as_int(), cutscene_node.attribute("end_time").as_int(), App->entities->GetActor(cutscene_node.attribute("actor").as_int()), cutscene_node.attribute("speed").as_int()));
		}

		if (action == 2) {
			CutsceneActions.push_back(new CutsceneDialog(cutscene_node.attribute("start_time").as_int(), cutscene_node.attribute("end_time").as_int(), cutscene_node.attribute("text").as_string()));
		}

		if (action == 3) {
			App->cutscene_manager->CutsceneActions.push_back(new MoveCameraX(cutscene_node.attribute("start_time").as_int(), cutscene_node.attribute("end_time").as_int(), cutscene_node.attribute("speed").as_int()));
		}

		if (action == 4) {
			CutsceneActions.push_back(new MoveCameraY(cutscene_node.attribute("start_time").as_int(), cutscene_node.attribute("end_time").as_int(), cutscene_node.attribute("speed").as_int()));
		}

		if (action == 5) {
			CutsceneActions.push_back(new ExecuteAnimation(cutscene_node.attribute("start_time").as_int(), cutscene_node.attribute("end_time").as_int(), App->entities->GetActor(cutscene_node.attribute("actor").as_int()), cutscene_node.attribute("animation").as_string()));
		}
		cutscene_node = cutscene_node.next_sibling();
	}

	return true;
}

void ctCutsceneManager::ExecuteCutscene()
{
	if (Executing_Cutscene == true)
	{
		if (CutsceneActions.front() != nullptr) {
			for (std::list<CutsceneAction*>::iterator it = CutsceneActions.begin(); it != CutsceneActions.end(); it++) {

				if ((*it)->start_time <= Cutscene_timer.Read()) {

					if ((*it)->Execute()) {

						CutsceneActions.remove((*it));
						(*it)->~CutsceneAction();
					}
				}
			}
		}
	}

	if (CutsceneActions.size() == 0) {
		Executing_Cutscene = false;
	}
}

