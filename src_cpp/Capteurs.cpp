#include "Capteurs.h"
#include "Parameters.h"
using namespace std;



Capteurs::Capteurs(MapRobot &map, SDL_Texture * image, vector<float> const &position) :_texture_capteur_on(image, &SDL_Rect{ 625,500,5,3 }), _texture_capteur_off(image, &SDL_Rect{ 750,500,5,3 }),
_texture_rectangle_capteur(image, &SDL_Rect{ 875,500,10,70 }), _positions_capteurs_base_cm(position), _circle_rectangle_capteurs(),_mapRobot(map), _value_capteurs(position.size())
{
	Cross cross;
	Point point{ 0,0 };
	for (int i = 0; i < _positions_capteurs_base_cm.size(); i++) {
		_position_capteurs.push_back(point);
		_cross_capteurs.push_back(cross);
		_capteurs_on.push_back(false);
	}
}
void Capteurs::update(float x_cm, float y_cm, float theta) {
	_position_rectangle_capteur.x_cm = x_cm + 9 * cos(-theta)+ 0.5*sin(-theta);
	_position_rectangle_capteur.y_cm = y_cm - 9 * sin(-theta)+ 0.5*cos(-theta);
	//_position_rectangle_capteur.x_cm = x_cm + 8 * cos(-theta);// +(-7)*sin(-theta);
	//_position_rectangle_capteur.y_cm = y_cm - 8 * sin(-theta);// +(-7)*cos(-theta);
	for (int i = 0; i < _position_capteurs.size(); i++) {
		_position_capteurs[i].x_cm = x_cm + 9 * cos(-theta) + (-6 + _positions_capteurs_base_cm[i])*sin(-theta);
		_position_capteurs[i].y_cm = y_cm - 9 * sin(-theta) + (-6 + _positions_capteurs_base_cm[i])*cos(-theta);
		_cross_capteurs[i].update(Parameters::convWidthCmToPix(_position_capteurs[i].x_cm), Parameters::convHeightCmToPix(_position_capteurs[i].y_cm));
	}
	_theta_deg = -theta * 180.0 / M_PI;
	collideWith();
}
void Capteurs::draw(Graphics &graphics) {
	_circle_rectangle_capteurs.draw(graphics);//on affiche le cercle associé aux capteurs
	_texture_rectangle_capteur.draw(graphics, _position_rectangle_capteur.x_cm-1, _position_rectangle_capteur.y_cm+7, _theta_deg);
	for (int i = 0; i < _capteurs_on.size(); i++)
	{
		if (_capteurs_on[i])
				_texture_capteur_on.draw(graphics, _position_capteurs[i].x_cm, _position_capteurs[i].y_cm, _theta_deg);
			else
				_texture_capteur_off.draw(graphics, _position_capteurs[i].x_cm, _position_capteurs[i].y_cm, _theta_deg);
	}
}
void Capteurs::collideWith() {

	// on fait d'abord un test grossier entre le cercle qui engloble tous les capteurs et les rectangles des lignes et des arcs
	//on crée un cercle autour du rectangle capteur
	SDL_Point point_centre = Parameters::conv_point_to_SDL_Point(_position_rectangle_capteur);
	_circle_rectangle_capteurs.center(point_centre);
	_circle_rectangle_capteurs.radius(35);
	vector<int> collision_rect;
	vector<int> collision_arc;
	int numero_ligne;
	Rectangle rect_ligne, rect_arc;
	int size_capteurs = _cross_capteurs.size();
	for (int i = 0; i < size_capteurs; i++) 
		_value_capteurs[i] = 0;
	// il peut y avoir plusieurs collision pour les rectangles.
	for (numero_ligne = 0; numero_ligne < _mapRobot.lignes_rect.size(); numero_ligne++) {
		rect_ligne = _mapRobot.lignes_rect[numero_ligne];
		if (_circle_rectangle_capteurs.collideWith(rect_ligne))
			collision_rect.push_back(numero_ligne);
	}
	// il peut y avoir plusieurs collision pour les arcs
	int numero_arc;
	for (numero_arc = 0; numero_arc < _mapRobot.arcs_rect.size(); numero_arc++) {
		rect_arc = _mapRobot.arcs_rect[numero_arc];
		if (_circle_rectangle_capteurs.collideWith(rect_arc))
			collision_arc.push_back(numero_arc);
	}
	//puis on fait un test fin pour chaque capteur
	vector<Rectangle> cross_capteur_rect(size_capteurs);
	vector <int> collision_multiples(size_capteurs);
	//pour les rectangles
	if (collision_rect.size() > 0) {
		for (int i = 0; i < size_capteurs; i++) {
			_value_capteurs[i] = 0;
			cross_capteur_rect[i]=_cross_capteurs[i].getRectangle();
			collision_multiples[i] = 0;
		}
		for (int i = 0; i < collision_rect.size(); i++) {//pour toutes les lignes en collision
			for (int j = 0; j < size_capteurs; j++) {
				if (cross_capteur_rect[j].intersect(_mapRobot.lignes_rect[collision_rect[i]])) {
					collision_multiples[j]++;
					if(collision_multiples[j]>1)
						_value_capteurs[j] += _cross_capteurs[j].collidesWithBorder(_mapRobot.lignes_rect[collision_rect[i]]); //ajout non testé
					else
						_value_capteurs[j] = _cross_capteurs[j].collidesWith(_mapRobot.lignes_rect[collision_rect[i]]);

				}
			}
		}
	}
	//pour les arcs
	if (collision_arc.size() > 0) {
		for (int i = 0; i < collision_arc.size(); i++) {//pour toutes les lignes en collision
			for (int j = 0; j < size_capteurs; j++) {
				int current_value = _value_capteurs[j];
				if (collision_multiples[j] > 0) {				
					_value_capteurs[j] += _cross_capteurs[j].collidesWith(_mapRobot.arcs[collision_arc[i]]);
				}
				else {
					_value_capteurs[j] = _cross_capteurs[j].collidesWith(_mapRobot.arcs[collision_arc[i]]);
				}
				if (current_value <= _value_capteurs[j])
					collision_multiples[j]++;
			}
		}
	}
	for (int i = 0; i < collision_multiples.size(); i++) {
		if (collision_multiples[i] > 1) {
			if (_value_capteurs[i] > 13)
				_value_capteurs[i] = 13;
			//a définir... (pb effet de bord entre 2 lignes_rect) la ligne d'intersection n'est pas compté, value_capteur est plus petite
//			postTraitement(i, _mapRobot.lignes_rect[collision_rect[0]], _mapRobot.lignes_rect[collision_rect[1]]);
		}
	}
	for (int i = 0; i < _value_capteurs.size(); i++)
		if (_value_capteurs[i] > 0)
			_capteurs_on[i] = true;
		else
			_capteurs_on[i] = false;
}

