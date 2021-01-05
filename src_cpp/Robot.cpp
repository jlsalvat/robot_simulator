#include "Robot.h"
#include <random>
#include <chrono>


constexpr float L_CM = 20.0;// distance entre les roues du robot = 20cm
constexpr float R_CM=3; // Rayon des roues
constexpr float VMAX = 15.0;// vitesse linéaire des roues en cm/s
constexpr float DT = 10;//période d'échantillonnage en MS 100ms
constexpr float D_CM = VMAX * DT / 1000; // distance en cm parcourue pendant DT à vitesse max
constexpr int POS_Y_CAPTEUR = 33;
constexpr int POS_X_CAPTEUR = 30;
constexpr int TAILLE_TRAINE = 10000;
constexpr int PWM_MAX = 100;
constexpr int SIZE_ROBOT_H = 120; //en pixel
constexpr int SIZE_ROBOT_W = 116; //en pixel
using namespace std;

Robot::Robot(MapRobot &map,SDL_Texture * image, Point position_robot,Direction direction_robot, std::vector<float> const &position_capteurs) :
	_texture_robot(image, &SDL_Rect{ 0,0,SIZE_ROBOT_W,SIZE_ROBOT_H }), _capteurs(map, image, position_capteurs),
	_tab_traine(TAILLE_TRAINE),_tab_traine_roue_droite(TAILLE_TRAINE),_tab_traine_roue_gauche(TAILLE_TRAINE)
{
	init(position_robot, direction_robot);

}
void Robot::init(Point position_robot, Direction direction_robot) {
	_direction_robot=direction_robot;
	_position_robot=position_robot;
	_position_roue_gauche.x_cm = position_robot.x_cm;
	_position_roue_gauche.y_cm = position_robot.y_cm + L_CM / 2;
	_position_roue_droite.x_cm = position_robot.x_cm;
	_position_roue_droite.y_cm = position_robot.y_cm - L_CM / 2;
	switch (_direction_robot) {
	case Direction::NORTH: _theta = M_PI / 2.0; break;
	case Direction::EST: _theta = -M_PI; break;
	case Direction::WEST: _theta = 0; break;
	case Direction::SUD: _theta = -M_PI / 2.0; break;
	}
	default_random_engine re(chrono::system_clock::now().time_since_epoch().count());
	uniform_int_distribution<int> distrib{ 5,8 };
	_seuil_moteur_gauche = distrib(re);
	_seuil_moteur_droit = distrib(re);

}
void Robot::stop() {
	sens(true, true);
	update(0, 0);
}
Rectangle Robot::getRectangle() {
	int x = Parameters::convWidthCmToPix(_position_robot.x_cm);
	int y = Parameters::convHeightCmToPix(_position_robot.y_cm);
	Rectangle rect{ x - SIZE_ROBOT_W / 2,y - SIZE_ROBOT_H / 2,SIZE_ROBOT_W ,SIZE_ROBOT_H };
	return rect;
}

void Robot::traine_aff(Graphics & graphics)
{
	SDL_SetRenderDrawColor(graphics.gRenderer(),0xFF, 0, 0x00,255);
	if (_indice_traine < TAILLE_TRAINE)
	{
		for (int i = 0; i < _indice_traine; i++)
		{
			SDL_RenderDrawPoint(graphics.gRenderer(), _tab_traine[i].x, _tab_traine[i].y);
		}
		SDL_SetRenderDrawColor(graphics.gRenderer(), 0xFF, 0xFF, 0x00, 255);
		for (int i = 0; i < _indice_traine; i++)
		{
			SDL_RenderDrawPoint(graphics.gRenderer(), _tab_traine_roue_droite[i].x, _tab_traine_roue_droite[i].y);
			SDL_RenderDrawPoint(graphics.gRenderer(), _tab_traine_roue_gauche[i].x, _tab_traine_roue_gauche[i].y);
		}
	}
}

void Robot::traine_update() {
	int x = Parameters::convWidthCmToPix(_position_robot.x_cm);
	int y = Parameters::convHeightCmToPix(_position_robot.y_cm);
	_tab_traine[_indice_traine].x = x;
	_tab_traine[_indice_traine].y = y;
	_tab_traine_roue_droite[_indice_traine].x = Parameters::convWidthCmToPix(_position_roue_droite.x_cm);
	_tab_traine_roue_droite[_indice_traine].y = Parameters::convHeightCmToPix(_position_roue_droite.y_cm);
	_tab_traine_roue_gauche[_indice_traine].x = Parameters::convWidthCmToPix(_position_roue_gauche.x_cm);
	_tab_traine_roue_gauche[_indice_traine].y = Parameters::convHeightCmToPix(_position_roue_gauche.y_cm);
	_indice_traine++;
	if (_indice_traine > TAILLE_TRAINE)
		_depassement = true;
}


void Robot::update(int pwm_gauche, int pwm_droit) {
	cinematics(pwm_gauche, pwm_droit);
	//traine_update();
	_capteurs.update(_position_robot.x_cm, _position_robot.y_cm, _theta);

}
void Robot::draw(Graphics & graphics) {
	int theta_deg = -_theta * 180.0 / M_PI;
	int x = Parameters::convWidthCmToPix(_position_robot.x_cm);
	int y = Parameters::convHeightCmToPix(_position_robot.y_cm);

	_texture_robot.draw(graphics,x- SIZE_ROBOT_W / 2, y- SIZE_ROBOT_H / 2, theta_deg);
	_capteurs.draw(graphics);
	//traine_aff(graphics);
}



void Robot::cinematics(int &pwm_gauche, int &pwm_droit) {
	//https://globaljournals.org/GJRE_Volume14/1-Kinematics-Localization-and-Control.pdf
		static float phi_L, phi_R;
		static float d_s, d_theta;
		static float VL, VR;
		//prise en compte de la linéarité
		if (abs(pwm_droit) < _seuil_moteur_droit)
			VR = 0;
		else
			VR = pwm_droit;
		if (abs(pwm_gauche) < _seuil_moteur_gauche)
			VL = 0;
		else
			VL = pwm_gauche;
		//mise à jour du sens des roues
		VL = (_sens_av_l ? VL : -PWM_MAX + VL);
		VR = (_sens_av_r ? VR : -PWM_MAX + VR);
		// mise à jour de l'angle des roues Left et right à chaque DT
		phi_L = VL / (float)PWM_MAX*D_CM / R_CM * 2 * M_PI;
		phi_R = VR / (float)PWM_MAX *D_CM / R_CM * 2 * M_PI;
		// mise à jour de la distance parcourue par le robot
		d_s = R_CM / 2 * (phi_L + phi_R);
		d_theta = R_CM / L_CM * (phi_R - phi_L);
		// mise à jour des positions en x et y du robot en cm
		_position_robot.x_cm= _position_robot.x_cm + (d_s * cos(_theta));
		_position_robot.y_cm = _position_robot.y_cm + (d_s * sin(_theta));
		//mise à jour des positions des roues
		_position_roue_gauche.x_cm= _position_roue_gauche.x_cm + (R_CM*phi_L * cos(_theta));
		_position_roue_droite.x_cm = _position_roue_droite.x_cm + (R_CM*phi_R  * cos(_theta));
		_position_roue_droite.y_cm = _position_roue_droite.y_cm + (R_CM*phi_R * sin(_theta));
		_position_roue_gauche.y_cm = _position_roue_gauche.y_cm + (R_CM*phi_L * sin(_theta));
		//mise à jour de l'angle
		_theta = _theta + d_theta;
}