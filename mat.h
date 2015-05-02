#ifndef __MATRIX_H_
#define __MATRIX_H_

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

class Mat : cv::Matx44d
{
public:
	Mat() : cv::Matx44d() {}

	static Mat all();
	static Mat identity();
	static Mat zero();
	static Mat diag(double t);


};

#endif