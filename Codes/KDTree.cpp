#include "KDTree.h"

using namespace std;

void KDTree::buildTree()	// Build the tree
{
	_root = new KDNode;
	_root->dim = 0;
	_root->objects = _objects;
	// Update the bounding box
	double x_small = _objects[0]->getXLeft().x;
	double x_big = x_small;
	double y_small = _objects[0]->getYLeft().y;
	double y_big = y_small;
	double z_small = _objects[0]->getZLeft().z;
	double z_big = z_small;
	for (auto i : _objects)
	{
		double t1 = i->getXLeft().x;
		double tt1 = i->getXRight().x;
		double t2 = i->getYLeft().y;
		double tt2 = i->getYRight().y;
		double t3 = i->getZLeft().z;
		double tt3 = i->getZRight().z;
		if(t1 < x_small)
			x_small = t1;
		if(tt1 > x_big)
			x_big = tt1;
		if(t2 < y_small)
			y_small = t2;
		if(tt2 > y_big)
			y_big = tt2;
		if(t3 < z_small)
			z_small = t3;
		if(tt3 > z_big)
			z_big = tt3;
	}
	_root->small = Point(x_small, y_small, z_small);
	_root->big = Point(x_big, y_big, z_big);
	divideAndBuild(_root);	// Recursively build tree
	cout << "KDTree Build Complete!" << endl;
}

void KDTree::divideAndBuild(KDNode* root)
{
	int n = root->objects.size();
	if (n < 3)	// Recursive base
	{
		return;
	}
	root->left = new KDNode;
	root->right = new KDNode;
	root->left->small = root->small;
	root->right->big = root->big;
	root->left->dim = root->right->dim = ((root->dim + 1) % 3);
	// Divide the box
	if (root->left->dim == 0)		// X
	{
		// Find the middle
		nth_element(root->objects.begin(), root->objects.begin()+n/2, root->objects.end(),
			[](Primitive* a, Primitive* b){
			return b->getXLeft().x > a->getXLeft().x;
		});
		root->left->objects = vector<Primitive*>(root->objects.begin(), root->objects.begin()+n/2);
		root->right->objects = vector<Primitive*>(root->objects.begin()+n/2, root->objects.end());
	}
	else if(root->left->dim == 1)	// Y
	{
		nth_element(root->objects.begin(), root->objects.begin()+n/2, root->objects.end(),
			[](Primitive* a, Primitive* b){
				return b->getYLeft().y > a->getYLeft().y;
		});
		root->left->objects = vector<Primitive*>(root->objects.begin(), root->objects.begin()+n/2);
		root->right->objects = vector<Primitive*>(root->objects.begin()+n/2, root->objects.end());
	}
	else							// Z
	{
		nth_element(root->objects.begin(), root->objects.begin()+n/2, root->objects.end(),
			[](Primitive* a, Primitive* b){
				return b->getZLeft().z > a->getZLeft().z;
		});
		root->left->objects = vector<Primitive*>(root->objects.begin(), root->objects.begin()+n/2);
		root->right->objects = vector<Primitive*>(root->objects.begin()+n/2, root->objects.end());
	}
	// Update the bounding box
	double x_big = root->left->objects[0]->getXRight().x;
	double y_big = root->left->objects[0]->getYRight().y;
	double z_big = root->left->objects[0]->getZRight().z;
	for (auto i : root->left->objects)
	{
		double t1 = i->getXRight().x;
		double t2 = i->getYRight().y;
		double t3 = i->getZRight().z;
		if(t1 > x_big)
			x_big = t1;
		if(t2 > y_big)
			y_big = t2;
		if(t3 > z_big)
			z_big = t3;
	}
	root->left->big = Point(x_big, y_big, z_big);
	double x_small = root->right->objects[0]->getXLeft().x;
	double y_small = root->right->objects[0]->getYLeft().y;
	double z_small = root->right->objects[0]->getZLeft().z;
	for (auto i : root->right->objects)
	{
		double t1 = i->getXLeft().x;
		double t2 = i->getYLeft().y;
		double t3 = i->getZLeft().z;
		if(t1 < x_small)
			x_small = t1;
		if(t2 < y_small)
			y_small = t2;
		if(t3 < z_small)
			z_small = t3;
	}
	root->right->small = Point(x_small, y_small, z_small);
	divideAndBuild(root->left);
	divideAndBuild(root->right);
}

// Find objects in tree
bool KDTree::findNearestPrimitive(KDNode* root, const Ray& ray, double& t_hit, LocalGeometry& local_geo, BRDF*& brdf, Color* texture_color)
{
	if (root->objects.size() == 0)
	{
		return false;
	}
	if(root->objects.size() < 3)
	{
		bool is_intersect = false;
		for(auto i : root->objects)
		{
			if(i->intersect(ray, t_hit, local_geo, texture_color))
			{
				is_intersect = true;
				brdf = i->getBRDF();
			}
		}
		return is_intersect;
	}
	bool flag1 = false, flag2 = false;
	if(root->left->intersect(ray, t_hit))
	{
		flag1 = findNearestPrimitive(root->left, ray, t_hit, local_geo, brdf, texture_color);
	}
	if(root->right->intersect(ray, t_hit))
	{
		flag2 = findNearestPrimitive(root->right, ray, t_hit, local_geo, brdf, texture_color);
	}
	return flag1 || flag2;
}

bool KDTree::intersectWithLight(KDNode* root, const Ray& ray)
{
	if (root->objects.size() == 0)
	{
		return false;
	}
	if(root->objects.size() < 3)
	{
		bool is_intersect = false;
		for(auto i : root->objects)
		{
			if(i->intersectWithLight(ray))
			{
				is_intersect = true;
				break;
			}
		}
		return is_intersect;
	}
	bool flag1 = false, flag2 = false;
	if(root->left->intersect(ray, T_MAX))
	{
		flag1 = intersectWithLight(root->left, ray);
	}
	if(root->right->intersect(ray, T_MAX))
	{
		flag2 = intersectWithLight(root->right, ray);
	}
	return flag1 || flag2;
}

bool KDNode::intersect(const Ray& ray, double t_hit)
{
	double x = ray.start_position.x;
	double y = ray.start_position.y;
	double z = ray.start_position.z;
	double tx, ty, tz;
	double txx, tyy, tzz;
	tx = (small.x - x) / ray.direction.x;
	txx = (big.x - x) / ray.direction.x;
	ty = (small.y - y) / ray.direction.y;
	tyy = (big.y - y) / ray.direction.y;
	tz = (small.z - z) / ray.direction.z;
	tzz = (big.z - z) / ray.direction.z;
	double tmin = max(max(min(tx, txx), min(ty, tyy)), min(tz, tzz));
	double tmax = min(min(max(tx, txx), max(ty, tyy)), max(tz, tzz));
	if(tmin <= tmax && tmin < t_hit )
		return true;
	return false;
}
