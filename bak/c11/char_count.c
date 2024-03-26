
#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[])
{
	FILE *fp = NULL;
	int c = 0;
	int alpha_count = 0;
	int blank_count = 0;
	int cntrl_count = 0;
	int digit_count = 0;
	int graph_count = 0;
	int space_count = 0;
	int punct_count = 0;
	int unknow_count = 0;

	fp = fopen(argv[1], "r");

	while ((c = fgetc(fp)) != EOF)
	{
		if (isalpha(c))
			alpha_count++;
		else if (isblank(c))
			blank_count++;
		else if (iscntrl(c))
			cntrl_count++;
		else if (isdigit(c))
			digit_count++;
		else if (isgraph(c))
			graph_count++;
		else if (isspace(c))
			space_count++;
		else if (ispunct(c))
			punct_count++;
		else
			unknow_count++;
	}
	printf("alpha: %d\n", alpha_count);
	printf("blank: %d\n", blank_count);
	printf("cntrl: %d\n", cntrl_count);
	printf("digit: %d\n", digit_count);
	printf("graph: %d\n", graph_count);
	printf("space: %d\n", space_count);
	printf("punct: %d\n", punct_count);
	printf("unknow: %d\n", unknow_count);

	fclose(fp);
	return 0;
}
