#include "common.h"
#include "light.h"
#include "objfile.h"
#include "phong.h"
#include "primitive.h"
#include <iostream>
#include <thread>

using namespace std;

int main()
{
	Scene scene(1920, 1440, true);

	PhongBRDF brdf;
	Material material(Vec(0.1, 0.1, 0.1), Vec(0.3, 0.8, 0.3), Vec(0.2, 0.2, 0.2), 20);

	ObjFile* file = new ObjFile("dinosaur.2k.obj");
	if(!scene.loadFile(file))
	{
		return -1;
	}
	
	scene.setGlobalBRDF(&brdf);
	scene.setGlobalMaterial(&material);

	PointLight light(Point(-300, -300, 300), Color(0.9, 0.9, 0.9));
	PointLight light2(Point(50, 20, 20), Color(0.9, 0.9, 0.9));
	PointLight light3(Point(20, 50, 20), Color(0.9, 0.9, 0.9));
	scene.light_objects.push_back(&light);
	//scene.light_objects.push_back(&light2);
	//scene.light_objects.push_back(&light3);
	
	scene.camera.setCamera(Point(-150, -50, 10), Point(0, 0, 10), Vec(0, 0, 1), 45);
	clock_t t1, t2;
	scene.render(1, true);
	return 0;
}