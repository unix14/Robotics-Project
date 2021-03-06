/*
 * HamsterAPIClientSimpleBehaviourExample.cpp
 *
 *  Created on: Aug 10, 2016
 *      Author: ofir
 */

#include <HamsterAPIClientCPP/Hamster.h>
#include <HamsterAPICommon/Common/HamsterError.h>
#include <HamsterAPICommon/Common/Log.h>
#include <HamsterAPICommon/HamsterCommon.h>
#include <HamsterAPICommon/Messages/LidarScan.h>
#include <HamsterAPICommon/Messages/Pose.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/operations.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <stddef.h>
#include <unistd.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace HamsterAPI;
HamsterAPI::Hamster * hamster;

void getScansBetween(double min, double max, std::vector<double> & distances) {
	HamsterAPI::LidarScan scan = hamster->getLidarScan();

	for (size_t i = 0; i < scan.getScanSize(); i++) {
		double degree = scan.getScanAngleIncrement() * i;
		if (degree >= min && degree <= max)
			distances.push_back(scan.getDistance(i));
	}
}

bool willCollide(std::vector<double> distances, int angle_from_center) {
	HamsterAPI::LidarScan scan = hamster->getLidarScan();

	int collisions = 0;

	for (size_t i = distances.size() / 2 - angle_from_center / 2;
			i < distances.size() / 2 + angle_from_center / 2; i++)
		if (distances[i] < scan.getMaxRange() / 4.0)
			collisions++;

	return collisions >= angle_from_center / 4.0;
}

bool isFrontFree() {
	// Degrees : [90, 270]

	std::vector<double> distances;

	getScansBetween(90, 270, distances);

	return !willCollide(distances, 40);
}

bool isLeftFree() {
	// Degrees : [180,360]

	std::vector<double> distances;

	getScansBetween(180, 360, distances);

	return !willCollide(distances, 40);
}

bool isRightFree() {
	// Degrees : [0, 180]

	std::vector<double> distances;

	getScansBetween(0, 180, distances);

	return !willCollide(distances, 40);
}

bool isBackFree() {
	// Degrees : [270,360], [0, 90]

	std::vector<double> distances;

	getScansBetween(270, 360, distances);
	getScansBetween(0, 90, distances);

	return !willCollide(distances, 40);
}

void moveForward() {
	HamsterAPI::Log::i("Client", "Moving Forward");
	hamster->sendSpeed(0.4, 0.0);
}

void turnLeft() {
	HamsterAPI::Log::i("Client", "Turning Left");
	while (!isFrontFree())
		hamster->sendSpeed(0.04, 45.0);
}

void turnRight() {
	HamsterAPI::Log::i("Client", "Turning Right");
	while (!isFrontFree())
		hamster->sendSpeed(0.04, -45.0);
}

void moveBackwards() {
	HamsterAPI::Log::i("Client", "Moving Backwards");
	while (!isLeftFree() && !isRightFree() && isBackFree())
		hamster->sendSpeed(-0.4, 0.0);
	if (isLeftFree())
		turnLeft();
	else
		turnRight();
}

void stopMoving() {
	hamster->sendSpeed(0.0, 0.0);
}

int Oldmain(int argc, char ** argv) {
	try {
		hamster = new HamsterAPI::Hamster(1);
		while (hamster->isConnected()) {
			try {
				if (isFrontFree())
					moveForward();
				else {
					stopMoving();
					if (isLeftFree())
						turnLeft();
					else if (isRightFree())
						turnRight();
					else if (isBackFree())
						moveBackwards();
					else
						HamsterAPI::Log::i("Client", "I am stuck!");
				}

				// Speed Getter
				// HamsterAPI::Speed speed = hamster.getSpeed();
			} catch (const HamsterAPI::HamsterError & message_error) {
				HamsterAPI::Log::i("Client", message_error.what());
			}

		}
	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what());
	}
	return 0;
}

int Oldmain2(int argc, char ** argv) {
	try {
		hamster = new HamsterAPI::Hamster(1);
		/*OccupancyGrid ogrid = hamster->getSLAMMap();
		 cout<< "resolution: "<< ogrid.getResolution()<<endl;
		 cout<< "Width: "<< ogrid.getWidth()<<endl;
		 cout<< "height: "<< ogrid.getHeight()<<endl;
		 */
		while (hamster->isConnected()) {
			try {
				HamsterAPI::LidarScan ld = hamster->getLidarScan();
				if (ld.getDistance(180) < 0.4) {
					hamster->sendSpeed(-0.5, 0);
					cout << "Front: " << ld.getDistance(180) << endl;
				} else if (ld.getDistance(180) < 0.8) {
					hamster->sendSpeed(0.5, 45);
					cout << "Front: " << ld.getDistance(180) << endl;
				}

				else
					hamster->sendSpeed(1.0, 0);
				//cout<<"Front: "<<ld.getDistance(180)<<endl;
				//cout<<"Left: "<<ld.getDistance(90)<<endl;
				//cout<<"Right: "<<ld.getDistance(270)<<endl;
			} catch (const HamsterAPI::HamsterError & message_error) {
				HamsterAPI::Log::i("Client", message_error.what());
			}

		}
	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what());
	}
	return 0;
}

int main() {
	int random;
	srand(time(NULL));


	try {
		hamster = new HamsterAPI::Hamster(1);
		Pose initPose, finPose;
		sleep(1); // letting the hamster connect
		hamster->setInitialPose(initPose);

		while (hamster->isConnected()) {
			try {
				//MOVEMENT
				if (isFrontFree()) {
					hamster->sendSpeed(1.0, 0.0);
				} else if (!isFrontFree()) {
					random = rand() % 2;
					if (random == 1 && isLeftFree())
						hamster->sendSpeed(0.5, 45.0);
					else if (random == 0 && isRightFree())
						hamster->sendSpeed(0.5, -45.0);
					else
						hamster->sendSpeed(-0.2, 20.0);
				}

				//OCCUPANCY GRID
				try {
					sleep(1);
					finPose = hamster->getPose();

				} catch (const HamsterAPI::HamsterError & connection_error) {
					HamsterAPI::Log::i("Client", connection_error.what());
				}
				cv::Mat mat(500, 500, CV_8UC3, cv::Scalar(0, 0, 0));

				// Get laser scan
				HamsterAPI::LidarScan scan = hamster->getLidarScan();
				for (size_t i = 0; i < scan.getScanSize(); i++) {
					double dist = scan.getDistance(i);
					double angle = i * scan.getScanAngleIncrement() - (finPose.getHeading()-initPose.getHeading());

					for (int j = 0; j < dist ; j++) {
						dist=dist-j;
						// Find the position of the obstacle
						double meters_x = dist * cos((angle - 90) * DEG2RAD);
						double meters_y = dist * sin((angle - 90) * DEG2RAD);

						// Convert from meters to pixels
						int pixels_x = (int) (meters_x / 0.05 + 500 / 2);
						int pixels_y = (int) (meters_y / 0.05 + 500 / 2);

						// Draw white pixel at the location of the obstacle
						mat.at<cv::Vec3b>(pixels_x, pixels_y)[0] = 255;
						mat.at<cv::Vec3b>(pixels_x, pixels_y)[1] = 255;
						mat.at<cv::Vec3b>(pixels_x, pixels_y)[2] = 255;
					}
				}

				// Show the matrix on the window
				cv::imshow("Mapping", mat);

				// Delay for 1 millisecond to allow the window to process
				// incoming events
				cv::waitKey(1);

				/*OccupancyGrid ogrid = hamster->getSLAMMap();
				 int width = ogrid.getWidth();
				 int height = ogrid.getHeight();
				 unsigned char pixel;

				 cv::Mat m = cv::Mat(width, height, CV_8UC1);

				 for (int i = 0; i < height; i++)
				 for (int j = 0; j < width; j++) {
				 if (ogrid.getCell(i, j) == CELL_FREE)
				 pixel = 255;
				 else if (ogrid.getCell(i, j) == CELL_OCCUPIED)
				 pixel = 0;
				 else
				 pixel = 128;

				 m.at<unsigned char>(i, j) = pixel;
				 }

				 cv::imshow("OccupancyGrid-view", m);
				 cv::waitKey(1);*/

			} catch (const HamsterAPI::HamsterError & message_error) {
				HamsterAPI::Log::i("Client", message_error.what());
			}

		}
	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what());
	}
	return 0;

}

/*int main(int argc, char ** argv) {
 int random;
 srand(time(NULL));

 try {
 hamster = new HamsterAPI::Hamster(1);
 HamsterAPI::Image cameraImage;
 cv::Mat cam_in; //camera image is converted to matrix
 cv::Mat cam_out; //cam_in is converted to grayscale
 std::vector<cv::Vec3f> circles;
 cv::Mat map;
 HamsterAPI::Pose pose;

 while (hamster->isConnected()) {
 try {
 //Moving
 if (isFrontFree()) {
 hamster->sendSpeed(1.0, 0.0);
 } else if (!isFrontFree()) {
 random = rand() % 2;
 if (random == 1 && isLeftFree())
 hamster->sendSpeed(0.5, 45.0);
 else if (random == 0 && isRightFree())
 hamster->sendSpeed(0.5, -45.0);
 else
 hamster->sendSpeed(-0.2, 20.0);
 }

 //Camera
 cameraImage = hamster->getCameraImage();
 cam_in = cameraImage.toCVMat(); //convert to a matrix
 cv::cvtColor(cam_in, cam_out, CV_BGR2GRAY); //convert to grayscale
 cv::imshow("Hamster Camera", cam_out); //shows image
 cv::waitKey(1);
 cout << "image taken" << endl;

 //Detecting circles
 cv::HoughCircles(cam_out, circles, 3, 1, 10, 100, 30, 1, 40);
 for (size_t i = 0; i < circles.size(); i++) {
 cv::Vec3i c = circles[i];
 cv::circle(cam_in, cv::Point(c[0], c[1]), c[2],
 cv::Scalar(0, 0, 255), 3, 8);
 cv::circle(cam_in, cv::Point(c[0], c[1]), 2,
 cv::Scalar(0, 255, 0), 3, 8);
 }
 cv::imshow("Circles", cam_in);
 cv::waitKey(1);

 //Mapping
 try {
 sleep(1);
 pose = hamster->getPose();
 cout << pose.toString() << endl;
 } catch (const HamsterAPI::HamsterError & connection_error) {
 HamsterAPI::Log::i("Client", connection_error.what());
 }
 cv::Mat mat(500, 500, CV_8UC3, cv::Scalar(0, 0, 0));

 // Get laser scan
 HamsterAPI::LidarScan scan = hamster->getLidarScan();
 for (size_t i = 0; i < scan.getScanSize(); i++) {
 double dist = scan.getDistance(i);
 double angle = i * scan.getScanAngleIncrement();

 // Find the position of the obstacle
 double meters_x = dist * cos((angle - 90) * DEG2RAD);
 double meters_y = dist * sin((angle - 90) * DEG2RAD);

 // Convert from meters to pixels
 int pixels_x = (int) (meters_x / 0.05
 + 500 / 2);
 int pixels_y = (int) (meters_y / 0.05
 + 500 / 2);

 // Draw white pixel at the location of the obstacle
 mat.at<cv::Vec3b>(pixels_x, pixels_y)[0] = 255;
 mat.at<cv::Vec3b>(pixels_x, pixels_y)[1] = 255;
 mat.at<cv::Vec3b>(pixels_x, pixels_y)[2] = 255;
 }

 // Show the matrix on the window
 cv::imshow("Mapping", mat);

 // Delay for 1 millisecond to allow the window to process
 // incoming events
 cv::waitKey(1);

 } catch (const HamsterAPI::HamsterError & message_error) {
 HamsterAPI::Log::i("Client", message_error.what());
 }

 }
 } catch (const HamsterAPI::HamsterError & connection_error) {
 HamsterAPI::Log::i("Client", connection_error.what());
 }
 return 0;
 }*/
