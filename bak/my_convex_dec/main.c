
#include <stdio.h>
#include <stdlib.h>

//extern int convert_concave_polygon(float *x, float *y, int num);
extern int convert_concave_polygon(float *x, float *y, int num, float ***ox, float ***oy, int *pnum, int **records);

int main(int argc, char *argv[])
{
	float x[] = {
		0.15f, 
		2.475f, 
		1.15f, 
		1.0f, 
		2.325f, 
		4.725f, 
		6.225f, 
		6.95f, 
		6.625f, 
		4.775f, 
		3.4f, 
		2.425f, 
		2.575f, 
		1.825f, 
		1.05f, 
		-2.225f,
		-3.4f, 
		-3.95f,
		-3.35f,
		-1.05f
	};
	float y[] = {
		 -3.65f, 
		 -3.275f, 
		 -3.175f, 
		 -2.175f, 
		 -1.775f, 
		 -1.725f, 
		 -1.175f, 
		 -0.05f, 
		 1.775f, 
		 4.15f, 
		 4.275f, 
		 3.175f, 
		 1.925f, 
		 0.0f, 
		-0.15f, 
		1.175f, 
		0.3f, 
		-1.25f, 
		-2.5f, 
		-3.625f
	};
	float **ox = NULL;
	float **oy = NULL;
	int num = 0;
	int *records = NULL;

	convert_concave_polygon(x, y, 20, &ox, &oy, &num, &records);

	fprintf(stdout, "%s num: %d\n", __func__, num);
	for (int i = 0; i < num; i++)
	{
		fprintf(stdout, "i %d record %d\n", i, records[i]);
		for (int j = 0; j < records[i]; j++)
		{
			fprintf(stdout, "%f %f\n", ox[i][j], oy[i][j]);
		}
	}
	fprintf(stdout, "%s num: %d\n", __func__, num);

	for (int i = 0; i < num; i++)
	{
		free(ox[i]);
		free(oy[i]);
	}
	free(ox);
	free(oy);
	free(records);

	return 0;
}

