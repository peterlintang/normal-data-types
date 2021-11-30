
/*
 * 实现图的点边结构
 * 
 * date: 2021-11-30
 * author: lin
 */

#ifndef VE_H__
#define VE_H__

#define WHITE	0
#define GRAY	1
#define BLACK	2

#define NODE gve_node_t
#define EDGE_EXT gve_edge_ext_t

typedef struct NODE *NODE;
typedef struct EDGE_EXT *EDGE_EXT;

struct NODE {
	int index;
	int color;
	int d;
	int f;
	NODE prev;
	SenDlink_T l;
};

struct EDGE_EXT {
	int value;
};

/***************************print****************************/
void gve_V_print(NODE v);

void gve_E_print(EDGE_EXT edge);

void gve_G_print(GraphA_T g);

/*
 * 打印从原点到k的路径
 */
void G_tree_print(GraphA_T g, int k);

/*
 * 打印从s到v的路径
 */
void G_tree_print_path(GraphA_T g, int s, int v);

/***********************************************
 *
 * 图生成函数
 *
 ***********************************************/
/*
 * 根据文件内容创建gve图
 */
GraphA_T gve_create_graph(char *filename);

#undef NODE
#undef EDGE_EXT

#endif // end of VE_H__

