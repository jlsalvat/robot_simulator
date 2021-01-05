#pragma once
#include "../src_cpp/LTexture.h"
#include "../src_cpp/Utils.h"
#include "../src_cpp/Capteurs.h"
#include "MapRobot.h"
#define _USE_MATH_DEFINES 
#include <math.h>
#include <vector>
class Robot {
public:
	//on passe la texture qui vient de map.getImage ainsi que la position des différents capteurs
	Robot(MapRobot & map,SDL_Texture * image, Point position_robot, Direction direction_robot, std::vector<float> const &position_capteurs); //copy reference and not modify
	//calcule la nouvelle position du rectangle et des capteurs en fonction de la position du centre du robot
	void init(Point position_robot, Direction direction_robot);
	void  update(int pwm_gauche, int pwm_droit);
	void sens(bool sens_av_l, bool sens_av_r) {
		_sens_av_l = sens_av_l;
		_sens_av_r = sens_av_r;
	}
	Rectangle getRectangle();
	void draw(Graphics &graphics);
	std::vector<float> getCapteurs() { return _capteurs.getValues(); }
	void up() { _position_robot.y_cm += 0.6; }
	void down() { _position_robot.y_cm -= 0.6; }
	void right() { _position_robot.x_cm += 0.6; }
	void left() { _position_robot.x_cm -= 0.6; }
	void rotate(int theta_deg){_theta+=theta_deg * M_PI / 180.0; }
	void stop();

private:
	bool _sens_av_l=true, _sens_av_r=true;
	void cinematics(int &pwm_gauche, int &pwm_droit);
	void traine_update();
	void traine_aff(Graphics & graphics);
	LTexture _texture_robot;
	Capteurs _capteurs;
	Point _position_robot;
	Direction _direction_robot;
	Point _position_roue_gauche;
	Point _position_roue_droite;
	std::vector <SDL_Point> _tab_traine;
	std::vector <SDL_Point> _tab_traine_roue_gauche;
	std::vector <SDL_Point> _tab_traine_roue_droite;
	float _theta;
	int _indice_traine=0;
	int _seuil_moteur_gauche, _seuil_moteur_droit;
	bool _depassement = false;
}; 
