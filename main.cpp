//v2.0
#include <ctime>
#include <exception>
#include <iostream>
#include <cmath>
#include <cassert>
#include <limits>
#include <stack>
#include <cstring>
#include <vector>
#include "board.h"
#include "MCTS.h"
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
	int curcolor = 1;
	int x, y;

	srand(time(NULL));

	struct _state *state = (struct _state *)malloc(sizeof(struct _state));
	state->isEnd = 0;
	state->array = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, -1, 0, 0, 0,
		0, 0, 0,-1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};

	/*std::array<int, 64> array = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, -1, 0, 0, 0,
		0, 0, 0,-1, 1, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};*/
	/*std::array<int, 64> array = {
		1, -1, 1, 1, 1, 1, 1, 1,
		1, -1, 1, 1, 1, -1, -1, 1,
		1, -1, 1, 1, 1, -1, -1, 1,
		1, -1, 1, 1, 1, -1, -1, 1,
		1, -1, 1, 1, 1, -1, -1, 1,
		1, -1, 1, 1, 1, -1, -1, 1,
		1, -1, 1, 1, 1, -1, -1, 1,
		1, -1, 1, 1, 1, 1, 1, 1,
	};*/
	for (int i = 0; i < 64; i++) {
		if (!(i % 8))cout << endl;
		cout << state->array[i] << "\t";
	}
	cout << endl;
	cout << endl;

	while (true) {	
		//std::vector<uint64_t >possible =((curcolor==1)? tools.gen_movelist(W, B): tools.gen_movelist(B, W));
		std::vector<uint64_t >possible = tools.gen_movelist(state->array, curcolor);
		if (possible.size() == 0 ) {
			curcolor = -curcolor;
			possible= tools.gen_movelist(state->array, curcolor);
			if (possible.size() == 0) {
				cout << "game over, the winner is: " << ((tools.getwinner(state->array)) ? "black" : "white") << endl;
				break;
			}
			else{
				cout << ((curcolor == 1) ? "white" : "black") << " have no legal pos, jump turn" << endl;
			}
		}

		cout << "please input the pos of next chess" << "color: " << ((curcolor == 1) ? "black" : "white") << endl;
		cin >> x >> y;
		int temppos=(y-1)*8+x-1;
		bool flag=false;
		for (int i = 0; i < possible.size(); i++)
			if (temppos == possible[i])flag = true;
		if (flag) {
			tools.dochange(&state->array, temppos,curcolor);
			curcolor = -curcolor;
		}
		else {
			cout << "illegal pos, redo" << endl;
			continue;
		}
		for (int i = 1; i <= 8; i++){
			cout << i << "\t";

		}
		for (int i = 0; i < 64; i++) {
			if (!(i % 8))cout << i/8<<endl;
			cout << state->array[i] << "\t";
		}
		cout << endl;
		cout << endl;
		state->this_color = curcolor;
		UCTSearch(state);
		for (int i = 0; i < 64; i++) {
			if (!(i % 8))cout << i / 8 << endl;
			cout << state->array[i] << "\t";
		}
		curcolor = -curcolor;
		cout << endl;
	}
	

	system("pause");
}
