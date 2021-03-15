#include <iostream>
#include <array>
#include "Game.hpp"


namespace DraughtsGame {

Game::Game() : 
	whose_turn(Side::white)
{
	init_board();
}

void Game::main_loop() {
	std::array<Move, MAX_MOVES> poss_moves;

	while (game_is_on) {
		print_board();

		find_poss_moves(poss_moves);
		if (not poss_moves.size()) {
			game_is_on = false;
		}

		make_move(poss_moves[0]);

		std::cin.get();
	}
	print_board();
};

GameStatus Game::is_ended() const {
	return GameStatus{ true, Side::white };
}

void Game::print_board() const {
	BoardChar curr_symbol;

	for (int i = 0; i < BOARD_SIZE * 2; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			//std::cout << i << ' ' << j << std::endl;
			switch (m_board[(i / 2) * BOARD_SIZE + j])
			{
			case SquareContent::white_sq:
				curr_symbol = BoardChar::white_sq;
				break;
			case SquareContent::black_sq:
				curr_symbol = BoardChar::black_sq;
				break;
			case SquareContent::white_dr:
				curr_symbol = BoardChar::white_dr;
				break;
			case SquareContent::black_dr:
				curr_symbol = BoardChar::black_dr;
				break;
			case SquareContent::white_king:
				curr_symbol = BoardChar::white_k;
				break;
			case SquareContent::black_king:
				curr_symbol = BoardChar::black_k;
				break;
			}
			//curr_symbol = static_cast<int> (m_board[i * BOARD_SIZE + j]);
			/*if ((i / 2 + j) % 2) {
				curr_symbol = static_cast<char> (SquareContent::white_sq);
			}
			else {
				curr_symbol = static_cast<char> (SquareContent::black_sq);
			}*/
			std::cout << static_cast<char>(curr_symbol) << static_cast<char>(curr_symbol);
			std::cout << static_cast<char>(curr_symbol) << static_cast<char>(curr_symbol);
		}
		std::cout << std::endl;
	}
	std::cout << "XXXX" << std::endl;

	//for (auto i = 0; i < BOARD_SIZE * 2; i++) {
	//	for (auto j = 0; j < BOARD_SIZE * 2; j++) {
	//		std::cout << static_cast<char> (board_str[i * BOARD_SIZE + j]) << static_cast<char> (board_str[i * BOARD_SIZE + j]);
	//	}
	//	std::cout << std::endl;
	//}
}

int Game::position_analysis() {
	return 0;
}

void Game::AI() {

}

void Game::init_board() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if ((i + j) % 2) {
				get_square(i, j) = SquareContent::black_sq;
			}
			else {
				get_square(i, j) = SquareContent::white_sq;
			}
		}
	}

	// populate board with draughts
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			SquareContent& temp_square = get_square(i, j);
			if (temp_square == SquareContent::black_sq) {
				if (i < ENGAGED_LINES) {
					temp_square = SquareContent::black_dr;
				}
				else if (i > BOARD_SIZE - ENGAGED_LINES - 1) {
					temp_square = SquareContent::white_dr;
				}
			}
		}
	}
	std::cout << std::endl;
}

inline SquareContent& Game::get_square(int y, int x) {
	return m_board[y * BOARD_SIZE + x];
}

}
