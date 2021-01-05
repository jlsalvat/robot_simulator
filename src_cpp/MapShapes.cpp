#include "../src_cpp/MapShapes.h"
#include "../src_cpp/Shape.h"

using namespace std;

std::vector<Rectangle*> MapShapes::getRectangles() {
	vector<Rectangle*> tmpRects;
	if (_objectgroup.map_objects.size() > 0) {
		tmpRects = getRectInObjectGroup("ligne");
		if (tmpRects.size() == 0) {
			tmpRects = getRectInObjectGroup("lignes");
			if (tmpRects.size() == 0)
				throw std::runtime_error("no ligne or lignes in map_robot.tmx ");
		}
	}
	else
		throw std::runtime_error("no objects in map_robot.tmx ");
	return tmpRects;
}
std::vector<Polyline*> MapShapes::getPolylines() {
	vector<Polyline*> tmpPoly;
	if (_objectgroup.map_objects.size() > 0) {
		tmpPoly = getPolyInObjectGroup("arc");
		if (tmpPoly.size() == 0) {
			tmpPoly = getPolyInObjectGroup("arcs");
			if (tmpPoly.size() == 0)
				throw std::runtime_error("no arc or arcs in map_robot.tmx ");
		}
	}
	else
		throw std::runtime_error("no objects in map_robot.tmx ");
	return tmpPoly;
}
//on est dans le dur
// le but est de définir les rectangles et leur position relative des lignes et arcs dans la carte de base des formes.
//
MapShapes::MapShapes(Graphics &graphics) :TileMap(graphics, "../assets/map_robot.tmx") {
	//on récup les rectangles dans les objets dans le groupe ligne
	vector<Rectangle*> tmpRects = getRectangles();
	vector<Polyline*> tmpPoly = getPolylines();
	ShapesLignes *shape_lignes=nullptr;
	ShapesArcs *shape_arcs=nullptr;
	std::map<int, map_Rectangle>::iterator it;
	int counter = 1;
	//pour toutes les tuiles se trouvant dans la map
	for (it = map_rectangles.begin(); it != map_rectangles.end(); it++, counter++) {
		shape_lignes = new ShapesLignes();
		for (int i = 0; i < tmpRects.size(); i++) {
			if (it->second.rect->collidesWith(*tmpRects[i])) {
				it->second.used = true; // la tuile est utilisée
				std::cout << "Rectangle:" << *(it->second.rect) << "  collision with " << *tmpRects[i] << "\n";
				tmpRects[i]->x = tmpRects[i]->x - it->second.rect->x;
				tmpRects[i]->y = tmpRects[i]->y - it->second.rect->y;
				shape_lignes->vector_lignes.push_back(tmpRects[i]);
			}
		}
		if (shape_lignes->vector_lignes.size() != 0)
			//map_tuiles_lignes = numero de la tuile et le rectangle associé
			map_tuiles_lignes.insert(make_pair(it->first, shape_lignes));
		else {
			delete shape_lignes;
		}
	}
	//pour tous les polylines

	//pour chaque Polyline
	for (int i = 0; i < tmpPoly.size(); i++) {
		tmpPoly[i]->findCorners();//on cherches les SDL_Points min et max
		Rectangle rectangle = tmpPoly[i]->getRectangle(Color{ 200,0,0,255 });// par défaut on est en rouge...
		//On regarde pour chaque tuile
		for (it = map_rectangles.begin(); it != map_rectangles.end(); it++) {
			if (it->second.used == false) {
				if (it->second.rect->collidesWith(rectangle)) { //collision ?
					shape_arcs = new ShapesArcs();
					if (it->second.rect->isInside(rectangle)) { //à l'intérieur ?
						//la polyline se trouve dans une seule tuile
						std::cout << "Only one Tile:" << *(it->second.rect) << "  collision with " << tmpPoly[i]->corner_left.x << "," << tmpPoly[i]->corner_left.y << " && "
							<< tmpPoly[i]->corner_right.x << "," << tmpPoly[i]->corner_right.y << " rectangle=" << rectangle<<"\n";
						it->second.used = true;
						break; // on arrête de lire les tuiles à suivre. On passe au polygone suivant
					}
					else {
						//on suppose que l'on ne peut avoir que 2 tuiles adjacente pour le polyline
							std::cout << "TWO  Tiles :" << endl;
							it->second.used = true;
							int adjacent_tile = -1;
							//la polyline se trouve dans deux tuiles
							std::cout << "first Tile:" << *(it->second.rect) << "  collision with " << tmpPoly[i]->corner_left.x << "," << tmpPoly[i]->corner_left.y << " && "
								<< tmpPoly[i]->corner_right.x << "," << tmpPoly[i]->corner_right.y << " rectangle=" << rectangle << "\n";
							int tile_up, tile_down, tile_left, tile_right;
							updateRowCol(it->first);
							if ((tile_up = getTileUp(it->first)) >= 0) 
								if (map_rectangles[tile_up].used == false && map_rectangles[tile_up].rect->collidesWith(rectangle)) {
									map_rectangles[tile_up].used = true;
									std::cout << "second Polyline:" << *(map_rectangles[tile_up].rect) << "  up collision with " << tmpPoly[i]->corner_right.x << "," << tmpPoly[i]->corner_right.y << endl;
								}
							if ((tile_down = getTileDown(it->first)) >= 0)
								if (map_rectangles[tile_down].used == false && map_rectangles[tile_down].rect->collidesWith(rectangle)) {
									map_rectangles[tile_down].used = true;
									std::cout << "second Polyline:" <<*( map_rectangles[tile_down].rect) << "  down collision with " << tmpPoly[i]->corner_right.x << "," << tmpPoly[i]->corner_right.y << endl;
								}
							if ((tile_left = getTileLeft(it->first)) >= 0)//normalement toujours faux on pourra enlever aprés le test
								if (map_rectangles[tile_left].used == false && map_rectangles[tile_left].rect->collidesWith(rectangle)) {
									map_rectangles[tile_left].used = true;
									std::cout << "second Polyline:" << *(map_rectangles[tile_left].rect) << "  left collision with " << tmpPoly[i]->corner_right.x << "," << tmpPoly[i]->corner_right.y << endl;
								}
							 if ((tile_right = getTileRight(it->first)) >= 0)
								 if (map_rectangles[tile_right].used == false && map_rectangles[tile_right].rect->collidesWith(rectangle)) {
									 map_rectangles[tile_right].used = true;
									 std::cout << "second Polyline:" << *(map_rectangles[tile_right].rect) << "  right collision with " << tmpPoly[i]->corner_right.x << "," << tmpPoly[i]->corner_right.y << endl;
								 }
							break;//on arrête de lire les tuiles
						}

				}
			}
		}
		tmpPoly[i]->substract(it->second.rect->x, it->second.rect->y);
		shape_arcs->vector_arcs.push_back(tmpPoly[i]);
		rectangle.x -= it->second.rect->x;
		rectangle.y -= it->second.rect->y;
		shape_arcs->vector_rects.push_back(rectangle);
		if (shape_arcs->vector_rects.size() != 0) {
			//map_tuiles_lignes = numero de la tuile et le rectangle associé
			map_tuiles_arcs.insert(make_pair(it->first, shape_arcs));
		}
		else
			delete shape_arcs;
	}
}

SDL_Texture* MapShapes::getImageTexture() {
	return vector_texture_images[0];
}

std::map<int, ShapesLignes *> MapShapes::getLignes() {
	return map_tuiles_lignes;
}
std::map<int, ShapesArcs *> MapShapes::getArcs() {
	return map_tuiles_arcs;
}
void MapShapes::draw(Graphics &graphics) {
	std::map<int, ShapesLignes*>::iterator it;
	for (it = map_tuiles_lignes.begin(); it != map_tuiles_lignes.end(); it++) {
		for (int i = 0; i < it->second->vector_lignes.size(); i++) {
			it->second->vector_lignes[i]->draw(graphics);
		}
	}
	std::map<int, ShapesArcs*>::iterator itArc;
	for (itArc = map_tuiles_arcs.begin(); itArc != map_tuiles_arcs.end(); itArc++) {
		for (int i = 0; i < itArc->second->vector_rects.size(); i++) {
			itArc->second->vector_rects[i].draw(graphics);
		}
		for (int i = 0; i < itArc->second->vector_arcs.size(); i++) {
			itArc->second->vector_arcs[i]->draw(graphics);
		}
	}

}
std::vector<Rectangle*> MapShapes::getRectInObjectGroup(string name_object) {
	std::vector<Rectangle*> tmp;
	if (_objectgroup.map_objects[name_object] != nullptr)
		return _objectgroup.map_objects[name_object]->vector_rectangle;
	else
		return tmp;
}

std::vector<Polyline*>  MapShapes::getPolyInObjectGroup(string name_object) {
	std::vector<Polyline*> tmp;
	if (_objectgroup.map_objects[name_object] != nullptr)
		return _objectgroup.map_objects[name_object]->vector_polyline;
	else
		return tmp;
}
bool  MapShapes::updateRowCol(int value) {
	for (unsigned int i = 0; i < _layers.vector_layers[0].height; ++i) {
		for (unsigned int j = 0; j < _layers.vector_layers[0].width; ++j) {
			if (value == _layers.vector_layers[0].tile_vector[i][j]) {
				row = i;
				col = j;
				return true;
			}
		}
	}
	return false;
}
int  MapShapes::getTileUp(int tile) {
	int value = -1;
	//on n'est pas sur la première ligne ?
	if(row>0)
		value=_layers.vector_layers[0].tile_vector[row-1][col];
	return value;
}
int  MapShapes::getTileDown(int tile) {
	int value=-1;
	//on n'est pas sur la dernière ligne ?
	if (row < _layers.vector_layers[0].height -1)
		value = _layers.vector_layers[0].tile_vector[row + 1][col];
	return value;
}
int  MapShapes::getTileLeft(int tile) {
	int value=-1;
	//On n'est pas sur la première colonne ?
	if (col>0)
		value = _layers.vector_layers[0].tile_vector[row ][col-1];
	return value;
}
int  MapShapes::getTileRight(int tile) {
	int value=-1;
//on n'est pas sur la dernière colonne ?
	if (col < _layers.vector_layers[0].width -1)
		value = _layers.vector_layers[0].tile_vector[row][col+1];
	return value;
}


