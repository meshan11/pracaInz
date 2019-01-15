#include "opencv2\videoio.hpp"
#include "opencv2\video\video.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\imgcodecs\imgcodecs.hpp"
#include "opencv2\dnn\dnn.hpp"
#include "opencv2\core.hpp"

#include "HSVColors.h"
#include "Capture.h"
#include "Timer.h"

#include <iostream>



#define MPI // MPI or COCO caffee model

#ifdef MPI
const int POSE_PAIRS[14][2] =
{
	{0,1}, {1,2}, {2,3},
	{3,4}, {1,5}, {5,6},
	{6,7}, {1,14}, {14,8},
	{8,9}, {9,10}, {14,11},
	{11,12}, {12,13}
};

std::string protoFile = "pose/mpi/pose_deploy_linevec_faster_4_stages.prototxt";
std::string weightsFile = "pose/mpi/pose_iter_160000.caffemodel";

int nPoints = 15;
#endif

#ifdef COCO
const int POSE_PAIRS[17][2] =
{
	{ 1,2 },{ 1,5 },{ 2,3 },
	{ 3,4 },{ 5,6 },{ 6,7 },
	{ 1,8 },{ 8,9 },{ 9,10 },
	{ 1,11 },{ 11,12 },{ 12,13 },
	{ 1,0 },{ 0,14 },
	{ 14,16 },{ 0,15 },{ 15,17 }
};

std::string protoFile = "pose/coco/pose_deploy_linevec.prototxt";
std::string weightsFile = "pose/coco/pose_iter_440000.caffemodel";

int nPoints = 18;
#endif



void morphologicalOperation(cv::Mat &image);
void backgroundSubtraction();


int main(int argc, char** argv)
{
	using namespace cv::dnn;

	Capture capture = Capture();

	Net net = readNetFromCaffe(protoFile, weightsFile); // read the caffee model

	while (cv::waitKey(1) < 0)
	{
		Timer();

		cv::Mat& frame = capture.getFrame();
		if (frame.empty())
			break;

		cv::Mat inputBlob = blobFromImage(frame, 1.0 / 255, capture.getCaffeeInputDim(), cv::Scalar(0, 0, 0), false, false);
		net.setInput(inputBlob);

		cv::Mat output = net.forward();
	}
	
	
	/*

	Capture capture;
	capture.openVideo();

	Mat frame, frameCopy;
	int frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int frameHeight = cap.get(CAP_PROP_FRAME_HEIGHT);

	VideoWriter video("Output-Skeleton.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(frameWidth, frameHeight));

	Net net = readNetFromCaffe(protoFile, weightsFile);
	//net.setPreferableTarget(DNN_TARGET_OPENCL);
	//net.setPreferableBackend(DNN_BACKEND_HALIDE);

	while (waitKey(1) < 0)
	{
		
		double t = (double)cv::getTickCount();

		cap >> frame;
		frameCopy = frame.clone();

		Mat inpBlob = blobFromImage(frame, 1.0 / 255, Size(inWidth, inHeight), Scalar(0, 0, 0), true, false);
		
		net.setInput(inpBlob);

		Mat output = net.forward();

		int H = output.size[2];
		int W = output.size[3];

		// position of body parts
		std::vector<Point> points(nPoints);
		for (int i = 0; i < nPoints; i++)
		{
			// Probability map of corresponding body's part.
			Mat probMap(H, W, CV_32F, output.ptr(0, i));

			Point2f p(-1, -1);
			Point maxLoc;
			double prob = 0.0;
			minMaxLoc(probMap, 0, &prob, 0, &maxLoc);

			if (prob > thresh)
			{
				p = maxLoc;
				p.x *= (float)frameWidth / W;
				//p.y *= (float)frameHeight / H;

				circle(frameCopy, cv::Point((int)p.x, (int)p.y), 8, Scalar(0, 255, 255), -1);
				cv::putText(frameCopy, cv::format("%d", i), cv::Point((int)p.x, (int)p.y), cv::FONT_HERSHEY_COMPLEX, 1.1, cv::Scalar(0, 0, 255), 2);
			}
			points[i] = p;
		}

		int nPairs = sizeof(POSE_PAIRS) / sizeof(POSE_PAIRS[0]);

		// connect body parts and draw it !
		//float SX = float(frame.cols) / W;
		//float SY = float(frame.rows) / H;

		for (int n = 0; n < nPairs; n++)
		{
			// lookup 2 connected body/hand parts
			Point2f partA = points[POSE_PAIRS[n][0]];
			Point2f partB = points[POSE_PAIRS[n][1]];

			if (partA.x <= 0 || partA.y <= 0 || partB.x <= 0 || partB.y <= 0)
				continue;

			// scale to image size
			//partA.x *= SX; partA.y *= SY;
			//partB.x *= SX; partB.y *= SY;

			line(frame, partA, partB, Scalar(0, 255, 255), 8);
			circle(frame, partA, 8, Scalar(0, 0, 255), -1);
			circle(frame, partB, 8, Scalar(0, 0, 255), -1);
		}

		t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
		cv::putText(frame, cv::format("time taken = %.2f sec", t), cv::Point(50, 50), cv::FONT_HERSHEY_COMPLEX, .8, cv::Scalar(255, 50, 0), 2);
		//imshow("Outpu-Keypoints", frameCopy);
		imshow("Output-Skeleton", frame);
		video.write(frame);
	}


	cap.release();
	video.release();

	return 0;
	*/
	std::cin.get();
}

void morphologicalOperation(cv::Mat &image)
{
	cv::Mat structElement = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));

	erode(image, image, structElement);

	//erode(image, image, structElement);
	dilate(image, image, structElement);

	//dilate(image, image, structElement);
	//erode(imgTreshold, imgTreshold, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
}

void backgroundSubtraction()
{
	/*
	Ptr<BackgroundSubtractor> pMOG2;
	//pMOG2 = bgsegm::createBackgroundSubtractorMOG();
	pMOG2 = createBackgroundSubtractorKNN();

	VideoCapture cam = VideoCapture(0);

	if (!cam.isOpened())
	{
		cout << "Camera did not open!";
	}
		

	Mat frame;
	Mat fgMaskMOG2;
	Mat back;

	vector<vector<Point> > countours;

	while (true)
	{
		cam >> frame;
		Mat binaryFrame;
		cvtColor(frame, binaryFrame, CV_BGR2GRAY);
		pMOG2->apply(frame, fgMaskMOG2);
		pMOG2->getBackgroundImage(back);

		//erode(fgMaskMOG2, fgMaskMOG2, Mat());
		//erode(fgMaskMOG2, fgMaskMOG2, Mat());
		//dilate(fgMaskMOG2, fgMaskMOG2, Mat());
		//morphologicalOperation(fgMaskMOG2);

		findContours(fgMaskMOG2, countours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		drawContours(frame, countours, -1, Scalar(0,0,255), 2);

		imshow("Frame and contours", frame);
		imshow("Foreground", fgMaskMOG2);
		imshow("Background", back);

		if (waitKey(30) == 27) break; // break the loop, if ESC was pushed
	}

	cam.release();
	*/
}

void allFromMain2()
{
	/*
	if (true)
		backgroundSubtraction();
	else
	{

		VideoCapture cam = VideoCapture(0);

		if (!cam.isOpened())
			cout << "Camera did not open!";

		HSVColors hsvColors = HSVColors(160, 179, 100, 255, 100, 255); // create instance of class
		hsvColors.controlPanel();

		Mat frame;
		Mat hsvFrame;
		Mat tempFrame;
		cam.read(tempFrame);
		Mat drawingFrame = Mat::zeros(tempFrame.size(), CV_8UC3);

		namedWindow("Camera Feed", WINDOW_NORMAL);
		namedWindow("HSV Frame", WINDOW_NORMAL);

		// for tracking purpose
		int iLastX = -1;
		int iLastY = -1;


		while (false)
		{
			cam >> frame;

			cvtColor(frame, hsvFrame, COLOR_BGR2HSV);

			Mat imgTreshold;
			inRange(hsvFrame, hsvColors.getLowScalar(), hsvColors.getHighScalar(), imgTreshold);



			imshow("Camera Feed", frame);
			imshow("HSV Frame", imgTreshold);
			imshow("Track Frame", frame + drawingFrame);

			if (waitKey(30) == 27) break; // break the loop, if ESC was pushed
		}
	}
	destroyAllWindows();
	//return 0;

	*/
}
