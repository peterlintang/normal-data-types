
#include "ConcavePolygon.h"

using namespace cxd;

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


void drawVertices(std::vector<Vertex > const & vertices)
{
    for(unsigned int i=0; i<vertices.size(); ++i)
    {
        fprintf(stdout, "%s: %f, %f\n", __func__, vertices[i].position.x, vertices[i].position.y);
    }
}


void scaleVerts(std::vector<Vertex > & verts, float amount)
{
    for(auto& vert : verts)
    {
        vert.position = vert.position * amount;
    }
}

int main()
{
    std::vector<Vertex > verts = {
        /*{{-2.15f, -3.1f}},
        {{1.225f, -3.3f}},
        {{2.4f, -1.45f}},
        {{2.3f, 0.35f}},
        {{1.725f, 2.1f}},
        {{0.85f, -0.475f}},
        {{-1.7f, 2.375f}},
        {{-4.625f, -0.35f}}*/
        {{0.15f, -3.65f}},
{{2.475f, -3.275f}},
{{1.15f, -3.175f}},
{{1.0f, -2.175f}},
{{2.325f, -1.775f}},
{{4.725f, -1.725f}},
{{6.225f, -1.175f}},
{{6.95f, -0.05f}},
{{6.625f, 1.775f}},
{{4.775f, 4.15f}},
{{3.4f, 4.275f}},
{{2.425f, 3.175f}},
{{2.575f, 1.925f}},
{{1.825f, 0.0f}},
{{1.05f, -0.15f}},
{{-2.225f, 1.175f}},
{{-3.4f, 0.3f}},
{{-3.95f, -1.25f}},
{{-3.35f, -2.5f}},
{{-1.05f, -3.625f}}
    };

    std::vector<Vertex > mouseVerts;


    //scaleVerts(verts, 1000.0f);
    ConcavePolygon poly(verts);
    //poly.convexDecomp();

    std::cout << poly.checkIfRightHanded() << "\n";

    //poly.slicePolygon(1, 3);
    LineSegment segment;
    segment.finalPos = {0.0f, 1.0f};
    segment.startPos = {1.0f, -2.0f};
    //poly.convexDecomp();

	poly.convexDecomp();

        drawPolygon(poly);

    	fprintf(stdout, "\n\n%s total: %d\n", __func__, poly.getTotalSubPolys()); 

	return 0;
}
