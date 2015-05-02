#ifndef __COMMON_H_
#define __COMMON_H_

#include "vec.h"
#include <climits>

#define T_MAX INT_MAX

typedef Vec Normal;
typedef Vec Point;
typedef Vec Color;

struct Sample
{
	double x, y;
};

/************************************************************************/
/* It Represents the ray P(t) = pos + t*dir, where t in [t_min, t_max]  */
/************************************************************************/
struct Ray
{
	Point start_position;
	Vec direction;
	double t_min, t_max;
	Ray(Point start_position, Vec direction, double t_min = 0, double t_max = T_MAX)
		: start_position(start_position), direction(direction), t_min(t_min), t_max(t_max) {}
};

class BRDF
{

};

#endif