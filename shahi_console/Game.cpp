#include <array>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include "Game.hpp"

//#define DEBUG_DEEP_TAKE_KING
//#define DEBUG_DEEP_TAKE_DR
//#define DEBUG_UNMAKE_TAKE
//#define DEBUG_MAKE_TAKE
//#define DEBUG_ALREADY_TAKEN
//#define DEBUG_AI
//#define DEBUG_THINK

namespace DraughtsGame {
static long num_of_leaves = 0;

Game::Game() : 
	m_whose_turn(Side::white)
{
	init_board();
	std::srand(std::time(nullptr));
}

void Game::main_loop() {
	std::vector<Move> poss_moves;
	std::vector<TakeList> poss_takes;

	poss_moves.reserve(POSS_VARIANTS_SIZE);
	poss_takes.reserve(POSS_VARIANTS_SIZE);

	SqContent e = SqContent::empty_sq, s = SqContent::white_sq,
		b = SqContent::black_dr, B = SqContent::black_king,
		w = SqContent::white_dr, W = SqContent::white_king;
	BoardType new_board{
		s,W,s,b,s,e,s,e,
		e,s,e,s,e,s,e,s,
		s,b,s,e,s,e,s,e,
		w,s,e,s,e,s,e,s,
		s,e,s,e,s,b,s,b,
		w,s,e,s,e,s,e,s,
		s,w,s,e,s,e,s,e,
		e,s,w,s,w,s,e,s,
		//
		//s,e,s,e,s,b,s,e,
		//e,s,e,s,e,s,e,s,
		//s,e,s,e,s,e,s,e,
		//e,s,w,s,e,s,W,s,
		//s,e,s,e,s,e,s,w,
		//e,s,w,s,e,s,e,s,
		//s,e,s,e,s,e,s,e,
		//w,s,e,s,e,s,B,s,
		//
		//s,e,s,e,s,e,s,e,
		//W,s,e,s,e,s,b,s,
		//s,e,s,e,s,b,s,e,
		//e,s,b,s,e,s,e,s,
		//s,e,s,e,s,b,s,w,
		//e,s,e,s,e,s,e,s,
		//s,w,s,e,s,e,s,e,
		//e,s,e,s,e,s,e,s,
		//
		//s,e,s,e,s,e,s,e,
		//W,s,e,s,e,s,b,s,
		//s,e,s,e,s,b,s,e,
		//e,s,b,s,e,s,b,s,
		//s,e,s,e,s,e,s,w,
		//e,s,e,s,e,s,e,s,
		//s,w,s,e,s,e,s,e,
		//e,s,e,s,e,s,e,s,
		//
		//s,e,s,W,s,e,s,b,
		//e,s,e,s,b,s,b,s,
		//s,e,s,e,s,b,s,b,
		//b,s,w,s,e,s,w,s,
		//s,e,s,e,s,w,s,w,
		//B,s,e,s,w,s,w,s,
		//s,e,s,e,s,e,s,w,
		//w,s,e,s,e,s,e,s,
		//
		//s,b,s,e,s,b,s,b,
		//b,s,b,s,b,s,b,s,
		//s,e,s,b,s,e,s,b,
		//w,s,b,s,b,s,b,s,
		//s,e,s,e,s,e,s,w,
		//w,s,e,s,w,s,w,s,
		//s,w,s,w,s,w,s,w,
		//w,s,w,s,e,s,w,s,
		//
		//s,e,s,e,s,e,s,e,
		//e,s,e,s,b,s,e,s,
		//s,b,s,e,s,e,s,e,
		//e,s,b,s,b,s,e,s,
		//s,b,s,w,s,e,s,e,
		//e,s,e,s,b,s,e,s,
		//s,e,s,w,s,e,s,e,
		//e,s,e,s,e,s,e,s,
		// 
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

	//set_state(new_board, Side::black);

	std::cout << "GAME STARTS!" << std::endl;
	std::cout << "position analysis:" << position_analysis(m_board) << std::endl;
	print_board(true);
	std::cin.get();

	while (m_game_is_on) {

		find_poss_variants(m_whose_turn, poss_moves, poss_takes);
		num_of_leaves = 0;
		if (poss_takes.size()) {
			//for (BNT i = 0; i < poss_takes.size(); ++i) {
			//	TakeList& curr_takelist = poss_takes[i];
			//	std::cout << i << "| " << curr_takelist << std::endl;
			//}

			make_take(poss_takes[think(m_whose_turn)]);
		}
		else if (poss_moves.size()) {
			//for (BNT i = 0; i < poss_moves.size(); ++i) {
			//	Move& curr_move = poss_moves[i];
			//	std::cout << i << "| " << curr_move << std::endl;
			//}

			make_move(poss_moves[think(m_whose_turn)]);
		}
		else {
			std::cout << "END" << std::endl;
			m_game_is_on = false;
			m_winner = get_opponent(m_whose_turn);
			m_winner_text = (m_whose_turn == Side::black) ? "White" : "Black";
		}

		std::cout << "leaves analyzed:" << num_of_leaves << std::endl;
		std::cout << "position analysis:" << position_analysis(m_board) << std::endl;
		std::cout << "NOW POSITION IS:" << std::endl;

		print_board(true);
		std::cin.get();

		m_whose_turn = get_opponent(m_whose_turn);
	}
	std::cout << "Winner is.... " << m_winner_text << "!" << std::endl;
	print_board(true);
};

void Game::print_board(bool is_big) const {
	enum class BoardChar : unsigned char {
		white_sq = 219, white_dr = '0', black_dr = '-',
		black_sq = 176, white_k = 'W', black_k = 'B'
	};

	BoardChar curr_char{ BoardChar::white_sq }; // added just random value because of compiler warning
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

BNT Game::position_analysis(BoardType& board) {
	static const std::array<BNT, BOARD_SQUARES_NUM> white_pos_heat_map = {
		0, +1, 0, +1, 0, +1, 0, +1,
		-1, 0, +2, 0, +2, 0, +1, 0,
		0, +1, 0, +2, 0, +2, 0, -1,
		-1, 0, +3, 0, +2, 0, +1, 0,
		0, +2, 0, +2, 0, +3, 0, -1,
		-1, 0, +2, 0, +2, 0, +1, 0,
		0, +1, 0, +2, 0, +2, 0, -2,
		-2, 0, +1, 0, +3, 0, +1, 0,
	};
	static const std::array<BNT, BOARD_SQUARES_NUM> black_pos_heat_map = {
		0, +1, 0, +3, 0, +1, 0, -2,
		-2, 0, +2, 0, +2, 0, +1, 0,
		0, +1, 0, +2, 0, +2, 0, -1,
		-1, 0, +3, 0, +2, 0, +2, 0,
		0, +1, 0, +2, 0, +3, 0, -1,
		-1, 0, +2, 0, +2, 0, +1, 0,
		0, +1, 0, +2, 0, +2, 0, -1,
		+1, 0, +1, 0, +1, 0, +1, 0, 
	};
	static const BNT temp_val = 4; // value of one temp
	static const BNT draught_val = temp_val * 3;
	static const BNT king_val = draught_val * 3;
	static const BNT endgame_val = 10000; // if win or lose
	static const BNT count_temps_forces_num = DRAUGHTS_NUM * 2 / 3; // num of forces by side
	BNT white_draughts = 0, black_draughts = 0,
		white_kings = 0, black_kings = 0,
		white_temps = 0, black_temps = 0,
		white_pos_score = 0, black_pos_score = 0;
	ForcesType white{ SqContent::white_dr, SqContent::white_king },
		black = { SqContent::black_dr, SqContent::black_king };

	for (BNT i = 0; i < BOARD_SQUARES_NUM; ++i) {
		if (m_board[i] == white.draught) { 
			++white_draughts;
			white_pos_score += white_pos_heat_map[i];
			white_temps += BOARD_SIZE - 1 - get_pos(i).y;
			//++white_forces_cnt;
		}
		else if (m_board[i] == white.king) { 
			++white_kings; 
			white_temps += BOARD_SIZE - 1;
			//++white_forces_cnt;
		}
		else if (m_board[i] == black.draught) { 
			++black_draughts; 
			black_pos_score += black_pos_heat_map[i];
			black_temps += get_pos(i).y;
		}
		else if (m_board[i] == black.king) { 
			++black_kings;
			black_temps += BOARD_SIZE - 1;
		}
	}

	if (white_draughts + white_kings == 0) { // if no white forces is on board
		return -endgame_val;
	}
	else if (black_draughts + black_kings == 0) { // if no black forces is on board
		return endgame_val;
	}
	else {
		BNT white_sum_score = white_draughts * draught_val + white_kings * king_val;
		BNT black_sum_score = black_draughts * draught_val + black_kings * king_val;
		float score_coeff = std::max(white_sum_score, black_sum_score)
			/ static_cast<float>(std::min(white_sum_score, black_sum_score));

		white_sum_score += white_pos_score;
		black_sum_score += black_pos_score;

		if ((white_draughts + white_kings) + (black_draughts + black_kings)
			<= count_temps_forces_num * 2)
		{
			white_sum_score += white_temps * temp_val;
			black_sum_score += black_temps * temp_val;
		}

		// bigger difference between side scores — worse (or better) result
		// for example 200 - 140 (3-4 vs. 2-3 draughts) was equal to 90 - 30  (3 vs. 1 dr.)
		// now second option will be more valueable
		return (white_sum_score - black_sum_score)
			* static_cast<BNT>(score_coeff);
	}
}

BNT Game::think(Side curr_side) {
	BNT final_index,
		temp_result;
	BNT curr_max = std::numeric_limits<BNT>::min(),
		index_max = -1,
		for_ind = 0;
	std::vector<Move> poss_moves;
	std::vector<TakeList> poss_takes;
#ifdef DEBUG_THINK
	std::string tab(1, 204);
#endif

	poss_moves.reserve(POSS_VARIANTS_SIZE);
	poss_takes.reserve(POSS_VARIANTS_SIZE);

	find_poss_variants(curr_side, poss_moves, poss_takes);

	if (poss_takes.size()) {
		for (auto take_iter = poss_takes.begin(); take_iter != poss_takes.end(); ++take_iter, ++for_ind) {
			auto& take = *take_iter;
#ifdef DEBUG_THINK
			std::cout << tab << "(think) take: " << take << std::endl;
			print_board();
			std::cout << std::endl;
#endif
			make_take(take);
#ifdef DEBUG_THINK
			print_board();
			std::cin.get();
#endif
			temp_result = -AI(0, curr_side, get_opponent(curr_side));;
			if (temp_result > curr_max) { curr_max = temp_result; index_max = for_ind; }

			unmake_take(take);
#ifdef DEBUG_AI
			std::cout << tab << "(think) take "
				<< take  << " result: " << temp_result << std::endl;
#endif

		}
	}
	else if (poss_moves.size()) {
		for (auto move_iter = poss_moves.begin(); move_iter != poss_moves.end(); ++move_iter, ++for_ind) {
			auto& move = *move_iter;
#ifdef DEBUG_THINK
			std::cout << tab << "(think) move: " << move << std::endl;
			print_board();
			std::cout << std::endl;
#endif
			make_move(move);
#ifdef DEBUG_THINK
			print_board();
			std::cin.get();
#endif
			temp_result = -AI(0, curr_side, get_opponent(curr_side));
			if (temp_result > curr_max) { curr_max = temp_result; index_max = for_ind; }

			unmake_move(move);
#ifdef DEBUG_THINK
			std::cout << tab << "(think) move " 
				<< move << " result: " << temp_result << std::endl;
#endif
		}
	}

	final_index = index_max;
	return final_index;
}

// min-max algorithm
BNT Game::AI(BNT deep_lvl, Side ai_side, Side curr_side) {
	BNT final_result,
		temp_result;
	BNT curr_max = std::numeric_limits<BNT>::min();;
	std::vector<Move> poss_moves;
	std::vector<TakeList> poss_takes;
	//std::vector<BNT> analyze_results;

	poss_moves.reserve(POSS_VARIANTS_SIZE);
	poss_takes.reserve(POSS_VARIANTS_SIZE);

	find_poss_variants(curr_side, poss_moves, poss_takes);
#ifdef DEBUG_AI
	std::string tab(deep_lvl, 204);
	std::cout << std::endl;
	std::cout << tab << "(AI) deep_lvl: " << deep_lvl << std::endl;
	std::cout << tab << "(AI) poss_takes: "
		<< poss_takes.size() 
		<< " poss_moves: " 
		<< poss_moves.size() 
		<< std::endl;
#endif
	if ((poss_takes.size() + poss_moves.size() == 0) // end of the game
		|| (poss_takes.size() == 0 && deep_lvl >= CALCULATION_LVL))
	{
		++num_of_leaves;
		auto temp = position_analysis(m_board);
		if (curr_side == Side::black) { temp = -temp; }
#ifdef DEBUG_AI
		std::cout << tab << "(AI) deepest result on "
			<< deep_lvl << ": " << temp <<
			" for " << (bool)curr_side << " side" << std::endl;
#endif
		return temp;
	}

	//analyze_results.reserve(poss_takes.size() + poss_moves.size());

	if (poss_takes.size()) {
		for (auto& take : poss_takes) {
#ifdef DEBUG_AI
			std::cout << tab << "(AI) take: " << take << std::endl;
			print_board();
			std::cout << std::endl;
#endif
			make_take(take);
#ifdef DEBUG_AI
			print_board();
			std::cin.get();
#endif
			temp_result = -AI(deep_lvl, ai_side, get_opponent(curr_side));
			//temp_result = -AI(deep_lvl + 1, ai_side, get_opponent(curr_side));
			if (temp_result > curr_max) { curr_max = temp_result; }
			
			unmake_take(take);
#ifdef DEBUG_AI
			std::cout << tab << "(AI) take " 
				<< take << " result: " << temp_result << std::endl;
#endif

		}
	}
	else if (poss_moves.size()) {
		for (auto& move : poss_moves) {
#ifdef DEBUG_AI
			std::cout << tab << "(AI) move: " << move << std::endl;
			print_board();
			std::cout << std::endl;
#endif
			make_move(move);
#ifdef DEBUG_AI
			print_board();
			std::cin.get();
#endif
			temp_result = -AI(deep_lvl + 1, ai_side, get_opponent(curr_side));
			if (temp_result > curr_max) { curr_max = temp_result; }

			unmake_move(move);
#ifdef DEBUG_AI
			std::cout << tab << "(AI) move " 
				<< move << " result: " << temp_result << std::endl;
#endif
		}
	}

	final_result = curr_max;

	if (deep_lvl == -1) {
		std::cout << "(AI) before return" << std::endl;
		std::cout << "  final_res: " << final_result << std::endl;
		std::cout << "  poss variants is:" << std::endl;
		std::cout << "  takes:" << std::endl;
		for (auto& elt : poss_takes) {
			std::cout << "    " << elt << std::endl;
		}
		std::cout << "  moves:" << std::endl;
		for (auto& elt : poss_moves) {
			std::cout << "    " << elt << std::endl;
		}
		std::cout << "  board:" << std::endl;
		print_board(true);
	}

	return final_result;
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
				if (i < OCCUPIED_LINES) {
					temp_square = SqContent::black_dr;
				}
				else if (i > BOARD_SIZE - OCCUPIED_LINES - 1) {
					temp_square = SqContent::white_dr;
				}
			}
		}
	}
	std::cout << std::endl;
}

void Game::set_state(BoardType& new_board, Side new_whose_turn) {
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
				find_poss_moves(Pos{ i, j }, poss_moves, side);
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
	Pos enemy_pos,
		curr_empty_pos;
	SqContent curr_sq;
	bool is_king_take = false,
		became_king = false;
	
	for (BNT i = 1; i < max_range + 1; ++i) {
		enemy_pos = { curr_pos.y + coeffs.y * i, curr_pos.x + coeffs.x * i };
		if (check_coord(enemy_pos)) {
			SqContent& enemy_sq = get_square(enemy_pos);
			if (enemy_sq == enemy.draught or enemy_sq == enemy.king)
			{
				curr_empty_pos = Pos{ curr_pos.y + coeffs.y * (i + 1), curr_pos.x + coeffs.x * (i + 1) };
				if (check_coord(curr_empty_pos)){
					if (get_square(curr_empty_pos) == SqContent::empty_sq) {
						poss_takes.push_back(TakeList{});

						is_king_take = false,
						became_king = false;

						if (get_square(curr_pos) == SqContent::black_dr && curr_empty_pos.y == BOARD_SIZE - 1) {
							curr_sq = SqContent::black_king;
							became_king = true;
						}
						else if (get_square(curr_pos) == SqContent::white_dr && curr_empty_pos.y == 0) {
							curr_sq = SqContent::white_king;
							became_king = true;
						}
						else if (is_king(get_square(curr_pos))) {
							curr_sq = get_square(curr_pos);
							is_king_take = true;
						}

						if (is_king_take || became_king) {
							deep_take_king(curr_pos, curr_sq, enemy_pos,
								enemy, poss_takes, poss_takes.size() - 1, 0, became_king);
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

#ifdef DEBUG_DEEP_TAKE_DR
	std::cout << std::endl << "(deep_take_dr) curr_pos: " << get_notation(curr_pos)
		<< " to_take: " << get_notation(to_take)
		<< " prev_list_len: " << prev_list_len
		<< std::endl;
	if (poss_takes.size() > DRAUGHTS_NUM) {
		std::cout << "(deep_take_dr) STOP IT!" << std::endl;
		exit(666);
	}

#endif

	Pos curr_empty_pos{ to_take.y + diag_coeffs.y, to_take.x + diag_coeffs.x };
	TakeNode temp_node{
		curr_dr_type, get_index(curr_pos), get_index(curr_empty_pos), 
		false, get_index(to_take), get_square(to_take)
	};

	if (!poss_takes[curr_list_ind].add_take(temp_node, prev_list_len)) {
#ifdef DEBUG_DEEP_TAKE_DR
		std::cout << "(deep_take_dr) can't add_take &(" << std::endl;
		std::cout << "(deep_take_dr) curr_list: " << poss_takes[curr_list_ind] << std::endl;
#endif
		
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

#ifdef DEBUG_DEEP_TAKE_DR
		printf("(deep_take_dr) diag_coeffs: %d %d, side_coeffs: %d %d\n",
			diag_coeffs.y, diag_coeffs.x, side_coeffs.y, side_coeffs.x);
		std::cout << "(deep_take_dr) "<< get_notation(curr_pos) 
			<< " poss_enemy_pos: " << get_notation(poss_enemy_pos) << std::endl;
#endif
		if (check_coord(poss_enemy_pos)) {
			SqContent& poss_enemy_sq = get_square(poss_enemy_pos);
#ifdef DEBUG_DEEP_TAKE_DR
			printf("(deep_take_dr) is_enemy: %d, !already_taken: %d\n",
				(poss_enemy_sq == enemy.draught or poss_enemy_sq == enemy.king),
				!poss_takes[curr_list_ind].check_already_taken(poss_enemy_pos, prev_list_len));
			std::cout << "(deep_take_dr) checked in this list: " << poss_takes[curr_list_ind] 
				<< "until " << prev_list_len << " elt" << std::endl;
#endif
			if ((poss_enemy_sq == enemy.draught or poss_enemy_sq == enemy.king)
				and !poss_takes[curr_list_ind].check_already_taken(poss_enemy_pos, prev_list_len)) {
				Pos poss_empty_pos{
					poss_enemy_pos.y + side_coeffs.y,
					poss_enemy_pos.x + side_coeffs.x
				};
				if (check_coord(poss_empty_pos)) {
#ifdef DEBUG_DEEP_TAKE_DR
					std::cout << "(deep_take_dr) poss_empty_pos: " << get_notation(poss_empty_pos)
						<< " " << (get_square(poss_empty_pos) == SqContent::empty_sq) << std::endl;
#endif
					if (get_square(poss_empty_pos) == SqContent::empty_sq) {
						if (curr_dr_type == SqContent::white_dr 
							and poss_empty_pos.y == 0) 
						{
#ifdef DEBUG_DEEP_TAKE_DR
							std::cout << "(deep_take_dr) call king" << std::endl;
#endif
							deep_take_king(curr_empty_pos, SqContent::white_king, poss_enemy_pos, 
								enemy, poss_takes, curr_list_ind, prev_list_len, true);
						}
						else if (curr_dr_type == SqContent::black_dr 
							and poss_empty_pos.y == BOARD_SIZE - 1) 
						{
#ifdef DEBUG_DEEP_TAKE_DR
							std::cout << "call king" << std::endl;
#endif
							deep_take_king(curr_empty_pos, SqContent::black_king, poss_enemy_pos, 
								enemy, poss_takes, curr_list_ind, prev_list_len, true);
						}
						else {
#ifdef DEBUG_DEEP_TAKE_DR
							std::cout << "call pathethic creature" << std::endl;
#endif
							deep_take_dr(curr_empty_pos, curr_dr_type, poss_enemy_pos, enemy,
								poss_takes, curr_list_ind, prev_list_len);
						}
					}
				}
			}
		}
	}
}

void Game::deep_take_king(Pos curr_pos, SqContent curr_dr_type, Pos to_take, const ForcesType& enemy, std::vector<TakeList>& poss_takes, BNT curr_list_ind, BNT prev_list_len, bool became_king) {
	BNT initial_prev_list_len = prev_list_len;
	Vect2 diag_coeffs = make_diag_coeffs(curr_pos, to_take),
		iter_coeffs[]{
		Vect2{-diag_coeffs.y, diag_coeffs.x},
		Vect2{diag_coeffs.y, -diag_coeffs.x},
		Vect2{diag_coeffs.y, diag_coeffs.x},
	};
#ifdef DEBUG_DEEP_TAKE_KING
	std::cout << std::endl << "(deep_take_king) curr_pos: " << get_notation(curr_pos)
		<< " to_take: " << get_notation(to_take) 
		<< " prev_list_len: " << prev_list_len 
		<< std::endl;
	std::cout << "(deep_take_king) poss_takes:" << std::endl;
	for (auto& elt : poss_takes) {
		std::cout << elt << std::endl;
	}

	if (poss_takes.size() > DRAUGHTS_NUM) {
		std::cout << "(deep_take_dr) STOP IT!" << std::endl;
		exit(666);
	}
#endif

	Pos curr_empty_pos{ to_take.y + diag_coeffs.y, to_take.x + diag_coeffs.x };

	bool add_takes{ false };
	BNT curr_prev_list_len;
	Pos curr_start_pos, poss_enemy_pos, poss_empty_pos;
	for (BNT i = 1; i < m_king_max_take_range; ++i) {
		curr_start_pos = Pos{
			to_take.y + diag_coeffs.y * i,
			to_take.x + diag_coeffs.x * i,
		};
		if (!check_coord(curr_start_pos)) { break; }
		if (get_square(curr_start_pos) != SqContent::empty_sq) { break; }
#ifdef DEBUG_DEEP_TAKE_KING
		std::cout << "curr_start_pos: " << get_notation(curr_start_pos)
			<< std::endl;
#endif

		for (auto& side_coeffs : iter_coeffs) {
			curr_prev_list_len = initial_prev_list_len;
#ifdef DEBUG_DEEP_TAKE_KING
			printf("(deep_take_king) diag_coeffs: %d %d, side_coeffs: %d %d\n",
				diag_coeffs.y, diag_coeffs.x, side_coeffs.y, side_coeffs.x);
#endif
			for (BNT k = 1; k < m_king_max_take_range; ++k) {
				poss_enemy_pos = Pos{
					curr_start_pos.y + side_coeffs.y * k,
					curr_start_pos.x + side_coeffs.x * k
				};
#ifdef DEBUG_DEEP_TAKE_KING
				std::cout << "(deep_take_king) curr_start_pos: " << get_notation(curr_start_pos)
					<< " poss_enemy_pos: " << get_notation(poss_enemy_pos) << std::endl;
#endif
				if (check_coord(poss_enemy_pos)) {
					SqContent& poss_enemy_sq = get_square(poss_enemy_pos);
#ifdef DEBUG_DEEP_TAKE_KING
					printf("(deep_take_king) is_enemy: %d, !already_taken: %d (%d)\n",
						(poss_enemy_sq == enemy.draught or poss_enemy_sq == enemy.king),
						!poss_takes[curr_list_ind].check_already_taken(poss_enemy_pos, prev_list_len), prev_list_len);
#endif
					if ((poss_enemy_sq == enemy.draught or poss_enemy_sq == enemy.king)) {
						if (!poss_takes[curr_list_ind].check_already_taken(poss_enemy_pos, prev_list_len)) {
							poss_empty_pos = Pos{
								poss_enemy_pos.y + side_coeffs.y,
								poss_enemy_pos.x + side_coeffs.x
							};
							if (check_coord(poss_empty_pos)) {
#ifdef DEBUG_DEEP_TAKE_KING
								std::cout << "(deep_take_king) poss_empty_pos: " << get_notation(poss_empty_pos)
									<< " " << (get_square(poss_empty_pos) == SqContent::empty_sq) << std::endl;
#endif
								if (get_square(poss_empty_pos) == SqContent::empty_sq) {
									TakeNode temp_node{
										curr_dr_type, get_index(curr_pos), get_index(curr_start_pos),
										became_king, get_index(to_take), get_square(to_take)
									};
									if (!poss_takes[curr_list_ind].add_take(temp_node, prev_list_len)) {
#ifdef DEBUG_DEEP_TAKE_KING
										std::cout << "(deep_take_king) can't add_take &(" << std::endl;
										std::cout << "(deep_take_king) poss_takes BEF:" << std::endl;
										for (auto& elt : poss_takes) {
											std::cout << elt << std::endl;
										}
#endif
	
										TakeList temp_takelist;
										poss_takes[curr_list_ind].copy_list(temp_takelist, prev_list_len + add_takes);
										if (add_takes) {
#ifdef DEBUG_DEEP_TAKE_KING
											std::cout << "NEW RULE WORKING" << std::endl;
#endif
											temp_takelist.correct_last_empty_pos(curr_start_pos);
										}
										temp_takelist.add_take(temp_node, prev_list_len);
										poss_takes.push_back(temp_takelist);
										curr_list_ind = poss_takes.size() - 1;
										
#ifdef DEBUG_DEEP_TAKE_KING
										std::cout << "(deep_take_king) poss_takes AFT:" << std::endl;
										for (auto& elt : poss_takes) {
											std::cout << elt << std::endl;
										}
										std::cout << "(deep_take_king) curr_list_ind: " << curr_list_ind
											<< " prev_list_len: " << prev_list_len << std::endl;
										std::cout << "(deep_take_king) poss_takes: size " << poss_takes.size()
											<< " capacity " << poss_takes.capacity() << std::endl;
#endif
									}
									add_takes = true;

									deep_take_king(curr_start_pos, curr_dr_type, poss_enemy_pos, enemy,
										poss_takes, curr_list_ind, prev_list_len + add_takes, false);
									//deep_take_king(curr_start_pos, curr_dr_type, poss_enemy_pos, enemy,
									//	poss_takes, curr_list_ind, prev_list_len + 1, false);
									break;
								}
								else { break; }
							}
							else { break; }
						}
						else { break; }
					}
					// if on this square our forces (draught or king)
					else if (poss_enemy_sq != SqContent::empty_sq) { break; }
				}
				else { break; }
			}
		}
	}

	if (!add_takes) {
#ifdef DEBUG_DEEP_TAKE_KING
		std::cout << "(deep_take_king) No ADD TAKINGS" << std::endl;
#endif
		if (poss_takes[curr_list_ind].length()) {
			poss_takes[curr_list_ind].correct_last_empty_pos(curr_pos);
		}

		for (BNT i = 1; i < m_king_max_take_range; ++i) {
			curr_empty_pos = Pos{
				to_take.y + diag_coeffs.y * i,
				to_take.x + diag_coeffs.x * i,
			};
			if (!check_coord(curr_empty_pos)) { break; }
			if (get_square(curr_empty_pos) != SqContent::empty_sq) { break; }

			TakeNode temp_node{
				curr_dr_type, get_index(curr_pos), get_index(curr_empty_pos), 
				became_king, get_index(to_take), get_square(to_take)
			};

#ifdef DEBUG_DEEP_TAKE_KING
			std::cout << "(deep_take_king) curr temp_node:" << temp_node << std::endl;
			std::cout << "(deep_take_king) prev_list_len:" << prev_list_len << std::endl;
#endif
			if (!poss_takes[curr_list_ind].add_take(temp_node, prev_list_len)) {
#ifdef DEBUG_DEEP_TAKE_KING
				std::cout << "(deep_take_king) can't add_take &(" << std::endl;
				std::cout << "(deep_take_king) poss_takes BEF:" << std::endl;
				for (auto& elt : poss_takes) {
					std::cout << elt << std::endl;
				}
#endif

				TakeList temp_takelist;
				poss_takes[curr_list_ind].copy_list(temp_takelist, prev_list_len);
				temp_takelist.add_take(temp_node, prev_list_len);

				poss_takes.push_back(temp_takelist);
				curr_list_ind = poss_takes.size() - 1;
	
#ifdef DEBUG_DEEP_TAKE_KING
				std::cout << "(deep_take_king) curr_list_ind: " << curr_list_ind
					<< " prev_list_len: " << prev_list_len << std::endl;
				std::cout << "(deep_take_king) size: " << poss_takes.size()
					<< " capacity: " << poss_takes.capacity() << std::endl;
#endif
			}
#ifdef DEBUG_DEEP_TAKE_KING
			std::cout << "(deep_take_king) final poss_takes:" << std::endl;
			for (auto& elt : poss_takes) {
				std::cout << elt << std::endl;
			}
#endif
		}
	}
}

void Game::find_poss_moves(Pos pos, std::vector<Move>& poss_moves, Side side) {
	SqContent& curr_sq = get_square(pos);

	if (is_draught(curr_sq)) {
		find_poss_moves_dr(pos, poss_moves, curr_sq, side);
	}
	else {
		find_poss_moves_king(pos, poss_moves, curr_sq, side);
	}
}

void Game::find_poss_moves_dr(Pos pos, std::vector<Move>& poss_moves, SqContent curr_sq, Side side) {
	BNT row_add = (side == Side::white) ? row_add = -1 : row_add = 1;
	Pos poss_empty_pos;
	bool became_king;

	for (BNT j : { -1, 1 }) {
		poss_empty_pos = Pos{ pos.y + row_add, pos.x + j };
		if (can_move_on_square(poss_empty_pos)) 
		{
			became_king = (side == Side::white && poss_empty_pos.y == 0) 
				|| (side == Side::black && poss_empty_pos.y == BOARD_SIZE - 1);
			poss_moves.push_back(Move{
				curr_sq, get_index(pos), get_index(poss_empty_pos), became_king 
			});
		}
	}
}

void Game::find_poss_moves_king(Pos pos, std::vector<Move>& poss_moves, SqContent curr_sq, Side side) {
	Pos poss_empty_pos;

	for (BNT i : { -1, 1 }) {
		for (BNT j : { -1, 1 }) {
			for (BNT k = 1; k < BOARD_SIZE - 1; ++k) {
				poss_empty_pos = Pos{ pos.y + i * k, pos.x + j * k };
				if (can_move_on_square(poss_empty_pos)) {
					poss_moves.push_back(
						Move{ curr_sq, get_index(pos), get_index(poss_empty_pos), false }
					);
				}
				else { break; }
			}
		}
	}
}

inline bool Game::can_move_on_square(Pos square_to_move_pos) const {
	return check_coord(square_to_move_pos) && get_square_c(square_to_move_pos) == SqContent::empty_sq;
}

void Game::make_move(const Move& move)
{
	m_board[move.from] = SqContent::empty_sq;
	m_board[move.to] = (move.became_king) ? get_king(move.who) : move.who;
}

void Game::make_take(const TakeList& take_list) {
#ifdef DEBUG_MAKE_TAKE
	std::cout << std::endl;
	std::cout << "(unmake_take) VERY START: " << std::endl;
	std::cout << "(unmake_take) takelist: " << take_list << std::endl;
	print_board();
#endif
	for (auto& take : take_list.get_list()) {
#ifdef DEBUG_MAKE_TAKE
		std::cout << std::endl;
		std::cout << "(make_take) bef: " << std::endl;
		print_board();
		std::cout << "(make_take): "
			<< " take: " << take
			<< " is_draught:" << is_draught(take.who)
			<< " is_king:" << is_king(take.who)
			<< " is_became_king:" << take.became_king
			<< std::endl;
#endif
		m_board[take.from] = SqContent::empty_sq;
		m_board[take.to] = take.who;
		m_board[take.taken_ind] = SqContent::empty_sq;
#ifdef DEBUG_MAKE_TAKE
		std::cout << "(make_take) aft: " << std::endl;
		print_board();
#endif
	}
#ifdef DEBUG_MAKE_TAKE
	std::cout << "(make_take) aft: " << std::endl;
	print_board();
#endif
}

void Game::unmake_move(const Move& move)
{
	m_board[move.from] = (is_king(move.who) && move.became_king) ? get_draught(move.who) : move.who;
	m_board[move.to] = SqContent::empty_sq;
}

void Game::unmake_take(const TakeList& take_list) {
	bool became_king = false;
	auto& curr_list = take_list.get_list();

#ifdef DEBUG_UNMAKE_TAKE
	std::cout << std::endl;
	std::cout << "(unmake_take) VERY START: " << std::endl;
	std::cout << "(unmake_take) takelist: " << take_list << std::endl;
	print_board();
#endif
	for (auto& take : take_list.get_list()) {
#ifdef DEBUG_UNMAKE_TAKE
		std::cout << std::endl;
		std::cout << "(unmake_take) bef: " << std::endl;
		print_board();
		std::cout << "(unmake_take): "
			<< " take: " << take
			<< " is_draught:" << is_draught(take.who)
			<< " is_king:" << is_king(take.who)
			<< " is_became_king:" << take.became_king
			<< std::endl;
#endif
		m_board[take.taken_ind] = take.taken_cont;
		became_king |= take.became_king;
#ifdef DEBUG_UNMAKE_TAKE
		std::cout << "(unmake_take) aft: " << std::endl;
		print_board();
#endif
	}

	auto& first_take = curr_list.front();
	auto& last_take = curr_list.back();
	m_board[first_take.from] = (is_king(last_take.who) && became_king) ? get_draught(last_take.who) : last_take.who;
	m_board[last_take.to] = SqContent::empty_sq;

#ifdef DEBUG_UNMAKE_TAKE
	std::cout << std::endl;
	std::cout << "(unmake_take) VERY END: " << std::endl;
	print_board();
#endif
}

Side Game::get_winner(BoardType& endgame_board) {
	for (int i = 0; i < BOARD_SQUARES_NUM; ++i) {
		SqContent& curr_sq = endgame_board[i];
		if (curr_sq == SqContent::white_dr || curr_sq == SqContent::white_king) {
			return Side::white;
		}
		else if (curr_sq == SqContent::white_dr || curr_sq == SqContent::white_king) {
			return Side::black;
		}
	}
}

inline Pos Game::get_pos(BNT index) {
	return Pos{ index / BOARD_SIZE, index % BOARD_SIZE };
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

inline Side Game::get_opponent(Side side) {
	return static_cast<Side>(!static_cast<bool>(side));
}

std::string Game::get_notation(Pos pos) {
	return std::string{ static_cast<char>('a' + pos.x) } + std::to_string(BOARD_SIZE - pos.y);
};

std::string Game::get_notation(BNT index) {
	return get_notation(Pos{ index / BOARD_SIZE, index % BOARD_SIZE});
}

inline bool Game::is_draught(SqContent smth) const {
	return smth == SqContent::white_dr or smth == SqContent::black_dr;
}

inline bool Game::is_king(SqContent smth) const {
	return smth == SqContent::white_king or smth == SqContent::black_king;
}

// get king SqContent piece from king of the same side
inline SqContent Game::get_draught(SqContent king) const {
	return (king == SqContent::white_king) ? SqContent::white_dr : SqContent::black_dr;
}

// get king SqContent piece from draught of the same side
inline SqContent Game::get_king(SqContent draught) const {
	return (draught == SqContent::white_dr) ? SqContent::white_king : SqContent::black_king;
}

inline Vect2 Game::make_diag_coeffs(Pos from, Pos to) const {
	return Vect2{
		(to.y - from.y) / static_cast<BNT>(abs(to.y - from.y)),
		(to.x - from.x) / static_cast<BNT>(abs(to.x - from.x))
	};
}

bool TakeList::add_take(TakeNode take, BNT control_length) {
	//std::cout << "(add_take) take: " << take
	//	<< "ctrl_len: " << control_length
	//	<< ", real size: " << m_takes.size() 
	//	<< std::endl;

	//if (m_takes.size() >= control_length) { // test solution for problem
	if (m_takes.size() == control_length) {
		m_takes.push_back(take);
		return true;
	}
	else { return false; }
}

void TakeList::copy_list(TakeList& copy_to, BNT copy_size) {
	//std::cout << "(copy_list) BEFORE copying " << copy_size << " elt" << std::endl;
	//std::cout << "(copy_list) curr_list: " << *this << std::endl;
	//std::cout << "(copy_list) copy_to list: " << copy_to << std::endl;
	copy_to.m_takes = std::vector<TakeNode>(m_takes.begin(), m_takes.begin() + copy_size);
	//std::cout << "(copy_list) AFTER" << std::endl;
	//std::cout << "(copy_list) curr_list: " << *this << std::endl;
	//std::cout << "(copy_list) copy_to list: " << copy_to << std::endl;
}

bool TakeList::check_already_taken(Pos pos, BNT check_len) const {
	BNT taken_ind = Game::get_index(pos),
		for_cnt = 0;
	for (auto take_iter = m_takes.begin(); for_cnt < check_len && take_iter != m_takes.end(); ++take_iter, ++for_cnt) {
#ifdef DEBUG_ALREADY_TAKEN
		std::cout << "(ch_alrd_taken) "
			<< "(" << for_cnt
			<< "/" << check_len << ") "
			<< Game::get_notation(take_iter->taken_ind)
			<< "=="
			<< Game::get_notation(taken_ind)
			<< std::endl;
#endif
		if (take_iter->taken_ind == taken_ind) { return true; }
	}
	return false;
}

BNT TakeList::length() const {
	return m_takes.size();
}

const std::vector<TakeNode>& TakeList::get_list() const {
	return m_takes;
}

void TakeList::correct_last_empty_pos(Pos destination_pos) {
	m_takes.back().to = Game::get_index(destination_pos);
}

std::ostream& operator<< (std::ostream& outs, const TakeList& takelist) {
	auto& curr_list = takelist.get_list();
	for (auto& takenode : curr_list) {
		outs << takenode;
	}
	return outs;
}

std::ostream& operator<< (std::ostream& outs, const TakeNode& takenode) {
	return outs << "|" << Game::get_notation(takenode.from) 
		<< "--" << Game::get_notation(takenode.taken_ind) 
		<< "->" << Game::get_notation(takenode.to) 
		<< "| ";
}

std::ostream& operator<< (std::ostream& outs, const Move& move) {
	return outs << "(" << Game::get_notation(move.from) 
		<< "->" << Game::get_notation(move.to) << ")  ";
}

//inline bool operator<(const AIResult& l, const AIResult& r) {
//	return l.score < r.score;
//}
//
//inline AIResult operator-(const AIResult& l, const AIResult& r) {
//	return AIResult{ 0, l.score - r.score };
//}

}
