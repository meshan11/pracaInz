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
#include "Skeleton.h"

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

		int H = output.size[2];
		int W = output.size[3];

		
		// position of body parts
		std::vector<cv::Point> points(nPoints);

		for (int i = 0; i < nPoints; i++)
		{
			// Probability map of corresponding body's part.
			cv::Mat probMap(H, W, CV_32F, output.ptr(0, i));

			cv::Point2f p(-1, -1);
			cv::Point maxLoc;
			double prob = 0.0;
			minMaxLoc(probMap, 0, &prob, 0, &maxLoc);

			if (prob > capture.getTresh() )
			{
				p = maxLoc;
				p.x *= (float)capture.getFrameWidth() / W;
				p.y *= (float)capture.getFrameHeight() / H;
			}
			points[i] = p;
		}
		

		int nPairs = sizeof(POSE_PAIRS) / sizeof(POSE_PAIRS[0]);
		
		//float SX = float(frame.cols) / W;
		//float SY = float(frame.rows) / H;

		// connect body parts and draw it !
		for (int n = 0; n < nPairs; n++)
		{
			// lookup 2 connected body/hand parts
			cv::Point2f partA = points[POSE_PAIRS[n][0]];
			cv::Point2f partB = points[POSE_PAIRS[n][1]];

			if (partA.x <= 0 || partA.y <= 0 || partB.x <= 0 || partB.y <= 0)
				continue;

			// scale to image size
			//partA.x *= SX; partA.y *= SY;
			//partB.x *= SX; partB.y *= SY;

			line(frame, partA, partB, cv::Scalar(0, 255, 255), 8);
			circle(frame, partA, 8, cv::Scalar(0, 0, 255), -1);
			circle(frame, partB, 8, cv::Scalar(0, 0, 255), -1);
		}

		//imshow("Outpu-Keypoints", frameCopy);
		imshow("Output-Skeleton", frame);
		capture.writeFrame(frame);
	}


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