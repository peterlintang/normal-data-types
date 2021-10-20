
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define NODE_LEN	1024
#define EDGE_LEN	1024
#define FILE_NAME	"data.txt"

int main(int argc, char *argv[])
{
	int node_len = NODE_LEN;
	int edge_len = EDGE_LEN;
	int node_value = 0;
	int edge_value1 = 0;
	int edge_value2 = 0;
	char str[64] = { 0 };
	char *filename = FILE_NAME;
	FILE *file = NULL;

	if (argc == 4)
	{
		node_len = atoi(argv[1]);
		edge_len = atoi(argv[2]);
		filename = argv[3];
	}
	fprintf(stdout, "nodes: %d, edges: %d, file: %s\n", node_len, edge_len, filename);

	file = fopen(filename, "w+");
	if (file == NULL)
	{
		fprintf(stderr, "failed to open file: %s %s\n", filename, strerror(errno));
		return 0;
	}

	snprintf(str, 64, "%d\n%d\n", node_len, edge_len);

	fwrite(str, strlen(str), 1, file);

	for (int i = 0; i < node_len; i++)
	{
		memset(str, 0, 64);
		node_value = i + 1;
		snprintf(str, 64, "%d\n", node_value);
		fwrite(str, strlen(str), 1, file);
	}

	for (int i = 0; i < edge_len; )
	{
		edge_value1 = random() % node_len + 1;
		edge_value2 = random() % node_len + 1;
		if (edge_value1 != edge_value2)
		{
			i++;
			memset(str, 0, 64);
			snprintf(str, 64, "%d,%d\n", edge_value1, edge_value2);
			fwrite(str, strlen(str), 1, file);
		}
	}

	fclose(file);

	return 0;
}


