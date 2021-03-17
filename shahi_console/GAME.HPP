#pragma once
#include <array>
#include <vector>

namespace DraughtsGame {
using BoardNumType = unsigned short;
const BoardNumType BOARD_SIZE = 8; // must be even and >2
const BoardNumType BOARD_SQUARES_NUM = BOARD_SIZE * BOARD_SIZE;
const BoardNumType ENGAGED_LINES = BOARD_SIZE / 2 - 1; // number for one side
const BoardNumType DRAUGHTS_NUM = ENGAGED_LINES * BOARD_SIZE;
const BoardNumType MAX_MOVES = BOARD_SQUARES_NUM; // just took it from my mind 

enum class SquareContent : char { 
	white_sq, white_dr, white_king,
	empty_sq, black_dr, black_king
};

enum class BoardChar : unsigned char {
	white_sq = 219, white_dr = '0', black_dr = '-',
	black_sq = 176, white_k = 'W', black_k = 'B'
};

enum class Side : bool { white = 0, black = 1 };

struct ForcesType {
	SquareContent draught;
	SquareContent king;
};

// position
struct Pos {
	BoardNumType y;
	BoardNumType x;
};

//struct GameState {
//	std::array<DraughtsType, BOARD_SIZE> white_draughts;
//	std::array<DraughtsType, BOARD_SIZE> white_kings;
//	std::array<DraughtsType, BOARD_SIZE> black_draughts;
//	std::array<DraughtsType, BOARD_SIZE> black_kings;
//};

struct Move {
	SquareContent who;
	BoardNumType from;
	BoardNumType to;
};

struct TakeNode : Move{
	BoardNumType taken;
	int prev_take_ind;
};

class TakeTree;

class Game {
public:
	Game();
	void main_loop();
	void print_board() const;
	void print_board_big() const;
	static inline BoardNumType get_index(Pos pos);
private:
	void init_board();
	void find_poss_variants(Side side, std::vector<Move>& poss_moves, 
		std::vector<TakeTree>& poss_takes);
	void find_poss_moves(BoardNumType y, BoardNumType x, std::vector<Move>& poss_moves);
	void find_poss_takes(BoardNumType y, BoardNumType x, std::vector<TakeTree>& poss_takes,
		const ForcesType& enemy, TakeTree* curr_tree = nullptr);
	void find_poss_takes_diagonal(BoardNumType y, BoardNumType x, int y_coeff, int x_coeff, 
		BoardNumType max_range, std::vector<TakeTree>& poss_takes, 
		const ForcesType& enemy, TakeTree* curr_tree = nullptr);
	void make_move(Move& move);
	void make_take(TakeTree& take_tree);
	int position_analysis();
	void AI();
	inline SquareContent& get_square(BoardNumType  y, BoardNumType x);
	inline const SquareContent& get_square_c(BoardNumType y, BoardNumType x) const;
	inline bool check_coord(BoardNumType y, BoardNumType x);

	std::array<SquareContent, BOARD_SQUARES_NUM> m_board;
	Side m_whose_turn;
	bool m_game_is_on = true;
};

class TakeTree {
public:
	TakeTree(BoardNumType y, BoardNumType x)
		: m_start_y(y), m_start_x(x)
	{
	}

	void add_take(TakeNode take) {
		m_takes.emplace_back(take);
	}

	bool check_already_taken(short y, short x) {
		for (const auto& take : m_takes) {
			if (take.taken == Game::get_index(y, x)) {
				return true;
			}
		}
		return false;
	}
protected:
	BoardNumType m_start_y;
	BoardNumType m_start_x;
	std::vector<TakeNode> m_takes;
};



}

