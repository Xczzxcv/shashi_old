#pragma once
#include <array>
#include <vector>

namespace DraughtsGame {
using BNT = short; // Board Numeric Type
const BNT BOARD_SIZE = 8; // must be even and >2
const BNT BOARD_SQUARES_NUM = BOARD_SIZE * BOARD_SIZE;
const BNT ENGAGED_LINES = BOARD_SIZE / 2 - 1; // number for one side
const BNT DRAUGHTS_NUM = ENGAGED_LINES * BOARD_SIZE;
const BNT MAX_MOVES = BOARD_SQUARES_NUM; // just took it from my mind 

enum class SquareContent : char { 
	white_sq, white_dr, white_king,
	empty_sq, black_dr, black_king
};

enum class Side : bool { white = 0, black = 1 };

struct ForcesType {
	SquareContent draught;
	SquareContent king;
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

//struct GameState {
//	std::array<DraughtsType, BOARD_SIZE> white_draughts;
//	std::array<DraughtsType, BOARD_SIZE> white_kings;
//	std::array<DraughtsType, BOARD_SIZE> black_draughts;
//	std::array<DraughtsType, BOARD_SIZE> black_kings;
//};

struct Move {
	SquareContent who;
	BNT from;
	BNT to;
};

struct TakeNode : Move{
	BNT taken;
	BNT prev_take_ind;
};

class TakeTree;

class Game {
public:
	Game();
	void main_loop();
	void print_board(bool is_big = false) const;
	//void print_board_big() const;
	static inline BNT get_index(Pos pos);
private:
	void init_board();
	void find_poss_variants(Side side, std::vector<Move>& poss_moves, 
		std::vector<TakeTree>& poss_takes);
	void find_poss_moves(Pos pos, std::vector<Move>& poss_moves);
	void find_poss_takes(Pos pos, std::vector<TakeTree>& poss_takes, const ForcesType& enemy);
	void find_poss_takes_diagonal(Pos pos, Vect2 coeffs,
		BNT max_range, std::vector<TakeTree>& poss_takes, const ForcesType& enemy);
	void check_for_takings(Pos me, Pos to_take, BNT max_range, TakeTree& tree);
	void make_move(Move& move);
	void make_take(TakeTree& take_tree);
	BNT position_analysis();
	void AI();
	inline SquareContent& get_square(Pos pos);
	inline const SquareContent& get_square_c(Pos pos) const;
	inline bool check_coord(Pos pos);

	std::array<SquareContent, BOARD_SQUARES_NUM> m_board;
	Side m_whose_turn;
	bool m_game_is_on = true;
};

class TakeTree {
public:
	TakeTree(Pos pos)
		: m_start_pos(pos)
	{
	}

	void add_take(TakeNode take) {
		m_takes.emplace_back(take);
	}

	bool check_already_taken(Pos pos) {
		for (const auto& take : m_takes) {
			if (take.taken == Game::get_index(pos)) {
				return true;
			}
		}
		return false;
	}
protected:
	Pos m_start_pos;
	std::vector<TakeNode> m_takes;
};



}

