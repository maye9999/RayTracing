#include "vec.h"
#include "mat.h"
#include "common.h"
#include "phong.h"
#include "light.h"
#include "primitive.h"
#include <iostream>

using namespace std;

int mainnnn()
{
	Scene scene(1920, 1440, true);
	//Scene scene(640, 480, false);
	BRDF *brdf = new PhongBRDF;

	Triangle t1(Point(100, 100, 0), Point(-100, -100, 0), Point(0, 100, 0));
	Triangle t2(Point(100, 100, 0), Point(-100, -100, 0), Point(100, 0, 0));
	Sphere s1(Point(0, 0, 8), 8);
	Sphere s2(Point(9, -6, 2), 2);
	Sphere s3(Point(11, -1, 2), 2);
	Sphere s4(Point(1, -14, 4), 4);
	Sphere s5(Point(15, -15, 3), 3);

	Material* m1 = new Material(Color(0.05, 0.05, 0.05), Color(0.1, 0.1, 0.1), Color(0.1, 0.1, 0.1), 20, Color(0.5, 0.5, 0.5));
	Material* m2 = new Material(Color(0.1, 0.1, 0.2), Color(0.1, 0.1, 0.1), Color(0.2, 0.2, 0.2), 20, Color(0.2, 0.2, 0.2));
	Material* m3 = new Material(Color(0.1, 0.1, 0.1), Color(0.5, 0.0, 0.0), Color(0.2, 0.2, 0.2), 20);
	Material* m4 = new Material(Color(0.1, 0.1, 0.1), Color(0, 0.4, 0.4), Color(0.2, 0.2, 0.2), 20, Color(0.2, 0.2, 0.2));
	Material* m5 = new Material(Color(0.1, 0.1, 0.1), Color(0.0, 0.0, 0.0), Color(0.1, 0.1, 0.1), 20, Color(0, 0, 0), Color(0.9, 0.9, 0.9), 1.5);

	t1.setMaterial(m1);
	t2.setMaterial(m1);
	s1.setMaterial(m2);
	s2.setMaterial(m3);
	s3.setMaterial(m3);
	s4.setMaterial(m4);
	s5.setMaterial(m5);
	
	PointLight light(Point(0, 0, 50), Color(0.5, 0.5, 0.5));
	PointLight light2(Point(15, -10, 9), Color(0.2, 0.2, 0.2));

	scene.objects.insert(scene.objects.end(), &t1);
	scene.objects.insert(scene.objects.end(), &t2);
	scene.objects.insert(scene.objects.end(), &s1);
	scene.objects.insert(scene.objects.end(), &s2);
	scene.objects.push_back(&s3);
	scene.objects.push_back(&s4);
	scene.objects.push_back(&s5);
	scene.light_objects.insert(scene.light_objects.end(), &light);
	scene.light_objects.insert(scene.light_objects.end(), &light2);
	scene.camera.setCamera(Point(30, -30, 4), Point(0, 0, 4), Vec(0, 0, 1), 40);
	scene.setGlobalBRDF(brdf);
	scene.render();
	return 0;
}