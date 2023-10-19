#ifndef CONCAVE_POLY_H
#define CONCAVE_POLY_H


struct Vec2
{
    double x;
    double y;

};

struct Vertex
{
    struct Vec2 position;
};

struct SliceVertex
{
    int index;
    double distanceToSlice;
    struct Vec2 position;
};

struct LineSegment
{
    struct Vec2 startPos;
    struct Vec2 finalPos;
};

struct ConcavePolygon
{
	struct Vertex *vertices;
	int vertices_num;

	struct ConcavePolygon *subPolygons[2];
	int subPolygons_num;
};

int ConcavePolygon(struct ConcavePolygon *polygon, struct Vertex *_vertices, int _vertices_num);
int checkIfRightHanded(struct ConcavePolygon *polygon);
void slicePolygon(struct ConcavePolygon *polygon, int vertex1, int vertex2);
void slicePolygon2(struct ConcavePolygon *Poly, struct LineSegment *segment);
void convexDecomp(struct ConcavePolygon *Poly);
struct Vertex *getVertices(struct ConcavePolygon *Poly);
struct ConcavePolygon *getSubPolygon(struct ConcavePolygon *Poly, int subPolyIndex);
int getNumberSubPolys(struct ConcavePolygon *Poly);
int getTotalSubPolys(struct ConcavePolygon *Poly);
void returnLowestLevelPolys(struct ConcavePolygon *Poly, struct ConcavePolygon **PolyArray, int *PolyArrayNum);
void reset(struct ConcavePolygon *Poly);
struct Vec2 getPoint(struct ConcavePolygon *Poly, unsigned int index);
int getPointCount(struct ConcavePolygon *Poly);

#endif // CONCAVE_POLY_H
