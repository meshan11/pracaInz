#pragma once
#include "opencv2\core\core.hpp"

class CustomTracking
{
private:
	int lastPosX, lastPosY;
	int posX, posY;


public:
	CustomTracking();
	~CustomTracking();

	cv::Point getPoint() 
	{
		return cv::Point(posX, posY);
	}

	cv::Point getLastPoint() 
	{
		return cv::Point(lastPosX, lastPosY);
	}
};

