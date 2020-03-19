
# include <opencv2/opencv.hpp>
# include <opencv2/highgui/highgui.hpp>
# include <opencv2/core/core.hpp>
# include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{


	Mat in, dummy, out;
	int i, j, k, l, rows, cols, up, down, left, right;
	float sum = 0, avg = 0;

	in = imread(argv[1], CV_LOAD_IMAGE_ANYDEPTH);

	if(in.empty())
	{
		cout << "SORRY !!! Image cannot be read\n";
		exit(0);
	}

	Mat mask = imread("./mask.tif", CV_LOAD_IMAGE_ANYDEPTH);	

	up = mask.rows/2;
	down = mask.rows/2;
	left = mask.cols/2;
	right = mask.cols/2;

	//copying the source image into the middle of the destination image and then forms a border around it.
	copyMakeBorder( in, dummy, up, down, left, right, BORDER_CONSTANT, 0);

	//Convert image type from 8U to 32F
	dummy.convertTo(dummy, CV_32F, 1, 0);
	mask.convertTo(mask, CV_32F, 1, 0);
	
	//Filling matrix with zeros
	out = Mat::zeros(in.rows, in.cols, CV_32F);

	for(i = 0; i < mask.rows; i++)
		for(j = 0; j < mask.cols; j++)
		{
			sum = sum + mask.at<float>(i, j);
		}

	for(i = up;i < in.rows; i++)	
		for(j = left; j < in.cols; j++)	
		{
			avg = 0;

			for(k = 0; k < mask.rows; k++)
				for(l = 0; l < mask.cols; l++)
					avg = avg + mask.at<float>(k, l) * dummy.at<float>(k+i-up, l+j-left);

			avg = avg / sum;
			out.at<float>(i, j) = avg;
		}

	//Convert image type back to 8U from 32F
	out.convertTo(out, CV_8U);

	namedWindow("in", WINDOW_AUTOSIZE );
	namedWindow("out", WINDOW_AUTOSIZE );

	imshow("in", in);
	imshow("out", out);

	waitKey(0);

	return 0;


}
