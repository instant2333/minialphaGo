#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <array>



// board: include two uint64_t variable. show the black keys and white keys
struct pos {
public:
	int x = 0;
	int y = 0;
	pos(int xin, int yin) {
		x = xin;
		y = yin;
	}

	~pos() {}
};
struct board {
public:
	uint64_t B = 0;
	uint64_t W = 0;
	board(uint64_t Bin, uint64_t Win) {
		B = Bin;
		W = Win;
	}
	board() {

	}
	~board() {}
};
typedef struct boardstate {
public:

	int color;
	board curBoard;
	boardstate() {
		this->color = 0;
		this->curBoard = board();
	}
} * root;

class boardfunc {
public:
	std::vector<uint64_t> BIT = std::vector<uint64_t >();
	root selfRoot = nullptr;
	//std::vector<uint64_t> LSB_TABLE = std::vector<uint64_t>(64);
	//std::map<int, std::vector<uint64_t >> RADIAL_MAP = std::map<int, std::vector<uint64_t >>();
	std::vector<int > dir = {-1, 1, -7, 7, -8, 8, -9, 9};


	std::vector<uint64_t > gen_movelist(std::array<int, 64> myboard,int curcolor );
	//std::tuple<uint64_t, uint64_t > pop_lsb(uint64_t bitmap);
	bool getwinner(std::array<int, 64> myboard);
	void dochange(std::array<int, 64> * myboard, int temppos, int curcolor);
	/*struct pos to_move(int bitmove) {
		return pos(bitmove % 8, bitmove / 8);
	}*/
	boardfunc() {
		//init_map();
		selfRoot = nullptr;
	}
};