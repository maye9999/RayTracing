#include "common.h"
#include "light.h"
#include "objfile.h"
#include "phong.h"
#include "primitive.h"
#include <iostream>

using namespace std;

int main()
{
	Scene scene(1920, 1440, true);

	PhongBRDF brdf;
	Material material(Vec(0.1, 0.1, 0.1), Vec(0.4, 0.4, 0.4), Vec(0.2, 0.2, 0.2), 20);

	Triangle t1(Point(0, 0, 0), Point(0, 1, 0), Point(1, 1, 0));
	Triangle t2(Point(0, 0, 0), Point(1, 0, 0), Point(1, 1, 0));
	
	Texture texture("floor.bmp");
	TextureMapping tm;
	tm.ut = make_pair<double, double>(0, 1);
	tm.vt = make_pair<double, double>(0, 0);
	tm.wt = make_pair<double, double>(1, 0);
	TextureMapping tm2;
	tm2.ut = make_pair<double, double>(0, 1);
	tm2.vt = make_pair<double, double>(1, 1);
	tm2.wt = make_pair<double, double>(1, 0);
	t1.setTexture(&texture);
	t1.setTextureMapping(tm);
	t2.setTexture(&texture);
	t2.setTextureMapping(tm2);
	scene.objects.push_back(&t1);
	scene.objects.push_back(&t2);
	scene.setGlobalBRDF(&brdf);
	scene.setGlobalMaterial(&material);	

	PointLight light(Point(1, 1, 6), Color(0.9, 0.9, 0.9));
	scene.light_objects.insert(scene.light_objects.end(), &light);
	scene.camera.setCamera(Point(-1, -1, 3), Point(0.5, 0.5, 0), Vec(0, 1, 0), 30);
	scene.render();
	return 0;
}