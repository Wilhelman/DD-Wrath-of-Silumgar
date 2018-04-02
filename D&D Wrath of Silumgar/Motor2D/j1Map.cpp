#include "ctDefs.h"
#include "ctLog.h"
#include "ctApp.h"
#include "ctRender.h"
#include "ctTextures.h"
#include "j1Map.h"
#include "ctEntities.h"
#include <math.h>


j1Map::j1Map() : ctModule(), map_loaded(false)
{
	name = "map";
	
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder = config.child("folder").child_value();

	pugi::xml_document data;

	pugi::xml_node lvlData = App->LoadConfig(data);

	for (lvlData = lvlData.child("map").child("scene").child("data"); lvlData && ret; lvlData = lvlData.next_sibling("data"))
	{
		if (lvlData.attribute("start").as_bool()) {
			sceneName = lvlData.attribute("name").as_string();
			lvlData.attribute("currentLvl").set_value(true);
		}
		else {
			lvlData.attribute("currentLvl").set_value(false);
		}
	}
	data.save_file("config.xml");

	return ret;
}

void j1Map::LayersSetUp()
{
	
	std::list<TileSet*>::iterator tilesetsToCheck = this->data.tilesets.begin();

	while (tilesetsToCheck != this->data.tilesets.end())
	{
		if ((*tilesetsToCheck)->name == "background_map")
		(*tilesetsToCheck)->tileset_type = BACKGROUND_TILESET;

		if ((*tilesetsToCheck)->name == "RedGreen")
		(*tilesetsToCheck)->tileset_type = RED_GREEN_TILESET;

		if ((*tilesetsToCheck)->name == "skill_tree")
			(*tilesetsToCheck)->tileset_type = BACKGROUND_TILESET;

		if ((*tilesetsToCheck)->name == "Sprite-0001")
			(*tilesetsToCheck)->tileset_type = BACKGROUND_TILESET;

		std::list<MapLayer*>::iterator layersToCheck = this->data.layers.begin();

		while (layersToCheck != this->data.layers.end())
		{
			for (uint i = 0; i < (*layersToCheck)->width; i++)
			{
				for (uint j = 0; j < (*layersToCheck)->height; j++)
				{
					if ((*layersToCheck)->Get(i, j) != 0)
					{
						if ((*layersToCheck)->name == "background")
							(*layersToCheck)->layer_type = BACKGROUND;
						else if ((*layersToCheck)->name == "logic")
							(*layersToCheck)->layer_type = LOGIC;
						else if ((*layersToCheck)->name == "tier_1")
							(*layersToCheck)->layer_type = TIER_1;
						else if ((*layersToCheck)->name == "tier_2")
							(*layersToCheck)->layer_type = TIER_2;
						else if ((*layersToCheck)->name == "tier_3")
							(*layersToCheck)->layer_type = TIER_3;
						else if ((*layersToCheck)->name == "tier_4")
							(*layersToCheck)->layer_type = TIER_4;
						else if ((*layersToCheck)->name == "heroes_position")
							(*layersToCheck)->layer_type = HEROES_POSITION;
						else if ((*layersToCheck)->name == "enemies_position")
							(*layersToCheck)->layer_type = ENEMIES_POSITION;
					}
				}
			}
			layersToCheck++;
		}
		tilesetsToCheck++;
	}
}

void j1Map::setAllLogicForMap()
{
	std::list<TileSet*>::iterator tilesetsBlit = this->data.tilesets.begin();

	while (tilesetsBlit != this->data.tilesets.end())
	{
		std::list<MapLayer*>::iterator layersBlit = this->data.layers.begin();

		while (layersBlit != this->data.layers.end())
		{
			for (uint i = 0; i < (*layersBlit)->width; i++)
			{
				for (uint j = 0; j < (*layersBlit)->height; j++)
				{
					if ((*layersBlit)->Get(i, j) != 0)
					{
						if ((*layersBlit)->layer_type == TIER_1 && (*tilesetsBlit)->tileset_type == RED_GREEN_TILESET)
							tier_1_coords.push_back(iPoint(MapToWorld(i, j).x, MapToWorld(i, j).y));
						else if ((*layersBlit)->layer_type == TIER_2 && (*tilesetsBlit)->tileset_type == RED_GREEN_TILESET)
							tier_2_coords.push_back(iPoint(MapToWorld(i, j).x, MapToWorld(i, j).y));
						else if ((*layersBlit)->layer_type == TIER_3 && (*tilesetsBlit)->tileset_type == RED_GREEN_TILESET)
							tier_3_coords.push_back(iPoint(MapToWorld(i, j).x, MapToWorld(i, j).y));
						else if ((*layersBlit)->layer_type == TIER_4 && (*tilesetsBlit)->tileset_type == RED_GREEN_TILESET)
							tier_4_coords.push_back(iPoint(MapToWorld(i, j).x, MapToWorld(i, j).y));
						else if ((*layersBlit)->layer_type == HEROES_POSITION && (*tilesetsBlit)->tileset_type == RED_GREEN_TILESET)
							heroes_position_coords.push_back(iPoint(MapToWorld(i, j).x, MapToWorld(i, j).y));
						else if ((*layersBlit)->layer_type == ENEMIES_POSITION && (*tilesetsBlit)->tileset_type == RED_GREEN_TILESET)
							enemies_position_coords.push_back(iPoint(MapToWorld(i, j).x, MapToWorld(i, j).y));
					}
				}
			}
			layersBlit++;
		}
		tilesetsBlit++;
	}
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;

	std::list<TileSet*>::iterator tilesetsBlit = this->data.tilesets.begin();
	
	while (tilesetsBlit != this->data.tilesets.end())
	{
		std::list<MapLayer*>::iterator layersBlit = this->data.layers.begin();

		while (layersBlit != this->data.layers.end())
		{
			for (uint i = 0; i < (*layersBlit)->width; i++) 
			{
				for (uint j = 0; j < (*layersBlit)->height; j++)
				{
					if ((*layersBlit)->Get(i, j) != 0)
					{
						SDL_Rect rect = (*tilesetsBlit)->GetTileRect((*layersBlit)->Get(i, j));

						iPoint world = MapToWorld(i, j);

							if ((*layersBlit)->layer_type == BACKGROUND && App->render->Is_inScreen(world.x, world.y))
								App->render->MapBlit((*tilesetsBlit)->texture, world.x, world.y, &rect,1.0f); 
					}
				}
			}

			layersBlit++;
		}
		tilesetsBlit++;
	}
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	switch (data.type)
	{
	case MapTypes::MAPTYPE_ORTHOGONAL:
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
		break;
	case MapTypes::MAPTYPE_ISOMETRIC:
		ret.x = (x - y)*(data.tile_width / 2);
		ret.y = (x + y)*(data.tile_height / 2);
		break;
	default:
		break;
	}

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const {

	iPoint ret;

	switch (data.type)
	{
	case MapTypes::MAPTYPE_ORTHOGONAL:
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
		break;
	case MapTypes::MAPTYPE_ISOMETRIC:

		ret.x = (x/data.width) + (y/data.tile_height);
		ret.y = (y / data.tile_height) - (x / data.width);
		break;
	default:
		break;
	}
	
	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	std::list<TileSet*>::iterator it = data.tilesets.begin();

	while (it != data.tilesets.end())
	{
		RELEASE((*it))
		it++;
	}

	data.tilesets.clear();

	// Remove all layers

	std::list<MapLayer*>::iterator it_layer = data.layers.begin();

	while (it_layer != data.layers.end())
	{
		RELEASE((*it_layer))
			it_layer++;
	}

	data.layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(std::string file_name)
{
	bool ret = true;
	std::string tmp =  folder + file_name.c_str();

	pugi::xml_parse_result result = map_file.load_file(tmp.c_str());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.push_back(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(layer, set);
		}

		data.layers.push_back(set);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name.c_str());
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		std::list<TileSet*>::iterator item = this->data.tilesets.begin();

		while(item != data.tilesets.end())
		{
			TileSet* s = (*item);
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.c_str(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item++;
		}
		
		std::list<MapLayer*>::iterator item_layer = this->data.layers.begin();
		while(item_layer != data.layers.end())
		{
			MapLayer* l = (*item_layer);
			LOG("Layer ----");
			LOG("name: %s", l->name.c_str());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer++;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		std::string bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.size() > 0)
		{
			std::string red, green, blue;
			bg_color = (1, 2, red);
			bg_color = (3, 4, green);
			bg_color = (5, 6, blue);

			int v = 0;

			sscanf_s(red.c_str(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.c_str(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.c_str(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		std::string orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name = (tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.c_str(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	LoadProperties(node, layer->properties);

	layer->data = new uint[layer->width * layer->height];

	memset(layer->data, 0, layer->width * layer->height);
	
	int i = 0;

	for (pugi::xml_node tile_gid = node.child("data").child("tile"); tile_gid; tile_gid =  tile_gid.next_sibling("tile")) {
		layer->data[i++] = tile_gid.attribute("gid").as_uint();
	}

	return ret;
}

inline uint MapLayer::Get(int x, int y) const {

	return data[width * y + x];
}

// Load a group of properties from a node and fill a list with it
bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_int();

			properties.list.push_back(p);
		}
	}

	return ret;
}

/*void j1Map::LoadMapAnimation(pugi::xml_node animation_node, ctAnimation * animation)
{
	bool ret = true;

	for (pugi::xml_node frame = animation_node.child("frame"); frame && ret; frame = frame.next_sibling("frame"))
		animation->PushBack({ frame.attribute("x").as_int() , frame.attribute("y").as_int(), frame.attribute("width").as_int(), frame.attribute("height").as_int() });

	animation->speed = animation_node.attribute("speed").as_float();
	animation->loop = animation_node.attribute("loop").as_bool();
}

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	/*bool ret = false;
	std::list<MapLayer*>* item;
	item = data.layers.start;

	for (item = data.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		//TODO CHECK THIS
		if (layer->properties.Get("Navigation", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
					map[i] = ts->properties.Get("walkable", 1);
					}
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}
	
	return true;
}*/

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	/*std::list<TileSet*>::iterator item = this->data.tilesets.begin();
	TileSet* set = (*item);

	while ((*item))
	{
		if (id < (*item)->firstgid)
		{
			set = (*item)--;
			break;
		}
		set = (*item);
		item++;
	}

	return set;*/
	return nullptr;
}

int Properties::Get(const char* value, int default_value) const
{
	/*std::list<Property*>::iterator item = list.begin();

	while ((*item))
	{
		if ((*item)->name == value)
			return (*item)->value;
		item++;
	}

	return default_value;*/
	return 1;
}