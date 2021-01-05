#pragma once
#include "../src_cpp/LTexture.h"
#include "Shape.h"
#include "../src_cpp/Utils.h"
#include "../src_cpp/MapRobot.h"
#define _USE_MATH_DEFINES 
#include <math.h>
#include <vector>
class Capteurs {
	public : 
		//on passe la texture qui vient de map.getImage ainsi que la position des différents capteurs
		Capteurs(MapRobot &map, SDL_Texture * image, std::vector<float> const &position);
		//calcul de la distance avec une ligne
/*		float distance(SDL_Point const &p1, SDL_Point const &p2) {
			int m_A;
			int m_B;
			int m_C;
			m_A = p1.y - p2.y;
			m_B = p2.x - p1.x;
			m_C = p1.x * p2.y - p2.x * p1.y;
			for (int i = 0; i < _position_capteurs.size(); i++) {
				_position_capteurs[i].x = x_cm + 6.7 * cos(theta) + (-7 + _positions_capteurs_base_cm[i])*sin(theta);
				_position_capteurs[i].y = y_cm - 6.7 * sin(theta) + (-7 + _positions_capteurs_base_cm[i])*cos(theta);
			}
			return abs(m_A * _x + m_B * _y + m_C);
		}*/
		//calcule la nouvelle position du rectangle et des capteurs en fonction de la position du centre du robot
		void  update(float x, float y, float theta);
		void draw(Graphics &graphics);
		std::vector<float> getValues() {
			return _value_capteurs;
		}

	private :
		void collideWith();
		void postTraitement(int indice, const Rectangle &other1, const Rectangle & other2) {

		}
		const MapRobot &_mapRobot;
		Circle _circle_rectangle_capteurs;
		LTexture _texture_capteur_on, _texture_capteur_off, _texture_rectangle_capteur;
		Point _position_rectangle_capteur;
		std::vector <Point> _position_capteurs;
		std::vector <Cross> _cross_capteurs;
		std::vector <float> _positions_capteurs_base_cm;
		std::vector<float> _value_capteurs;
		std::vector <bool> _capteurs_on;
		int _theta_deg;
};