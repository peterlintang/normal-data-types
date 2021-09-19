
#ifndef MY_SET_LIST_H__
#define MY_SET_LIST_H__


/*
 * use list to implement set (bu xiang jiao)
 */

#define SET_ERROR	-1

struct set;

struct set_item {
	struct set *set;
	struct set_item *next;
//	void *p;
	int p;
};

struct set {
	struct set_item *head;
	struct set_item *tail;
	int n;
	struct set *next;
	struct set *prev;
};

struct sets {
	struct set *head;
	struct set *tail;
	int n;
};

int set_make(struct sets *sets, int x);

int set_find(struct sets *sets, int x);

/*
 *
 *  union operation, the @sets must have 2 sets in the list
 **/
int set_union(struct sets *sets, int x, int y);

void set_print(struct sets *sets);

#endif // MY_SET_LIST_H__
