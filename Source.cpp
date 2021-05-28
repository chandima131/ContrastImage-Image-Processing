#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"

//Assignment 1 - 174144P Samankumari S.M.C

using namespace cv;
using namespace std;

int main(int argc , char* argv[]) {
	Mat img = imread(argv[1], 1); 

	if (!img.data) {
		cout << " Could not find the image!" << endl;
		return -1;
	}

	int height = img.rows;
	int width = img.cols;
	int widthstep =  img.step;
	int ch = img.channels();

	printf("Height : %d , Width: %d, WidthStep %d, Channels: %d",height,width,widthstep,ch);

	img.at<uchar>(10, 10) = 255;

	Mat gray(height, width, CV_8UC1, Scalar(0));
	cvtColor(img, gray, COLOR_BGR2GRAY); 

	// Contrast streching

	Mat contrast_image = gray.clone(); // Get a copy of gray image to do contast streaching
	int v;
	int output;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			v = (int)gray.at<uchar>(y, x);
			if ((v >= 0) && (v <= 58)) {          // function and the conditions for point 1, point 2 and point 3
				output = int((5 / 58.0) * v);  
			}
			else if (v > 58 && v <= 202) {
				output = int((245 / 144.0) * (v - 58) + 5); 
			}
			else if (v > 202 && v <= 255) {
				output = int((5/53.0)* (v - 202) + 250); 
			}
			contrast_image.at<uchar>(y, x) = (uchar)output;
		}
	}

	//Histogram creating part

	int contrast_height = contrast_image.rows;
	int contrast_width = contrast_image.cols;
	int hist[256];

	for (int i = 0; i < 256; i++) {
		hist[i] = 0;
	}

	for (int y = 0; y < contrast_height; y++) {
		for (int x = 0; x < contrast_width; x++) {
			hist[(int)contrast_image.at<uchar>(y, x)] = hist[(int)contrast_image.at<uchar>(y, x)] + 1;

		}
	}

	for (int i = 0; i < 256; i++) {
		cout << i << ":" << hist[i] << endl;
	}

	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / 256);


	Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0));

	int max = hist[0];
	for (int i = 0; i < 256; i++) {
		if (max < hist[i]) {
			max = hist[i];
		}
	}

	for (int i = 0; i < 256; i++) {
		hist[i] = ((double)hist[i] / max) * histImage.rows;
	}

	for (int i = 0; i < 256; i++) {
		line(histImage, Point(bin_w * (i), hist_h), Point(bin_w * (i), hist_h - hist[i]), Scalar(255), 1, 8, 0); // create histogram foe contrasted image

	}

	namedWindow("Image");
	imshow("Image", img); // Display given image
	namedWindow("Gray");
	imshow("Gray", gray); // Display gray colord image
	namedWindow("Contrast_Image");
	imshow("Contrast_Image", contrast_image); // Display contrasted image
	namedWindow("Histogram");
	imshow("Histogram", histImage); // Display contrasted histogram
	imwrite("contrastImage.jpg", contrast_image); // Save contrasted image
	imwrite("Histogram.jpg", histImage); //Save contasted histogram
	
	waitKey(0);
	return 0;

	
}