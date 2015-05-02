#ifndef __MATRIX_H_
#define __MATRIX_H_

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

class Mat
{
public:
	Mat() : mat() {}
	Mat(const cv::Matx44d& mat_) : mat(mat_) {}
	Mat(const Mat& mat_) : mat(mat_.mat) {}

	static Mat all(double t);
	static Mat identity();
	static Mat zeros();
	static Mat diag(double a, double b, double c, double d);


	Mat t() const;		// Transpose
	Mat inv() const;	// Inverse
	const double operator()(int i, int j) const;
	double& operator()(int i, int j);

	friend Mat operator+(const Mat& lhs, const Mat& rhs);
	friend Mat operator-(const Mat& lhs, const Mat& rhs);
	friend Mat operator*(const Mat& lhs, const Mat& rhs);
	friend Mat operator*(const Mat& lhs, const double k);
	friend Mat operator*(const double k, const Mat& rhs);
	friend Mat operator/(const Mat& lhs, const double k);

	friend std::ostream& operator<<(std::ostream& os, const Mat& mat);

private:
	cv::Matx44d mat;
};

inline double& Mat::operator()(int i, int j)
{
	return mat(i, j);
}

inline const double Mat::operator()(int i, int j) const
{
	return mat(i, j);
}

inline Mat Mat::all(double t)
{
	return Mat(cv::Matx44d::all(t));
}

inline Mat Mat::identity()
{
	return Mat(cv::Matx44d::eye());
}

inline Mat Mat::zeros()
{
	return Mat(cv::Matx44d::zeros());
}

inline Mat Mat::diag(double a, double b, double c, double d)
{
	cv::Matx41d m(a, b, c, d);
	return Mat(cv::Matx44d::diag(m));
}

Mat Mat::t() const
{
	return Mat(mat.t());
}

Mat Mat::inv() const
{
	return Mat(mat.inv());
}

inline Mat operator+(const Mat& lhs, const Mat& rhs)
{
	return Mat(lhs.mat + rhs.mat);
}

inline Mat operator-(const Mat& lhs, const Mat& rhs)
{
	return Mat(lhs.mat - rhs.mat);
}

inline Mat operator*(const Mat& lhs, const Mat& rhs)
{
	return Mat(lhs.mat * rhs.mat);
}

inline Mat operator*(const Mat& lhs, const double k)
{
	return Mat(lhs.mat * k);
}

inline Mat operator*(const double k, const Mat& rhs)
{
	return Mat(rhs.mat * k);
}

inline Mat operator/(const Mat& lhs, const double k)
{
	return Mat(lhs.mat / k);
}

inline std::ostream& operator<<(std::ostream& os, const Mat& mat)
{
	os << "[" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			os << mat(i, j) << ", ";
		}
		os << std::endl;
	}
	os << "]";
	return os;
}

#endif