#include <iostream>
#include "Game.hpp"

int main() {
	std::cout << "HEKLIO" << std::endl;
	DraughtsGame::Game g;

	g.main_loop();

	//for (int i = 0; i < 255; i++) {
	//	std::cout << i << ' ' << static_cast<char> (i) << std::endl;
	//}

}

//
//176 black
//178 white