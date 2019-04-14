#include "board.h"

void boardfunc::init_map() {
	// *********************
	// fill_bit_table
	for (int i = 0; i < 64; i++) {
		BIT.push_back((uint64_t)0x1 << i);
	}
	// *********************
	// fill_radial_map
	std::map<int, std::vector<int>> rad_map = { { -1,{ -1,0 } },{ 1,{ 1,0 } },{ -8,{ 0,-1 } },{ 8,{ 0,1 } },{ -7,{ 1,-1 } },{ 7,{ -1,1 } },{ -9,{ -1,-1 } },{ 9,{ 1,1 } } };
	for (auto &it : rad_map) {
		std::vector<uint64_t > lis(64);
		for (int sqr = 0; sqr < 64; sqr++) {
			uint64_t mask = 0;
			int sq = sqr;
			pos p = to_move(sq);  // 0~64 --> x,y
			sq += it.first;  // check this direction from current point
			int x = p.x + it.second[0];
			int y = p.y + it.second[1];
			while (x >= 0 && x < 8 && y >= 0 && y < 8 && sq >= 0 && sq <= 64) {
				mask |= BIT[sq];   //able to move to such position
				sq += it.first;  // keep going on such direction(0~64)
				x += it.second[0];
				y += it.second[1];
			}
			lis[sqr] = mask;
		}
		RADIAL_MAP[it.first] = lis;
	}
	// *********************
	// fill_lsh_table
	uint64_t bitmap = 1;
	for (uint64_t i = 0; i < 64; i++) {
		LSB_TABLE[(((bitmap & (~bitmap + 1)) * LSB_HASH) & FULL_MASK) >> 58] = i;
		bitmap <<= 1;
	}
}



uint64_t boardfunc::move_gen_sub(uint64_t P, uint64_t mask, int dir) {
	//Search for 1 direction
	int dir2 = dir * 2;
	uint64_t flip1 = mask & (P << dir);
	uint64_t flip2 = mask & (P >> dir);
	flip1 |= mask & (flip1 << dir);
	flip2 |= mask & (flip2 >> dir);
	uint64_t mask1 = mask & (mask << dir);
	uint64_t mask2 = mask & (mask >> dir);
	flip1 |= mask1 & (flip1 << dir2);
	flip2 |= mask2 & (flip2 >> dir2);
	flip1 |= mask1 & (flip1 << dir2);
	flip2 |= mask2 & (flip2 >> dir2);
	return (flip1 << dir) | (flip2 >> dir);
}
uint64_t boardfunc::move_gen(uint64_t P, uint64_t O) {
	/*Check possible move by 8 direction*/
	uint64_t mask = O & 0x7E7E7E7E7E7E7E7E;
	return ((move_gen_sub(P, mask, 1) \
		| move_gen_sub(P, O, 8)  \
		| move_gen_sub(P, mask, 7) \
		| move_gen_sub(P, mask, 9)) & ~(P | O)) & FULL_MASK;
}

//std::array<int, 64> myboard;
//std::vector<uint64_t > boardfunc::gen_movelist(uint64_t W, uint64_t B) {
//	std::vector<uint64_t> mlist = std::vector<uint64_t>();
//	uint64_t leagal_moves = move_gen(W, B);
//	uint64_t leagal_binary = leagal_moves;
//	while (leagal_binary != 0) {
//		std::tuple<uint64_t, uint64_t > res = pop_lsb(leagal_binary);
//		leagal_binary = std::get<1>(res);
//		mlist.push_back(std::get<0>(res));
//	}
//	return mlist;
//}

//std::vector<uint64_t > boardfunc::gen_movelist(std::array<int, 64> myboard,int color) {
//	this->selfRoot = new boardstate();
//	uint64_t W = 0;
//	uint64_t B = 0;
//	uint64_t temp = 0;
//	/*Convert board array to bitboard*/
//	for (int r = 0; r < 8; r++) {
//		for (int c = 0; c < 8; c++) {
//			if (myboard[c * 8 + r] == -1) {
//				B |= BIT[8 * r + c];
//			}
//			else if (myboard[c * 8 + r] == 1) {
//				W |= BIT[8 * r + c];
//			}
//		}
//	}
//	if (color == -1) {
//		temp = W;
//		W = B;
//		B = temp;
//	}
//	selfRoot->curBoard = board(B, W);
//	selfRoot->color = color;
//
//	std::vector<uint64_t> mlist = std::vector<uint64_t>();
//	uint64_t leagal_moves = move_gen(W, B);
//	uint64_t leagal_binary = leagal_moves;
//	while (leagal_binary != 0) {
//		std::tuple<uint64_t, uint64_t > res = pop_lsb(leagal_binary);
//		leagal_binary = std::get<1>(res);
//		mlist.push_back(std::get<0>(res));
//	}
//	return mlist;
//}

std::vector<uint64_t > boardfunc::gen_movelist(uint64_t W, uint64_t B) {
	/*Concert the possible move(bit board) to move list*/
	std::vector<uint64_t> mlist = std::vector<uint64_t>();
	uint64_t leagal_moves = move_gen(W, B);
	uint64_t leagal_binary = leagal_moves;
	while (leagal_binary != 0) {
		std::tuple<uint64_t, uint64_t > res = pop_lsb(leagal_binary);
		leagal_binary = std::get<1>(res);
		mlist.push_back(std::get<0>(res));
	}
	return mlist;
}

std::tuple<uint64_t, uint64_t > boardfunc::pop_lsb(uint64_t bitmap) {
	/*Calculate the least bit for 1*/
	uint64_t l = LSB_TABLE[(((bitmap & (~bitmap + 1)) * LSB_HASH) & FULL_MASK) >> 58];
	bitmap &= bitmap - 1;
	return std::tuple<uint64_t, uint64_t >(l, bitmap & FULL_MASK);
};
bool boardfunc::getwinner(std::array<int, 64> myboard){
	int countB=0,countW=0;
	for (int i = 0; i < 64; i++) {
		if (myboard[i] == 1)countB++;
		else if (myboard[i] == -1)countW++;
	}
	if (countB > countW)return true;
	else return false;
}
void boardfunc::dochange(std::array<int, 64> * myboard, int temppos, int curcolor) {
	bool flag = false;
	(*myboard)[temppos] = curcolor;
	for (int i = 0; i < 8; i++) {
		int curpos=temppos;
		while (true) {
			//dir.at(i);
			curpos += dir.at(i);
			if ((*myboard)[curpos] == -(*myboard)[temppos]) {
				flag = true;
			}
			else if((*myboard)[curpos] == (*myboard)[temppos]&&flag){
				curpos -= dir.at(i);
				while (curpos != temppos) {
					(*myboard)[curpos] = curcolor;
					curpos -= dir.at(i);
				}
				break;
				
			}
			else {
				flag = false;
				break;
			}
			
		}

	}

}