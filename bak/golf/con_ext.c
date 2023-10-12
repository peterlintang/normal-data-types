

#include <stdio.h>
#include <stdlib.h>

#include "ui_manager.h"
#include "os_common_api.h"
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
    	printf("%s: count: %d\n", __func__, count++);
    	for(unsigned int i = 0; i < poly->vertices_num; ++i)
    	{
        	printf("%f, %f\n", poly->vertices[i].position.x, poly->vertices[i].position.y);

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


    x = (float *)app_mem_malloc(poly->vertices_num * sizeof(float));
    y = (float *)app_mem_malloc(poly->vertices_num * sizeof(float));
	if (x == NULL || y == NULL)
	{
		printf("%s %d: malloc %d * %d bytes failed\n", __func__, __LINE__, poly->vertices_num, sizeof(float));
		app_mem_free(x);
		app_mem_free(y);
		return;
	}

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
//	struct Vertex verts[num];
	struct Vertex *verts = NULL;

	verts = (struct Vertex*)app_mem_malloc(num * sizeof(struct Vertex));
	if (verts == NULL)
	{
		printf("%s %d: malloc %d * %d bytes failed\n", __func__, __LINE__, num, sizeof(struct Vertex));
		return -1;
	}

	for (int i = 0; i < num; i++)
	{
		verts[i].position.x = x[i];
		verts[i].position.y = y[i];
	}

	struct ConcavePolygon poly;

	printf("%s %d: init poly\n", __func__, __LINE__);
	ConcavePolygon(&poly, verts, num);
	convexDecomp(&poly);
	total = getTotalSubPolys(&poly);
//	fprintf(stdout, "%s total: %d\n", __func__, total);
	ox = (float **)app_mem_malloc(total * sizeof(float *));
	oy = (float **)app_mem_malloc(total * sizeof(float *));
	records = (int *)app_mem_malloc(total * sizeof(int));
	if (ox == NULL || oy == NULL || records == NULL)
	{
		printf("%s %d: malloc %d * %d bytes failed\n", __func__, __LINE__, total, sizeof(float *));
		app_mem_free(verts);
		app_mem_free(ox);
		app_mem_free(oy);
		app_mem_free(records);
		return -1;
	}

//	fprintf(stdout, "%s total: %d before\n", __func__, total);
	get_polygons_allVertices(&poly, ox, oy, &index, records, total);
//	fprintf(stdout, "%s total: %d after\n", __func__, total);
	*pox = ox;
	*poy = oy;
	*pnum = total;
	*precords = records;
	//drawPolygon(&poly);
	app_mem_free(verts);
	reset(&poly);
	return 0;
}


