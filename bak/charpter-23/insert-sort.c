
#include <stdio.h>
#include <stdlib.h>

/*
 * insert sort
 */
int sort_weight_e(int *wes, int wes_num)
{
    int w;
	int i; 
	int j;
	int k;

    for (i = 1; i < wes_num; i++)
    {
        w = wes[i];
        for (j = i; j > 0; j--)
        {
            if (w < wes[j - 1])
            {
                wes[j] = wes[j - 1];
            }
            else
            {
                break;
            }
        }
    	wes[j] = w;

		for (k = 0; k < wes_num; k++)
			fprintf(stdout, "%d ", wes[k]);
		fprintf(stdout, "\n");
    }


    return 0;
}

int main(int argc, char *argv[])
{
	int num = 14;
	int wes[] = {
			4,8,8,11,
			7,4,2,9,
			14,10,2,6,
			1,7};

	fprintf(stdout, "before sort\n");
	for (int i = 0; i < num; i++)
	{
		fprintf(stdout, "%d ", wes[i]);
	}
	fprintf(stdout, "\n");
	sort_weight_e(wes, num);
	fprintf(stdout, "after sort\n");
	for (int i = 0; i < num; i++)
	{
		fprintf(stdout, "%d ", wes[i]);
	}
	fprintf(stdout, "\n");

	return 0;
}




