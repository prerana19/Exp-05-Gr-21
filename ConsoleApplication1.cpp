#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;
using namespace cv;

#define BINARY_THRESHOLD 127

/*Function to create a diagonal 3x3 structuring element
The function checks for the flag if it is Dilate or Erode.
In either case it assigns apppropriate value to the intensity
and creates an output image.*/

Mat cellType3(Mat inputImage, string flag)
{
	Mat outputImage = inputImage.clone();
	int intensity;

	if (flag == "Dilate")
	{
		cout << "Dilating the Binary Image" << endl;
		intensity = 255;
	}
	else if (flag == "Erode")
	{
		cout << "Eroding the Binary Image" << endl;
		intensity = 0;
	}

	for (int i = 0; i < inputImage.rows; i++)
	{
		for (int j = 0; j < inputImage.cols; j++)
		{
			if (inputImage.at <uchar>(i, j) == intensity)
			{
				outputImage.at<uchar>(i, j) = intensity;
				if (i > 0)
					outputImage.at<uchar>(i - 1, j) = intensity;
				if (j > 0)
					outputImage.at<uchar>(i, j - 1) = intensity;
				if ((i + 1) < inputImage.rows)
					outputImage.at<uchar>(i + 1, j) = intensity;
				if ((j + 1) < inputImage.cols)
					outputImage.at<uchar>(i, j + 1) = intensity;
			}
		}
	}
	return outputImage;
}

/*Function to create a square 3x3 structuring element
The function checks for the flag if it is Dilate or Erode.
In either case it assigns apppropriate value to the intensity
and creates an output image.*/

Mat cellType2(Mat inputImage, string flag)
{
	Mat outputImage = inputImage.clone();

	int intensity = 0;
	if (flag == "Dilate")
	{
		cout << "Dilating the Binary Image" << endl;
		intensity = 255;
	}
	else if (flag == "Erode")
	{
		cout << "Eroding the Binary Image" << endl;
		intensity = 0;
	}

	for (int i = 0; i < inputImage.rows; i++)
	{
		for (int j = 0; j < inputImage.cols; j++)
		{
			if (inputImage.at <uchar>(i, j) == intensity)
			{
				outputImage.at<uchar>(i, j) = intensity;
				if (i > 0)
					outputImage.at<uchar>(i - 1, j) = intensity;
				if (j > 0)
					outputImage.at<uchar>(i, j - 1) = intensity;
				if (i > 0 && j > 0)
					outputImage.at<uchar>(i - 1, j - 1) = intensity;
				if ((i + 1) < inputImage.rows)
					outputImage.at<uchar>(i + 1, j) = intensity;
				if ((j + 1) < inputImage.cols)
					outputImage.at<uchar>(i, j + 1) = intensity;
				if ((i + 1) < inputImage.rows && (j + 1) < inputImage.cols)
					outputImage.at<uchar>(i + 1, j + 1) = intensity;
				if (i > 0 && (j + 1) < inputImage.cols)
					outputImage.at<uchar>(i - 1, j + 1) = intensity;
				if ((i + 1) < inputImage.rows && j > 0)
					outputImage.at<uchar>(i + 1, j - 1) = intensity;
			}
		}
	}
	return outputImage;
}


/*Function to create a square 1x2 structuring element
The function checks for the flag if it is Dilate or Erode.
In either case it assigns apppropriate value to the intensity
and creates an output image.*/

Mat cellType1(Mat inputImage, string flag)
{
	Mat outputImage = inputImage.clone();
	int intensity = 0;
	if (flag == "Dilate")
	{
		cout << "Dilating the Binary Image" << endl;
		intensity = 255;
	}
	else if (flag == "Erode")
	{
		cout << "Eroding the Binary Image" << endl;
		intensity = 0;
	}

	for (int i = 0; i < inputImage.rows; i++)
	{
		for (int j = 0; j < inputImage.cols; j++)
		{
			if (inputImage.at <uchar>(i, j) == intensity)
			{
				outputImage.at<uchar>(i, j) = intensity;
				if ((j + 1) < inputImage.cols)
					outputImage.at<uchar>(i, j + 1) = intensity;
			}
		}
	}
	return outputImage;
}

/*Function to create binary image from an input grayscale image.
Thresholding is done with respect to BINARY_THRESHOLD.*/
Mat createBinaryImage(Mat inputImage)
{
	Mat binaryImage = inputImage.clone();

	for (int i = 0; i < inputImage.rows; i++)
	{
		for (int j = 0; j < inputImage.cols; j++)
		{
			if (inputImage.at<uchar>(i, j) >= BINARY_THRESHOLD)
				binaryImage.at<uchar>(i, j) = saturate_cast<uchar>(255);
			else
				binaryImage.at<uchar>(i, j) = saturate_cast<uchar>(0);
		}
	}
	return binaryImage;
}

/*A compound function to handle the cell type option chosen by the user.
This function in turn calls function cellType1, cellType2, cellType3.
In case of 9x9 square structuring element the funtion cellType2 is called 4 times.
similarly, in case of 15x15 square structuring element the funtion cellType2 is called 7 times.*/

Mat callStructure(Mat binaryImage, string flag, int cellOption)
{
	Mat outputImage;
	switch (cellOption)
	{
	case 1:
		cout << "Structure Type " << cellOption << endl;
		outputImage = cellType1(binaryImage, flag);
		break;
	case 2:
		cout << "Structure Type " << cellOption << endl;
		outputImage = cellType3(binaryImage, flag);
		break;
	case 3:
		cout << "Structure Type " << cellOption << endl;
		outputImage = cellType2(binaryImage, flag);
		break;
	case 4:
		cout << "Structure Type " << cellOption << endl;
		for (int k = 0; k < 4; k++) {
			outputImage = cellType2(binaryImage, flag);
			binaryImage = outputImage;
		}
		break;
	case 5:
		cout << "Structure Type " << cellOption << endl;
		for (int k = 0; k < 7; k++) {
			outputImage = cellType2(binaryImage, flag);
			binaryImage = outputImage;
		}
		break;
	default:
		cout << "Structure Type Not Found" << endl;
	}
	return outputImage;
}

/*Funtion to read an image using the file name given by the user*/
Mat readImage(string& fileName, string type)
{
	cout << endl << "Please Select " << type << " Image." << endl;
	cout << "Example <ricegrains.bmp> " << endl;

	cin >> fileName;

	cout << "File Selected: " << fileName << endl;
	Mat inputImage = imread(fileName, 0);

	return inputImage;
}


int main() {

	int option;
	int cellOption;
	string fileName;
	char exit_key;

	cout << "Assignment 5: Morphological Operations" << endl;
	cout << "Select an Image " << endl;

	Mat inputImage = readImage(fileName, "Input");
	if (inputImage.empty()) {
		cerr << "Error: Loading image" << endl;
		char c = getchar();
		return -1;
	}
	do{
		Mat binaryImage = createBinaryImage(inputImage);

		cout << "Select the following options" << endl;
		cout << "   1.  Erode Binary" << endl;
		cout << "   2.  Dilate Binary" << endl;
		cout << "   3.  Open Binary" << endl;
		cout << "   4.  Close Binary" << endl;
		cin >> option;

		cout << "Please enter the type of stuctural Element" << endl;
		cout << "   1. A rectangle with of 1x2" << endl;
		cout << "   2. A diamond with all 1 3x3" << endl;
		cout << "   3. A Square with all 1 3x3" << endl;
		cout << "   4. A Square with all 1 9x9" << endl;
		cout << "   5. A Square with all 1 15x15" << endl;
		cin >> cellOption;

		namedWindow("Input Image");
		imshow("Input Image", inputImage);

		namedWindow("Input Binary Image");
		imshow("Input Binary Image", binaryImage);

		Mat outputImage;
		Mat outputImage_2;

		switch (option)
		{
		case 1:
			cout << "Image Erode Binary" << endl;
			outputImage = callStructure(binaryImage, "Erode", cellOption);
			namedWindow("Output Image");
			imshow("Output Image", outputImage);
			break;
		case 2:
			cout << "Image Dilate Binary" << endl;
			outputImage = callStructure(binaryImage, "Dilate", cellOption);
			namedWindow("Output Image");
			imshow("Output Image", outputImage);
			break;
		case 3:
			cout << "Image Open Binary" << endl;
			outputImage = callStructure(binaryImage, "Erode", cellOption);
			outputImage_2 = callStructure(outputImage, "Dilate", cellOption);
			namedWindow("Output Eroded Image");
			imshow("Output Eroded Image", outputImage);
			namedWindow("Output Open Image");
			imshow("Output Open Image", outputImage_2);
			break;
		case 4:
			cout << "Image  Close Binary" << endl;
			outputImage = callStructure(binaryImage, "Dilate", cellOption);
			outputImage_2 = callStructure(outputImage, "Erode", cellOption);
			namedWindow("Output Dilated Image");
			imshow("Output Dilated Image", outputImage);
			namedWindow("Output Close Image");
			imshow("Output Close Image", outputImage_2);
			break;
		default:
			cout << "Option not found!!" << endl;
			char c = getchar();
			return -1;
		}
		waitKey();
			cout << "\nWould you like to exit? (y/n):";
			cin >> exit_key;
			cout << endl << endl;
	} while (exit_key == 'n');

	return 0;

}

