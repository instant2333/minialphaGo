#include"MCTS.h"


//typedef struct MCTStree {
//	int laststep;
//	int curcolor;
//	Tree *parent;
//	std::map<int, pTree> childmap;
//	int visitcount;
//	int wincount;
//	std::array<int, 64> array;
//	double UCBvalue;
//	MCTStree() {
//		this->childmap = std::map<int, pTree>();
//		this->laststep = -0;
//		this->curcolor = 0;
//		this->visitcount = 0;
//		this->wincount = 0;
//		this->UCBvalue = 0;
//	}
//}Tree, *pTree;




int MCTS::getbeststep(pTree root) {
	std::array<int, 64> board=root->board;
	int curcolor = root->curcolor;
	std::vector<int >pmoves=tools.gen_movelist(board, curcolor);
	MCTSpolicy(root, pmoves);
}

void MCTS::Backvisit(pTree cur) {
	while (cur->parent!=NULL){
		cur = cur->parent;
		cur->visitcount++;
	}
}
void MCTS::Backwin(pTree cur) {
	while (cur->parent != NULL) {
		cur = cur->parent;
		cur->wincount++;
	}
}
pTree MCTS::MCTSpolicy(pTree root, std::vector<int>pmoves) {
	if (root->childmap.empty()) {
		for (int i = 0; i < pmoves.size(); i++) {
			pTree child = &MCTStree();
			child->curcolor = -(root->curcolor);
			child->mycolor = root->mycolor;
			child->parent = root;
			child->laststep = pmoves[i];
			child->UCBvalue = 99999;
			child->board = tools.dochange(root->board,child->laststep,root->curcolor);
			root->childmap.insert(std::pair <int, pTree>(pmoves[i], child));
		}
		std::default_random_engine e;
		std::uniform_int_distribution<unsigned> u(0, pmoves.size()); //随机数分布对象 
		for (int i = 0; i < pmoves.size(); i++) {
			if (defaultpolicy(root->childmap[pmoves[i]]->board, root->childmap[pmoves[i]]->curcolor)) {
				root->childmap[pmoves[i]]->wincount++;
			}
			root->childmap[pmoves[i]]->visitcount++;
		}

		/*for (int i = 0; i < 100000; i++) {
			int temp = u(e);
			if (defaultpolicy(root->childmap[pmoves[temp]]->board, root->childmap[pmoves[temp]]->curcolor)) {
				root->childmap[pmoves[temp]]->wincount++;
			}
			root->childmap[pmoves[temp]]->visitcount++;
		}
		std::map<int, pTree>::iterator it=root->childmap.begin();
		while(it!=root->childmap.end()){
			root->visitcount+=it->second->visitcount;
			root->wincount += it->second->wincount;
		}*/
	}


}
bool MCTS::defaultpolicy(std::array<int, 64> array, int curcolor) {

}