
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Point {
	double x;
	double y;
};

int isLineIntersectSide(struct Point *A, struct Point *B, 
		struct Point *C, struct Point *D)
{
	double fc = 0.0f;
	double fd = 0.0f;

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
	double a1 = 0.0f;
	double a2 = 0.0f;
	double b1 = 0.0f;
	double b2 = 0.0f;
	double c1 = 0.0f;
	double c2 = 0.0f;
	double d = 0.0f;
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
		/*
		fprintf(stdout, "%s: a1: %.8f, a2: %.8f, b1: %.8f, b2: %.8f, c1: %.8f, c2: %.8f, d: %.8f\n",
				__func__, a1, a2, b1, b2, c1, c2, d);
		*/
		E->x = (b1 * c2 - b2 * c1) / d;
		E->y = (a2 * c1 - a1 * c2) / d;
		return 1;
	}
	else
	{
		return 0;
	}
}

double distance(struct Point *A, struct Point *B)
{
	double Ax = A->x * M_PI / 180.0;
	double Ay = A->y * M_PI / 180.0;
	double Bx = B->x * M_PI / 180.0;
	double By = B->y * M_PI / 180.0;
	double lat;
	double lon;
	double a;
	double c;
	double d;
	double R = 6371.0 * 1000;

	lat = Bx - Ax;
	lon = By - Ay;
	a = pow(sin(lat / 2), 2) + cos(Ax) * cos(Bx) * pow(sin(lon / 2), 2);
	c = 2 * atan2(sqrt(a), sqrt(1 - a));
	return d = R * c;
}

int main(int argc, char *argv[])
{
#if 0
	struct Point A;
	struct Point B;
	struct Point C;
	struct Point D;
	struct Point E;

	int result = 0;
	A.x = 0.0f;
	A.y = 0.0f;
	B.x = 0.5f;
	B.y = 0.5f;
	C.x = 3.0f;
	C.y = 0.0f;
	D.x = 0.0f;
	D.y = 3.0f;

	result = isLineIntersectSide(&A, &B, &C, &D);

	fprintf(stdout, "result: %d\n", result);
	result = intersectionPoint2LinesSegments(&A, &B, &C, &D, &E);
	fprintf(stdout, "result: %d, %f, %f\n", result, E.x, E.y);
	return 0;
#else

	struct Point A;
	struct Point B;
	struct Point E;
#if 0
	A.x = 1.0;
	A.y = 2.0;
	B.x = 2.0;
	B.y = 2.0;
	struct Point Points[] = {
		{0.0, 0.0},
		{8.0, 0.0},
		{8.0, 8.0},
		{0.0, 8.0},
		{0.0, 0.0},
	};
#else
	A.x = -95.263962;
	A.y = 38.977782;
	B.x = -95.267057;
	B.y = 38.978634;

	A.x = -95.263727;
	A.y = 38.977727;
	B.x = -95.267057;
	B.y = 38.978634;

	struct Point Points[] = {
		{-95.266989, 38.978656}, 
		{-95.26697, 38.978652}, 
		{-95.266956, 38.978645}, 
		{-95.266927, 38.978626}, 
		{-95.266915, 38.978616}, 
		{-95.266907, 38.978603}, 
		{-95.266903, 38.978591}, 
		{-95.266903, 38.97858}, 
		{-95.266915, 38.978554}, 
		{-95.266934, 38.978537}, 
		{-95.266963, 38.978523}, 
		{-95.266998, 38.978515}, 
		{-95.267012, 38.978516}, 
		{-95.267045, 38.978522}, 
		{-95.267078, 38.978536}, 
		{-95.267115, 38.978562}, 
		{-95.267147, 38.978593}, 
		{-95.267157, 38.978599}, 
		{-95.267206, 38.978621}, 
		{-95.267224, 38.978639}, 
		{-95.267228, 38.978649}, 
		{-95.26723, 38.978671}, 
		{-95.267229, 38.978683}, 
		{-95.267222, 38.978706}, 
		{-95.267208, 38.978725}, 
		{-95.267187, 38.978738}, 
		{-95.267173, 38.978743}, 
		{-95.267144, 38.978746}, 
		{-95.267123, 38.978743}, 
		{-95.267112, 38.978737}, 
		{-95.2671, 38.97873}, 
		{-95.26706, 38.978693}, 
		{-95.267035, 38.978674}, 
		{-95.267008, 38.978662}, 
		{-95.266989, 38.978656}
	};
#endif

	int result = 0;
	fprintf(stdout, "total num: %ld\n", sizeof(Points) / sizeof(Points[0]));
	fprintf(stdout, "A: (%.8f %.8f), B: (%.8f %.8f), distance: %.8f\n", 
			A.x, A.y, B.x, B.y,
			distance(&A, &B)
			);
	for (int i = 0; i < sizeof(Points) / sizeof(Points[0]) - 1; i++)
	{
		E.x = 0;
		E.y = 0;
		result = intersectionPoint2LinesSegments(&A, &B, &Points[i + 1], &Points[(i )], &E);
		fprintf(stdout, "result: %d, (%.8f %.8f) (%.8f %.8f): (%.8f %.8f), distance: %.8f\n", result, 
				Points[i].x, 
				Points[i].y, 
				Points[(i + 1)].x, 
				Points[(i + 1)].y, 
				E.x, E.y,
				result ? distance(&A, &E) : 0.0);
	}

	return 0;
#endif
}


























