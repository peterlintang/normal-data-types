
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN	1024

static int cmp(const void *s1, const void *s2)
{
	char **p1 = (char **)s1;
	char **p2 = (char **)s2;

	return strcmp(*p1, *p2);
}

static int cmplen(char *s1, char *s2)
{
	int len = 0;

	while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2) 
	{
		s1++;
		s2++;
		len++;
	}

	return len;
}

/*
 * 计算字符串中重复出现的最长子字符串
 */
int main(int argc, char *argv[])
{
	int maxlen = 0;
	int index_i = 0;
	int index_j = 0;
	int len = LEN;
	int ret = 0;
	char *words = NULL;
	char **pwords = NULL;
	int seed = 19;

	if (argc == 3)
	{
		len = atoi(argv[1]);
		seed = atoi(argv[2]);
	}

	srand(seed);

	words = (char *)calloc(len + 1, sizeof(char));
	pwords = (char **)calloc(len, sizeof(char *));


	for (int i = 0; i < len; i++)
	{
		words[i] = 'a' + rand() % 26;
		pwords[i] = &(words[i]);
	}
	words[len] = '\0';


	qsort(pwords, len, sizeof(char *), cmp);


	for (int i = 0; i < len - 1; i++)
	{
		ret = cmplen(pwords[i], pwords[i + 1]);
		if (ret > maxlen)
		{
			maxlen = ret;
			index_i = i;
			index_j = i + 1;
		}
	}

	fprintf(stdout, "i: %d, j: %d, max: %d\n", index_i, index_j, maxlen);

	for (int i = 0; i < maxlen; i++)
		fprintf(stdout, "%c", pwords[index_i][i]);
	fprintf(stdout, "\n");

	return 0;
}




