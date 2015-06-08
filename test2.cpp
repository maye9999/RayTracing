#include "common.h"
#include "light.h"
#include "objfile.h"
#include "phong.h"
#include "primitive.h"
#include <iostream>

using namespace std;

int mainn()
{
	Scene scene(256, 192);
	PhongBRDF brdf;
	Material material(Vec(0, 0, 0.1), Vec(0, 0, 0.8), Vec(0.2, 0.2, 0.2), 50);
	ObjFile file("dinosaur.2k.obj");
	scene.loadFile(&file);
	scene.setGlobalBRDF(&brdf);
	scene.setGlobalMaterial(&material);

	PointLight light(Point(30, 30, 30), Color(0.9, 0.9, 0.9));
	scene.light_objects.insert(scene.light_objects.end(), &light);
	scene.camera.setCamera(Point(30, 30, 30), Point(0, 0, 0), Vec(0, 0, 1), 90);
	scene.render();
	return 0;
}