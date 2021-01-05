#include "Game.h"
#include "Sprite.h"
#include "Text.h"
#include <iostream>


using namespace std;
enum class State_chronometre : unsigned int { stop, start, pause };
enum class State_course : unsigned int { stop, course, barre };
SDL_Rect rect_src_total = { 0,0,32 * 8,32 };
State_chronometre state_chronometre;
State_course state_course;

const static string map_base("../maps/");

Game::Game(string map, float positions_capteurs_cm[], int nb_capteurs) :_sdl(), _graphics(), _map_shapes(_graphics), _map(_graphics, map_base + map + ".tmx", _map_shapes.getLignes(), _map_shapes.getArcs()),
_positions_capteurs_cm(positions_capteurs_cm, positions_capteurs_cm + nb_capteurs),
_robot(_map, _map_shapes.getImageTexture(), _map.getRobotCenter(), _map.getRobotDirection(), _positions_capteurs_cm),
_barre(_map.getBarre()),
_coins(_graphics, &rect_src_total, _map.getAnimationCoins()),
_font("arial", 40),
_miniFont("arial",20),
_pwm_l(0),
_pwm_r(0),
_sens_l(true),
_sens_r(true),
_counter_coins(0)
{
	state_chronometre = State_chronometre::stop;
	state_course = State_course::stop;
	_size = _map.getSize();
	_graphics.resize(_size.x, _size.y);
	//fond en gris (moquette)
	SDL_SetRenderDrawColor(_graphics.gRenderer(), 100, 100, 100, 0);
	_text_state = "stop";
	_text_jeton = "jetons :"+ to_string(_counter_coins);
	_text_score = _tim.getTime();
}

bool Game::isKeyPressed(SDL_Keycode key) {
	return _input.wasKeyPressed(key);
}

bool Game::eventHandler()
{
    int running = true;
	SDL_Event event;
	_input.beginNewFrame();
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			_input.keyDownEvent(event);
			break;
		case SDL_KEYUP:
			_input.keyUpEvent(event);
			break;
		case SDL_QUIT:
			running = false;
			break;
		default:
			break;
		}
	}
	if (_input.wasKeyPressed(SDLK_q))
	{
		running = false;
	}
	else if (_input.wasKeyPressed(SDLK_ESCAPE))
	{
		running = false;
	}
	else if (_input.wasKeyPressed(SDLK_SPACE))
	{
		_jack = true;
		if (state_chronometre == State_chronometre::stop) {
			state_chronometre = State_chronometre::start;
			_tim.start();
		}
		//le chronomètre est lancé et ne peut être arrêté que par le fin de course
	/*	else
			if (state_chronometre == State_chronometre::start) {
				state_chronometre = State_chronometre::pause;
				_tim.pause();
			}
			else
				if (state_chronometre == State_chronometre::pause) {
					state_chronometre = State_chronometre::start;
					_tim.unpause();
				}*/
	}
	else if (_input.wasKeyPressed(SDLK_r))
	{
		_jack = false;
		_robot.init(_map.getRobotCenter(), _map.getRobotDirection());
		_pwm_l = _pwm_r = 0;
		_sens_l = _sens_r = true;
		_counter_coins = 0;
		state_course = State_course::stop;
		_text_state = "stop";
		_tim.reset();
		_tim.stop();
		state_chronometre = State_chronometre::stop;

	}
    if (_pwm_l == 0 && _pwm_r == 0) { // on prend la main si le robot est à l'arrêt.
	if (_input.wasKeyPressed(SDLK_RIGHT))
	{
		_robot.right();
	}
	else if (_input.wasKeyPressed(SDLK_LEFT))
	{
		_robot.left();
	}
	else if (_input.wasKeyPressed(SDLK_DOWN))
	{
		_robot.down();
	}
	else if (_input.wasKeyPressed(SDLK_UP))
	{
		_robot.up();
	}
	else if (_input.wasKeyPressed(SDLK_PAGEUP))
	{
		_robot.rotate(2);
	}
	else if (_input.wasKeyPressed(SDLK_PAGEDOWN))
	{
		_robot.rotate(-2);
	}
	}

	return running;
}

void Game::setPWM(int pwm_l, int pwm_r)
{
	_pwm_l = pwm_l;
	_pwm_r = pwm_r;

}
void Game::setSens(int sens_l, int sens_r)
{
	_sens_l = sens_l;
	_sens_r = sens_r;
	_robot.sens(sens_l, sens_r);
}
bool Game::getJack() {
	return _jack;
}
bool Game::getFDC() {
	return _fdc;
}


float * Game::getCapteurs() {
	_capteurs = _robot.getCapteurs();
	for (int i = 0; i < _capteurs.size(); i++)
		_capteurs[i] /= 13.0; //on remet le capteur entre 0 et 100%
	//for (int i = 0; i < capteurs.size(); i++) {
	//	cout << capteurs[i] << " ";
	//}
	//cout << endl;
	return &_capteurs[0];
}



void Game::update() {
	_fdc = _barre.collidesWith(_robot.getRectangle());
	if(_fdc)
		_tim.pause();
	_robot.update(_pwm_l, _pwm_r);
	_coins.update();
	course_update();
}

void Game::course_update() {
	if (_coins.collideWith(_robot.getRectangle())) {
		_counter_coins++;
		_text_jeton = "jetons :" + to_string(_counter_coins);
	}
	switch (state_course) {
	case State_course::stop:
		if (_jack) {
			state_course = State_course::course;
			_text_state = "course";
		}
		break;
	case State_course::course:		
		if (_fdc) {
			state_course = State_course::barre;
			_text_state = "fin";
		}
		break;
	case State_course::barre:
		if (_counter_coins == _coins.getNbCoins())
			calcul_gain(true);
		else
			calcul_gain(false);
		break;
	}
}

void Game::calcul_gain(bool all_coins) {
	if (all_coins) {
		_text_score="TIME : " + _tim.getTime();
	}
	else {
		_text_score = "TIME : " + to_string(180.0);
	}

}
void Game::drawMessage() {
	//affichage des messages Go, Course, End, Reset
}

void Game::draw(bool all)
{
	SDL_SetRenderDrawColor(_graphics.gRenderer(), 120, 120, 120, 255);
	_graphics.clear();
	Text text_time(_graphics, _tim.getTime().c_str(), _font, SDL_Color{230,230,100 });
	text_time.draw(_graphics, _size.y/2 - 50, 10);
    Text text_course(_graphics, _text_state.c_str(), _miniFont, SDL_Color{ 240,240,80 });
	text_course.draw(_graphics, _size.x / 2 - 50, _size.y - 100);
	Text text_jetons(_graphics, _text_jeton.c_str(), _miniFont, SDL_Color{ 240,240,80 });
	text_jetons.draw(_graphics, 20, _size.y  - 100);
	Text text_score(_graphics, _text_score.c_str(), _miniFont, SDL_Color{ 240,240,80 });
	text_score.draw(_graphics, _size.x -150, _size.y - 100);
	_map.draw(_graphics,all);
	_coins.draw(_graphics);
	_robot.draw(_graphics);
	drawMessage();
	//Update screen
	_graphics.flip();

}



