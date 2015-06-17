#ifndef __VEC_H_
#define __VEC_H_

#include <iostream>
#include <cmath>

struct Vec
{
	Vec(double xx = 0, double yy = 0, double zz = 0) : x(xx), y(yy), z(zz) {}
	double x, y, z;

	bool notZero() const;

	// Static Function
	static Vec normalize(const Vec& vec);
	static const Vec cross(const Vec& lhs, const Vec& rhs);
	static const Vec mul(const Vec& lhs, const Vec& rhs);

	// Member Binary Operator
	Vec& operator+=(const Vec& vec);
	Vec& operator-=(const Vec& vec);

	// Non-member Binary Operator
	friend const Vec operator+(const Vec& lhs, const Vec& rhs);
	friend const Vec operator-(const Vec& lhs, const Vec& rhs);
	friend const double operator*(const Vec& lhs, const Vec& rhs);
	friend const Vec operator*(const Vec& vec, const double k);
	friend const Vec operator*(const double k, const Vec& vec);
	friend const Vec operator/(const Vec& vec, const double k);
	friend std::ostream& operator<<(std::ostream& os, const Vec& vec);
};

inline const Vec operator+(const Vec& lhs, const Vec& rhs)
{
	return Vec(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}

inline const Vec operator-(const Vec& lhs, const Vec& rhs)
{
	return Vec(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}

inline const double operator*(const Vec& lhs, const Vec& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

inline const Vec operator*(const Vec& vec, const double k)
{
	return Vec(k*vec.x, k*vec.y, k*vec.z);
}

inline const Vec operator*(const double k, const Vec& vec)
{
	return Vec(k*vec.x, k*vec.y, k*vec.z);
}

inline const Vec operator/(const Vec& vec, const double k)
{
	return Vec(vec.x/k, vec.y/k, vec.z/k);
}

inline std::ostream& operator<<(std::ostream& os, const Vec& vec)
{
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}

inline Vec& Vec::operator+=(const Vec& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

inline Vec& Vec::operator-=(const Vec& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

inline Vec Vec::normalize(const Vec& vec)
{
	const double EPS = 1E-6;
	Vec ret(0, 0, 0);
	double sum = sqrt(vec * vec);
	if(fabs(sum) < EPS)
		return ret;
	ret = Vec(vec.x / sum, vec.y / sum, vec.z / sum);
	if(fabs(ret.x) < EPS)
		ret.x += EPS;
	if(fabs(ret.y) < EPS)
		ret.y += EPS;
	if(fabs(ret.z) < EPS)
		ret.z += EPS;
	return ret;
}

inline const Vec Vec::cross(const Vec& lhs, const Vec& rhs)
{
	return Vec(lhs.y*rhs.z - lhs.z*rhs.y, lhs.z*rhs.x - lhs.x*rhs.z, lhs.x*rhs.y - lhs.y*rhs.x);
}

inline const Vec Vec::mul(const Vec& lhs, const Vec& rhs)
{
	return Vec(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z);
}

inline bool Vec::notZero() const
{
	return (x > 1E-6) || (y > 1E-6) || (z > 1E-6);
}

#endif	// __VEC_H_