#include <iostream>
#include "Game.hpp"

int main() {
	std::cout << "HEKLIO" << std::endl;
	DraughtsGame::Game g;

	g.main_loop();

	//int max = 100;
	//int w = 10;
	//for (int i = 0; i < max ; ++i) {
	//	std::string arr;
	//	for (int j = 0; j < w; ++j) {
	//		arr.append(1, i * w + j);
	//		//arr.append(1, ' ');
	//	}
	//	std::cout << i * w << "-" << i * w + w - 1 << "|" << arr << "|" << std::endl;
	//	arr.clear();
	//}

	//std::cout << "tire: " << static_cast<char>(196) << std::endl;

}

//
//176 black
//178 white
