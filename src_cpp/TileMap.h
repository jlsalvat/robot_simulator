#pragma once
#include <SDL2/SDL.h>
#include "../src_cpp/Graphics.h"
#include "../src_cpp/LTexture.h"
#include "../src_cpp/Sprite.h"
#include "../src_cpp/tinyxml2.h"
#include "../src_cpp/Shape.h"
#include "../src_cpp/Utils.h"
#include <string>
#include <vector>

//associé au tag map dans le fichier txm
struct Map {
	Map(const std::string &file_name);
	const tinyxml2::XMLElement* root;
	int nb_tuiles_x, nb_tuiles_y;
private:
	void init( tinyxml2::XMLDocument &doc);
	void verif();
	tinyxml2::XMLDocument doc;
};
//un tileset permet de lire une image et ses caractéristiques
struct Tileset {
	std::string file_name_sprite;
	SDL_Point size;
	int tilecount;//nombre total de tuiles dans le tileset
	int columns; // nombre de colonnes de tuiles
	int firstgid;//premier id du tileset
	int tilewidth;//taille en x des tuiles
	int tileheight;//taill en y des tuiles
	//affiche les données du tileset
	void printData();
	//get SDL_rect en fonction du numéro de la tuile
	//les tuiles sont numérotés de 1 à n de gauche à droite en descendant
	SDL_Rect getClip(int value);
	//renvoie la taille des tuiles du tileset (tilewidth et tileheight)
	SDL_Point getSize();
};
//il peut y avoir plusieurs tileset (tag)
struct Tilesets{
	Tilesets(const tinyxml2::XMLElement* root);
	std::vector<Tileset> vector_tilesets;
};
//les tuiles disposées en matrice se trouvent sur une couche
struct Layer {
	std::string name;
	int width;
	int height;
	int id;
	bool visible;
	std::vector<std::vector<unsigned int>> tile_vector;
	void printData();
};
// il peut y avoir plusieurs couches de tuiles
struct Layers{
	Layers(const tinyxml2::XMLElement* root);
	std::vector<Layer> vector_layers;
private:
	//lecture des couches avec le format xml (obsolète)
	void read_xml(const tinyxml2::XMLElement* root, const tinyxml2::XMLElement* xml_data, Layer & layer);
	//lecture des couches avec le format csv
	void read_csv(const tinyxml2::XMLElement* root, const tinyxml2::XMLElement* xml_data, Layer & layer);
};
//il est possible d'afficher des formes
struct Objects {
	Objects(const tinyxml2::XMLElement* objectgroup, Color c);
	std::vector<Rectangle*> vector_rectangle;
	std::vector<Polyline*> vector_polyline;
	std::vector<Circle*> vector_circle;
};
//il peut y avoir plusieurs groupes d'objects avec des noms différents
struct Objectgroup {
	Objectgroup(const tinyxml2::XMLElement* root);
	//association d'un nom à un groupe d'objets
	std::map<std::string, Objects*> map_objects;
};

//l'affichage d'une Map utilie une texture et un point
struct LayerMap {
	std::vector <SDL_Point*> vector_position;
	std::vector <LTexture*> vector_maps;

};
//une tuile peut être utilisée ou non pour l'affichage, tambouille interne
struct map_Rectangle {
	Rectangle* rect;
	bool used;
	map_Rectangle() :used(false){}
};


class TileMap{
public:
	TileMap(Graphics &graphics, const std::string& file_name);
	virtual void draw(Graphics &graphics);
	void printData();
	SDL_Point getSize();
	//pour un numero de tuile courant, on cherche le numéro de tuile au dessus, au dessous, a gauche et à droite

protected:	
	Map _map;
	Tilesets _tilesets;
	Layers _layers;
	Objectgroup _objectgroup;
	std::vector <LayerMap> _layer_maps;
	std::vector <SDL_Texture*> vector_texture_images;
	//ajouté pour le robot afin d'avoir tous les rectangles associées aux tuiles de la map
	//chaque tuile a une numéro.
	std::map<int, map_Rectangle> map_rectangles;
};

//le fichier map est constitué de rectangles et de polylines
struct ShapesLignes {
	std::vector<Rectangle*> vector_lignes;
	ShapesLignes() {}
	~ShapesLignes() {}
};
struct ShapesArcs {
	std::vector<Polyline*> vector_arcs;
	std::vector<Rectangle> vector_rects;
};
