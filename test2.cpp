#include "common.h"
#include "light.h"
#include "objfile.h"
#include "phong.h"
#include "primitive.h"
#include <iostream>

using namespace std;

int main()
{
	Scene scene(800, 600, false);

	PhongBRDF brdf;
	Material material(Vec(1, 1, 1), Vec(0.2, 0.2, 0.2), Vec(0.2, 0.2, 0.2), 20);

	ObjFile* file = new ObjFile("box.obj");
	if(!scene.loadFile(file))
	{
		return -1;
	}
	scene.setGlobalBRDF(&brdf);
	scene.setGlobalMaterial(&material);

	PointLight light(Point(50, 50, 20), Color(0.9, 0.9, 0.9));
	PointLight light2(Point(50, 20, 20), Color(0.9, 0.9, 0.9));
	PointLight light3(Point(20, 50, 20), Color(0.9, 0.9, 0.9));
	scene.light_objects.push_back(&light);
	scene.light_objects.push_back(&light2);
	scene.light_objects.push_back(&light3);
	
	scene.camera.setCamera(Point(-5, -5, 5), Point(0, 0, 0), Vec(0, 0, 1), 45);
	scene.render();
	return 0;
}