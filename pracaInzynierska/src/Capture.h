#pragma once
#include <string>
#include "opencv2\core.hpp"
#include "opencv2\videoio.hpp"

class Capture
{
private:

	#define INPUT

	#ifdef INPUT
	int videoInput = 0; // camera feed
	#else
	std::string videoInput = "E:\\Studia\\@PD\\Markers22.mp4"; // video feed
	#endif

	const int inWidth= 320, inHeight = 160; // dimensions of the input image for caffee network
	int frameWidth, frameHeight;
	float thresh = 0.01f;
	 
	cv::VideoCapture cap;
	cv::VideoWriter videoWriter;
	cv::Mat frame;
	cv::Mat* framePtr = &frame;
	
	
	int openVideo();
	void setFrameDimension();
	void initVideoWriter();

	


public:
	Capture();
	~Capture();

	cv::Mat& getFrame();
	cv::Size getCaffeeInputDim();
};

