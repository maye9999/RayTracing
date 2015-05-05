#include "common.h"
#include <opencv/highgui.h>

using namespace cv;

Film::Film(int width, int height)
	: width(width), height(height)
{
	mat = Mat_<Vec3b>(height, width, Vec3b(255, 255, 255));
}

void Film::commit(const Sample& sample, const Color& color)
{
	int x = sample.x - 0.5;
	int y = sample.y - 0.5;

// 	assert(color.x <= 1 && color.x >= 0);
// 	assert(color.y <= 1 && color.y >= 0);
// 	assert(color.z <= 1 && color.z >= 0);
	int cx, cy, cz;
	cx = 255 * color.x;
	cy = 255 * color.y;
	cz = 255 * color.z;
	if(cx > 255)
		cx = 255;
	if(cy > 255)
		cy = 255;
	if(cz > 255)
		cz = 255;

	mat(height - 1 - y, x)[0] = cx;
	mat(height - 1 - y, x)[1] = cy;
	mat(height - 1 - y, x)[2] = cz;
}

void Film::writeImage()
{
	Mat image(mat);
	namedWindow( "image", CV_WINDOW_AUTOSIZE );
	imshow( "image", image );
	imwrite("output.jpg", mat);
	waitKey(0);
}