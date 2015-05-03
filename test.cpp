#include "vec.h"
#include "mat.h"
#include "common.h"
#include "primitive.h"
#include <iostream>

using namespace std;

int main()
{
	Scene scene(500, 500);
	Triangle t2(Point(0, 0, 0), Point(0, 1, 3), Point(1, 0, 0));
	Sphere t1(Point(2, 0, 0), 1);
	scene.objects.insert(scene.objects.end(), &t1);
	scene.objects.insert(scene.objects.end(), &t2);
	scene.camera.setCamera(Point(0, 0, 12), Point(0, 0, 0), Vec(0, 1, 0), 90);
	scene.render();
	return 0;
}