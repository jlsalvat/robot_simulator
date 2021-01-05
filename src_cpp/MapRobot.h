#pragma once
#include "../src_cpp/TileMap.h"
#include "../src_cpp/MapShapes.h"
#include "../src_cpp/MapRobot.h"
#include <vector>
class MapRobot :public TileMap {
public:
	virtual void draw(Graphics &graphics, bool all);
	MapRobot(Graphics &graphics, const std::string& file_name, std::map<int, ShapesLignes *>  lignes_map, std::map<int, ShapesArcs *>  arcs_map);
	Direction getRobotDirection() const { return _robot_direction; }
	Point getRobotCenter() const { return _robot_center; }
	Rectangle getBarre();
	std::vector<Circle*> getAnimationCoins();
private:
	void robot();
	std::vector<Rectangle> lignes_rect;
	std::vector<Rectangle> arcs_rect;
	std::vector<Polyline> arcs;
	Point _robot_center;
	Direction _robot_direction;
	friend class Capteurs;
};
