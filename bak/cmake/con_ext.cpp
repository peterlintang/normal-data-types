
#include "ConcavePolygon.h"

using namespace cxd;

extern "C" {
//	int convert_concave_polygon(float *x, float *y, int num);
	int convert_concave_polygon(float *x, float *y, int num, float ***pox, float ***poy, int *pnum, int **precords);
};

static int count = 0;
void drawPolygon(ConcavePolygon const & poly)
{
    if(poly.getNumberSubPolys() > 0)
    {
        drawPolygon(poly.getSubPolygon(0));
        drawPolygon(poly.getSubPolygon(1));
    }
    else
    {
    std::vector<Vertex > const & verts = poly.getVertices();



    fprintf(stdout, "%s: count: %d\n", __func__, count++);
    for(unsigned int i=0; i<verts.size(); ++i)
    {
        fprintf(stdout, "%f, %f\n", verts[i].position.x, verts[i].position.y);

    }
    }
}

void get_polygons_allVertices(ConcavePolygon const & poly, float **ox, float **oy, int *num, int *records, int max)
{
	float *x = NULL;
	float *y = NULL;

    if(poly.getNumberSubPolys() > 0)
    {
        get_polygons_allVertices(poly.getSubPolygon(0), ox, oy, num, records, max);
        get_polygons_allVertices(poly.getSubPolygon(1), ox, oy, num, records, max);
    }
    else
    {
    std::vector<Vertex > const & verts = poly.getVertices();


    x = (float *)calloc(verts.size(), sizeof(float));
    y = (float *)calloc(verts.size(), sizeof(float));

    	for(unsigned int i=0; i<verts.size(); ++i)
    	{
        	x[i] = verts[i].position.x; 
		y[i] = verts[i].position.y;
    	}

	ox[*num] = x;
	oy[*num] = y;
	records[*num] = verts.size();
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
	Vec2 position;
	std::vector<Vertex > verts(num);

	for (int i = 0; i < num; i++)
	{
		position.x = x[i];
		position.y = y[i];
		verts[i] = Vertex(position);
	}

	ConcavePolygon poly(verts);
	poly.convexDecomp();
	total = poly.getTotalSubPolys();
//	fprintf(stdout, "%s total: %d\n", __func__, total);
	ox = (float **)calloc(total, sizeof(float *));
	oy = (float **)calloc(total, sizeof(float *));
	records = (int *)calloc(total, sizeof(int));
//	fprintf(stdout, "%s total: %d before\n", __func__, total);
	get_polygons_allVertices(poly, ox, oy, &index, records, total);
//	fprintf(stdout, "%s total: %d after\n", __func__, total);
	*pox = ox;
	*poy = oy;
	*pnum = total;
	*precords = records;
//	drawPolygon(poly);
	return 0;
}


