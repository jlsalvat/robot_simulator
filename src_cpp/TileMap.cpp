#include "TileMap.h"
#include "Utils.h"
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;
using namespace tinyxml2;

void Map::init(XMLDocument &doc) {
	root = doc.FirstChildElement("map");
	if (root == nullptr) throw std::runtime_error("no tag map in xml file");
	verif();
	//open nb_tuiles en x et y dans tmx file
	root->QueryIntAttribute("width", &nb_tuiles_x);
	root->QueryIntAttribute("height", &nb_tuiles_y);
}

Map::Map(const string &file_name) {
	if (doc.LoadFile(file_name.c_str()) != XML_SUCCESS) {
		const auto error_str = "XMLDocument : LoadFile '" + file_name + "'!";
		throw std::runtime_error(error_str);
	}
	//print xml file
	doc.Print();
	init(doc);
}
void Map::verif() {
	//gestion de la version
	const char *sz_version = root->Attribute("version");
	string s_version = sz_version;
	cout << "version file = " << s_version.c_str() << endl;
	//on enlève les points de la version
	s_version.erase(std::remove(s_version.begin(), s_version.end(), '.'), s_version.end());
	int i_version = stoi(s_version);//passage en int
	if (i_version < 14) {
		const auto warning = "this version of tiled has not been tested !!! ";
		throw std::runtime_error(warning);
	}
	//verif orientation
	const char *sz_orientation = root->Attribute("orientation");
	if (strcmp(sz_orientation, "orthogonal") != 0)
		throw std::runtime_error("orientation must be orthogonal in file tiled: default config\n");
	//verif renderorder
	const char *sz_renderorder = root->Attribute("renderorder");
	if (strcmp(sz_renderorder, "right-down") != 0)
		//		throw std::runtime_error("renderorder must be right-down in file tiled: default config\n");
		root->QueryIntAttribute("width", &nb_tuiles_x);
	root->QueryIntAttribute("height", &nb_tuiles_y);
}

SDL_Rect Tileset::getClip(int value) {
	SDL_Rect clip;
	int lines = tilecount / columns;
	clip.w = size.x / columns;
	clip.h = size.y / lines;
	clip.x = (value - 1) % columns*clip.w;
	clip.y = (value - 1) / columns * clip.h;
	return clip;
}
SDL_Point Tileset::getSize() {
	SDL_Point size;
	size.x = tilewidth;
	size.y = tileheight;
	return size;
}

Tilesets::Tilesets(const XMLElement* root) {
	const XMLElement* xml_tileset = root->FirstChildElement("tileset");
	if (xml_tileset == nullptr) throw std::runtime_error("no tag tileset in xml file");
	while (xml_tileset) {
		Tileset tileset;
		xml_tileset->QueryIntAttribute("firstgid", &tileset.firstgid);
		xml_tileset->QueryIntAttribute("tilewidth", &tileset.tilewidth);
		xml_tileset->QueryIntAttribute("tileheight", &tileset.tileheight);
		xml_tileset->QueryIntAttribute("tilecount", &tileset.tilecount);
		xml_tileset->QueryIntAttribute("columns", &tileset.columns);
		const XMLElement* image = xml_tileset->FirstChildElement("image");
		if (image == nullptr) throw std::runtime_error("no tag image in xml file");
		//verif width and heigt in file
		const char *sz_file_sprite = image->Attribute("source");
		tileset.file_name_sprite = sz_file_sprite;
		SDL_Point size;
		int width_file, height_file;
		image->QueryIntAttribute("width", &size.x);
		image->QueryIntAttribute("height", &size.y);
		tileset.size = size;
		vector_tilesets.push_back(tileset);
		xml_tileset = xml_tileset->NextSiblingElement("tileset");//tileset suivant
	}
}


void Tileset::printData() {
	cout << "firstgid:" << firstgid << " tilecount:" << tilecount << " columns:" << columns << endl;
	cout << "image source:" << file_name_sprite << endl;
	cout << "width:" << size.x << " height:" << size.y << endl;
}

void Layer::printData() {
	std::cout << "\nName: " << name
		<< "\nWidth: " << width
		<< "\nHeight: " << height;
	std::cout << "\nTiles: \n";
	for (unsigned int row = 0; row < height; ++row)
	{
		for (unsigned int column = 0; column < width; ++column)
		{
			std::cout << tile_vector[row][column] << " ";
		}
		std::cout << std::endl;
	}
}

void Layers::read_xml(const XMLElement* root, const XMLElement* xml_data, Layer & layer) {
	const XMLElement* xml_tile = xml_data->FirstChildElement("tile");
	if (xml_tile == nullptr) {
		const auto error_string = "no tile tag id=" + layer.id;
		throw std::runtime_error(error_string);
	}
	for (int i = 0; i < layer.width; i++) {
		vector<unsigned int>inter_mat;       //Intermediate matrix 
		for (int j = 0; j < layer.height; j++) {
			const char *sz_gid = xml_tile->Attribute("gid");
			int value;
			if (sz_gid == nullptr)
				value = 0;
			else
				value = stoi(sz_gid);
			inter_mat.push_back(value);
			xml_tile = xml_tile->NextSiblingElement("tile");//tile suivant
		}
		layer.tile_vector.push_back(inter_mat);

	}
	vector_layers.push_back(layer);
}

void Layers::read_csv(const XMLElement* root, const XMLElement* xml_data, Layer & layer) {
	const char *sz_matrice = xml_data->GetText();
	string matrice = sz_matrice;
	matrice.erase(std::remove(matrice.begin(), matrice.end(), '\n'), matrice.end());
	std::string delimiter = ",";
	size_t pos = 0;
	std::string token;
	for (int i = 0; i < layer.height; i++) {
		vector<unsigned int>inter_mat;       //Intermediate matrix 
		for (int j = 0; j < layer.width; j++) {
			pos = matrice.find(delimiter);
			token = matrice.substr(0, pos);
			std::cout << token << " ";
			matrice.erase(0, pos + delimiter.length());
			int value = stoi(token);
			inter_mat.push_back(value);
		}
		cout << endl;
		layer.tile_vector.push_back(inter_mat);

	}
	vector_layers.push_back(layer);

}
Layers::Layers(const XMLElement* root) {
	const XMLElement* xml_layer = root->FirstChildElement("layer");
	if (xml_layer == nullptr) throw std::runtime_error("no tag layer in xml file");
	while (xml_layer) {
		Layer layer;
		xml_layer->QueryIntAttribute("id", &layer.id);
		const char *sz_name = xml_layer->Attribute("name");
		if (sz_name != nullptr)
			layer.name = sz_name;
		else
			layer.name = "";
		xml_layer->QueryIntAttribute("width", &layer.width);
		xml_layer->QueryIntAttribute("height", &layer.height);
		if (xml_layer->QueryIntAttribute("visible", (int*)&layer.visible) != XML_SUCCESS)
			layer.visible = true;
		const XMLElement* xml_data = xml_layer->FirstChildElement("data");
		if (xml_data == nullptr) {
			const auto error_string = "no tag data id=" + layer.id;
			throw std::runtime_error(error_string);
		}
		const char *sz_name_encoding = xml_data->Attribute("encoding");
		if (sz_name_encoding != nullptr && strcmp(sz_name_encoding, "csv") == 0)
			read_csv(root, xml_data, layer);
		else
			read_xml(root, xml_data, layer);
		xml_layer = xml_layer->NextSiblingElement("layer");//layer suivant
	}

}

TileMap::TileMap(Graphics & graphics, const string & file_name) : _map(file_name), _tilesets(_map.root), _layers(_map.root), _objectgroup(_map.root) {
	//lecture des images
	for (int i = 0; i < _tilesets.vector_tilesets.size(); i++) {
		SDL_Texture *texture_image = graphics.loadImage(_tilesets.vector_tilesets[i].file_name_sprite.c_str());
		vector_texture_images.push_back(texture_image);
	}
	//mis à jour des layers
	LayerMap layerMap;
	for (int i = 0; i < _layers.vector_layers.size(); i++) {
		if (_layers.vector_layers[i].visible) {
			for (unsigned int row = 0; row < _layers.vector_layers[i].height; ++row) {
				for (unsigned int column = 0; column < _layers.vector_layers[i].width; ++column) {
					int value = _layers.vector_layers[i].tile_vector[row][column];
					if (value != 0) {
						int indice = 0;
						bool error = false;
						while (_tilesets.vector_tilesets[indice].firstgid <= value) {
							indice++;
							if (indice >= _tilesets.vector_tilesets.size()) {
								break;
							}
						}
						SDL_Rect clip = _tilesets.vector_tilesets[indice - 1].getClip(value);
						//chaque tuile est un Rectangle avec value=le numéro de la tuile
						map_rectangles[value].rect = new Rectangle(clip.x,clip.y,clip.w,clip.h);
						layerMap.vector_maps.push_back(new LTexture(vector_texture_images[indice - 1], &clip));
						SDL_Point *p = new SDL_Point{ (int)column * clip.w, (int)row *clip.h };
						layerMap.vector_position.push_back(p);
					}
				}
			}
			_layer_maps.push_back(layerMap);
		}

	}

}

void TileMap::printData() {
	for (int i = 0; i < _tilesets.vector_tilesets.size(); i++) {
		_tilesets.vector_tilesets[i].printData();
		cout << "CLIP\n";
		for (int j = 1; j <= _tilesets.vector_tilesets[i].tilecount; j++) {
			SDL_Rect clip = _tilesets.vector_tilesets[i].getClip(j); // j est la valeur lue dans layer et i est le tileset courant
			cout << "x=" << clip.x << " y=" << clip.y << " w=" << clip.w << " h=" << clip.h << endl;
		}
	}
	for (int i = 0; i < _layers.vector_layers.size(); i++)
		_layers.vector_layers[i].printData();
	std::map<std::string, Objects*>::iterator it;
	for (it = _objectgroup.map_objects.begin(); it != _objectgroup.map_objects.end(); it++){
		cout << "name objecgroup:" << it->first << "\n";
		for (auto &i : it->second->vector_rectangle) cout << *i << "\n";
		for (auto &i : it->second->vector_circle) cout << *i << "\n";
		for (auto &i : it->second->vector_polyline) cout << *i << "\n";
	}
}

SDL_Point TileMap::getSize() {
	SDL_Point size_tmp, size;
	//non testé complètement.
	// testé avec un seul tileset et la première layer...
	//on suppose que chaque layer possède la même taille totale...
	size = _tilesets.vector_tilesets[0].getSize();
	size.x = size.x*_layers.vector_layers[0].width;
	size.y = size.y*_layers.vector_layers[0].height;
	return size;
}


void TileMap::draw(Graphics &graphics) {
	for (int i = 0; i < _layer_maps.size(); i++) //pour chaque couche (de la première à la dernière
		for (int j = 0; j < _layer_maps[i].vector_maps.size(); j++)
			_layer_maps[i].vector_maps[j]->draw(graphics, _layer_maps[i].vector_position[j]->x, _layer_maps[i].vector_position[j]->y);
	std::map<std::string, Objects*>::iterator it;
	for (it = _objectgroup.map_objects.begin(); it != _objectgroup.map_objects.end(); it++) {
		for (int i = 0; i < it->second->vector_rectangle.size(); i++) {
			it->second->vector_rectangle[i]->draw(graphics);
		}
		for (int i = 0; i < it->second->vector_circle.size(); i++) {
			it->second->vector_circle[i]->draw(graphics);
		}
		for (int i = 0; i < it->second->vector_polyline.size(); i++) {
			it->second->vector_polyline[i]->draw(graphics);
		}
	}
}


Objects::Objects(const tinyxml2::XMLElement* xml_objectgroup, Color c) {
	Shape *shape = nullptr;
	bool visible;
	//find if there is object in obectgroup
	const XMLElement* xml_object = xml_objectgroup->FirstChildElement("object");
	while (xml_object) { //for each object in objectgroup
		Shape shape;
		//read x and y , exist for Rectangle,Ellipse, polyline and polygon 
		xml_object->QueryIntAttribute("x", &shape.x);
		xml_object->QueryIntAttribute("y", &shape.y);
		// width and height exist only for Ellipse and Rectangle
		if (xml_object->QueryIntAttribute("width", &shape.width) == XML_SUCCESS) // c'est un rectangle ou un cercle
			xml_object->QueryIntAttribute("height", &shape.height);
		else
			shape.width = -1;// c'est une polyline ou polygon...
		//we display only if attribute of object is visible (visible doesn't exist)
		if (xml_object->QueryIntAttribute("visible", (int*)&visible) != XML_SUCCESS)
			visible = true;
		if (visible) {
			const XMLElement* xml_tmp1 = xml_object->FirstChildElement("ellipse");
			if (xml_tmp1 != nullptr) //is it a Circle ?
				vector_circle.push_back(new Circle(shape.x, shape.y, shape.width, shape.height, c));
			else {
				const XMLElement* xml_tmp2 = xml_object->FirstChildElement("polyline");
				const XMLElement* xml_tmp3 = xml_object->FirstChildElement("polygon");
				if (xml_tmp2 != nullptr || xml_tmp3 != nullptr) {//is it a polygon or a polyline ?
					const XMLElement* xml_tmp;
					//we need to get pointer to XMLElement*
					if (xml_tmp2 != nullptr)
						xml_tmp = xml_tmp2;
					else
						xml_tmp = xml_tmp3;
					//get all the points of the polylyne or polygon
					const char *sz_points = xml_tmp->Attribute("points");
					if (sz_points != nullptr) {
						//"x0,y0 x1,y1 x2,x2 ...." we need to get x,y for each point
						string s_points = sz_points;
						std::string delimiter = " ";
						size_t pos_space = 0;
						size_t pos_virgule = 0;
						std::string token;
						size_t size = 0;
						Polyline *lines = new Polyline(c);
						do {
							pos_space = s_points.find(delimiter);
							token = s_points.substr(0, pos_space);
							SDL_Point *p = new SDL_Point;
							pos_virgule = token.find(",");
							p->x = round(stof(token.substr(0, pos_virgule))) + shape.x;//all x0,x1,x2,... are refered to x position
							p->y = round(stof(token.substr(pos_virgule + 1, pos_space - pos_virgule))) + shape.y;//idem for y
							lines->add_point(p);
							size = s_points.size();//size before
							s_points.erase(0, pos_space + delimiter.length());//erase xi,yi for next turn
						} while (s_points.size() < size);// doit on recommencer ?
/*						if (xml_tmp3 != nullptr) {//c'est un polygon donc on ferme le contour
							SDL_Point *p = new SDL_Point;
							p->x = shape.x; p->y = shape.y;
							lines->add_point(p);
						}*/
						vector_polyline.push_back(lines);
					}
				}
				else {
					//c'est un rectangle
					if (shape.width > 0)//on vérif que c'est bien un rectangle.
						vector_rectangle.push_back(new Rectangle(shape.x, shape.y, shape.width, shape.height, c));
				}
			}
		}
		xml_object = xml_object->NextSiblingElement("object");// suivant
	}
}

Objectgroup::Objectgroup(const tinyxml2::XMLElement* root) {
	Color c;//pas de couleur
	//tmx file begin with objectgroup
	const XMLElement* xml_objectgroup = root->FirstChildElement("objectgroup");
	if (xml_objectgroup == nullptr) cout << "no objects founds";//warning
	int cpt_name = 0;
	while (xml_objectgroup) { //for each objectgroup
		//get the name of the objectgroup
		const char *sz_name = xml_objectgroup->Attribute("name");
		string name;
		if (sz_name != nullptr)
			name = sz_name;
		else {
			name = string("no_name");
			name += cpt_name++;
		}
		//read if there is a color for all objects of objectgroup
		const char *sz_color = xml_objectgroup->Attribute("color");
		if (sz_color != nullptr) {
			string hexCode = sz_color;
			// Remove the hashtag ...
			if (hexCode.at(0) == '#')
				hexCode = hexCode.erase(0, 1);
			// ... and extract the rgb values.
			istringstream(hexCode.substr(0, 2)) >> std::hex >> c.red;
			istringstream(hexCode.substr(2, 2)) >> std::hex >> c.green;
			istringstream(hexCode.substr(4, 2)) >> std::hex >> c.blue;
		}
		Objects *objects = new Objects(xml_objectgroup,c);
		map_objects[name] = objects;
		xml_objectgroup = xml_objectgroup->NextSiblingElement("objectgroup");// suivant
	}
}










