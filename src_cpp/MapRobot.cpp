#include "MapRobot.h"

MapRobot::MapRobot(Graphics &graphics, const std::string& file_name, std::map<int, ShapesLignes *>  lignes_map, std::map<int, ShapesArcs *>  arcs_map) : TileMap(graphics, file_name) {
	//pour chaque couche du fichier tmx
	for (int i = 0; i < _layers.vector_layers.size(); i++) {
		//on prend chaque tuile de la couche courante
		for (unsigned int row = 0; row < _layers.vector_layers[i].tile_vector.size(); ++row){
			for (unsigned int col = 0; col < _layers.vector_layers[i].tile_vector[row].size(); ++col) {
				//on lit le numéro de la tuile
				int value = _layers.vector_layers[i].tile_vector[row][col];
				if (value != 0) {
					if (lignes_map[value] != nullptr) {
						for (int i = 0; i < lignes_map[value]->vector_lignes.size(); i++) {
							Rectangle rect (lignes_map[value]->vector_lignes[i]);
							rect.x += col * _tilesets.vector_tilesets[0].tilewidth;
							rect.y += row * _tilesets.vector_tilesets[0].tileheight;
							lignes_rect.push_back(rect);
								//	SDL_SetRenderDrawColor(graphics.gRenderer(), 200, 0, 0, 255);
								//SDL_RenderFillRect(graphics.gRenderer(), &(rect->getRect()));
						}
					}
					if (arcs_map[value] != nullptr) {
						for (int i = 0; i < arcs_map[value]->vector_rects.size(); i++) {
							Rectangle rect(arcs_map[value]->vector_rects[i]);
							rect.x += col * _tilesets.vector_tilesets[0].tilewidth;
							rect.y += row * _tilesets.vector_tilesets[0].tileheight;
							arcs_rect.push_back(rect);
						}
						for (int i = 0; i < arcs_map[value]->vector_arcs.size(); i++) {
							Polyline poly = Polyline(arcs_map[value]->vector_arcs[i]->points, Color{ 0,250,120,255 });
							poly.move( col * _tilesets.vector_tilesets[0].tilewidth, row * _tilesets.vector_tilesets[0].tileheight);
							arcs.push_back(poly);
						}
					}

				}
			}
		}

	}
	robot();
}

void MapRobot::robot() {
	if (_objectgroup.map_objects["debut"]->vector_polyline.size() != 0) {
		_robot_center = Parameters::conv_SDL_Point_to_Point(*_objectgroup.map_objects["debut"]->vector_polyline[0]->points[0]);
//		center.x_cm = Parameters::convWidthPixToCm(_objectgroup.map_objects["debut"]->vector_polyline[0]->points[0]->x);
//		center.y_cm = Parameters::convHeightPixToCm(_objectgroup.map_objects["debut"]->vector_polyline[0]->points[0]->y);
		//on récup le deuxième point
		SDL_Point *ptr_point1 = _objectgroup.map_objects["debut"]->vector_polyline[0]->points[1];
		if (ptr_point1 != nullptr)
			_robot_direction = Utils::getDirection(_robot_center, Parameters::conv_SDL_Point_to_Point(*ptr_point1));
		else
			throw std::runtime_error("TileMap : error in file , debut");
	}
	else
		throw std::runtime_error("TileMap : error in file , debut");
}

Rectangle MapRobot::getBarre() {
	Rectangle rect;
	if (_objectgroup.map_objects["fin"]->vector_rectangle.size() != 0)
		rect = _objectgroup.map_objects["fin"]->vector_rectangle[0];
	else
		throw std::runtime_error("TileMap : error in file , fin");
	return rect;
}
std::vector<Circle*> MapRobot::getAnimationCoins() {
	if (_objectgroup.map_objects["tracer"]->vector_circle.size() != 0)
		return _objectgroup.map_objects["tracer"]->vector_circle;
	else
		throw std::runtime_error("TileMap : error in file , tracer");
}

void MapRobot::draw(Graphics &graphics, bool all) {
	TileMap::draw(graphics);
	if (all) {
	for (int i = 0; i < lignes_rect.size(); i++) {
		lignes_rect[i].draw(graphics);
	}
	for (int i = 0; i < arcs_rect.size(); i++) {
		arcs_rect[i].draw(graphics);
	}
	for (int i = 0; i < arcs.size(); i++) {
		arcs[i].draw(graphics);
	}
	}

}
