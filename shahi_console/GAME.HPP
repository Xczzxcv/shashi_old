#pragma once
#include <array>
#include <iostream>
#include <vector>
#include <string>

namespace DraughtsGame {
using BNT = short; // Board Numeric Type
const BNT BOARD_SIZE = 8; // must be even and >2
const BNT BOARD_SQUARES_NUM = BOARD_SIZE * BOARD_SIZE;
const BNT ENGAGED_LINES = BOARD_SIZE / 2 - 1; // number for one side
const BNT DRAUGHTS_NUM = ENGAGED_LINES * BOARD_SIZE;
const BNT MAX_MOVES = BOARD_SQUARES_NUM; // just took it from my mind 
const BNT MAX_NEXT_TAKES = 3; // forward, right, left

enum class SqContent : char { 
	white_sq, white_dr, white_king,
	empty_sq, black_dr, black_king
};

enum class Side : bool { white = 0, black = 1 };

struct ForcesType {
	SqContent draught;
	SqContent king;
};

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
};

struct TakeNode : Move{
	BNT taken;
	//BNT prev_take_ind = -1;
	//BNT next_take_ind = -1;
};

class TakeList;

class Game {
public:
	Game();
	void main_loop();
	void print_board(bool is_big = false) const;
	//void print_board_big() const;
	static inline BNT get_index(Pos pos);
	static inline std::string get_notation(Pos pos);
	static inline std::string get_notation(BNT index);
private:
	void init_board();
	void set_state(std::array<SqContent, BOARD_SQUARES_NUM>& new_board, Side new_whose_turn);
	void find_poss_variants(Side side, std::vector<Move>& poss_moves, 
		std::vector<TakeList>& poss_takes);
	void find_poss_moves(Pos pos, std::vector<Move>& poss_moves);
	void find_poss_takes(Pos pos, std::vector<TakeList>& poss_takes, const ForcesType& enemy, Side side);
	void find_poss_takes_diagonal(Pos pos, Vect2 coeffs, BNT max_range, 
		std::vector<TakeList>& poss_takes, const ForcesType& enemy, Side side);
	void deep_take_dr(Pos curr_pos, SqContent curr_dr_type, Pos to_take,
		const ForcesType& enemy, std::vector<TakeList>& poss_takes, 
		BNT curr_list_ind, BNT prev_list_len); 
	void deep_take_king(Pos curr_pos, SqContent curr_dr_type, Pos to_take, 
		const ForcesType& enemy, std::vector<TakeList>& poss_takes, 
		BNT curr_list_ind, BNT prev_list_len);
	void make_move(Move& move);
	void make_take(TakeList& take_list);
	BNT position_analysis();
	void AI();
	inline SqContent& get_square(Pos pos);
	inline const SqContent& get_square_c(Pos pos) const;
	inline bool check_coord(Pos pos) const;
	inline Side get_opponent() const;
	inline bool is_draught(SqContent smth);
	inline bool is_king (SqContent smth);

	std::array<SqContent, BOARD_SQUARES_NUM> m_board;
	Side m_whose_turn;
	Side m_winner;
	std::string m_winner_text;
	bool m_game_is_on = true;
	BNT m_king_max_take_range = BOARD_SIZE - 1;
};

class TakeList {
public:
	bool add_take(TakeNode take, BNT control_length) {
		//printf("(add_take) ctrl_len: %d, real size: %d\n", control_length, m_takes.size());

		if (m_takes.size() == control_length){
			m_takes.push_back(take);
			return true;
		}
		else { return false; }
	}

	void copy_list(TakeList& copy_to, BNT copy_size = -1) {
		//if (copy_size == -1) { copy_to.m_takes = m_takes; }
		//else{ copy_to.m_takes = std::vector<TakeNode>(m_takes.begin(), m_takes.begin() + copy_size + 1); }
		copy_to.m_takes = std::vector<TakeNode>(m_takes.begin(), m_takes.begin() + copy_size);
	}

	bool check_already_taken(Pos pos) {
		BNT taken_ind = Game::get_index(pos);
		for (auto& take : m_takes) {
			if (take.taken == taken_ind) { return true; }
		}
		return false;
	}

	BNT length() {
		return m_takes.size();
	}

	const std::vector<TakeNode>& get_list() const {
		return m_takes;
	}
	friend std::ostream& operator << (std::ostream& outs, const TakeList& takelist);
protected:
	std::vector<TakeNode> m_takes;
};

std::ostream& operator<< (std::ostream& outs, const TakeList& takelist);
}

