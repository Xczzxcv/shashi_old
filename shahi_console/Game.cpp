#include <iostream>
#include <array>
#include <vector>
#include <stdio.h>
#include "Game.hpp"


namespace DraughtsGame {

Game::Game() : 
	m_whose_turn(Side::white)
{
	init_board();
}

void Game::main_loop() {
	std::vector<Move> poss_moves;
	std::vector<TakeTree> poss_takes;

	while (m_game_is_on) {
		print_board_big();

		find_poss_variants(m_whose_turn, poss_moves, poss_takes);
		std::cout << "size " << poss_takes.size() << ' ' << poss_moves.size() << std::endl;
		if (poss_takes.size()) {
			make_take(poss_takes[0]);
		}
		else if (poss_moves.size()) {
			make_move(poss_moves[0]);
		}
		else {
			std::cout << "END" << std::endl;
			m_game_is_on = false;
		}
		std::cin.get();

		m_whose_turn = static_cast<Side>(not static_cast<bool>(m_whose_turn));
	}
	print_board_big();
};

void Game::print_board_big() const {
	BoardChar curr_symbol;

	for (int i = 0; i < BOARD_SIZE * 2; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			//std::cout << i << ' ' << j << std::endl;
			switch (static_cast<const SquareContent&>(get_square_c(i / 2, j)))
			{
			case SquareContent::white_sq:
				curr_symbol = BoardChar::white_sq;
				break;
			case SquareContent::empty_sq:
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
				curr_symbol = static_cast<char> (SquareContent::empty_sq);
			}*/
			std::cout << static_cast<char>(curr_symbol) << static_cast<char>(curr_symbol);
			std::cout << static_cast<char>(curr_symbol) << static_cast<char>(curr_symbol);
		}
		std::cout << std::endl;
	}

	//for (auto i = 0; i < BOARD_SIZE * 2; i++) {
	//	for (auto j = 0; j < BOARD_SIZE * 2; j++) {
	//		std::cout << static_cast<char> (board_str[i * BOARD_SIZE + j]) << static_cast<char> (board_str[i * BOARD_SIZE + j]);
	//	}
	//	std::cout << std::endl;
	//}
}

void Game::print_board() const {
	BoardChar curr_symbol;

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			switch (static_cast<const SquareContent&>(get_square_c(i, j)))
			{
			case SquareContent::white_sq:
				curr_symbol = BoardChar::white_sq;
				break;
			case SquareContent::empty_sq:
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
			std::cout << static_cast<char>(curr_symbol) << static_cast<char>(curr_symbol);
		}
		std::cout << std::endl;
	}

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
				get_square(i, j) = SquareContent::empty_sq;
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
			if (temp_square == SquareContent::empty_sq) {
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

void Game::find_poss_variants(Side side, std::vector<Move>& poss_moves, std::vector<TakeTree>& poss_takes) {
	ForcesType we;
	ForcesType enemy;
	
	poss_moves.clear();
	poss_takes.clear();
	
	if (side == Side::white) {
		we = { SquareContent::white_dr, SquareContent::white_king };
		enemy = { SquareContent::black_dr, SquareContent::black_king };
	}
	else {
		we = { SquareContent::black_dr, SquareContent::black_king };
		enemy = { SquareContent::white_dr, SquareContent::white_king };
	}

	// find possible takes
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			SquareContent& temp_sq = get_square(i, j);
			if (temp_sq == we.draught or temp_sq == we.king) {
				find_poss_takes(i, j, poss_takes, enemy);
			}
		}
	}
	if (poss_takes.size()) { return; }	

	//find possible moves
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			SquareContent& temp_sq = get_square(i, j);
			if (temp_sq == we.draught or temp_sq == we.king) {
				find_poss_moves(i, j, poss_moves);
			}
		}
	}
}

void Game::find_poss_takes(BoardNumType y, BoardNumType x, std::vector<TakeTree>& poss_takes, const ForcesType& enemy, TakeTree* curr_tree) {
	SquareContent& curr_sq = get_square(y, x);

	BoardNumType max_range;
	if (curr_sq == SquareContent::white_dr or curr_sq == SquareContent::black_dr) {
		max_range = 1;
	}
	else {
		max_range = BOARD_SIZE - 1;
	}

	for (auto i : { -1, 1 }) {
		for (auto j : { -1, 1 }) {
			find_poss_takes_diagonal(y, x, i, j, max_range, poss_takes, enemy, curr_tree);
			//for (int k = 1; k < max_range + 1; k++) {
			//	int y_coord = y + i * k,
			//		x_coord = x + j * k;
			//	if (check_coord(y_coord, x_coord)) {
			//		if (get_square(y_coord, x_coord) == enemy.draught or get_square(y_coord, x_coord) == enemy.king) {
			//			int y_poss_empty = y + i * (k + 1),
			//				x_poss_empty = x + j * (k + 1);
			//			if (check_coord(y_poss_empty, x_poss_empty) 
			//				and get_square(y_poss_empty, x_poss_empty) == SquareContent::empty_sq)
			//			{
			//				if (curr_tree) {
			//					if (not curr_tree->check_already_taken(y_coord, x_coord)) {

			//					}
			//				}
			//				else {
			//					poss_takes.emplace_back(TakeTree(y, x));
			//					//poss_takes.back().add_take(
			//					//	TakeNode{ get_square(y, x), get_index(y, x), get_index() }
			//					//);
			//				}
			//			}
			//		}
			//	}
			//	else { break; }
			//}
		}
	}
}

void Game::find_poss_takes_diagonal(BoardNumType y, BoardNumType x, int y_coeff, int x_coeff, BoardNumType max_range, std::vector<TakeTree>& poss_takes, const ForcesType& enemy, TakeTree* curr_tree) {
	for (int k = 1; k < max_range + 1; k++) {
		int y_coord = y + y_coeff * k,
			x_coord = x + x_coeff * k;
		if (check_coord(y_coord, x_coord)) {
			if (get_square(y_coord, x_coord) == enemy.draught or get_square(y_coord, x_coord) == enemy.king) {
				int y_poss_empty = y + y_coeff * (k + 1),
					x_poss_empty = x + x_coeff * (k + 1);
				if (check_coord(y_poss_empty, x_poss_empty)
					and get_square(y_poss_empty, x_poss_empty) == SquareContent::empty_sq)
				{
					if (curr_tree) {
						if (not curr_tree->check_already_taken(y_coord, x_coord)) {

						}
					}
					else {
						poss_takes.emplace_back(TakeTree(y, x));
						//poss_takes.back().add_take(
						//	TakeNode{ get_square(y, x), get_index(y, x), get_index() }
						//);
					}
				}
			}
		}
		else { break; }
	}
}

void Game::check_for_add_taking(BoardNumType y, BoardNumType x, )

void Game::find_poss_moves(BoardNumType y, BoardNumType x, std::vector<Move>& poss_moves) {
	SquareContent& curr_sq = get_square(y, x);
	if (curr_sq == SquareContent::white_dr or curr_sq == SquareContent::black_dr) {
		int row_add;
		if (curr_sq == SquareContent::white_dr) { row_add = -1; }
		else { row_add = 1; }
		
		for (auto j : { -1, 1 }) {
			int y_coord = y + row_add,
				x_coord = x + j;
			if (check_coord(y_coord, x_coord)) {
				if (get_square(y_coord, x_coord) == SquareContent::empty_sq) {
					poss_moves.push_back(
						Move{ curr_sq, get_index(y, x), get_index(y_coord, x_coord) }
					);
				}
			}
		}
	}
	else {
		for (auto i : { -1, 1 }) {
			for (auto j : { -1, 1 }) {
				for (int k = 0; k < BOARD_SIZE - 1; k++) {
					int y_coord = y + i * k,
						x_coord = x + j * k;
					if (check_coord(y_coord, x_coord)) {
						if (get_square(y_coord, x_coord) == SquareContent::empty_sq) {
							poss_moves.push_back(
								Move{ curr_sq, get_index(y, x), get_index(y_coord, x_coord) }
							);
						}
						else { break; }
					}
					else { break; }
				}
			}
		}
	}
}

void Game::make_move(Move& move)
{
	m_board[move.to] = m_board[move.from];
	m_board[move.from] = SquareContent::empty_sq;
}

void Game::make_take(TakeTree& take_tree) {

}

// invalid 'y' and 'x' may cause memory corruption and therefore error occurrence
inline SquareContent& Game::get_square(BoardNumType  y, BoardNumType x) {
	return m_board[get_index(y, x)];
}

inline const SquareContent& Game::get_square_c(BoardNumType y, BoardNumType x) const{
	return m_board[y * BOARD_SIZE + x];
}

inline BoardNumType Game::get_index(BoardNumType y, BoardNumType x) {
	return y * BOARD_SIZE + x;
}

inline bool Game::check_coord(BoardNumType y, BoardNumType x) {
	return (0 <= y and y < BOARD_SIZE) and (0 <= x and x < BOARD_SIZE);
}

}
