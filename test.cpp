#include "vec.h"
#include "mat.h"
#include "common.h"
#include "phong.h"
#include "light.h"
#include "primitive.h"
#include <iostream>

using namespace std;

int main()
{
	Scene scene(640, 480);
	BRDF *brdf = new PhongBRDF;
	Material * material = new Material(Vec(0, 0, 0), Vec(0, 0, 0), Vec(0.6, 0.6, 0.5), 20, Vec(0, 0, 0), Vec(0, 0, 0));
	//Triangle t1(Point(-1, -1, 0), Point(1, -1, 0), Point(1, 1, 0));
	//Triangle t2(Point(-1, -1, 0), Point(1, 1, 0), Point(-1, 1, 0));
	//t2.setBRDF(brdf);
	Sphere t1(Point(1, 1, 0), 2);
	t1.setBRDF(brdf);
	t1.setMaterial(material);
	PointLight light(Point(0, 0, 4), Color(0.5, 0.5, 0.5));
	PointLight light2(Point(-1, -1, 1), Color(0.5, 0.5, 0.5));

	scene.objects.insert(scene.objects.end(), &t1);
	scene.light_objects.insert(scene.light_objects.end(), &light);
	scene.light_objects.insert(scene.light_objects.end(), &light2);
	scene.camera.setCamera(Point(0, 0, 5), Point(0, 0, 0), Vec(0, 1, 0), 90);
	scene.render();
	return 0;
}