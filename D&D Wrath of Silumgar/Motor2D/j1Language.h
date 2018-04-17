#ifndef __j1LANGUAGE_H__
#define __j1LANGUAGE_H__

#include "ctModule.h"

#include <vector>
using namespace std;
struct Dictionary 
{
	string MM_quit_btn;
	string MM_new_game_btn;
	string MM_continue_btn;
	string MM_about_btn;
	string MM_settings_btn;
};

class j1Language : public ctModule
{
public:

	j1Language();

	// Destructor
	virtual ~j1Language();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void ChangeCurrentLanguage(string new_language);

	Dictionary GetDictionary()const;

public:
	string current_language;
	vector<string> posible_languages;

private:

	Dictionary dictionary;

};

#endif // __j1AUDIO_H__