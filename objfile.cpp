#include "objfile.h"
#include "primitive.h"
#include <iostream>
using namespace std;


ObjFile::ObjFile(const char* name)
{
	LoadFromObj(name);
}

void ObjFile::parse(std::vector<Primitive*>& objects)
{
	if (!IsLoaded())
		return;
	for (int i = 0; i < m_nTriangles; ++i)
	{
		int indices[3];
		for (int j = 0; j < 3; j++)
		{
			indices[j] = m_pTriangleList[i][j];
			
		}
		Point x[3];
		for (int j = 0; j < 3; j++)
		{
			x[j].x = m_pVertexList[indices[j]].x;
			x[j].y = m_pVertexList[indices[j]].y;
			x[j].z = m_pVertexList[indices[j]].z;
		}
		Triangle *t = new Triangle(x[0], x[1], x[2]);
		objects.insert(objects.end(), t);
	}
}
