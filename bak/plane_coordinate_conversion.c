
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int plane_coordinate_conversion(int orig_x, int orig_y, int x0, int y0, double angel, int *ox, int *oy)
{
	float dx = 0.0f;
	float dy = 0.0f;

	dx = orig_x * cos(angel) - orig_y * sin(angel) + x0;
	dy = orig_x * sin(angel) + orig_y * cos(angel) + y0;
	
	printf("test: %f %f\n", dx, dy);
	*ox = (int)dx;
	*oy = (int)dy;
	printf("test: %d %d\n", *ox, *oy);
	return 0;
}


int main(int argc, char *argv[])
{
	int orig_x = 1;
	int orig_y = 1;
	double angel = M_PI / 2.0f;
	int ox = 0;
	int oy = 0;
	int x0 = 0;
	int y0 = 0;

	plane_coordinate_conversion(orig_x, orig_y, x0, y0, angel, &ox, &oy);
	printf("%d %d, %d %d\n", orig_x, orig_y, ox, oy);
	
	return 0;
}
