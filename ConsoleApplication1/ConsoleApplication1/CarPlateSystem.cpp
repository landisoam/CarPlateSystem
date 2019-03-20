#include "stdafx.h"
#include "highgui\highgui.hpp"
#include "core\core.hpp"
#include <iostream>
#include "imgproc.hpp"
#include <string.h>
#include <fstream>

using namespace std;
using namespace cv;

// Convert image to Greyscale

Mat converttogrey(Mat RGB)
{
	Mat Grey = Mat::zeros(RGB.size(), CV_8UC1);
	int iH = RGB.rows;
	int iW = RGB.cols;

	for (int i = 0; i < iH; i++)

	{
		for (int j = 0; j < iW * 3; j = j + 3)
		{

			Grey.at<uchar>(i, j / 3) = (RGB.at<uchar>(i, j) + RGB.at<uchar>(i, j + 1) + RGB.at<uchar>(i, j + 2)) / 3;

		}
	}

	return Grey;

}
int otsu(Mat Grey)
{
	float count[256] = { 0 };
	float prob[256] = { 0 };
	float mew[256] = { 0 };
	float theta[256] = { 0 };
	float sigma[256] = { 0 };
	int iH = Grey.rows;
	int iW = Grey.cols;

	for (int i = 0; i < iH; i++)
	{
		for (int j = 0; j < iW; j++)
		{
			count[Grey.at<uchar>(i, j)]++;
		}
	}

	for (int i = 0; i < 256; i++)
	{
		prob[i] = count[i] / (float)(iH * iW);
	}

	mew[0] = 0;
	for (int i = 1; i < 256; i++)
	{
		mew[i] = mew[i - 1] + i * prob[i];
	}

	theta[0] = prob[0];
	for (int i = 1; i < 256; i++)
	{
		theta[i] = theta[i - 1] + prob[i];
	}

	int MaxSigma = 0;
	int MaxSigmaCount = 0;
	for (int i = 0; i < 256; i++)
	{
		sigma[i] = (pow((mew[255] * theta[i] - mew[i]), 2) / (theta[i] * (1 - theta[i])));
		if (MaxSigma < sigma[i])
		{
			MaxSigma = sigma[i];
			MaxSigmaCount = i;

		}

	}

	return MaxSigmaCount;

}
// Convert image to Binary
Mat converttoBinary(Mat Grey, int threshold)
{
	Mat Bin = Mat::zeros(Grey.size(), CV_8UC1);
	int iHG = Grey.rows;
	int iWG = Grey.cols;

	for (int i = 0; i < iHG; i++)
	{
		for (int j = 0; j < iWG; j++)
		{
			if (Grey.at<uchar>(i, j) > threshold)
				Bin.at<uchar>(i, j) = 255;

		}
	}
	return Bin;
}

// Equalized histogram

Mat equalizedhisto(Mat Grey)

{
	int icount[256] = { 0 };
	for (int i = 0; i < Grey.rows; i++)
	{
		for (int j = 0; j < Grey.cols; j++)
		{
			icount[Grey.at<uchar>(i, j)] ++;
		}
	}

	float prob[256] = { 0.0 };
	for (int i = 0; i < 256; i++)
	{
		prob[i] = (float)icount[i] / (float)(Grey.rows * Grey.cols);
	}

	float accprob[256] = { 0.0 };
	accprob[0] = prob[0];
	for (int i = 1; i < 256; i++)
	{
		accprob[i] = prob[i] + accprob[i - 1];
	}

	int newvalue[256] = { 0 };
	for (int i = 0; i < 256; i++)
	{
		newvalue[i] = 255 * accprob[i];
	}

	Mat EqHist = Mat::zeros(Grey.size(), CV_8UC1);

	for (int i = 0; i < Grey.rows; i++)
	{
		for (int j = 0; j < Grey.cols; j++)

			EqHist.at<uchar>(i, j) = newvalue[Grey.at<uchar>(i, j)];
	}

	return EqHist;
}

// Blur Function

Mat BlurFunc(Mat Grey, int winsize)
{
	Mat Blur = Mat::zeros(Grey.size(), CV_8UC1);

	for (int i = winsize; i < Grey.rows - winsize; i++)
	{
		for (int j = winsize; j < Grey.cols - winsize; j++)
		{
			int sum = 0;
			for (int ii = -winsize; ii <= winsize; ii++)
			{
				for (int jj = -winsize; jj <= winsize; jj++)
				{
					sum += Grey.at<uchar>(i + ii, j + jj);
				}
			}
			Blur.at<uchar>(i, j) = sum / ((winsize * 2 + 1) * (winsize * 2 + 1));
		}
	}
	return Blur;
}

// Blur Function

/*Mat SobelY(Mat Grey)
{
	int winsize = 1;
	Mat Blur = Mat::zeros(Grey.size(), CV_8UC1);

	for (int i = winsize; i < Grey.rows - winsize; i++)
	{
		for (int j = winsize; j < Grey.cols - winsize; j++)
		{
			double sum = 0;
			for (int ii = -winsize; ii <= winsize; ii++)
			{
				for (int jj = -winsize; jj <= winsize; jj++)
				{
					double temp = 1;


					if ((jj == -1 && ii == 1) || (jj == -1 && ii == -1))
					{
						temp = -1;
					}

					if ((jj == -1 && ii == 0))
					{
						temp = -1;
					}

					if ((jj == 1 && ii == 1) || (jj == 1 && ii == -1))
					{
						temp = 1;
					}

					if ((jj == 1 && ii == 0))
					{
						temp = 1;
					}

					if ((jj == 0))
					{
						temp = 0;
					}

					sum += Grey.at<uchar>(i + ii, j + jj) * temp;
				}
			}


			Blur.at<uchar>(i, j) = int(abs(sum));
		}
	}
	return Blur;
} */

Mat billateralfilter(Mat Blur)
{

	Mat dst;

	//Apply bilateral filter

	bilateralFilter(Blur, dst, 60, 70, 80);


	return dst;
}



/*Mat Invertcolor(Mat Grey)
{
	Mat Invert = Mat::zeros(Grey.size(), CV_8UC1);

	for (int i = 0; i < Grey.rows; i++)
	{

		for (int j = 0; j < Grey.cols; j++)
		{
			Invert.at<uchar>(i, j) = (255 - Grey.at<uchar>(i, j));
		}
	}

	return Invert;

} */

/*Mat Stepfunc(Mat Grey, int x1, int x2)
{
	Mat Step = Mat::zeros(Grey.size(), CV_8UC1);

	for (int i = 0; i < Grey.rows; i++)
	{
		for (int j = 0; j < Grey.cols; j++)
		{
			if (Grey.at<uchar>(i, j) >= x1 && Grey.at<uchar>(i, j) <= x2)
			{
				Step.at<uchar>(i, j) = 255;
			}
			else
			{
				Step.at<uchar>(i, j) = 0;
			}
		}
	}

	return Step;
}

void insertionSort(int window[])
{
	int temp, i, j;
	for (i = 0; i < 9; i++) {
		temp = window[i];
		for (j = i - 1; j >= 0 && temp < window[j]; j--) {
			window[j + 1] = window[j];
		}
		window[j + 1] = temp;
	}
}


Mat MedianFilter(Mat Blur)
{
	int window[9];

	Mat xyz;

	xyz = Blur.clone();
	//for (int y = 0; y < Blur.rows; y++)
		//for (int x = 0; x < Blur.cols; x++)
			//xyz.at<uchar>(y, x) = 0.0;
	for (int y = 1; y < Blur.rows - 1; y++)
	{
		for (int x = 1; x < Blur.cols - 1; x++)
		{
			window[0] = Blur.at<uchar>(y - 1, x - 1);
			window[1] = Blur.at<uchar>(y, x - 1);
			window[2] = Blur.at<uchar>(y + 1, x - 1);
			window[3] = Blur.at<uchar>(y - 1, x);
			window[4] = Blur.at<uchar>(y, x);
			window[5] = Blur.at<uchar>(y + 1, x);
			window[6] = Blur.at<uchar>(y - 1, x + 1);
			window[7] = Blur.at<uchar>(y, x + 1);
			window[8] = Blur.at<uchar>(y + 1, x + 1);

			insertionSort(window);

			// assign the median to centered element of the matrix
			xyz.at<uchar>(y, x) = window[4];
		}
	}

	return xyz;
} */
// Finding edges
Mat EdgeDetection(Mat xyz)
{
	Mat Edge = Mat::zeros(xyz.size(), CV_8UC1);
	for (int i = 1; i < xyz.rows - 1; i++)
	{
		for (int j = 1; j < xyz.cols - 1; j++)
		{
			int AVGL = ((xyz.at<uchar>(i - 1, j - 1)) + (xyz.at<uchar>(i, j - 1)) + (xyz.at<uchar>(i + 1, j - 1))) / 3;
			int AVGR = ((xyz.at<uchar>(i - 1, j + 1)) + (xyz.at<uchar>(i, j + 1)) + (xyz.at<uchar>(i + 1, j + 1))) / 3;
			if (abs(AVGR - AVGL) >= 50)
			{

				Edge.at<uchar>(i, j) = 255;
			}
		}
	}

	return Edge;
}

Mat Dilation(Mat Edge, int x)
{
	Mat Dil = Mat::zeros(Edge.size(), CV_8UC1);


	for (int i = x; i < Edge.rows - x; i++)
	{

		for (int j = x; j < Edge.cols - x; j++)
		{
			int flag = 0;
			for (int ii = -x; ii <= x; ii++)
			{
				for (int jj = -x; jj <= x; jj++)
				{
					if (Edge.at<uchar>(i + ii, j + jj) == 255)
					{
						Dil.at<uchar>(i, j) = 255;
					}
				}
			}

		}

	}
	return Dil;
}






int main()
{
	for (int i = 2; i <= 20; i++)
	{
		printf("-- %d --\n", i);
		Mat img;
		img = imread("Dataset\\" + to_string(i) + ".jpg");
		imshow("Original", img);
		waitKey();


		Mat test = billateralfilter(img);
		imshow("bilateral", test);
		waitKey();


		Mat Grey = converttogrey(test);
		imshow("Grey", Grey);
		waitKey();



		Mat EqHist = equalizedhisto(Grey);
		imshow("Equalized", EqHist);
		waitKey();
		Mat Blur = BlurFunc(EqHist, 1);
		imshow("Blur", Blur);
		waitKey();



		//Mat xyz = MedianFilter(Blur);
		//imshow("Median", xyz);
		//waitKey();


		Mat Edge = EdgeDetection(Blur);
		imshow("EdgeDetection", Edge);
		waitKey();
		Edge = converttoBinary(Edge, 100);
		imshow("Binary", Edge);
		//waitKey();
		Mat Dil = Dilation(Edge, 4);
		imshow("Dilation", Dil);
		//waitKey();

		Mat Blob;
		Blob = Dil.clone();

		vector<vector<Point> > contours1;
		vector<Vec4i> hierachy1;
		findContours(Dil, contours1, hierachy1, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));


		Mat dst = Mat::zeros(Grey.size(), CV_8UC3);

		if (!contours1.empty())
		{
			for (int i = 0; i < contours1.size(); i++)
			{
				Scalar colour((rand() & 255), (rand() & 255), (rand() & 255));
				drawContours(dst, contours1, i, colour, CV_FILLED, 8, hierachy1);
			}
		}

		imshow("test", dst);
		waitKey();

		Rect BlobRect;
		Mat plate;
		Scalar black = CV_RGB(0, 0, 0);
		const float aspect = 4.7272;
		int min = 15 * aspect * 15; // minimum area
		int max = 125 * aspect * 125; // maximum area

		int temp_width = 0;

		for (int j = 0; j < contours1.size(); j++)
		{

			BlobRect = boundingRect(contours1[j]);
			int area = BlobRect.height * BlobRect.width;

			area = (float)contourArea(contours1[j]);
			float density = area / ((float)BlobRect.height * (float)BlobRect.width);


			if (BlobRect.width < 51 || BlobRect.width > 162 || BlobRect.height < 20 || BlobRect.height > 47 || area < min || area > max || density < 0.57 || density > 0.9)
			{
				drawContours(Blob, contours1, j, black, CV_FILLED, 8, hierachy1);

			}

			else
			{
				if (BlobRect.width > temp_width)
				{
					temp_width = BlobRect.width;
					plate = Grey(BlobRect);
				}
				//printf("%d - %d - %d - %d - %f - %d - %d \n", i, BlobRect.width, BlobRect.height, area, density, BlobRect.x, BlobRect.y);

			}

		}




		imshow("Blob", Blob);
		waitKey();
		Mat Greyplate = plate;


		if (plate.empty())
		{
			continue;
		}

		int otsu_value = otsu(plate) + 70;

		plate = converttoBinary(plate, otsu_value);

		imshow("Plate", plate);
		waitKey();


		findContours(plate, contours1, hierachy1, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));

		Mat result = Mat::zeros(Grey.size(), CV_8UC1);

		for (int j = 0; j < contours1.size(); j++)
		{

			BlobRect = boundingRect(contours1[j]);

			if (BlobRect.width > 20 || BlobRect.height < 9 || BlobRect.height > 30)
			{
				drawContours(plate, contours1, j, black, CV_FILLED, 8, hierachy1);
				//printf(" %d - %d \n", BlobRect.width, BlobRect.height);
			}
			else
			{
				drawContours(result, contours1, j, Scalar(255, 255, 255), CV_FILLED, 8, hierachy1);
			}

		}

		imshow("Characters", plate);
		waitKey();

		plate = result;



		findContours(plate, contours1, hierachy1, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));

		if (contours1.size() > 0)
		{
			bool swapp = true;

			while (swapp) {

				swapp = false;

				//lloopp
				for (size_t i = 0; i < contours1.size() - 1; i++)
				{

					//swap
					if (boundingRect(contours1[i]).x > boundingRect(contours1[i + 1]).x)
					{
						auto temp1 = contours1[i];
						auto temp2 = hierachy1[i];
						hierachy1[i] = hierachy1[i + 1];
						contours1[i] = contours1[i + 1];
						hierachy1[i + 1] = temp2;
						contours1[i + 1] = temp1;
						swapp = true;
					}
				}
			}
		}

		for (int j = 0; j < contours1.size(); j++)
		{
			BlobRect = boundingRect(contours1[j]);
			Mat c = Greyplate(BlobRect);
			resize(c, c, Size(c.size().width* 1.15, c.size().height * 1.15));
			c = converttoBinary(c, otsu_value);
			copyMakeBorder(c, c, 2, 2, 2, 2, BORDER_CONSTANT, Scalar(0, 0, 0));


			imwrite("input.png", c);
			imwrite("Characters\\picture-" + to_string(i) + "-character-" + to_string(j) + ".png", c);
			system("\"Tesseract-OCR\\tesseract.exe\" input.png output --psm 10 >nul 2>&1");

			fstream myfile("output.txt");
			string line;

			myfile.is_open();
			while (getline(myfile, line))
			{
				cout << "\t" << line;
			}
			myfile.close();

			printf("\n");
		}
		waitKey();
	}
	return 0;
}
