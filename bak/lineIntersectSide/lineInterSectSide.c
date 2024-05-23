
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Point {
	float x;
	float y;
};

int isLineIntersectSide(struct Point *A, struct Point *B, 
		struct Point *C, struct Point *D)
{
	float fc = 0.0f;
	float fd = 0.0f;

	fc = (C->y - A->y) * (A->x - B->x) - (C->x - A->x) * (A->y - B->y);
	fd = (D->y - A->y) * (A->x - B->x) - (D->x - A->x) * (A->y - B->y);

	if (fc * fd > 0)
		return 0;
	else
		return 1;
}

int intersectionPoint2LinesSegments(struct Point *A, struct Point *B, 
		struct Point *C, struct Point *D,
		struct Point *E)
{
	float a1 = 0.0f;
	float a2 = 0.0f;
	float b1 = 0.0f;
	float b2 = 0.0f;
	float c1 = 0.0f;
	float c2 = 0.0f;
	float d = 0.0f;
	int result = 0;

	a1 = B->y - A->y;
	a2 = D->y - C->y;

	b1 = -(B->x - A->x);
	b2 = -(D->x - C->x);

	c1 = -A->x * B->y + B->x * A->y;
	c2 = -C->x * D->y + D->x * C->y;

	d = a1 * b2 - a2 * b1;

	// mei kao lv gong xian deng wen ti
	result = isLineIntersectSide(A, B, C, D);
	if (result == 1)
	{
		E->x = (b1 * c2 - b2 * c1) / d;
		E->y = (a2 * c1 - a1 * c2) / d;
		return 1;
	}
	else
	{
		return 0;
	}
}

int main(int argc, char *argv[])
{
	struct Point A;
	struct Point B;
	struct Point C;
	struct Point D;
	struct Point E;

	int result = 0;
	A.x = 0.0f;
	A.y = 0.0f;
	B.x = 3.0f;
	B.y = 3.0f;
	C.x = 2.0f;
	C.y = 0.0f;
	D.x = 0.0f;
	D.y = 2.0f;

	result = isLineIntersectSide(&A, &B, &C, &D);

	fprintf(stdout, "result: %d\n", result);
	result = intersectionPoint2LinesSegments(&A, &B, &C, &D, &E);
	fprintf(stdout, "result: %d, %f, %f\n", result, E.x, E.y);
	return 0;
}


























