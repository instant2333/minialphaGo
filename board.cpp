#include "board.h"

std::vector<uint64_t > boardfunc::gen_movelist(std::array<int, 64> myboard,int curcolor) {
	/*Concert the possible move(bit board) to move list*/
	std::vector<uint64_t> mlist = std::vector<uint64_t>();
	bool flag1 = false;
	bool flag2 = false;
	for (int i = 0; i < 64; i++){
		flag1 = false;
		flag2 = false;
		if (myboard[i] != 0)continue;
		for (int j = 0; j < 8; j++) {
			int curpos = i;
			while (true) {
				//dir.at(i);
				curpos += dir.at(j);
				if (curpos > 63 || curpos < 0)break;
				if (myboard[curpos]!=0&&myboard[curpos] == -curcolor) {
					flag1 = true;
				}
				else if (myboard[curpos] == curcolor && flag1) {
					flag2 = true;
					break;

				}
				else {
					flag1 = false;
					break;
				}
			}
			if (flag2) {
				mlist.push_back(i);
				break;
			}
		}
	}
	return mlist;
}

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
			if (curpos > 63 || curpos < 0)break;
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