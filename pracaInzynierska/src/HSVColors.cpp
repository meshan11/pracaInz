#include "HSVColors.h"
#include "opencv2\highgui\highgui.hpp"
#include <string>

HSVColors::HSVColors()
{
}

HSVColors::HSVColors(int hLow, int hHigh, int sLow, int sHigh, int vLow, int vHigh)
{
	if (hLow > 179)
		hLow = 179;
	if (hHigh > 179)
		hHigh = 179;

	this->lowH = hLow;
	this->lowS = sLow;
	this->lowV = vLow;

	this->highH = hHigh;
	this->highS = sHigh;
	this->highV = vHigh;
}

HSVColors::~HSVColors()
{
}

void HSVColors::controlPanel()
{
	const std::string windowName = "HSV Color Control";
	cv::namedWindow("HSV Color Control");

	cv::createTrackbar("LowHue", windowName, &lowH, 179);
	cv::createTrackbar("HighHue", windowName, &highH, 179);

	cv::createTrackbar("LowSaturation", windowName, &lowS, 255);
	cv::createTrackbar("HighSaturation", windowName, &highS, 255);

	cv::createTrackbar("LowValue", windowName, &lowV, 255);
	cv::createTrackbar("HighValue", windowName, &highV, 255);
}


void HSVColors::setLowHSV(int h, int s, int v)
{
	if (h > 179)
		h = 179;
	this->lowH = h;
	this->lowS = s;
	this->lowV = v;
}

void HSVColors::setHighHSV(int h, int s, int v)
{
	if (h > 179)
		h = 179;
	this->highH = h;
	this->highS = s;
	this->highV = v;
}
