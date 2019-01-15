#pragma once
#include "opencv2\core\core.hpp"

class HSVColors
{
private:
	int lowH = 0, highH = 179; // in openCV Hue is only in range 0-180 degrees
	int lowS = 0, highS = 255;
	int lowV = 0, highV = 255;

public:
	HSVColors();
	HSVColors(int hLow, int hHigh, int sLow, int sHigh, int vLow, int vHigh);
	~HSVColors();

	void controlPanel();
	void setLowHSV(int h, int s, int v);
	void setHighHSV(int h, int s, int v);
	
	
	cv::Scalar getLowScalar()
	{
		return cv::Scalar(lowH, lowS, lowV);
	}

	cv::Scalar getHighScalar()
	{
		return cv::Scalar(highH, highS, highV);
	}
};

