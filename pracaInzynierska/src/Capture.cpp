#include "Capture.h"
#include <iostream>


Capture::Capture()
{
	openVideo();
	setFrameDimension();
	initVideoWriter();
}


Capture::~Capture()
{
	cap.release();
	videoWriter.release();
}

cv::Mat& Capture::getFrame()
{
	cap.read(frame);
	return frame;
}

cv::Size Capture::getCaffeeInputDim()
{
	return cv::Size(inWidth, inHeight);
}

int Capture::openVideo()
{
	cap.open(videoInput);

	if (!cap.isOpened())
	{
		std::cerr << "Cannot open file " + videoInput << std::endl;
		return 0;
	}
	return 1;
}

void Capture::setFrameDimension()
{
	if (cap.isOpened())
	{
		frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
		frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	}
	
}

void Capture::initVideoWriter()
{
	videoWriter.open("Output-Skeleton.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, cv::Size(frameWidth, frameHeight));
}

void Capture::writeFrame(const cv::Mat& frame)
{
	videoWriter.write(frame);
}
