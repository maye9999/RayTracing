#include "common.h"
#include <opencv/highgui.h>

using namespace cv;

Film::Film(int width, int height, bool super_sampling)
	: width(width), height(height), super_sampling(super_sampling)
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

	mat(height - 1 - y, x)[0] = cz;
	mat(height - 1 - y, x)[1] = cy;
	mat(height - 1 - y, x)[2] = cx;
}

void Film::writeImage()
{
	if(!super_sampling)
	{
		Mat image(mat);
		namedWindow( "image", CV_WINDOW_AUTOSIZE );
		imshow( "image", image );
		imwrite("output.png", mat);
		waitKey(0);
	}
	else
	{
		Mat_<Vec3b> t(height / 3, width / 3, Vec3b(255, 255, 255));
		for (int i = 0; i < height / 3; i++)
		{
			for (int j = 0; j < width / 3; j++)
			{
				double cx = 1.0 / 16.0 * (mat(3*i, 3*j)[0] + 2.0*mat(3*i+1, 3*j)[0] + mat(3*i + 2, 3*j)[0] + 
										 2.0*mat(3*i, 3*j+1)[0] + 4.0*mat(3*i+1, 3*j+1)[0] + 2.0*mat(3*i+2, 3*j+1)[0] +
										 mat(3*i, 3*j+2)[0] + 2.0*mat(3*i+1, 3*j+2)[0] + mat(3*i+2, 3*j+2)[0]);
				double cy = 1.0 / 16.0 * (mat(3*i, 3*j)[1] + 2.0*mat(3*i+1, 3*j)[1] + mat(3*i + 2, 3*j)[1] + 
										 2.0*mat(3*i, 3*j+1)[1] + 4.0*mat(3*i+1, 3*j+1)[1] + 2.0*mat(3*i+2, 3*j+1)[1] +
										 mat(3*i, 3*j+2)[1] + 2.0*mat(3*i+1, 3*j+2)[1] + mat(3*i+2, 3*j+2)[1]);
				double cz = 1.0 / 16.0 * (mat(3*i, 3*j)[2] + 2.0*mat(3*i+1, 3*j)[2] + mat(3*i + 2, 3*j)[2] + 
										 2.0*mat(3*i, 3*j+1)[2] + 4.0*mat(3*i+1, 3*j+1)[2] + 2.0*mat(3*i+2, 3*j+1)[2] +
										 mat(3*i, 3*j+2)[2] + 2.0*mat(3*i+1, 3*j+2)[2] + mat(3*i+2, 3*j+2)[2]);
				t(i, j)[0] = cx;
				t(i, j)[1] = cy;
				t(i, j)[2] = cz;
			}
		}
		Mat image(t);
		namedWindow( "image", CV_WINDOW_AUTOSIZE );
		imshow( "image", image );
		imwrite("output.png", image);
		waitKey(0);
	}
}