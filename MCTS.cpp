#include "MCTS.h"
#include "board.h"
#include <math.h>

boardfunc tools = boardfunc();

//创建根节点
MCTS_node *new_MCT_root(struct _state *game)
{
	MCTS_node *this_node = (MCTS_node *)malloc(sizeof(struct _MCTS_node));
	this_node->state = (struct _state*)malloc(sizeof(struct _state));
	this_node->children_head = NULL;
	this_node->children_tail = NULL;
	memcpy(this_node->state, game, sizeof(struct _state));//保存当前棋局状态
	this_node->parent = NULL;
	this_node->search_tail = NULL;
	this_node->next_sibling = NULL;
	this_node->sim_times = 0;
	this_node->win_times = 0;
	return this_node;
}

//创建叶子节点
MCTS_node *new_MCT_leaf(MCTS_node *parent)
{
	MCTS_node *this_node = new_MCT_root(parent->state);
	this_node->parent = parent;
	return this_node;
}

//判断能否继续
bool can_continue(std::array<int, 64> array, int color)
{
	std::vector<uint64_t >possible = tools.gen_movelist(array, color);
	if (possible.size() == 0)
	{
		return false;
	}
	return true;
}

//判断获胜者
int get_winner(struct _state *game)
{
	int white_num = 0, black_num = 0;
	for (int i = 0; i < 64; i++)
	{
		if (game->array[i] == -1)
		{
			white_num++;//白棋
		}
		else if (game->array[i] == 1)
		{
			black_num++;//黑棋
		}
	}
	if (white_num == black_num)
	{
		game->isEnd = 2;//平局
	}
	else
	{
		game->isEnd = white_num > black_num ? -1 : 1;
	}
	return game->isEnd;
}

//变换棋子颜色
int get_another_color(int color)
{
	return color == 1 ? -1 : 1;
}

//变换棋盘格局进入侠义格局
struct _state *get_next_state(struct _state *game, uint64_t pos)
{
	//翻转棋子
	tools.dochange(&game->array, pos, game->this_color);
	//对方有棋子可下
	if (can_continue(game->array, get_another_color(game->this_color)))
	{
		game->this_color = get_another_color(game->this_color);
	}
	//对方无棋子可下
	else
	{
		//自己也无棋子可下
		if (!can_continue(game->array, game->this_color))
		{
			game->this_color = 0;
			game->isEnd = get_winner(game);
		}
	}
	return game;
}

//拓展一个节点
void expand(MCTS_node *parent)
{
	MCTS_node *children_head = NULL;
	MCTS_node *children_this = NULL;
	std::vector<uint64_t >possible = tools.gen_movelist(parent->state->array, parent->state->this_color);
	for (int i = 0; i < possible.size(); i++)
	{
		if (children_this == NULL)
		{
			children_head = new_MCT_leaf(parent);
			children_head->state = get_next_state(children_head->state, possible[i]);
			children_this = children_head;
		}
		else
		{
			MCTS_node *children_sibling = new_MCT_leaf(parent);
			children_sibling->state = get_next_state(children_sibling->state, possible[i]);
			children_this->next_sibling = children_sibling;
			children_this = children_sibling;
		}
	}
	parent->children_head = children_head;
	parent->children_tail = children_this;
	parent->search_tail = NULL;
}

//判断是否终局
bool isTerminal(MCTS_node *node)
{
	if (node->children_head == NULL)
	{
		expand(node);
	}
	return node->state->isEnd != 0;
}

//计算每一个节点的UCB值
double get_ucb(MCTS_node *this_node, int N_total)
{
	double N, W;
	N = (double)(this_node->sim_times);
	W = (double)(this_node->win_times);
	//double C = 1 / 2;
	if (N == 0) {
		return INFINITY - 1;
	}
	//return W / N + C * sqrt(2 * log(N_total) / N);
	return W / N + sqrt(log(N_total) / N);
}

//选取对手棋最低的UCB,我方棋最高的UCB
MCTS_node *best_child(MCTS_node *parent, int color)
{
	MCTS_node *best_node = parent;
	MCTS_node *this_node;
	double best_ucb_same = -1;
	double best_ucb_diff = INFINITY;
	int N = parent->sim_times;
	this_node = parent->children_head;
	while (this_node != NULL)
	{
		double ucb = get_ucb(this_node, N);
		if (parent->state->this_color == color)
		{//我方棋选取最高UCB
			if (ucb > best_ucb_same)
			{
				best_ucb_same = ucb;
				best_node = this_node;
			}
		}
		else
		{//对手棋UCB选取最低
			if (ucb < best_ucb_diff)
			{
				best_ucb_diff = ucb;
				best_node = this_node;
			}
		}
		this_node = this_node->next_sibling;
	}
	return best_node;
}

//随机下棋
struct _state *get_next_state_with_random_choice(struct _state *game)
{
	std::vector<uint64_t >possible = tools.gen_movelist(game->array, game->this_color);
	int c = rand() % possible.size();//随机下棋
	game = get_next_state(game, possible[c]);
	return game;
}

//进行一次蒙特卡洛模拟
int default_policy(MCTS_node *parent)
{
	struct _state *state = (struct _state *)malloc(sizeof(struct _state));
	memcpy(state, parent->state, sizeof(*parent->state));
	while (!state->isEnd)
	{
		state = get_next_state_with_random_choice(state);
	}
	int winner = state->isEnd;
	free(state);
	return winner;
}

MCTS_node *tree_policy(MCTS_node *root)
{
	MCTS_node *best_node = root;
	MCTS_node *this_node;
	while (!isTerminal(best_node))
	{
		if (best_node->search_tail != best_node->children_tail)
		{
			if (best_node->search_tail == NULL)
			{//拓展第一个节点
				this_node = best_node->children_head;
				best_node->search_tail = this_node;
			}
			else
			{//拓展后续节点
				best_node->search_tail = best_node->search_tail->next_sibling;
				this_node = best_node->search_tail;
			}
			return this_node;
		}
		else
		{
			best_node = best_child(best_node, root->state->this_color);
		}
	}
	return best_node;
}

void backup(MCTS_node *node, int winner)
{
	while (node->parent!=NULL)
	{
		node->sim_times += 1;
		if (winner == node->state->this_color)
		{
			node->win_times += 1;
		}
		node = node->parent;
	}
	node->sim_times += 1;//根节点
}



double UCTSearch(struct _state *game)
{
	clock_t start, finish;
	double duration = 0;
	start = clock();
	MCTS_node *root = new_MCT_root(game);
	for (int i = 0; i < SEARCH_TINES && duration < SEARCH_TIME; i++)
	{
		MCTS_node *best_node = tree_policy(root);
		int winner_color = default_policy(best_node);
		//printf("winner_color:%d\n",winner_color);
		backup(best_node, winner_color);
		//finish = clock();
		//duration = double(finish - start) / CLOCKS_PER_SEC;
		//printf("%f\n",&duration);
		//
	}

	//计算时间
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	/*printf("%d:%d\n", start,finish);*/
	printf("cost time: %lf\n", duration);
	MCTS_node *best_node = best_child(root,root->state->this_color);
	memcpy(game, best_node->state, sizeof(struct _state));
	
	return duration;
}








