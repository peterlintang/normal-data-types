

#include <stdio.h>
#include <stdlib.h>

#include "ConcavePolygon.h"




static int count = 0;
void drawPolygon(struct ConcavePolygon *poly)
{
    if(getNumberSubPolys(poly) > 0)
    {
        drawPolygon(getSubPolygon(poly, 0));
        drawPolygon(getSubPolygon(poly, 1));
    }
    else
    {
    	fprintf(stdout, "%s: count: %d\n", __func__, count++);
    	for(unsigned int i = 0; i < poly->vertices_num; ++i)
    	{
        	fprintf(stdout, "%f, %f\n", poly->vertices[i].position.x, poly->vertices[i].position.y);

    	}
    }
}

void get_polygons_allVertices(struct ConcavePolygon *poly, float **ox, float **oy, int *num, int *records, int max)
{
	float *x = NULL;
	float *y = NULL;

    if(getNumberSubPolys(poly) > 0)
    {
        get_polygons_allVertices(getSubPolygon(poly, 0), ox, oy, num, records, max);
        get_polygons_allVertices(getSubPolygon(poly, 1), ox, oy, num, records, max);
    }
    else
    {


    x = (float *)calloc(poly->vertices_num, sizeof(float));
    y = (float *)calloc(poly->vertices_num, sizeof(float));

	for(unsigned int i = 0; i < poly->vertices_num; ++i)
	{
		x[i] = poly->vertices[i].position.x; 
		y[i] = poly->vertices[i].position.y;
	}

	ox[*num] = x;
	oy[*num] = y;
	records[*num] = poly->vertices_num ;
	(*num)++;
    }
}


int convert_concave_polygon(float *x, float *y, int num, float ***pox, float ***poy, int *pnum, int **precords)
{
	int total = 0;
	int index = 0;
	float **ox = NULL;
	float **oy = NULL;
	int *records = NULL;
	struct Vertex verts[num];

	for (int i = 0; i < num; i++)
	{
		verts[i].position.x = x[i];
		verts[i].position.y = y[i];
	}

	struct ConcavePolygon poly;
	ConcavePolygon(&poly, verts, num);
	convexDecomp(&poly);
	total = getTotalSubPolys(&poly);
	fprintf(stdout, "%s total: %d\n", __func__, total);
	ox = (float **)calloc(total, sizeof(float *));
	oy = (float **)calloc(total, sizeof(float *));
	records = (int *)calloc(total, sizeof(int));
	fprintf(stdout, "%s total: %d before\n", __func__, total);
	get_polygons_allVertices(&poly, ox, oy, &index, records, total);
	fprintf(stdout, "%s total: %d after\n", __func__, total);
	*pox = ox;
	*poy = oy;
	*pnum = total;
	*precords = records;
	drawPolygon(&poly);
	return 0;
}


