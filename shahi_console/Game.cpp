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
		print_board(true);

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
	print_board(true);
};

void Game::print_board(bool is_big) const {
	enum class BoardChar : unsigned char {
		white_sq = 219, white_dr = '0', black_dr = '-',
		black_sq = 176, white_k = 'W', black_k = 'B'
	};

	BoardChar curr_char;
	BNT i_range = (is_big) ? BOARD_SIZE * 2 : BOARD_SIZE,
		char_num = (is_big) ? 4 : 2;

	for (BNT i = 0; i < i_range; i++) {
		for (BNT j = 0; j < BOARD_SIZE; j++) {
			Pos temp_shit = (is_big) ? Pos{ i / 2, j } : Pos{ i, j };
			switch (static_cast<const SquareContent&>(get_square_c(temp_shit)))
			{
			case SquareContent::white_sq:
				curr_char = BoardChar::white_sq;
				break;
			case SquareContent::empty_sq:
				curr_char = BoardChar::black_sq;
				break;
			case SquareContent::white_dr:
				curr_char = BoardChar::white_dr;
				break;
			case SquareContent::black_dr:
				curr_char = BoardChar::black_dr;
				break;
			case SquareContent::white_king:
				curr_char = BoardChar::white_k;
				break;
			case SquareContent::black_king:
				curr_char = BoardChar::black_k;
				break;
			}
			for (BNT k = 0; k < char_num; k++) {
				std::cout << static_cast<char>(curr_char);
			}
		}
		std::cout << std::endl;
	}
}

BNT Game::position_analysis() {
	return 0;
}

void Game::AI() {

}

void Game::init_board() {
	for (BNT i = 0; i < BOARD_SIZE; i++) {
		for (BNT j = 0; j < BOARD_SIZE; j++) {
			if ((i + j) % 2) {
				get_square(Pos{ i, j }) = SquareContent::empty_sq;
			}
			else {
				get_square(Pos{ i, j }) = SquareContent::white_sq;
			}
		}
	}

	// populate board with draughts
	for (BNT i = 0; i < BOARD_SIZE; i++) {
		for (BNT j = 0; j < BOARD_SIZE; j++) {
			SquareContent& temp_square = get_square(Pos{ i, j });
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
	for (BNT i = 0; i < BOARD_SIZE; i++) {
		for (BNT j = 0; j < BOARD_SIZE; j++) {
			SquareContent& temp_sq = get_square(Pos{ i, j });
			if (temp_sq == we.draught or temp_sq == we.king) {
				find_poss_takes(Pos{ i, j }, poss_takes, enemy);
			}
		}
	}
	if (poss_takes.size()) { return; }	

	//find possible moves
	for (BNT i = 0; i < BOARD_SIZE; i++) {
		for (BNT j = 0; j < BOARD_SIZE; j++) {
			SquareContent& temp_sq = get_square(Pos{ i, j });
			if (temp_sq == we.draught or temp_sq == we.king) {
				find_poss_moves(Pos{ i, j }, poss_moves);
			}
		}
	}
}

void Game::find_poss_takes(Pos pos, std::vector<TakeTree>& poss_takes, const ForcesType& enemy) {
	SquareContent& curr_sq = get_square(pos);

	BNT max_range;
	if (curr_sq == SquareContent::white_dr or curr_sq == SquareContent::black_dr) {
		max_range = 1;
	}
	else {
		max_range = BOARD_SIZE - 1;
	}

	for (BNT i : { -1, 1 }) {
		for (BNT j : { -1, 1 }) {
			find_poss_takes_diagonal(pos, Vect2{ i, j }, max_range, poss_takes, enemy);
		}
	}
}

void Game::find_poss_takes_diagonal(Pos pos, Vect2 coeffs, BNT max_range, std::vector<TakeTree>& poss_takes, const ForcesType& enemy) {
	Pos coord_enemy;
	
	for (BNT k = 1; k < max_range + 1; k++) {
		coord_enemy = { pos.y + coeffs.y * k, pos.x + coeffs.x * k };
		if (check_coord(coord_enemy)) {
			if (get_square(coord_enemy) == enemy.draught 
				or get_square(coord_enemy) == enemy.king)
			{
				Vect2 poss_empty{ pos.y + coeffs.y * (k + 1), pos.x + coeffs.x * (k + 1) };
				if (check_coord(coord_enemy)
					and get_square(coord_enemy) == SquareContent::empty_sq)
				{
					poss_takes.emplace_back(TakeTree(pos));
					check_for_takings(pos, coord_enemy, max_range, poss_takes.back());
				}
			}
		}
		else { break; }
	}
}

void Game::check_for_takings(Pos me, Pos to_take, BNT max_range, TakeTree& tree) {
	Vect2 diag_coeff { 
		(to_take.y - me.y) / static_cast<BNT>(abs(to_take.y - me.y)), 
		(to_take.x - me.x) / static_cast<BNT>(abs(to_take.x - me.x))
	},
		iter_coeffs[] { Vect2{-1, 1}, Vect2{1, -1} };
	std::vector<Pos> enemy_ind(max_range, Pos{-1, -1});

	for (BNT i = 1; i < max_range + 1; i++) {
		for (auto& coeffs : iter_coeffs) {
			for (BNT k = 1; k < max_range + 1; k++) {
				SquareContent& to_check = get_square(
					Pos{ to_take.y + diag_coeff.y + coeffs.y, to_take.x + diag_coeff.x }
				);
			}
		}
	}
}

void Game::find_poss_moves(Pos pos, std::vector<Move>& poss_moves) {
	SquareContent& curr_sq = get_square(pos);
	if (curr_sq == SquareContent::white_dr or curr_sq == SquareContent::black_dr) {
		BNT row_add;
		if (curr_sq == SquareContent::white_dr) { row_add = -1; }
		else { row_add = 1; }
		
		for (BNT j : { -1, 1 }) {
			Pos coords{ pos.y + row_add, pos.x + j };
			if (check_coord(coords)) {
				if (get_square(coords) == SquareContent::empty_sq) {
					poss_moves.push_back(
						Move{ curr_sq, get_index(pos), get_index(coords) }
					);
				}
			}
		}
	}
	else {
		for (BNT i : { -1, 1 }) {
			for (BNT j : { -1, 1 }) {
				for (BNT k = 0; k < BOARD_SIZE - 1; k++) {
					Pos coords{ pos.y + i * k, pos.x + j * k };
					if (check_coord(coords)) {
						if (get_square(coords) == SquareContent::empty_sq) {
							poss_moves.push_back(
								Move{ curr_sq, get_index(pos), get_index(coords) }
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
inline SquareContent& Game::get_square(Pos pos) {
	return m_board[get_index(pos)];
}

inline const SquareContent& Game::get_square_c(Pos pos) const{
	return m_board[pos.y * BOARD_SIZE + pos.x];
}

inline BNT Game::get_index(Pos pos) {
	return pos.y * BOARD_SIZE + pos.x;
}

inline bool Game::check_coord(Pos pos) {
	return (0 <= pos.y and pos.y < BOARD_SIZE) and (0 <= pos.x and pos.x < BOARD_SIZE);
}

}
