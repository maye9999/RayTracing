#include "vec.h"
#include "mat.h"
#include "common.h"
#include "phong.h"
#include "light.h"
#include "primitive.h"
#include <iostream>

using namespace std;

int mainb()
{


	Scene scene(1920, 1440, true);
	BRDF *brdf = new PhongBRDF;
	Material * material_blue = new Material(Vec(0, 0, 0.1), Vec(0, 0, 0.5), Vec(0.2, 0.2, 0.2), 50, Vec(0, 0, 0), Vec(0, 0, 0));
	Material * material_red = new Material(Vec(0, 0, 0), Vec(0.1, 0.1, 0.1), Vec(0.1, 0.1, 0.1), 50, Vec(0.8, 0.8, 0.8), Vec(0, 0, 0));
	Material * material_green = new Material(Vec(0, 0.1, 0), Vec(0, 0.5, 0), Vec(0.2, 0.2, 0.2), 50, Vec(0, 0, 0), Vec(0, 0, 0));
	Material * m = new Material(Vec(0.5, 0.5, 0.5), Vec(0, 0, 0), Vec(0.2, 0.2, 0.2), 50);

	Texture * texture = new Texture("floor.bmp");
	TextureMapping *tm = new TextureMapping;
	tm->ut = make_pair(0, 0);
	tm->vt = make_pair(1, 0);
	tm->wt = make_pair(1, 1);
	TextureMapping *tm2 = new TextureMapping;
	tm2->ut = make_pair(0, 0);
	tm2->vt = make_pair(0, 1);
	tm2->wt = make_pair(1, 1);
	// Y-Z plane
	Triangle t3(Point(0, -100, 0), Point(0, 100, 0), Point(0, -100, 100));
	Triangle t4(Point(0, 100, 0), Point(0, 100, 100), Point(0, -100, 100));
	// X-Y plane
	Triangle t5(Point(0, -100, 0), Point(100, -100, 0), Point(100, 100, 0));
	Triangle t6(Point(0, -100, 0), Point(100, 100, 0), Point(0, 100, 0));
	
	t3.setBRDF(brdf);
	t4.setBRDF(brdf);
	t5.setBRDF(brdf);
	t6.setBRDF(brdf);

	t3.setMaterial(material_green);
	t4.setMaterial(material_green);
	t5.setMaterial(m);
	t6.setMaterial(m);

	//t5.setTexture(texture);
	//t5.setTextureMapping(*tm);
	//t6.setTexture(texture);
	//t6.setTextureMapping(*tm2);

	Sphere s1(Point(10.5, 10.5, 11), 1);
	Sphere s2(Point(9.5, 9.5, 0.5), 0.5);
	Material *material_s1 = new Material(Vec(0.1, 0.1, 0.1), Vec(0, 0, 0), Vec(0.1, 0.1, 0.1), 50, Vec(0, 0, 0), Vec(0, 0, 0), 1.5);
	Material *material_s2 = new Material(Vec(0.1, 0.1, 0.1), Vec(0, 0, 0), Vec(0.1, 0.1, 0.1), 50, Vec(0, 0, 0), Vec(0.9, 0.9, 0.9), 1.5);

	s1.setBRDF(brdf);
	s2.setBRDF(brdf);
	s1.setMaterial(material_s1);
	s2.setMaterial(material_s2);

	PointLight light(Point(10, 10, 15), Color(1, 1, 1));
	PointLight light2(Point(8.5, 12.5, 0.5), Color(0.5, 0.5, 0.5));

	//scene.objects.insert(scene.objects.end(), &t3);
	//scene.objects.insert(scene.objects.end(), &t4);
	scene.objects.insert(scene.objects.end(), &t5);
	scene.objects.insert(scene.objects.end(), &t6);
	scene.objects.insert(scene.objects.end(), &s1);
	//scene.objects.insert(scene.objects.end(), &s2);

	scene.light_objects.insert(scene.light_objects.end(), &light);
	scene.light_objects.insert(scene.light_objects.end(), &light2);
	scene.camera.setCamera(Point(10, 10, 20), Point(10, 10, 0), Vec(0, 1, 0), 30);

	scene.render(1);
	return 0;
}