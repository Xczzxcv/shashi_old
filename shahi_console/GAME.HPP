#pragma once
#include <array>

namespace DraughtsGame {
using BoardNumType = unsigned short;
const BoardNumType BOARD_SIZE = 8; // must be even and >2
const BoardNumType BOARD_SQUARES_NUM = BOARD_SIZE * BOARD_SIZE;
const BoardNumType ENGAGED_LINES = BOARD_SIZE / 2 - 1; // number for one side
const BoardNumType DRAUGHTS_NUM = ENGAGED_LINES * BOARD_SIZE;
const BoardNumType MAX_MOVES = BOARD_SQUARES_NUM; // just took it from my mind 

enum class SquareContent : char { 
	white_sq = 0, white_dr = 3, white_king = 4,
	black_sq = 1, black_dr = 7, black_king = 8
};

enum class BoardChar : unsigned char {
	white_sq = 219, white_dr = '+', black_dr = '-',
	black_sq = 176, white_k = 'W', black_k = 'B'
};

enum class Side : bool { white, black };

//struct GameState {
//	std::array<DraughtsType, BOARD_SIZE> white_draughts;
//	std::array<DraughtsType, BOARD_SIZE> white_kings;
//	std::array<DraughtsType, BOARD_SIZE> black_draughts;
//	std::array<DraughtsType, BOARD_SIZE> black_kings;
//};

struct GameStatus {
	bool is_on;
	Side winner;
};

struct Move {
	SquareContent who;
	BoardNumType from;
	BoardNumType to;
	BoardNumType has_taken;
};

class Game {
public:
	Game();
	void main_loop();
	GameStatus is_ended() const;
	void print_board() const;
private:
	void init_board();
	void find_poss_moves(std::array<Move, MAX_MOVES>& poss_moves);
	void make_move(Move& move);
	int position_analysis();
	void AI();
	inline SquareContent& get_square(int y, int x);

	std::array<SquareContent, BOARD_SQUARES_NUM> m_board;
	Side whose_turn;
	bool game_is_on = true;
};

}

