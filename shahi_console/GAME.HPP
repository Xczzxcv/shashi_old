#pragma once
#include <array>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

namespace DraughtsGame {
using BNT = short; // Board Numeric Type
using AIResult = std::pair<BNT, BNT>; // 1. index in sequence 2. position score
const BNT BOARD_SIZE = 8; // must be even and >2
const BNT BOARD_SQUARES_NUM = BOARD_SIZE * BOARD_SIZE;
const BNT ENGAGED_LINES = BOARD_SIZE / 2 - 1; // number for one side
const BNT DRAUGHTS_NUM = ENGAGED_LINES * BOARD_SIZE;
//const BNT MAX_MOVES = BOARD_SQUARES_NUM; // just took it from my mind 
const BNT MAX_NEXT_TAKES = 3; // forward, right, left
const BNT CALCULATION_LVL = 2; // for min-max algorithm

enum class SqContent : char { 
	white_sq, white_dr, white_king,
	empty_sq, black_dr, black_king
};

enum class Side : bool { white = 0, black = 1 };

struct ForcesType {
	SqContent draught;
	SqContent king;
};

using BoardType = std::array<SqContent, BOARD_SQUARES_NUM>; // Board Type

// position
struct Pos {
	BNT y;
	BNT x;
};

struct Vect2 {
	BNT y;
	BNT x;
};

struct Move {
	SqContent who;
	BNT from;
	BNT to;
	bool became_king;
};

struct TakeNode : Move{
	BNT taken_ind;
	SqContent taken_cont;
};

class TakeList;

class Game {
public:
	Game();
	void main_loop();
	void print_board(bool is_big = false) const;
	static BNT get_index(Pos pos);
	static std::string get_notation(Pos pos);
	static std::string get_notation(BNT index);
private:
	void init_board();
	void set_state(BoardType& new_board, Side new_whose_turn);
	void find_poss_variants(Side side, std::vector<Move>& poss_moves, 
		std::vector<TakeList>& poss_takes);
	void find_poss_moves(Pos pos, std::vector<Move>& poss_moves, Side side);
	void find_poss_moves_dr(Pos pos, std::vector<Move>& poss_moves, 
		SqContent curr_sq, Side side);
	void find_poss_moves_king(Pos pos, std::vector<Move>& poss_moves, 
		SqContent curr_sq, Side side);
	inline bool can_move_on_square(Pos square_to_move_pos) const;
	void find_poss_takes(Pos pos, std::vector<TakeList>& poss_takes, const ForcesType& enemy, Side side);
	void find_poss_takes_diagonal(Pos curr_pos, Vect2 coeffs, BNT max_range,
		std::vector<TakeList>& poss_takes, const ForcesType& enemy, Side side);
	void deep_take_dr(Pos curr_pos, SqContent curr_dr_type, Pos to_take, 
		const ForcesType& enemy, std::vector<TakeList>& poss_takes, 
		BNT curr_list_ind, BNT prev_list_len);
	void deep_take_king(Pos curr_pos, SqContent curr_dr_type, Pos to_take,
		const ForcesType& enemy, std::vector<TakeList>& poss_takes,
		BNT curr_list_ind, BNT prev_list_len, bool became_king);
	void make_move(const Move& move);
	void make_take(const TakeList& take_list);
	void unmake_move(const Move& move);
	void unmake_take(const TakeList& take_list);
	BNT position_analysis(BoardType& board, Side side);
	AIResult AI(BNT deep_lvl, Side ai_side, Side side);
	inline Side get_winner(BoardType& endgame_board);
	inline SqContent& get_square(Pos pos);
	inline const SqContent& get_square_c(Pos pos) const;
	inline bool check_coord(Pos pos) const;
	inline bool is_draught(SqContent smth) const;
	inline bool is_king (SqContent smth) const;
	SqContent get_draught(SqContent king) const;
	SqContent get_king(SqContent draught) const;
	inline Vect2 make_diag_coeffs (Pos from, Pos to) const;
	static inline Side get_opponent(Side side);

	std::array<SqContent, BOARD_SQUARES_NUM> m_board;
	Side m_whose_turn;
	Side m_winner;
	std::string m_winner_text;
	bool m_game_is_on = true;
	BNT m_king_max_take_range = BOARD_SIZE - 1;
};

class TakeList {
public:
	bool add_take(TakeNode take, BNT control_length);
	void copy_list(TakeList& copy_to, BNT copy_size);
	bool check_already_taken(Pos pos, BNT check_len) const;
	BNT length() const;
	const std::vector<TakeNode>& get_list() const;
	void correct_last_empty_pos(Pos destination_pos);

	friend std::ostream& operator << (std::ostream& outs, const TakeList& takelist);
protected:
	std::vector<TakeNode> m_takes;
};

std::ostream& operator<< (std::ostream& outs, const TakeList& takelist);
std::ostream& operator<< (std::ostream& outs, const TakeNode& takenode);
std::ostream& operator<< (std::ostream& outs, const Move& takenode);
}

