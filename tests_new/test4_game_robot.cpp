#include "../src_cpp/Game.h"
#include "../src_cpp/Fps.h"

// Initialisation
using namespace std;
enum State{STOP,AVANT,ARRIERE};
State state = State::STOP;
bool running = true;
float positions_capteurs[] = { 6,10 };
float *capteurs;
Game game("course2",positions_capteurs, 2);

void state_machine() {
	static int count = 0;
	switch (state) {
	case State::STOP:
		if (game.isKeyPressed(SDLK_s))
			state = State::AVANT;
		break;
	case State::AVANT:
		if (game.isKeyPressed(SDLK_s))
			state = State::STOP;
		count++;
		if (count > 100) {
			state = State::ARRIERE;
			count = 0;
		}
		break;
	case State::ARRIERE:
		if (game.isKeyPressed(SDLK_s))
			state = State::STOP;
		count++;
		if (count > 100) {
			state = State::AVANT;
			count = 0;
		}
		break;
	}
	switch (state) {
	case STOP: game.setPWM(0, 0);  game.setSens(true, true); break;
	case AVANT:game.setPWM(30, 30); game.setSens(true, true); break;
	case ARRIERE: game.setPWM(70, 70); game.setSens(false, false); break;
	}
}
int main(int argc, char* args[])
{

	//Fps fps(game.getGraphics());
	while (running) {
		running = game.eventHandler();
		state_machine();
		capteurs=game.getCapteurs();
		int jack = game.getJack();
		int fdc = game.getFDC();
		cout << "fdc=" << fdc << "  ";
		cout << "jack=" << jack << "  ";
		for (int i = 0; i < 2; i++) {
			cout << capteurs[i] << " ";
		}
		cout << endl;
		game.update();
		//fps.update();
		game.draw(false);
		//fps.draw(0, game.getScreenSize().y - 20);
	}



    return 0;
}


