#include <array>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "Game.hpp"

namespace DraughtsGame {

Game::Game() : 
	m_whose_turn(Side::white)
{
	init_board();
	std::srand(std::time(nullptr));
}

void Game::main_loop() {
	std::vector<Move> poss_moves;
	std::vector<TakeList> poss_takes;

	SqContent e = SqContent::empty_sq, s = SqContent::white_sq,
		b = SqContent::black_dr, B = SqContent::black_king,
		w = SqContent::white_dr, W = SqContent::white_king;
	std::array<SqContent, BOARD_SQUARES_NUM> new_board{
		s,e,s,e,s,e,s,e,
		e,s,b,s,e,s,e,s,
		s,w,s,e,s,b,s,e,
		e,s,e,s,e,s,e,s,
		s,b,s,e,s,e,s,e,
		e,s,e,s,b,s,e,s,
		s,e,s,e,s,e,s,e,
		e,s,e,s,e,s,e,s,
		//s,e,s,e,s,e,s,e,
		//e,s,e,s,e,s,e,s,
		//s,e,s,e,s,b,s,e,
		//e,s,e,s,e,s,e,s,
		//s,e,s,b,s,b,s,e,
		//e,s,e,s,e,s,e,s,
		//s,b,s,b,s,b,s,e,
		//w,s,e,s,e,s,e,s,
		//
		//s,e,s,e,s,e,s,e,
		//e,s,b,s,b,s,b,s,
		//s,e,s,e,s,e,s,e,
		//e,s,b,s,e,s,e,s,
		//s,w,s,e,s,b,s,e,
		//e,s,e,s,e,s,e,s,
		//s,b,s,e,s,e,s,e,
		//e,s,e,s,e,s,e,s,
	};

	set_state(new_board, Side::white);

	while (m_game_is_on) {
		print_board(true);

		find_poss_variants(m_whose_turn, poss_moves, poss_takes);
		std::cout << "size " << poss_takes.size() << ' ' << poss_moves.size() << std::endl;
		if (poss_takes.size()) {
			for (BNT i = 0; i < poss_takes.size(); ++i) {
				TakeList& curr_takelist = poss_takes[i];
				std::cout << i << "| " << curr_takelist << std::endl;
			}

			make_take(poss_takes[std::rand() % poss_takes.size()]);
		}
		else if (poss_moves.size()) { 
			make_move(poss_moves[std::rand() % poss_moves.size()]);
		}
		else {
			std::cout << "END" << std::endl;
			m_game_is_on = false;
			m_winner = get_opponent();
			m_winner_text = (m_whose_turn == Side::black) ? "White" : "Black";
		}
		std::cin.get();

		m_whose_turn = get_opponent();
	}
	std::cout << "Winner is.... " << m_winner_text << "!" << std::endl;
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

	for (BNT i = 0; i < i_range; ++i) {
		for (BNT j = 0; j < BOARD_SIZE; ++j) {
			Pos temp_shit = (is_big) ? Pos{ i / 2, j } : Pos{ i, j };
			switch (static_cast<const SqContent&>(get_square_c(temp_shit)))
			{
			case SqContent::white_sq:
				curr_char = BoardChar::white_sq;
				break;
			case SqContent::empty_sq:
				curr_char = BoardChar::black_sq;
				break;
			case SqContent::white_dr:
				curr_char = BoardChar::white_dr;
				break;
			case SqContent::black_dr:
				curr_char = BoardChar::black_dr;
				break;
			case SqContent::white_king:
				curr_char = BoardChar::white_k;
				break;
			case SqContent::black_king:
				curr_char = BoardChar::black_k;
				break;
			}
			for (BNT k = 0; k < char_num; ++k) {
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
	for (BNT i = 0; i < BOARD_SIZE; ++i) {
		for (BNT j = 0; j < BOARD_SIZE; ++j) {
			if ((i + j) % 2) {
				get_square(Pos{ i, j }) = SqContent::empty_sq;
			}
			else {
				get_square(Pos{ i, j }) = SqContent::white_sq;
			}
		}
	}

	// populate board with draughts
	for (BNT i = 0; i < BOARD_SIZE; ++i) {
		for (BNT j = 0; j < BOARD_SIZE; ++j) {
			SqContent& temp_square = get_square(Pos{ i, j });
			if (temp_square == SqContent::empty_sq) {
				if (i < ENGAGED_LINES) {
					temp_square = SqContent::black_dr;
				}
				else if (i > BOARD_SIZE - ENGAGED_LINES - 1) {
					temp_square = SqContent::white_dr;
				}
			}
		}
	}
	std::cout << std::endl;
}

void Game::set_state(std::array<SqContent, BOARD_SQUARES_NUM>& new_board, Side new_whose_turn) {
	m_board = new_board;
	m_whose_turn = new_whose_turn;
}

void Game::find_poss_variants(Side side, std::vector<Move>& poss_moves, std::vector<TakeList>& poss_takes) {
	ForcesType we;
	ForcesType enemy;
	
	poss_moves.clear();
	poss_takes.clear();
	
	if (side == Side::white) {
		we = { SqContent::white_dr, SqContent::white_king };
		enemy = { SqContent::black_dr, SqContent::black_king };
	}
	else {
		we = { SqContent::black_dr, SqContent::black_king };
		enemy = { SqContent::white_dr, SqContent::white_king };
	}

	// find possible takes
	for (BNT i = 0; i < BOARD_SIZE; ++i) {
		for (BNT j = 0; j < BOARD_SIZE; ++j) {
			SqContent& temp_sq = get_square(Pos{ i, j });
			if (temp_sq == we.draught or temp_sq == we.king) {
				find_poss_takes(Pos{ i, j }, poss_takes, enemy, side);
			}
		}
	}
	if (poss_takes.size()) { return; }	

	//find possible moves
	for (BNT i = 0; i < BOARD_SIZE; ++i) {
		for (BNT j = 0; j < BOARD_SIZE; ++j) {
			SqContent& temp_sq = get_square(Pos{ i, j });
			if (temp_sq == we.draught or temp_sq == we.king) {
				find_poss_moves(Pos{ i, j }, poss_moves);
			}
		}
	}
}

void Game::find_poss_takes(Pos pos, std::vector<TakeList>& poss_takes, const ForcesType& enemy, Side side) {
	SqContent& curr_sq = get_square(pos);

	BNT max_range;
	if (curr_sq == SqContent::white_dr or curr_sq == SqContent::black_dr) {
		max_range = 1;
	}
	else {
		max_range = BOARD_SIZE - 1;
	}

	for (BNT i : { -1, 1 }) {
		for (BNT j : { -1, 1 }) {
			find_poss_takes_diagonal(pos, Vect2{ i, j }, max_range, poss_takes, enemy, side);
		}
	}
}

void Game::find_poss_takes_diagonal(Pos curr_pos, Vect2 coeffs, BNT max_range, std::vector<TakeList>& poss_takes, const ForcesType& enemy, Side side) {
	Pos enemy_pos;
	
	for (BNT i = 1; i < max_range + 1; ++i) {
		enemy_pos = { curr_pos.y + coeffs.y * i, curr_pos.x + coeffs.x * i };
		if (check_coord(enemy_pos)) {
			SqContent& enemy_sq = get_square(enemy_pos);
			if (enemy_sq == enemy.draught or enemy_sq == enemy.king)
			{
				Pos curr_empty_pos{ curr_pos.y + coeffs.y * (i + 1), curr_pos.x + coeffs.x * (i + 1) };
				if (check_coord(curr_empty_pos)){
					if (get_square(curr_empty_pos) == SqContent::empty_sq) {
						poss_takes.push_back(TakeList{});
						
						SqContent curr_sq;
						bool is_king_take = false;
						if (side == Side::black && curr_empty_pos.y == BOARD_SIZE - 1) {
							curr_sq = SqContent::black_king;
							is_king_take = true;
						}
						else if (side == Side::white && curr_empty_pos.y == 0) {
							curr_sq = SqContent::white_king;
							is_king_take = true;
						}
						else if (is_king(get_square(curr_pos))) {
							curr_sq = get_square(curr_pos);
							is_king_take = true;
						}

						if (is_king_take) {
							deep_take_king(curr_pos, curr_sq, enemy_pos,
								enemy, poss_takes, poss_takes.size() - 1, 0);
						}
						else {
							deep_take_dr(curr_pos, get_square(curr_pos), enemy_pos,
								enemy, poss_takes, poss_takes.size() - 1, 0);
						}
					}
				}
				else { break; }
			}
			else if (enemy_sq != SqContent::empty_sq) { break; }
		}
		else { break; }
	}
}

void Game::deep_take_dr(Pos curr_pos, SqContent curr_dr_type, Pos to_take, const ForcesType& enemy, std::vector<TakeList>& poss_takes, BNT curr_list_ind, BNT prev_list_len) {
	Vect2 diag_coeffs = make_diag_coeffs(curr_pos, to_take),
		iter_coeffs[]{
		Vect2{ -diag_coeffs.y, diag_coeffs.x },
		Vect2{ diag_coeffs.y, -diag_coeffs.x },
		Vect2{ diag_coeffs.y, diag_coeffs.x },
	};
	//TakeList curr_list = poss_takes[curr_list_ind];
	std::cout << std::endl << "(deep_take_dr) curr_pos: " << get_notation(curr_pos)
		<< " to_take: " << get_notation(to_take)
		<< " prev_list_len: " << prev_list_len
		<< std::endl;

	Pos curr_empty_pos{ to_take.y + diag_coeffs.y, to_take.x + diag_coeffs.x };
	TakeNode temp_node{
		curr_dr_type, get_index(curr_pos),
		get_index(curr_empty_pos), get_index(to_take), 
	};

	if (!poss_takes[curr_list_ind].add_take(temp_node, prev_list_len)) {
		std::cout << "(deep_take_dr) can't add_take &(" << std::endl;
		std::cout << "(deep_take_dr) curr_list: " << poss_takes[curr_list_ind] << std::endl;

		TakeList temp_takelist;
		poss_takes[curr_list_ind].copy_list(temp_takelist, prev_list_len);		
		temp_takelist.add_take(temp_node, prev_list_len);
		
		poss_takes.push_back(temp_takelist);
		curr_list_ind = poss_takes.size() - 1;
	}
	prev_list_len++;

	for (auto& side_coeffs : iter_coeffs) {
		Pos poss_enemy_pos{
			curr_empty_pos.y + side_coeffs.y,
			curr_empty_pos.x + side_coeffs.x
		};

		printf("(deep_take_dr) diag_coeffs: %d %d, side_coeffs: %d %d\n",
			diag_coeffs.y, diag_coeffs.x, side_coeffs.y, side_coeffs.x);
		std::cout << "(deep_take_dr) "<< get_notation(curr_pos) 
			<< " poss_enemy_pos: " << get_notation(poss_enemy_pos) << std::endl;
		if (check_coord(poss_enemy_pos)) {
			SqContent& poss_enemy_sq = get_square(poss_enemy_pos);
			printf("(deep_take_dr) is_enemy: %d, !already_taken: %d\n",
				(poss_enemy_sq == enemy.draught or poss_enemy_sq == enemy.king),
				!poss_takes[curr_list_ind].check_already_taken(poss_enemy_pos, prev_list_len));
			std::cout << "(deep_take_dr) checked in this list: " 
				<< poss_takes[curr_list_ind] << std::endl;
			if ((poss_enemy_sq == enemy.draught or poss_enemy_sq == enemy.king)
				and !poss_takes[curr_list_ind].check_already_taken(poss_enemy_pos, prev_list_len)) {
				Pos poss_empty_pos{
					poss_enemy_pos.y + side_coeffs.y,
					poss_enemy_pos.x + side_coeffs.x
				};
				if (check_coord(poss_empty_pos)) {
					std::cout << "(deep_take_dr) poss_empty_pos: " << get_notation(poss_empty_pos)
						<< " " << (get_square(poss_empty_pos) == SqContent::empty_sq) << std::endl;
					if (get_square(poss_empty_pos) == SqContent::empty_sq) {
						//std::cout << "(deep_take_dr) takelist bef add_take: " <<
						//	poss_takes[curr_list_ind] << std::endl;
						
						//std::cout << "(deep_take_dr) takelist aft add_take: " <<
						//	poss_takes[curr_list_ind] << std::endl;

						if (curr_dr_type == SqContent::white_dr 
							and poss_empty_pos.y == 0) 
						{
							std::cout << "call king" << std::endl;
							deep_take_king(curr_empty_pos, SqContent::white_king, poss_enemy_pos, 
								enemy, poss_takes, curr_list_ind, prev_list_len);
							//deep_take_king(curr_empty_pos, SqContent::white_king, poss_enemy_pos, 
							//	enemy, poss_takes, curr_list_ind, curr_list.length());
						}
						else if (curr_dr_type == SqContent::black_dr 
							and poss_empty_pos.y == BOARD_SIZE - 1) 
						{
							std::cout << "call king" << std::endl;
							deep_take_king(curr_empty_pos, SqContent::black_king, poss_enemy_pos, 
								enemy, poss_takes, curr_list_ind, prev_list_len);
							//deep_take_king(curr_empty_pos, SqContent::black_king, poss_enemy_pos, 
							//	enemy, poss_takes, curr_list_ind, curr_list.length());
						}
						else {
							std::cout << "call pathethic creature" << std::endl;
							deep_take_dr(curr_empty_pos, curr_dr_type, poss_enemy_pos, enemy,
								poss_takes, curr_list_ind, prev_list_len);
							//deep_take_dr(curr_empty_pos, curr_dr_type, poss_enemy_pos, enemy,
							//	poss_takes, curr_list_ind, curr_list.length());
						}
					}
					//else { break; }
				}
				//else { break; }
			}
			// if on this square our forces (draught or king)
			//else if (poss_enemy_sq != SqContent::empty_sq) { break; }
		}
		//else { break; }
	}
}

void Game::deep_take_king(Pos curr_pos, SqContent curr_dr_type, Pos to_take, const ForcesType& enemy, std::vector<TakeList>& poss_takes, BNT curr_list_ind, BNT prev_list_len) {
	Vect2 diag_coeffs = make_diag_coeffs(curr_pos, to_take),
		iter_coeffs[]{
		Vect2{-diag_coeffs.y, diag_coeffs.x},
		Vect2{diag_coeffs.y, -diag_coeffs.x},
		Vect2{diag_coeffs.y, diag_coeffs.x},
	};
	//TakeList& curr_list = poss_takes[curr_list_ind];
	std::cout << std::endl << "(deep_take_king) curr_pos: " << get_notation(curr_pos)
		<< " to_take: " << get_notation(to_take) 
		<< " prev_list_len: " << prev_list_len 
		<< std::endl;

	Pos curr_empty_pos{ to_take.y + diag_coeffs.y, to_take.x + diag_coeffs.x };
	

	//std::vector<Pos> enemy_ind;
	//enemy_ind.reserve(max_range);

	bool no_add_takes{ true };
	Pos curr_start_pos, poss_enemy_pos, poss_empty_pos;
	for (BNT i = 1; i < m_king_max_take_range; ++i) {
		curr_start_pos = Pos{
			to_take.y + diag_coeffs.y * i,
			to_take.x + diag_coeffs.x * i,
		};
		if (!check_coord(curr_start_pos)) { break; }
		
		for (auto& side_coeffs : iter_coeffs) {
			printf("(deep_take_king) diag_coeffs: %d %d, side_coeffs: %d %d\n",
				diag_coeffs.y, diag_coeffs.x, side_coeffs.y, side_coeffs.x);
			for (BNT k = 1; k < m_king_max_take_range; ++k) {
				poss_enemy_pos = Pos{
					curr_start_pos.y + side_coeffs.y * k,
					curr_start_pos.x + side_coeffs.x * k
				};

				std::cout << "(deep_take_king) curr_start_pos: " << get_notation(curr_start_pos)
					<< " poss_enemy_pos: " << get_notation(poss_enemy_pos) << std::endl;
				if (check_coord(poss_enemy_pos)) {
					SqContent& poss_enemy_sq = get_square(poss_enemy_pos);
					printf("(deep_take_king) is_enemy: %d, !already_taken: %d\n",
						(poss_enemy_sq == enemy.draught or poss_enemy_sq == enemy.king),
						!poss_takes[curr_list_ind].check_already_taken(poss_enemy_pos, prev_list_len));
					if ((poss_enemy_sq == enemy.draught or poss_enemy_sq == enemy.king)) {
						if (!poss_takes[curr_list_ind].check_already_taken(poss_enemy_pos, prev_list_len)) {
							poss_empty_pos = Pos{
								poss_enemy_pos.y + side_coeffs.y,
								poss_enemy_pos.x + side_coeffs.x
							};
							if (check_coord(poss_empty_pos)) {
								std::cout << "(deep_take_king) poss_empty_pos: " << get_notation(poss_empty_pos)
									<< " " << (get_square(poss_empty_pos) == SqContent::empty_sq) << std::endl;
								if (get_square(poss_empty_pos) == SqContent::empty_sq) {
									//std::cout << "(deep_take_king) takelist bef add_take: " <<
									//	poss_takes[curr_list_ind] << std::endl;
									//std::cout << "(deep_take_king) takelist aft add_take: " <<
									//	poss_takes[curr_list_ind] << std::endl;

									no_add_takes = false;

									TakeNode temp_node{
											curr_dr_type, get_index(curr_pos),
											get_index(curr_empty_pos), get_index(to_take),
									};

									if (!poss_takes[curr_list_ind].add_take(temp_node, prev_list_len)) {
										std::cout << "(deep_take_dr) can't add_take &(" << std::endl;
										TakeList temp_takelist;
										poss_takes[curr_list_ind].copy_list(temp_takelist, prev_list_len);
										poss_takes.push_back(temp_takelist);
										curr_list_ind = poss_takes.size() - 1;
									}

									deep_take_king(curr_start_pos, curr_dr_type, poss_enemy_pos, enemy,
										poss_takes, curr_list_ind, prev_list_len + 1);
									break;
								}
							}
							else { break; }
							//else { break_side = true; }
						}
						else { break; }
						//else { break_side = true; }
					}
					// if on this square our forces (draught or king)
					else if (poss_enemy_sq != SqContent::empty_sq) { break; }
					//else if (poss_enemy_sq != SqContent::empty_sq) { break_side = true; }
				}
				else { break; }
				//else { break_side = true; }
			}
		}
	}

	if (no_add_takes) {
		for (BNT i = 1; i < m_king_max_take_range; ++i) {
			curr_empty_pos = Pos{
				to_take.y + diag_coeffs.y * i,
				to_take.x + diag_coeffs.x * i,
			};
			if (!check_coord(curr_empty_pos)) { break; }

			TakeNode temp_node{
				curr_dr_type, get_index(curr_pos),
				get_index(curr_empty_pos), get_index(to_take),
			};

			if (!poss_takes[curr_list_ind].add_take(temp_node, prev_list_len)) {
				std::cout << "(deep_take_dr) can't add_take &(" << std::endl;
				TakeList temp_takelist;
				poss_takes[curr_list_ind].copy_list(temp_takelist, prev_list_len);
				poss_takes.push_back(temp_takelist);
				curr_list_ind = poss_takes.size() - 1;
			}
		}
	}
}

void Game::find_poss_moves(Pos pos, std::vector<Move>& poss_moves) {
	SqContent& curr_sq = get_square(pos);
	if (curr_sq == SqContent::white_dr or curr_sq == SqContent::black_dr) {
		BNT row_add;
		if (curr_sq == SqContent::white_dr) { row_add = -1; }
		else { row_add = 1; }
		
		for (BNT j : { -1, 1 }) {
			Pos coords{ pos.y + row_add, pos.x + j };
			if (check_coord(coords)) {
				if (get_square(coords) == SqContent::empty_sq) {
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
				for (BNT k = 0; k < BOARD_SIZE - 1; ++k) {
					Pos coords{ pos.y + i * k, pos.x + j * k };
					if (check_coord(coords)) {
						if (get_square(coords) == SqContent::empty_sq) {
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
	m_board[move.from] = SqContent::empty_sq;
}

void Game::make_take(TakeList& take_list) {
	for (auto& take : take_list.get_list()) {
		std::cout << "(make_take) who: "
			<< " is_draught:" << is_draught(take.who)
			<< " is_king:" << is_king(take.who)
			<< std::endl;
		m_board[take.to] = take.who;
		m_board[take.from] = SqContent::empty_sq;
		m_board[take.taken] = SqContent::empty_sq;
	}
}

// invalid 'y' && 'x' may cause memory corruption && therefore error occurrence
inline SqContent& Game::get_square(Pos pos) {
	return m_board[get_index(pos)];
}

// invalid 'y' && 'x' may cause memory corruption && therefore error occurrence
inline const SqContent& Game::get_square_c(Pos pos) const{
	return m_board[pos.y * BOARD_SIZE + pos.x];
}

inline BNT Game::get_index(Pos pos) {
	return pos.y * BOARD_SIZE + pos.x;
}

inline bool Game::check_coord(Pos pos) const {
	return (0 <= pos.y && pos.y < BOARD_SIZE) && (0 <= pos.x && pos.x < BOARD_SIZE);
}

inline Side Game::get_opponent() const {
	return static_cast<Side>(!static_cast<bool>(m_whose_turn));
}

inline std::string Game::get_notation(Pos pos) {
	return std::string{ static_cast<char>('a' + pos.x) } + std::to_string(BOARD_SIZE - pos.y);
};

inline std::string Game::get_notation(BNT index) {
	return get_notation(Pos{ index / BOARD_SIZE, index % BOARD_SIZE});
}

inline bool Game::is_draught(SqContent smth) {
	return smth == SqContent::white_dr or smth == SqContent::black_dr;
}

inline bool Game::is_king(SqContent smth) {
	return smth == SqContent::white_king or smth == SqContent::black_king;
}

inline Vect2 Game::make_diag_coeffs(Pos from, Pos to) {
	return Vect2{
		(to.y - from.y) / static_cast<BNT>(abs(to.y - from.y)),
		(to.x - from.x) / static_cast<BNT>(abs(to.x - from.x))
	};
}

std::ostream& operator<< (std::ostream& outs, const TakeList& takelist) {
	auto& curr_list = takelist.get_list();
	for (auto& takenode : curr_list) {
		outs << takenode;
	}
	return outs;
}

std::ostream& operator<< (std::ostream& outs, const TakeNode& takenode) {
	return outs << "(" << Game::get_notation(takenode.from) << "->" << Game::get_notation(takenode.to) << ")  ";
}

}
