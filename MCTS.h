#pragma once
#ifndef MCTS_H
#define MCTS_H
#define SEARCH_TINES 200000//最大搜索次数
#define SEARCH_TIME 30//最长搜索时间


#include<iostream>
#include <vector>
#include <array>

struct _state {
	int this_color;//当前下棋的颜色
	std::array<int, 64> array;//棋盘格局
	int isEnd;//判断是否终局
};

typedef struct _MCTS_node {
	struct _state *state;//棋局
	struct _MCTS_node *parent;//父节点
	struct _MCTS_node *children_head;//第一个子节点
	struct _MCTS_node *children_tail;//最后一个子节点
	struct _MCTS_node *search_tail;//当前模拟的最后一个子节点
	struct _MCTS_node *next_sibling;//下一个兄弟节点
	int sim_times;//当前模拟次数
	int win_times;//当前获胜次数
}MCTS_node;

double UCTSearch(struct _state *game);

#endif // ! MCTS_H