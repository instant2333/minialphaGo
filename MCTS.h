#pragma once
#ifndef MCTS_H
#define MCTS_H
#define SEARCH_TINES 200000//�����������
#define SEARCH_TIME 30//�����ʱ��


#include<iostream>
#include <vector>
#include <array>

struct _state {
	int this_color;//��ǰ�������ɫ
	std::array<int, 64> array;//���̸��
	int isEnd;//�ж��Ƿ��վ�
};

typedef struct _MCTS_node {
	struct _state *state;//���
	struct _MCTS_node *parent;//���ڵ�
	struct _MCTS_node *children_head;//��һ���ӽڵ�
	struct _MCTS_node *children_tail;//���һ���ӽڵ�
	struct _MCTS_node *search_tail;//��ǰģ������һ���ӽڵ�
	struct _MCTS_node *next_sibling;//��һ���ֵܽڵ�
	int sim_times;//��ǰģ�����
	int win_times;//��ǰ��ʤ����
}MCTS_node;

double UCTSearch(struct _state *game);

#endif // ! MCTS_H