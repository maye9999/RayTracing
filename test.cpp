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
	BRDF *brdf = new PhongBRDF(Vec(0.1, 0.1, 0.1), Vec(0, 0, 1), Vec(0, 0, 0), 1);
	Triangle t1(Point(-1, -1, 0), Point(1, -1, 0), Point(1, 1, 0));
	Triangle t2(Point(-1, -1, 0), Point(1, 1, 0), Point(-1, 1, 0));
	t2.setBRDF(brdf);
	t1.setBRDF(brdf);
	PointLight light(Point(4, 0, 4), Color(0.5, 0.5, 0.5));

	scene.objects.insert(scene.objects.end(), &t1);
	scene.objects.insert(scene.objects.end(), &t2);
	scene.light_objects.insert(scene.light_objects.end(), &light);
	scene.camera.setCamera(Point(0, 0, 4), Point(0, 0, 0), Vec(0, 1, 0), 30);
	scene.render();
	return 0;
}