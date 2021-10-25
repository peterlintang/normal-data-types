
/*
 * 用双向链表实现集合数据结构
 * 集合中的元素应该唯一
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "set-list.h"

#define T SetL_T


struct T {
	ListD_T l;
	int (*cmp)(void *priv, void *arg);
};

/*
 * 新建一个链表集合，成功返回指针，失败NULL
 */
T MODULE_FUN_NAME(SetL, new)(int (*cmp)(void *, void *))
{
	assert(cmp);

	T set = NULL;

	set = (T)calloc(1, sizeof(*set));
	if (set)
	{
		set->l = MODULE_FUN_NAME(ListD, new)();
		if (set->l)
		{
			set->cmp = cmp;
		}
		else
		{
			free(set);
			set = NULL;
		}
	}

	return set;
}

/*
 * 释放一个链表集合内容
 */
void MODULE_FUN_NAME(SetL, free)(T *setp)
{
	assert(setp);
	assert(*setp);

	MODULE_FUN_NAME(ListD, free)(&((*setp)->l));

	free(*setp);
	*setp = NULL;
}

/*
 * 将新元素priv添加到集合set中
 * 成功返回0，失败-1, -2 已存在
 */
int MODULE_FUN_NAME(SetL, add)(T set, void *priv)
{
	assert(set);

	ListDNode_T node = NULL;

	if (MODULE_FUN_NAME(SetL, isMember)(set, priv))
	{
		return -2;
	}

	node = MODULE_FUN_NAME(ListDNode, new)(priv);
	if (node == NULL)
	{
		return -1;
	}

	MODULE_FUN_NAME(ListD, insert)(set->l, node);
	return 0;
}

/*
 * 将元素priv从集合set中删除
 * 成功返回指针，失败NULL
 */
void *MODULE_FUN_NAME(SetL, remove)(T set, void *priv)
{
	assert(set);

	ListDNode_T node = NULL;
	void *key = NULL;

	node = MODULE_FUN_NAME(ListD, search)(set->l, set->cmp, priv);
	if (node)
	{
		key = node->priv;
		MODULE_FUN_NAME(ListD, remove)(set->l, node);
		MODULE_FUN_NAME(ListDNode, free)(&node);
		return priv;
	}
	else
	{
		return NULL;
	}
}

/*
 * 集合set中查找元素,
 * 成功返回指针，失败NULL
 */
void *MODULE_FUN_NAME(SetL, search)(T set, void *priv)
{
	assert(set);

	ListDNode_T node = NULL;

	node = MODULE_FUN_NAME(ListD, search)(set->l, set->cmp, priv);
	if (node)
	{
		return node->priv;
	}
	else
	{
		return NULL;
	}
}

/*
 * 判断priv是否在集合set中，
 * 是返回1，不是0
 */
int MODULE_FUN_NAME(SetL, isMember)(T set, void *priv)
{
	assert(set);

	ListDNode_T node = NULL;

	node = MODULE_FUN_NAME(ListD, search)(set->l, set->cmp, priv);
	if (node)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
 * 返回集合set中元素的数目
 */
int MODULE_FUN_NAME(SetL, count)(T set)
{
	assert(set);

	return MODULE_FUN_NAME(ListD, count)(set->l);
}

/*
 * 根据索引返回集合中的第index个元素
 * 超出元素个数返回NULL,成功返回指针
 */
void *MODULE_FUN_NAME(SetL, get)(T set, int index)
{
	assert(set);

	ListDNode_T node = NULL;

	if (index < 0 || index >= MODULE_FUN_NAME(SetL, count)(set))
	{
		return NULL;
	}

	node = MODULE_FUN_NAME(ListD, get)(set->l, index);
	return node->priv;
}

/********************集合高级操作*******************/

static void copy(T set, T s)
{
	int count = 0;
	void *priv = NULL;

	if (s == NULL)
	{
		return ;
	}

	count = MODULE_FUN_NAME(SetL, count)(s);
	for (int i = 0; i < count; i++)
	{
		priv = MODULE_FUN_NAME(SetL, get)(s, i);
		MODULE_FUN_NAME(SetL, add)(set, priv);
	}
}

/*
 * 新的集合为s U t，失败NULL
 * 集合s与t的并集
 */
T MODULE_FUN_NAME(SetL, union)(T s, T t)
{
	assert(s != NULL || t != NULL);

	T set = NULL;

	assert(s->cmp == t->cmp);

	if (s == NULL)
	{
		assert(t);

		set = MODULE_FUN_NAME(SetL, new)(t->cmp);
		if (set == NULL)
		{
			return NULL;
		}

		copy(set, t);

		return set;
	}
	else if (t == NULL)
	{
		set = MODULE_FUN_NAME(SetL, new)(s->cmp);
		if (set == NULL)
		{
			return NULL;
		}

		copy(set, s);

		return set;
	}
	else
	{
		set = MODULE_FUN_NAME(SetL, new)(s->cmp);
		if (set == NULL)
		{
			return NULL;
		}

		copy(set, t);

		int count = MODULE_FUN_NAME(SetL, count)(s);
		void *priv = NULL;
		for (int i = 0; i < count; i++)
		{
			priv = MODULE_FUN_NAME(SetL, get)(s, i);
			if (MODULE_FUN_NAME(SetL, isMember)(set, priv) == 0)
			{
				MODULE_FUN_NAME(SetL, add)(set, priv);
			}
		}
	}

	return set;
}

/*
 * 新的集合为s * t，失败NULL
 * 集合s与t的交集
 */
T MODULE_FUN_NAME(SetL, inter)(T s, T t)
{
	assert(s != NULL || t != NULL);

	T set = NULL;
	int count = 0;

	assert(s->cmp == t->cmp);

	if (s == NULL)
	{
		set = MODULE_FUN_NAME(SetL, new)(t->cmp);
		return set;
	}
	else if (t == NULL)
	{
		set = MODULE_FUN_NAME(SetL, new)(s->cmp);
		return set;
	}
	else if (MODULE_FUN_NAME(SetL, count)(s) < MODULE_FUN_NAME(SetL, count)(t))
	{
		return MODULE_FUN_NAME(SetL, inter)(t, s);
	}
	else
	{
		set = MODULE_FUN_NAME(SetL, new)(s->cmp);
		if (set == NULL)
		{
			return set;
		}

		int count = MODULE_FUN_NAME(SetL, count)(t);
		void *priv = NULL;

		for (int i = 0; i < count; i++)
		{
			priv = MODULE_FUN_NAME(SetL, get)(t, i);
			if (MODULE_FUN_NAME(SetL, isMember)(s, priv))
			{
				MODULE_FUN_NAME(SetL, add)(set, priv);
			}
		}

		return set;
	}

}

/*
 * 新的集合为s / t，失败NULL
 * 新集合的元素：仅在集合s,或仅在集合t
 */
T MODULE_FUN_NAME(SetL, diff)(T s, T t)
{
	assert(s != NULL || t != NULL);

	T set = NULL;
	int count = 0;

	assert(s->cmp == t->cmp);

	if (s == NULL)
	{
		set = MODULE_FUN_NAME(SetL, new)(t->cmp);
		copy(set, t);
		return set;
	}
	else if (t == NULL)
	{
		set = MODULE_FUN_NAME(SetL, new)(s->cmp);
		copy(set, s);
		return set;
	}
	else
	{
		set = MODULE_FUN_NAME(SetL, new)(s->cmp);
		if (set == NULL)
		{
			return set;
		}

		int count = 0;
		void *priv = NULL;

		count = MODULE_FUN_NAME(SetL, count)(t);
		for (int i = 0; i < count; i++)
		{
			priv = MODULE_FUN_NAME(SetL, get)(t, i);
			if (MODULE_FUN_NAME(SetL, isMember)(s, priv) == 0)
			{
				MODULE_FUN_NAME(SetL, add)(set, priv);
			}
		}

		count = MODULE_FUN_NAME(SetL, count)(s);
		for (int i = 0; i < count; i++)
		{
			priv = MODULE_FUN_NAME(SetL, get)(s, i);
			if (MODULE_FUN_NAME(SetL, isMember)(t, priv) == 0)
			{
				MODULE_FUN_NAME(SetL, add)(set, priv);
			}
		}

		return set;
	}

}

/*
 * 新的集合为s - t，失败NULL
 * 新集合的元素：在集合s,但不在t
 */
T MODULE_FUN_NAME(SetL, minus)(T s, T t)
{
	assert(s != NULL || t != NULL);

	T set = NULL;

	if (s == NULL)
	{
		set = MODULE_FUN_NAME(SetL, new)(t->cmp);
		return set;
	}
	else if (t == NULL)
	{
		set = MODULE_FUN_NAME(SetL, new)(s->cmp);
		if (set == NULL)
		{
			return NULL;
		}

		copy(set, s);
		return set;
	}
	else
	{
		set = MODULE_FUN_NAME(SetL, new)(s->cmp);
		if (set == NULL)
		{
			return NULL;
		}

		int count = MODULE_FUN_NAME(SetL, count)(s);
		void *priv = NULL;

		for (int i = 0; i < count; i++)
		{
			priv = MODULE_FUN_NAME(SetL, get)(s, i);
			if (MODULE_FUN_NAME(SetL, isMember)(t, priv) == 0)
			{
				MODULE_FUN_NAME(SetL, add)(set, priv);
			}
		}

		return set;
	}
}




#if 0

static void _set_destroy(struct set **setp)
{
	assert(setp);
	assert(*setp);

	free(*setp);
	*setp = NULL;
}

static struct set *_set_make(int x)
{
	struct set *set = NULL;
	struct set_item *item = NULL;

	assert(x);

	set = (struct set *)calloc(1, sizeof(*set));
	if (set)
	{
		item = (struct set_item *)calloc(1, sizeof(*item));
		if (item)
		{
			set->head = item;
			set->tail = item;
			set->next = NULL;
			set->prev = NULL;
			set->n = 1;
			item->p = x;
			item->set = set;
			item->next = NULL;
		}
		else
		{
			free(set);
			set = NULL;
		}
	}

	return set;
}

/*
 * attention: user free @x and @y
 */
static struct set *_set_union(struct set *x, struct set *y)
{
	struct set *z = NULL;
	struct set_item *item = NULL;

	assert(x);
	assert(y);

	/* not empty set */
	assert(x->head);
	assert(y->head);

	z = (struct set *)calloc(1, sizeof(*z));
	if (NULL == z)
	{
		return NULL;
	}

	z->head = x->head;
	z->tail = x->tail;
	z->n = x->n;

	do 
	{
		item = y->head;
		y->head = item->next;
		y->n -= 1;

		z->tail->next = item;
		item->next = NULL;
		z->tail = item;
		z->n += 1;

	} while (y->head != NULL);

	x->head = NULL;
	x->tail = NULL;
	x->n = 0;

	y->head = NULL;
	y->tail = NULL;
	y->n = 0;

	for (item = z->head; item != NULL; item = item->next)
	{
		item->set = z;
	}

	return z;
}

static struct set *_set_find(struct set *set, int x)
{
	struct set_item *item = NULL;

	assert(set);
	assert(x);

	for (item = set->head; item != NULL; item = item->next)
	{
		if (item->p == x)
			return set;
	}

	return NULL;
}


/*****************************/
int set_make(struct sets *sets, int x)
{
	struct set *set = NULL;

	assert(sets);

	set = _set_make(x);
	if (set)
	{
		set->prev = NULL;
		set->next = NULL;

		if (sets->head == NULL)
		{
			sets->head = set;
			sets->tail = set;
			sets->n = 1;
		}
		else
		{
			sets->tail->next = set;
			set->prev = sets->tail;
			sets->tail = set;
			sets->n += 1;
		}

		return x;
	}
	else
	{
		fprintf(stderr, "%s: no mem\n", __func__);
		return SET_ERROR;
	}
}

int set_find(struct sets *sets, int x)
{
	struct set *set = NULL;

	assert(sets);

	for (set = sets->head; set != NULL; set = set->next)
	{
		if (_set_find(set, x))
			return set->head->p;
	}

	return SET_ERROR;
}

/*
 *
 *  union operation, the @sets must have 2 sets in the list
 **/
int set_union(struct sets *sets, int x, int y)
{
	struct set *set_x = NULL;
	struct set *set_y = NULL;
	struct set *set = NULL;

	assert(sets);

	for (set = sets->head; set != NULL; set = set->next)
	{
		if (set_x == NULL)
			set_x = _set_find(set, x);
		if (set_y == NULL)
			set_y = _set_find(set, y);

		if ((set_x != NULL) && (set_y != NULL))
			break;
	}

	assert(set_x);
	assert(set_y);
	assert(set_x != set_y);

	set = _set_union(set_x, set_y);
	if (NULL == set)
	{
		fprintf(stderr, "%s: no mem", __func__);
		return SET_ERROR;
	}

	if (set_x == sets->head)
	{
		sets->head = set_x->next;
		if (sets->head)
			sets->head->prev = NULL;
	}
	else if (set_x == sets->tail)
	{
		sets->tail = set_x->prev;
		if (sets->tail)
			sets->tail->next = NULL;
	}
	else
	{
		set_x->prev->next = set_x->next;
		set_x->next->prev = set_x->prev;
	}
	set_x->prev = NULL;
	set_x->next = NULL;
	sets->n -= 1;

	if (set_y == sets->head)
	{
		sets->head = set_y->next;
		if (sets->head)
			sets->head->prev = NULL;
	}
	else if (set_y == sets->tail)
	{
		sets->tail = set_y->prev;
		if (sets->tail)
			sets->tail->next = NULL;
	}
	else
	{
		set_y->prev->next = set_y->next;
		set_y->next->prev = set_y->prev;
	}
	set_y->prev = NULL;
	set_y->next = NULL;
	sets->n -= 1;

	_set_destroy(&set_x);
	_set_destroy(&set_y);

	if (sets->n == 0)
	{
		sets->head = set;
		sets->tail = set;
		set->next = NULL;
		set->prev = NULL;
	}
	else
	{
		sets->tail->next = set;
		set->prev = sets->tail;
		sets->tail = set;
		set->next = NULL;
	}
	sets->n += 1;

	return set->head->p;
}

void set_print(struct sets *sets)
{
	struct set *set = NULL;
	struct set_item *item = NULL;

	assert(sets);

	for (set = sets->head; set != NULL; set = set->next)
	{
		for (item = set->head; item != NULL; item = item->next)
		{
			fprintf(stdout, " %d, %p;", item->p, item->set);
		}
		fprintf(stdout, "\n");
	}
}

/***************************/

struct V {
	int v;
};

struct E {
	int u;
	int v;
};

struct G {
	struct V *vs;
	int vs_num;
	struct E *es;
	int es_num;
};

void connected_components(struct sets *sets, struct G *g)
{
	for (int i = 0; i < g->vs_num; i++)
	{
		set_make(sets, g->vs[i].v);
	}

	for (int i = 0; i < g->es_num; i++)
	{
		if (set_find(sets, g->es[i].u) != set_find(sets, g->es[i].v))
		{
			set_union(sets, g->es[i].u, g->es[i].v);
		}
	}
}

int same_components(struct sets *sets, int x, int y)
{
	int x_r = 0;
	int y_r = 0;

	x_r = set_find(sets, x);
	y_r = set_find(sets, y);

	if ((x_r != SET_ERROR) && (x_r == y_r))
		return 1;
	else
		return 0;
}


/*
 * test code
int main(int argc, char *argv[])
{
	struct sets sets;
	struct G g;
	struct V *vs = NULL;
	struct E *es = NULL;
	int x;
	int y;

	if (argc != 3)
	{
		fprintf(stderr, "usage: %s num1 num2\n", argv[0]);
		exit(0);
	}


	vs = (struct V *)calloc(10, sizeof(*vs));
	es = (struct E *)calloc(7, sizeof(*es));

	for (int i = 0; i < 10; i++)
	{
		vs[i].v = i + 1;
	}

	es[0].u = 1;
	es[0].v = 2;
	es[1].u = 1;
	es[1].v = 3;
	es[2].u = 2;
	es[2].v = 3;
	es[3].u = 2;
	es[3].v = 4;
	es[4].u = 5;
	es[4].v = 6;
	es[5].u = 5;
	es[5].v = 7;
	es[6].u = 8;
	es[6].v = 9;

	g.vs = vs;
	g.vs_num = 10;
	g.es = es;
	g.es_num = 7;

	sets.head = NULL;
	sets.tail = NULL;
	sets.n = 0;

	connected_components(&sets, &g);

	x = atoi(argv[1]);
	y = atoi(argv[2]);
	if (same_components(&sets, x, y))
		fprintf(stdout, "%d, %d is in the same set\n", x, y);
	else
		fprintf(stdout, "%d, %d is not in the same set\n", x, y);

	set_print(&sets);

	return 0;
}
 */


#endif


