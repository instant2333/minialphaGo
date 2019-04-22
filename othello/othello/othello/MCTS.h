#pragma once
#include"board.h"
#include<iostream>
#include<utility>
#include<random>
typedef struct MCTStree {
	int laststep;
	int curcolor;
	int mycolor;
	Tree *parent;
	std::map<int, pTree> childmap;
	//Tree *nextsilbing;
	//Tree *firstchild;
	int visitcount;
	int wincount;
	std::array<int, 64> board;
	double UCBvalue;
	MCTStree() {
		this->childmap = std::map<int, pTree>();
		this->board = std::array<int, 64>();
		this->laststep = -0;
		this->curcolor = 0;
		this->mycolor = 0;
		this->visitcount = 0;
		this->wincount = 0;
		this->UCBvalue = 0;
	}
}Tree,*pTree;
class MCTS {
public:
	boardfunc tools;
	//pTree curroot;
	int getbeststep(pTree root);
	pTree MCTSpolicy(pTree root, std::vector<int>pmoves);
	bool defaultpolicy(std::array<int, 64> array,int curcolor);
	void Backvisit(pTree cur);
	void Backwin(pTree cur);
	/*MCTS(pTree curroot) {
		this->curroot = curroot;
	}*/
	//~MCTS();
};

