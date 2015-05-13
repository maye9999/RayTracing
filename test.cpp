#include "vec.h"
#include "mat.h"
#include "common.h"
#include "phong.h"
#include "light.h"
#include "primitive.h"
#include <iostream>

using namespace std;

int mainn()
{
	Scene scene(1920, 1080);
	BRDF *brdf = new PhongBRDF;
	Material * material_blue = new Material(Vec(0, 0, 0.1), Vec(0, 0, 0.5), Vec(0.2, 0.2, 0.2), 50, Vec(0, 0, 0), Vec(0, 0, 0));
	Material * material_red = new Material(Vec(0, 0, 0), Vec(0.1, 0.1, 0.1), Vec(0.1, 0.1, 0.1), 50, Vec(0.8, 0.8, 0.8), Vec(0, 0, 0));
	Material * material_green = new Material(Vec(0, 0.1, 0), Vec(0, 0.5, 0), Vec(0.2, 0.2, 0.2), 50, Vec(0, 0, 0), Vec(0, 0, 0));
	// X-Z plane
	Triangle t1(Point(0, 0, 0), Point(5, 0, 5), Point(5, 0, 0));
	Triangle t2(Point(0, 0, 0), Point(0, 0, 5), Point(5, 0, 5));
	// Y-Z plane
	Triangle t3(Point(0, 0, 0), Point(0, 5, 0), Point(0, 0, 5));
	Triangle t4(Point(0, 5, 0), Point(0, 5, 5), Point(0, 0, 5));
	// X-Y plane
	Triangle t5(Point(0, 0, 0), Point(5, 0, 0), Point(5, 5, 0));
	Triangle t6(Point(0, 0, 0), Point(5, 5, 0), Point(0, 5, 0));
	// X-Z plane
	Triangle t7(Point(0, 5, 0), Point(5, 5, 5), Point(5, 5, 0));
	Triangle t8(Point(0, 5, 0), Point(0, 5, 5), Point(5, 5, 5));
	
	t1.setBRDF(brdf);
	t2.setBRDF(brdf);
	t3.setBRDF(brdf);
	t4.setBRDF(brdf);
	t5.setBRDF(brdf);
	t6.setBRDF(brdf);
	t8.setBRDF(brdf);
	t7.setBRDF(brdf);

	t1.setMaterial(material_blue);
	t2.setMaterial(material_blue);
	t3.setMaterial(material_green);
	t4.setMaterial(material_green);
	t5.setMaterial(material_red);
	t6.setMaterial(material_red);
	t7.setMaterial(material_blue);
	t8.setMaterial(material_blue);

	Sphere s1(Point(1.5, 1, 1), 1);
	Sphere s2(Point(2.5, 3.5, 1.5), 1.5);
	Material *material_s1 = new Material(Vec(0.05, 0, 0), Vec(0.4, 0, 0), Vec(0.1, 0.1, 0.1), 50, Vec(0, 0, 0), Vec(0, 0, 0), 1.5);
	Material *material_s2 = new Material(Vec(0, 0.05, 0), Vec(0, 0.6, 0), Vec(0.1, 0.1, 0.1), 50, Vec(0, 0, 0), Vec(0, 0, 0), 1.5);

	s1.setBRDF(brdf);
	s2.setBRDF(brdf);
	s1.setMaterial(material_s1);
	s2.setMaterial(material_s2);

	PointLight light(Point(2, 2, 5), Color(0.5, 0.5, 0.5));
	PointLight light2(Point(8.5, 2.5, 1), Color(0.5, 0.5, 0.5));

	//scene.objects.insert(scene.objects.end(), &t1);
	//scene.objects.insert(scene.objects.end(), &t2);
	scene.objects.insert(scene.objects.end(), &t3);
	scene.objects.insert(scene.objects.end(), &t4);
	scene.objects.insert(scene.objects.end(), &t5);
	scene.objects.insert(scene.objects.end(), &t6);
	//scene.objects.insert(scene.objects.end(), &t8);
	//scene.objects.insert(scene.objects.end(), &t7);
	scene.objects.insert(scene.objects.end(), &s1);
	scene.objects.insert(scene.objects.end(), &s2);

	scene.light_objects.insert(scene.light_objects.end(), &light);
	scene.light_objects.insert(scene.light_objects.end(), &light2);
	scene.camera.setCamera(Point(7, 2.5, 2.5), Point(0, 2, 2.0), Vec(0, 0, 1), 90);



// 	Triangle t1(Point(0, 0, 0), Point(1, 0, 0), Point(1, 1, 0));
// 	Triangle t2(Point(0, 0, 0), Point(1, 1, 0), Point(0, 1, 0));
// 	t1.setBRDF(new PhongBRDF);
// 	t2.setBRDF(new PhongBRDF);
// 
// 	Material * material_blue = new Material(Vec(0, 0, 0.5), Vec(0, 0, 0.5), Vec(0.2, 0.2, 0.2), 50);
// 	t1.setMaterial(material_blue);
// 	t2.setMaterial(material_blue);
// 
// 	Sphere s(Point(0.5, 0.5, 4), 3.8);
// 	s.setBRDF(new PhongBRDF);
// 	s.setMaterial(new Material(Vec(0, 0, 0), Vec(0.1, 0.1, 0.1), Vec(0.1, 0.1, 0.1), 50, Vec(0, 0, 0), Vec(0.9, 0.9, 0.9), 1.5));
// 
// 	scene.objects.insert(scene.objects.end(), &t1);
// 	scene.objects.insert(scene.objects.end(), &t2);
// 	scene.objects.insert(scene.objects.end(), &s);
// 
// 	PointLight light(Point(-1, -1, 9), Color(0.5, 0.5, 0.5));
// 	scene.light_objects.insert(scene.light_objects.end(), &light);
// 
// 	scene.camera.setCamera(Point(0.5, 0.5, 8), Point(0.5, 0.5, 0), Vec(0, 1, 0), 30);

	scene.render();
	return 0;
}