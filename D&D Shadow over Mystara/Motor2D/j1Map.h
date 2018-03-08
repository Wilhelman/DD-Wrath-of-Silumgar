#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "ctPoint.h"
#include "ctModule.h"
#include "ctAnimation.h"
#include <string>
#include <list>

class p2String;
struct SDL_Texture;
// ----------------------------------------------------

enum LayerTypes
{
	BACKGROUND = 0,
	PARALLAX,
	DEAD_ZONE,
	GROUND_1,
	GROUND_2,
	LOGIC,
	COLLISIONS,
	ENEMIES,

	LAYER_NOT_DEF
};

enum TileSetTypes
{
	PLATFORM = 0,
	FLAG,
	

	TILESET_NOT_DEF
};

struct Properties
{
	struct Property
	{
		std::string name;
		int value = 0;
	};

	~Properties()
	{

		std::list<Property*>::iterator it = list.begin();

		while (it != list.end())
		{
			RELEASE((*it))
			it++;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	std::list<Property*>	list;
};

struct MapLayer {

	std::string name;
	uint width = 0; //number of tiles in the x axis
	uint height = 0; //number of tiles in the y axis
	Properties	properties;

	uint* data = nullptr;
	uint size_data = 0;

	LayerTypes layer_type = LAYER_NOT_DEF;

	~MapLayer() {
		RELEASE(data);
	}

	inline uint Get(int x, int y) const;
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	std::string			name;
	int					firstgid = 0;
	int					margin = 0;
	int					spacing = 0;
	int					tile_width = 0;
	int					tile_height = 0;
	SDL_Texture*		texture = nullptr;
	int					tex_width = 0;
	int					tex_height = 0;
	int					num_tiles_width = 0;
	int					num_tiles_height = 0;
	int					offset_x = 0;
	int					offset_y = 0;

	TileSetTypes		tileset_type = TILESET_NOT_DEF;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// ----------------------------------------------------
struct MapData
{
	int					width = 0;
	int					height = 0;
	int					tile_width = 0;
	int					tile_height = 0;
	SDL_Color			background_color = { 0, 0, 0, 0 };
	MapTypes			type = MAPTYPE_UNKNOWN;
	std::list<TileSet*>	tilesets;
	std::list<MapLayer*>   layers;
};

// ----------------------------------------------------
class j1Map : public ctModule
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;

	iPoint WorldToMap(int x, int y) const;

	void setAllLogicForMap();
	void LayersSetUp();

	TileSet* GetTilesetFromTileId(int id) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;


private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);
	void LoadMapAnimation(pugi::xml_node animation_node, ctAnimation* animation);

public:

	MapData data;
	std::string sceneName;
	iPoint spawn = iPoint({ -1, -1 });

	//for comodity c:
	MapLayer* collisionLayer = nullptr;

private:

	pugi::xml_document	map_file;

	std::string			folder;
	std::string			spritesheetName;

	bool				map_loaded = false;

	ctAnimation			lava_waterfall = ctAnimation();
	ctAnimation			lava_animation = ctAnimation();
	ctAnimation			water_animation = ctAnimation();
	ctAnimation			waterfall = ctAnimation();

};

#endif // __j1MAP_H__