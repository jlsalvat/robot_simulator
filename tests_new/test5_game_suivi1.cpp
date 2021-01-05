#include "../src_cpp/Game.h"
#include "../src_cpp/Fps.h"

// Initialisation
using namespace std;
enum State { STOP, AVANT, FIN};
State state = State::STOP;
bool running = true;
float positions_capteurs[] = { 2,10 };

Game game("entrainement", positions_capteurs, 2);

int jack, fdc;
float *capteurs;
int pwm_l, pwm_r;

void state_machine() {
	static int count = 0;
	switch (state) {
	case State::STOP:
		if (jack) {
			state = State::AVANT;
			pwm_l = 40;
			pwm_r = 40;
		}
		break;
	case State::AVANT:
		if (capteurs[0] > 0.5) {
			pwm_l = 30;
			pwm_r = 10;
		}
		if (capteurs[1] > 0.5) {
			pwm_l = 10;
			pwm_r = 30;
		}
		if (fdc)
			state = State::FIN;
			
		break;
	case State::FIN:		
		pwm_l = 0;
		pwm_r = 0;	
		break;
	}
	game.setPWM(pwm_l, pwm_r);  
	game.setSens(true, true);
}
int main(int argc, char* args[])
{

	//Fps fps(game.getGraphics());
	while (running) {
		running = game.eventHandler();
		capteurs = game.getCapteurs();
		jack = game.getJack();
		fdc = game.getFDC();
		cout << "fdc=" << fdc << "  ";
		cout << "jack=" << jack << "  ";
		for (int i = 0; i < 2; i++) {
			cout << capteurs[i] << " ";
		}
		cout << "pwm_l=" << pwm_l << "  ";
		cout << "pwm_r=" << pwm_r << "  ";
		cout << endl;
		state_machine();
		game.update();
		//fps.update();
		game.draw(false);
		if (game.isKeyPressed(SDLK_r))
			state = State::STOP;

		//fps.draw(0, game.getScreenSize().y - 20);
	}



	return 0;
}


