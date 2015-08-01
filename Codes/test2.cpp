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
	//Scene scene(7200, 5400, true, 40);
	Scene scene(800, 600, false, 40);

	PhongBRDF brdf;
	Material material(Vec(0.0, 0.1, 0.0), Vec(0.2, 0.4, 0.2), Vec(0.1, 0.1, 0.1), 20, Vec(0, 0, 0)/*, Vec(0.9, 0.9, 0.9), 1.5*/);
	Material mat_refr(Color(0.15, 0.15, 0.15), Color(0., 0., 0.), Color(0.4, 0.4, 0.4), 20, Color(0, 0, 0), Color(0.9, 0.9, 0.9), 1.5);
	Material mat_refl(Color(0.1, 0.1, 0.1), Color(0.3, 0.3, 0.3), Color(0.4, 0.4, 0.4), 20, Color(0.4, 0.4, 0.4));
	Material mat_red(Color(0.1, 0.0, 0.0), Color(0.5, 0.2, 0.2), Color(0.3, 0.3, 0.3), 20);
	Material mat_green(Color(0, 0.1, 0.0), Color(0.2, 0.5, 0.2), Color(0.3, 0.3, 0.3), 20);
	Material mat_blue(Color(0, 0.0, 0.1), Color(0.2, 0.2, 0.5), Color(0.3, 0.3, 0.3), 20);
	//Material material(Vec(1, 1, 1), Vec(0.1, 0.1, 0.1), Vec(0.1, 0.1, 0.1), 20);

	PlaneLight pl(&scene, Point(0.5, -0.25, 2), Vec(0.5, 0, 0), Vec(0, 0.5, 0), Color(1, 1, 1), 2);
	scene.plane_lights.push_back(&pl);
	scene.light_objects.push_back(&pl);

	ObjFile* file = new ObjFile("dragon.obj");
	if(!scene.loadFile(file))
	{
		return -1;
	}
	int t = scene.objects.size();
	for(auto p : scene.objects)
	{
		auto pp = (Triangle*)p;
		pp->a.x += 1.2;
		pp->a.y += 1;
		pp->b.x += 1.2;
		pp->b.y += 1;
		pp->c.x += 1.2;
		pp->c.y += 1;
		pp->setMaterial(&mat_green);
	}
	t = scene.objects.size();
	ObjFile* file2 = new ObjFile("bunny.obj");
	if(!scene.loadFile(file2))
	{
		return -1;
	}
	for(int i = t; i < scene.objects.size(); ++i)
	{
		auto pp = (Triangle*)scene.objects[i];
		pp->setMaterial(&mat_blue);
		pp->a += Vec(0.8, -1, 0.1);
		pp->b += Vec(0.8, -1, 0.1);
		pp->c += Vec(0.8, -1, 0.1);
	}
	t = scene.objects.size();
	ObjFile* file3 = new ObjFile("dinosaur.obj");
	if(!scene.loadFile(file3))
	{
		return -1;
	}
	for(int i = t; i < scene.objects.size(); ++i)
	{
		auto pp = (Triangle*)scene.objects[i];
		pp->setMaterial(&mat_red);
		pp->a += Vec(0.8, 0, 0.07);
		pp->b += Vec(0.8, 0, 0.07);
		pp->c += Vec(0.8, 0, 0.07);
	}
	// X-Y
	Triangle t1(Point(-2, -2, -0.35), Point(-2, 2, -0.35), Point(2, 2, -0.35));
	Triangle t2(Point(-2, -2, -0.35), Point(2, -2, -0.35), Point(2, 2, -0.35));
	// Y-Z
	Triangle t3(Point(-2, -2, -1), Point(-2, 2, -1), Point(-2, 2, 2));
	Triangle t4(Point(-2, -2, -1), Point(-2, -2, 2), Point(-2, 2, 2));
	// X-Z
	Triangle t5(Point(-2, -2, -1), Point(2, -2, -1), Point(2, -2, 2));
	Triangle t6(Point(-2, -2, -1), Point(-2, -2, 2), Point(2, -2, 2));
	Triangle t7(Point(-2, 2, -1), Point(2, 2, -1), Point(2, 2, 2));
	Triangle t8(Point(-2, 2, -1), Point(-2, 2, 2), Point(2, 2, 2));
	Material m2(Color(0.1, 0.1, 0.1), Color(0.6, 0.6, 0.6), Color(0.1, 0.1, 0.1), 20, Color(0.2, 0.2, 0.2));
	Material m3(Color(0.1, 0.1, 0.1), Color(0.9, 0.9, 0.9), Color(0.1, 0.1, 0.1), 20, Color(0.2, 0.2, 0.2));
	t1.setMaterial(&m2);
	t2.setMaterial(&m2);
	t3.setMaterial(&mat_red);
	t4.setMaterial(&mat_red);
	t5.setMaterial(&mat_green);
	t6.setMaterial(&mat_green);
	t7.setMaterial(&mat_blue);
	t8.setMaterial(&mat_blue);

	Texture * texture = new Texture("stone-color.png");
	Texture * texture2 = new Texture("BricksLgMortar.bmp");
	TextureMapping *tm2 = new TextureMapping;
	tm2->ut = make_pair(0, 0);
	tm2->vt = make_pair(1, 0);
	tm2->wt = make_pair(1, 1);
	TextureMapping *tm = new TextureMapping;
	tm->ut = make_pair(0, 0);
	tm->vt = make_pair(0, 1);
	tm->wt = make_pair(1, 1);
	t1.setTexture(texture);
	t2.setTexture(texture);
	t1.setTextureMapping(*tm);
	t2.setTextureMapping(*tm2);

	Sphere sp1(Point(0.25, 0.75, 0.5), 0.5);
	Sphere sp2(Point(-0.5, -0.5, 0.5), 0.7);
	sp1.setMaterial(&mat_refl);
	sp2.setMaterial(&mat_refl);
	scene.objects.push_back(&sp1);
	scene.objects.push_back(&sp2);
	scene.objects.push_back(&t1);
	scene.objects.push_back(&t2);
	scene.objects.push_back(&t3);
	scene.objects.push_back(&t4);
	scene.objects.push_back(&t5);
	scene.objects.push_back(&t6);
	scene.objects.push_back(&t7);
	scene.objects.push_back(&t8);

	scene.setGlobalBRDF(&brdf);

	PointLight light(&scene, Point(10, 0, 1.5), Color(0.2, 0.2, 0.2));
	PointLight light2(&scene, Point(2, -2, 2), Color(0.9, 0.9, 0.9));
	PointLight light3(&scene, Point(20, 50, 20), Color(0.9, 0.9, 0.9));
	scene.light_objects.push_back(&light);
	//scene.light_objects.push_back(&light2);
	//scene.light_objects.push_back(&light3);
	
	scene.camera.setCamera(Point(5, 0, 1.0), Point(0, 0, 1.0), Vec(0, 0, 1), 45, false, 0.5, 4.0);
	scene.render(8, true);
	return 0;
}