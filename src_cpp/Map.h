#pragma once
#include <SDL2/SDL.h>
#include "../src_cpp/Graphics.h"
#include "../src_cpp/tinyxml2.h"
#include <string>
#include <vector>

//associé au tag map dans le fichier txm
private struct Map {
	Map(const tinyxml2::XMLDocument &doc);
	const tinyxml2::XMLElement* root;
	int nb_tuiles_x, nb_tuiles_y;
private:
	void verif();
};
//un tileset permet de lire une image et ses caractéristiques 
private struct Tileset {
	std::string file_name_sprite;
	SDL_Point size;
	int tilecount;
	int columns;
	int firstgid;
	void printData();
};
//il peut y avoir plusieurs tileset (tag)
private struct Tilesets{
	Tilesets(const tinyxml2::XMLElement* root);
	std::vector<Tileset> vector_tilesets;

};

private struct Layer {
	std::string name;
	int width;
	int height;
	int id;
	std::vector<std::vector<unsigned int>> tile_vector;
	void printData();
};

private struct Layers{
	Layers(const tinyxml2::XMLElement* root);
	std::vector<Layer> vector_layers;
};

public class TileMap{
	TileMap(Graphics& graphics,const std::string& file);

}
