#include <ctime>
#include <exception>
#include <iostream>
#include <cmath>
#include <cassert>
#include <limits>
#include <stack>
#include <cstring>
#include <vector>
#include"board.h"
using namespace std;


//bool judgelegal(int i, int j, int gridInfo[8][8]) {
//
//}
//
//bool getlegal_points(int legal_points[8][8], int gridInfo[8][8],int curcolor) {
//	bool havelegalpos = false;
//	bool legalpos=false;
//	for(int i=0;i<8;i++)
//		for (int j = 0; j < 8; j++) {
//			legalpos = judgelegal(i, j, gridInfo);
//			if (legalpos) {
//				legal_points[i][j] = 1;
//				havelegalpos = true;
//			}
//			else legal_points[i][j] = 0;
//		}
//	return havelegalpos;
//		 
//}
//int main()
//{
//	int x, y;
//	int curcolor=1;
//	bool unfinished=1;
//	int legal_points[8][8] = {0};
//	int gridInfo[8][8] = { 0 }; // ÏÈxºóy£¬¼ÇÂ¼ÆåÅÌ×´Ì¬
//	gridInfo[3][4] = gridInfo[4][3] = 1;  //|°×|ºÚ|
//	gridInfo[3][3] = gridInfo[4][4] = -1; //|ºÚ|°×|
//	getlegal_points(legal_points, gridInfo, unfinished);
//
//	while (unfinished) {
//		
//		cout << "please input the x y of pos " << endl;
//		cin >> x >> y;
//		if (legal_points[x][y] != 1) {
//			cout << "illegal pos, replay." << endl;
//			continue;
//		}
//		else {
//			gridInfo[x][y] = curcolor;
//			curcolor = -curcolor;
//		}
//		unfinished =getlegal_points(legal_points, gridInfo, curcolor );
//		if (!unfinished && !getlegal_points(legal_points, gridInfo, -curcolor))cout << "game over" << endl;
//		else if(!unfinished ){
//			cout << "color " << ((curcolor == 1 )? "black" : "white")<< "have no legal pos, change to another"<<endl;
//			curcolor = -curcolor;
//			unfinished = true;
//		}
//	}
//	return 0;
//}


int main() {
	boardfunc tools = boardfunc();
	int curcolor = -1;
	int x, y;

	std::array<int, 64> array = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 0, 0,
		0, 0, 0, 1, 1, 0, 0, 0,
		0, 0, 0,-1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};
	for (int i = 0; i < 64; i++) {
		if (!(i % 8))cout << endl;
		cout << array[i] << "\t";
	}
	cout << endl;
	cout << endl;
	while (true) {	
		uint64_t W = 0;
		uint64_t B = 0;
		for (int r = 0; r < 8; r++) {
			for (int c = 0; c < 8; c++) {
				if (array[c * 8 + r] == -1) {
					B |= tools.BIT[8 * r + c];
				}
				else if (array[c * 8 + r] == 1) {
					W |= tools.BIT[8 * r + c];
				}
			}
		}
		//std::vector<uint64_t >possible =((curcolor==1)? tools.gen_movelist(W, B): tools.gen_movelist(B, W));
		std::vector<uint64_t >possible = tools.gen_movelist(W, B);
		if (possible.size() == 0 && ( (curcolor == 1) ? tools.gen_movelist(B, W) : tools.gen_movelist(W, B)).size() == 0) {
			cout << "game over, the winner is: " << ((tools.getwinner(array)) ? "black" : "white") << endl;
			break;
		}
		else if (possible.size()==0){
			cout << ((curcolor == 1) ? "black" : "white") << " have no legal pos, jump turn" << endl;
			curcolor = -curcolor;
			possible = (curcolor == 1) ? tools.gen_movelist(B, W) : tools.gen_movelist(W, B);
		}


		cout << "please input the pos of next chess" << "color: " << ((curcolor == 1) ? "black" : "white") << endl;
		cin >> x >> y;
		int temppos=(y-1)*8+x-1;
		bool flag=false;
		for (int i = 0; i < possible.size(); i++)
			if (temppos == possible[i])flag = true;
		if (flag) {
			tools.dochange(&array, temppos,curcolor);
			curcolor = -curcolor;
		}
		else {
			cout << "illegal pos, redo" << endl;
			continue;
		}
		for (int i = 0; i < 64; i++) {
			if (!(i % 8))cout << endl;
			cout << array[i] << "\t";
		}
		cout << endl;
		cout << endl;
	}
	

	system("pause");
}
