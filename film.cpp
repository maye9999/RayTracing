#include "common.h"
#include <opencv/highgui.h>

using namespace cv;

Film::Film(int width, int height)
{
	mat = Mat_<Vec3b>(height, width, Vec3b(255, 255, 255));
}

void Film::commit(const Sample& sample, const Color& color)
{
	int x = sample.x - 0.5;
	int y = sample.y - 0.5;

	assert(color.x <= 1 && color.x >= 0);
	assert(color.y <= 1 && color.y >= 0);
	assert(color.z <= 1 && color.z >= 0);

	mat(x, y)[0] = color.x * 255;
	mat(x, y)[1] = color.y * 255;
	mat(x, y)[2] = color.z * 255;
}

void Film::writeImage()
{
	Mat image(mat);
	namedWindow( "image", CV_WINDOW_AUTOSIZE );
	imshow( "image", image );
	imwrite("output.jpg", mat);
	waitKey(0);
}