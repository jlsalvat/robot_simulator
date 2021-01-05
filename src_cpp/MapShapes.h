#pragma once
#include "TileMap.h"
#include "Shape.h"
#include <vector>

//chaque numéro de tuile est constituée de formes
class MapShapes :public TileMap {
public:
	MapShapes(Graphics &graphics);
	std::map<int, ShapesLignes *> getLignes();
	std::map<int, ShapesArcs *> getArcs();
	virtual void draw(Graphics &graphics);
//	0, 2, 3, 4, 5, 6, 7, 8,
//		9, 10, 11, 12, 13, 14, 15, 16,
//		17, 18, 19, 20, 21, 22, 23, 24,
//		25, 26, 27, 28, 29, 30, 31, 32
//all tiles have an increment  number 
	int  getTileUp(int tile);
	int  getTileDown(int tile);
	int  getTileLeft(int tile);
	int  getTileRight(int tile);
	SDL_Texture* getImageTexture();
private:
	bool updateRowCol(int value);
	std::map<int, ShapesLignes *> map_tuiles_lignes;
	std::map<int, ShapesArcs *> map_tuiles_arcs;
	int row, col;
//read all rect in objectsgroups
	std::vector<Rectangle*> getRectInObjectGroup(std::string name_object);
	std::vector<Rectangle*> getRectangles();
	std::vector<Polyline*>  getPolyInObjectGroup(std::string name_object);
	std::vector<Polyline*>  getPolylines();
};