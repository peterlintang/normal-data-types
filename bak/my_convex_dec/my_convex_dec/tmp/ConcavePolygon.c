
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "ConcavePolygon.h"

int findFirstReflexVertex(struct Vertex *_vertices, int vertices_num);
int checkIfRightHanded(struct ConcavePolygon *polygon);
void slicePolygon2(struct ConcavePolygon *Poly, struct LineSegment *segment);
int verticesAlongLineSegment(struct LineSegment *segment,
							struct Vertex *_vertices, int _vertices_num,
							struct Vertex *out_vertices, int *out_vertices_map, 
							int *out_vertices_num);

float Vec2_length(struct Vec2 *v)
{
	return sqrtf(v->x * v->x + v->y * v->y);
}

struct Vec2 Vec2_norm(struct Vec2 *v)
{
	struct Vec2 nv;

	nv.x = v->x;
	nv.y = v->y;
	if(Vec2_length(v) < 1e-30)
	{
		nv.x = 0.0f;
		nv.y = 0.0f;
	}
	return nv;
}

float Vec2_dot(struct Vec2 *v1, struct Vec2 *v2)
{
	return v1->x * v2->x + v1->y * v2->y;
}

float Vec2_square(struct Vec2 *v)
{
	return Vec2_dot(v,v);
}

float Vec2_cross(struct Vec2 *v1, struct Vec2 *v2)
{
	return v1->x * v2->y - v1->y * v2->x;
}

struct Vec2 Vec2_sub(struct Vec2 *v, struct Vec2 *v1) 
{
	struct Vec2 nv;
	nv.x = v->x - v1->x;
	nv.y = v->y - v1->y;
	return nv;
}

struct Vec2 Vec2_add(struct Vec2 *v, struct Vec2 *v1) 
{
	struct Vec2 nv;
	nv.x = v->x + v1->x; 
	nv.y = v->y + v1->y;

	return nv;
}

struct Vec2 Vec2_multiply(struct Vec2 *v, float f) 
{
	struct Vec2 nv;

	nv.x = f * v->x;
	nv.y = f * v->y;
	return nv;
}

struct Vec2 Vec2_div(struct Vec2 *v, float f)
{
	struct Vec2 nv;
	nv.x = v->x / f;
	nv.y = v->y / f;
	return nv;
}

float Vec2_getSignedArea(struct Vec2 *v1,
						struct Vec2 *v2)
{
	return (v2->x - v1->x) * (v2->y + v1->y);
}


int Vertex_set(struct Vertex *VXposition, struct Vec2 *_position)
{
	VXposition->position.x = _position->x;
	VXposition->position.y = _position->y;
	return 0;
}

float Vertex_getHandedness(struct Vertex *v1,
							struct Vertex *v2,
							struct Vertex *v3)
{
	struct Vec2 edge1;
	struct Vec2 edge2;

	edge1.x = v2->position.x - v1->position.x;
	edge1.y = v2->position.y - v1->position.y;
	edge2.x = v3->position.x - v2->position.x;
	edge2.y = v3->position.y - v2->position.y;

	return Vec2_cross(&edge1, &edge2);
}


int SliceVertex_set(struct SliceVertex *sliceVer, struct Vec2 *_position)
{
		sliceVer->position = *_position;
		return 0;
}



int LineSegment_set(struct LineSegment *lineSeg,
				struct Vec2 _startPos,
                struct Vec2 _finalPos)
{
	lineSeg->startPos = _startPos;
	lineSeg->finalPos = _finalPos;

	return 0;
}

struct Vec2 LineSegment_direction(struct LineSegment *lineSeg) 
{
	struct Vec2 dir;

	dir.x = lineSeg->finalPos.x - lineSeg->startPos.x;
	dir.y = lineSeg->finalPos.y - lineSeg->startPos.y;

	return dir;
}

struct Vec2 LineSegment_normalisedDirection(struct LineSegment *lineSeg)
{
	struct Vec2 dir;

	dir = LineSegment_direction(lineSeg);

	return Vec2_norm(&dir);
}

int LineSegment_add(struct LineSegment *ls, struct LineSegment *s2, struct LineSegment *out)
{
	struct Vec2 tmp = Vec2_add(&(s2->startPos),  &(ls->startPos));
	struct Vec2 tmp2 = Vec2_add(&(s2->finalPos), &(ls->finalPos));
	struct Vec2 newStartPos = Vec2_div(&tmp, 2.0f);
	struct Vec2 newFinalPos = Vec2_div(&tmp2, 2.0f);

	out->startPos = newStartPos;
	out->finalPos = newFinalPos;
	return 0;
}

int intersects(struct LineSegment *s1, struct LineSegment *s2, int *status, struct Vec2 *v)
{
	const float TOLERANCE = 1e-2;

	struct Vec2 p1 = s1->startPos;
	struct Vec2 p2 = s2->startPos;
	struct Vec2 d1 = LineSegment_direction(s1);
	struct Vec2 d2 = LineSegment_direction(s2);

	if(fabs(Vec2_cross(&d1, &d2)) < 1e-30)
	{
		*status = 0;
		v->x = 0.0f;
		v->y = 0.0f;
		return 0;
	}

	struct Vec2 tmp5 = Vec2_sub(&p2, &p1); 
	float t1 = Vec2_cross(&tmp5, &d2) / Vec2_cross(&d1, &d2);

	if((t1 < (0.0f - TOLERANCE)) || (t1 > (1.0f + TOLERANCE)))
	{
		*status = 0;
		v->x = 0.0f;
		v->y = 0.0f;
		return 0;
	}

	struct Vec2 tmp = Vec2_multiply(&d1, t1);
	struct Vec2 pIntersect = Vec2_add(&p1, &tmp);

	struct Vec2 tmp2 = Vec2_sub(&pIntersect, &p2);
	struct Vec2 tmp3 = Vec2_sub(&(s2->finalPos), &p2);
	float t2 = Vec2_dot(&tmp2, &tmp3);

	struct Vec2 tmp4 = Vec2_sub(&(s2->finalPos), &p2);
	if(t2 < (0.0f-TOLERANCE) || t2 / Vec2_square(&tmp4) >= 1.0f - TOLERANCE)
	{
		*status = 0;
		v->x = 0.0f;
		v->y = 0.0f;
		return 0;
	}

	*status = 1;
	v->x = pIntersect.x;
	v->y = pIntersect.y;
	return 0;
}


int mod(int x, int m)
{
	int r = x % m;
	return r < 0 ? r + m : r;
}

void _flipPolygon(struct Vertex *_verts, int num)
{
        int iMax = num / 2;

        if(num % 2 != 0)
            iMax += 1;

        for(int i=1; i<iMax; ++i)
        {
			struct Vertex v;

			v.position.x = _verts[i].position.x;
			v.position.y = _verts[i].position.y;

			_verts[i].position.x = _verts[num - i].position.x;
			_verts[i].position.y = _verts[num - i].position.y;

			_verts[num - i].position.x = v.position.x;
			_verts[num - i].position.y = v.position.y;
        }
}

int _checkIfRightHanded(struct Vertex *_verts, int num)
{
	if(num < 3)
		return 0;

	float signedArea = 0.0f;

	for(unsigned int i=0; i < num; ++i)
	{
		signedArea += Vec2_getSignedArea(&(_verts[i].position),
							&(_verts[mod(i + 1, num)].position));
	}

	if(signedArea < 0.0f)
		return 1;

	return 0;
}



int _isVertexInCone(struct LineSegment *ls1,
                        struct LineSegment *ls2,
                        struct Vec2 *origin,
                        struct Vertex *vert)
{
        struct Vec2 relativePos = Vec2_sub(&(vert->position), origin);

		struct Vec2 tmp1 = LineSegment_direction(ls1);
		struct Vec2 tmp2 = LineSegment_direction(ls2);
        float ls1Product = Vec2_cross(&relativePos, &tmp1);
        float ls2Product = Vec2_cross(&relativePos, &tmp2);

		printf("%s (%f %f), (%f %f)\n", __func__, vert->position.x, vert->position.y, origin->x, origin->y);
		printf("%s (%f %f), (%f %f), (%f %f)\n", 
						__func__, tmp1.x, tmp1.y, tmp2.x, tmp2.y,
						relativePos.x, relativePos.y);
		printf("%s: %f %f\n", __func__, ls1Product, ls2Product);

        if(ls1Product < 0.0f && ls2Product > 0.0f)
            return 1;

        return 0;
}


int _findVerticesInCone(struct LineSegment *ls1,
                                   struct LineSegment *ls2,
                                   struct Vec2 *origin,
                                   struct Vertex *inputVerts,
								   int inputVerts_num,
								   int *out,
								   int *out_num)
{
	int count = 0;
	printf("%s: %d\n", __func__, inputVerts_num);

	for(int i=0; i < inputVerts_num; ++i)
	{
		if(_isVertexInCone(ls1, ls2, origin, &(inputVerts[i])))
		{
			out[count] = i;
			count++;
		}
	}
	*out_num = count;
	printf("%s: %d\n", __func__, count);

	return 0;
}

int _checkVisibility(struct Vec2 *originalPosition,
                         struct Vertex *vert,
                         struct Vertex *polygonVertices,
						 int polygonVertices_num)
{
	struct LineSegment ls;
	ls.startPos.x = originalPosition->x; 
	ls.startPos.y = originalPosition->y; 
	ls.finalPos.x = vert->position.x; 
	ls.finalPos.y = vert->position.y; 

	struct Vertex outVertexs[polygonVertices_num];
	int outVertexs_map[polygonVertices_num];
	int outVertexs_num = 0;

	verticesAlongLineSegment(&ls,
							polygonVertices, polygonVertices_num,
							outVertexs, outVertexs_map, &outVertexs_num);


	if(outVertexs_num > 3)
		return 0;

	return 1;
}


int _getBestVertexToConnect(int *indices, int indices_num,
                               struct Vertex *polygonVertices, int polygonVertices_num,
                               struct Vec2 *origin)
{
        if(indices_num ==1)
        {
            if(_checkVisibility(origin, &(polygonVertices[indices[0]]), polygonVertices, polygonVertices_num))
                return indices[0];
        }
        else if(indices_num > 1)
        {
            for(unsigned int i=0; i<indices_num; ++i)
            {
                int index = indices[i];
                int vertSize = polygonVertices_num;

                struct Vertex prevVert = polygonVertices[mod(index-1, vertSize)];
                struct Vertex currVert = polygonVertices[index];
                struct Vertex nextVert = polygonVertices[mod(index+1, vertSize)];

                struct LineSegment ls1 = {prevVert.position, currVert.position};
                struct LineSegment ls2 = {nextVert.position, currVert.position};

				struct Vertex originX;
				originX.position = *origin;
                if((Vertex_getHandedness(&prevVert, &currVert, &nextVert) < 0.0f) &&
                   _isVertexInCone(&ls1, &ls2, &(polygonVertices[index].position), (&originX)) &&
					_checkVisibility(origin, &(polygonVertices[index]), polygonVertices, polygonVertices_num))
                    return index;
            }

            for(unsigned int i=0; i<indices_num; ++i)
            {
                int index = indices[i];
                int vertSize = polygonVertices_num;

                struct Vertex prevVert = polygonVertices[mod(index-1, vertSize)];
                struct Vertex currVert = polygonVertices[index];
                struct Vertex nextVert = polygonVertices[mod(index+1, vertSize)];

                struct LineSegment ls1 = {prevVert.position, currVert.position};
                struct LineSegment ls2 = {nextVert.position, currVert.position};

                if((Vertex_getHandedness(&prevVert, &currVert, &nextVert) < 0.0f) &&
                  _checkVisibility(origin, &(polygonVertices[index]), polygonVertices, polygonVertices_num))
                    return index;
            }


            float minDistance = 1e+15;
            int closest = indices[0];
            for(unsigned int i=0; i < indices_num; ++i)
            {
                int index = indices[i];
				struct Vec2 v = Vec2_sub(&(polygonVertices[index].position), (origin));
                float currDistance = Vec2_square(&v);
                if(currDistance < minDistance)
                {
                    minDistance = currDistance;
                    closest = index;
                }
            }

            return closest;
        }


        return -1;
}



void _convexDecomp(struct Vertex *_vertices, int vertices_num, struct ConcavePolygon *polygon)
{
        if(polygon->subPolygons_num > 0)
        {
            return;
        }

        int reflexIndex = findFirstReflexVertex(_vertices, vertices_num);
        if(reflexIndex == -1)
            return;

        struct Vec2 prevVertPos = _vertices[mod(reflexIndex-1, vertices_num)].position;
        struct Vec2 currVertPos = _vertices[reflexIndex].position;
        struct Vec2 nextVertPos = _vertices[mod(reflexIndex+1, vertices_num)].position;

        struct LineSegment ls1 = {prevVertPos, currVertPos};
        struct LineSegment ls2 = {nextVertPos, currVertPos};

		int vertsInCone[vertices_num];
		int vertsInCone_num = 0;

        _findVerticesInCone(&ls1, &ls2, &currVertPos, _vertices, vertices_num, vertsInCone, &vertsInCone_num);

        int bestVert = -1;

	printf("%s: bestVert: %d, size: %d\n", __func__, bestVert, (int)vertsInCone_num);
        if(vertsInCone_num > 0)
        {
            bestVert = _getBestVertexToConnect(vertsInCone, vertsInCone_num, _vertices, vertices_num, &currVertPos);
	printf("%s: bestVert2: %d, size: %d\n", __func__, bestVert, (int)vertsInCone_num);
            if(bestVert != -1)
            {
                struct LineSegment newLine = {currVertPos, _vertices[bestVert].position};
		 printf("%s %d: (%f %f), (%f %f)\n",
				__func__, __LINE__,
				currVertPos.x, currVertPos.y,
				_vertices[bestVert].position.x,
				_vertices[bestVert].position.y);


                slicePolygon2(polygon, &newLine);
            }
        }
        if(vertsInCone_num == 0 || bestVert == -1)
        {
			struct Vec2 tmp = LineSegment_direction(&ls1);
			struct Vec2 tmp2 = LineSegment_direction(&ls2);
			struct Vec2 tmp3 = Vec2_add(&tmp, &tmp2); 
			struct Vec2 tmp4 = Vec2_multiply(&tmp3, 1e+10);

            struct LineSegment newLine = {currVertPos, tmp4};
		 printf("%s %d: (%f %f), (%f %f)\n",
				__func__, __LINE__,
				currVertPos.x, currVertPos.y,
				tmp4.x,
				tmp4.y);
            slicePolygon2(polygon, &newLine);
        }

        for(unsigned int i = 0; i < polygon->subPolygons_num; ++i)
        {
			_convexDecomp(polygon->subPolygons[i]->vertices, polygon->subPolygons[i]->vertices_num, (polygon->subPolygons[i]));
        }
}

int findFirstReflexVertex(struct Vertex *_vertices, int vertices_num)
{
	for(unsigned int i = 0; i < vertices_num; ++i)
	{
		float handedness = Vertex_getHandedness(&(_vertices[mod(i-1, vertices_num)]),
                                                     &(_vertices[i]),
                                                     &(_vertices[mod(i + 1, vertices_num)]));
		if(handedness < 0.0f)
			return i;
	}

	return -1;
}

void flipPolygon(struct ConcavePolygon *polygon)
{
	_flipPolygon(polygon->vertices, polygon->vertices_num);
}


int cullByDistance(struct Vertex *input, int *input_map, int input_num,
					struct Vec2 *origin, int maxVertsToKeep,
					struct Vertex *output, int *output_map, int *output_num)
{
	if(maxVertsToKeep >= input_num)
	{
		for (int i = 0; i < input_num; i++)
		{
			output[i] = input[i];
		}
		*output_num = input_num;
		return 0;
	}

	struct SliceVertex sliceVertices[input_num] ;
	int sliceVertices_num = input_num;
	for (int i = 0; i < sliceVertices_num ; i++)
	{
		struct Vec2 tmp = { 0.0f, 0.0f };
		tmp.x = input[i].position.x = origin->x;
		tmp.y = input[i].position.y = origin->y;
		sliceVertices[i].position = input[i].position;
		sliceVertices[i].index = input_map[i];
		sliceVertices[i].distanceToSlice = Vec2_square(&tmp);
	}


	for(unsigned int i = 1; i < sliceVertices_num ; ++i)
	{
		for(unsigned int j = i; j > 0 && sliceVertices[j].distanceToSlice < sliceVertices[j-1].distanceToSlice; --j)
		{
			struct SliceVertex tmp;

			tmp.position = sliceVertices[j].position;
			tmp.index = sliceVertices[j].index;
			tmp.distanceToSlice = sliceVertices[j].distanceToSlice;

			sliceVertices[j].position = sliceVertices[j - 1].position;
			sliceVertices[j].index = sliceVertices[j - 1].index;
			sliceVertices[j].distanceToSlice = sliceVertices[j - 1].distanceToSlice;

			sliceVertices[j - 1].position = tmp.position;
			sliceVertices[j - 1].index = tmp.index;
			sliceVertices[j - 1].distanceToSlice = tmp.distanceToSlice;
		}
	}

	sliceVertices_num = maxVertsToKeep;

	for(unsigned int i = 1; i < sliceVertices_num; ++i)
	{
		for(unsigned int j = i; j > 0 && sliceVertices[j].index < sliceVertices[j-1].index; --j)
		{
			struct SliceVertex tmp;

			tmp.position = sliceVertices[j].position;
			tmp.index = sliceVertices[j].index;
			tmp.distanceToSlice = sliceVertices[j].distanceToSlice;

			sliceVertices[j].position = sliceVertices[j - 1].position;
			sliceVertices[j].index = sliceVertices[j - 1].index;
			sliceVertices[j].distanceToSlice = sliceVertices[j - 1].distanceToSlice;

			sliceVertices[j - 1].position = tmp.position;
			sliceVertices[j - 1].index = tmp.index;
			sliceVertices[j - 1].distanceToSlice = tmp.distanceToSlice;
		}
	}

	for(unsigned int i=0; i < sliceVertices_num; ++i)
	{
		output[i].position = sliceVertices[i].position;
		output_map[i] = sliceVertices[i].index;
	}

	*output_num = sliceVertices_num;

	return 0;
}

int verticesAlongLineSegment(struct LineSegment *segment,
							struct Vertex *_vertices, int _vertices_num,
							struct Vertex *out_vertices, int *out_vertices_map, 
							int *out_vertices_num)
{
	struct LineSegment tempSegment;
	int index = 0;

	for(unsigned int i = 0; i < _vertices_num; ++i)
	{
		int status = 0;
		struct Vec2 v = { 0 };

		tempSegment.startPos = _vertices[i].position;
		tempSegment.finalPos = _vertices[mod(i + 1, _vertices_num)].position;

		intersects(segment, &tempSegment, &status, &v);

		if (status == 1)
		{
			out_vertices[index].position = v;
			out_vertices_map[index] = i;
			index++;
		}
	}

	return 0;
}

/*
 * interface below
public:
*/
int ConcavePolygon(struct ConcavePolygon *polygon, struct Vertex *_vertices, int _vertices_num)
{
	polygon->vertices = (struct Vertex *)calloc(_vertices_num, sizeof(struct Vertex));

	for (int i = 0; i < _vertices_num; i++)
	{
		polygon->vertices[i] = _vertices[i];
	}
	polygon->vertices_num = _vertices_num;

	polygon->subPolygons_num = 0;
	polygon->subPolygons[0] = NULL;
	polygon->subPolygons[1] = NULL;

	if(_vertices_num > 2)
		if(checkIfRightHanded(polygon) == 1)
			flipPolygon(polygon);
	return 0;
}

int checkIfRightHanded(struct ConcavePolygon *polygon)
{
        return _checkIfRightHanded(polygon->vertices, polygon->vertices_num);
}

void slicePolygon(struct ConcavePolygon *polygon, int vertex1, int vertex2)
{
	if(vertex1 == vertex2 ||
			vertex2 == vertex1+1 ||
			vertex2 == vertex1-1)
		return;

	if(vertex1 > vertex2)
	{
		int tmp = 0;

		tmp = vertex1;
		vertex1 = vertex2;
		vertex2 = tmp;
	}

	struct Vertex returnVerts[polygon->vertices_num];
	int returnVerts_num = 0;
	struct Vertex newVerts[polygon->vertices_num];
	int newVerts_num = 0;

	for(int i = 0; i < polygon->vertices_num; ++i)
	{
            if(i == vertex1 || i == vertex2)
            {
				returnVerts[returnVerts_num++] = polygon->vertices[i];
				newVerts[newVerts_num++] = polygon->vertices[i];
            }
            else if(i > vertex1 && i <vertex2)
			{
				returnVerts[returnVerts_num++] = polygon->vertices[i];
			}
            else
			{
				newVerts[newVerts_num++] = polygon->vertices[i];
			}
	}

	polygon->subPolygons[0] = (struct ConcavePolygon *)calloc(1, sizeof(struct ConcavePolygon ));;
	polygon->subPolygons[1] = (struct ConcavePolygon *)calloc(1, sizeof(struct ConcavePolygon ));;
	ConcavePolygon(polygon->subPolygons[0] , returnVerts, returnVerts_num);
	ConcavePolygon(polygon->subPolygons[1] , newVerts, newVerts_num);
	polygon->subPolygons_num = 2;

}

static int _slicedVertices_find(struct Vertex *vertexs, int *vertexs_map, int vertexs_num, int index)
{
	for (int i = 0; i < vertexs_num; i++)
	{
		if (vertexs_map[i] == index)
			return i;
	}

	return -1;
}

void slicePolygon2(struct ConcavePolygon *Poly, struct LineSegment *segment)
{
        printf("%s: startPos: (%f, %f), finalPos: (%f, %f), subPlys: %d\n",
                        __func__,
                        segment->startPos.x, segment->startPos.y,
                        segment->finalPos.x, segment->finalPos.y,
                        (int)Poly->subPolygons_num );


	if(Poly->subPolygons_num > 0)
	{
		slicePolygon2((Poly->subPolygons[0]), segment);
		slicePolygon2((Poly->subPolygons[1]), segment);
		return;
	}

	const float TOLERANCE = 1e-5;

	struct Vertex out_vertices[Poly->vertices_num];
	int out_vertices_map[Poly->vertices_num];
	int out_vertices_num = 0;

	verticesAlongLineSegment(segment,
							Poly->vertices, Poly->vertices_num,
							out_vertices, out_vertices_map, 
							&out_vertices_num);


	struct Vertex out_vertices2[out_vertices_num];
	int out_vertices_map2[out_vertices_num];
	int out_vertices_num2 = 0;

	cullByDistance(out_vertices, out_vertices_map, out_vertices_num,
					&(segment->startPos), 2,
					out_vertices2, out_vertices_map2, &out_vertices_num2);

	if(out_vertices_num2 < 2)
		return;

	struct Vertex leftVerts[out_vertices_num2];
	int leftVerts_num = 0;
	struct Vertex rightVerts[out_vertices_num2];
	int rightVerts_num = 0;

	for(int i = 0; i < Poly->vertices_num; ++i)
	{
		struct Vec2 relativePosition;
		relativePosition.x = Poly->vertices[i].position.x - segment->startPos.x;
		relativePosition.y = Poly->vertices[i].position.y - segment->startPos.y;

		struct Vec2 v_dir = LineSegment_direction(segment);
		float perpDistance = fabs(Vec2_cross(&relativePosition, &v_dir));

		int find_result = _slicedVertices_find(out_vertices2, out_vertices_map2, out_vertices_num2, i);
		if( perpDistance > TOLERANCE ||
				( perpDistance <= TOLERANCE && (find_result == -1)))
		{
			if((i > out_vertices_map2[0]) && (i <= out_vertices_map2[1]))
			{
				leftVerts[leftVerts_num++] = Poly->vertices[i];
			}
			else
			{
				rightVerts[rightVerts_num++] = Poly->vertices[i];
			}

		}

		if(find_result != -1)
		{
			rightVerts[rightVerts_num++] = out_vertices2[i];
			leftVerts[leftVerts_num++] = out_vertices2[i];
		}
	}

        printf("%s: left size: %d\n", __func__, (int)leftVerts_num);
        for (int i = 0; i < (int)leftVerts_num; i++)
        {
                printf("i: %d, (%f, %f)\n", i,
                                leftVerts[i].position.x,
                                leftVerts[i].position.y
                                );
        }
        printf("%s: right size: %d\n", __func__, (int)rightVerts_num);
        for (int i = 0; i < (int)rightVerts_num; i++)
        {
                printf("i: %d, (%f, %f)\n", i,
                                rightVerts[i].position.x,
                                rightVerts[i].position.y
                                );
        }


	Poly->subPolygons[0] = (struct ConcavePolygon *)calloc(1, sizeof(struct ConcavePolygon ));;
	Poly->subPolygons[1] = (struct ConcavePolygon *)calloc(1, sizeof(struct ConcavePolygon ));;
	ConcavePolygon(Poly->subPolygons[0], leftVerts, leftVerts_num);
	ConcavePolygon(Poly->subPolygons[1], rightVerts, rightVerts_num);
	Poly->subPolygons_num = 2;
}

void convexDecomp(struct ConcavePolygon *Poly)
{
	if(Poly->vertices_num > 3)
		_convexDecomp(Poly->vertices, Poly->vertices_num, Poly);
}

struct Vertex *getVertices(struct ConcavePolygon *Poly)
{
	return Poly->vertices;
}

struct ConcavePolygon *getSubPolygon(struct ConcavePolygon *Poly, int subPolyIndex)
{
	if(Poly->subPolygons_num > 0 && subPolyIndex < Poly->subPolygons_num)
		return (Poly->subPolygons[subPolyIndex]);

	return Poly;
}

int getNumberSubPolys(struct ConcavePolygon *Poly)
{
	return Poly->subPolygons_num;
}

int getTotalSubPolys(struct ConcavePolygon *Poly)
{
	int total = 0;
	if (Poly->subPolygons_num == 0)
	{
		return 1;
	}

	if (Poly->subPolygons_num > 0)
	{
		total += getTotalSubPolys(Poly->subPolygons[0]);
	}
	if (Poly->subPolygons_num > 1)
	{
		total += getTotalSubPolys(Poly->subPolygons[1]);
	}

	printf("%s: total %d\n", __func__, total);
	return total;
}

void returnLowestLevelPolys(struct ConcavePolygon *Poly, struct ConcavePolygon **PolyArray, int *PolyArrayNum)
{
	if(Poly->subPolygons_num > 0)
	{
            returnLowestLevelPolys(Poly->subPolygons[0], PolyArray, PolyArrayNum);
            returnLowestLevelPolys(Poly->subPolygons[1], PolyArray, PolyArrayNum);
	}
	else
	{
		PolyArray[*PolyArrayNum] = Poly;
		(*PolyArrayNum)++;
	}
}

void reset(struct ConcavePolygon *Poly)
{
	if(Poly->subPolygons_num > 0)
	{
		reset((Poly->subPolygons[0]));
		reset((Poly->subPolygons[1]));

		free(Poly->subPolygons[0]);
		free(Poly->subPolygons[1]);
		Poly->subPolygons_num = 0;

		free(Poly->vertices);
		Poly->vertices_num = 0;
	}
}

struct Vec2 getPoint(struct ConcavePolygon *Poly, unsigned int index)
{
	struct Vec2 v = { 0.0f, 0.0f };
	if(index >= 0 && index < Poly->vertices_num)
		return Poly->vertices[index].position;

	return v;
}

int getPointCount(struct ConcavePolygon *Poly)
{
	return Poly->vertices_num;
}


