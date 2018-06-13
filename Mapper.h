/*
 * Mapper.h
 *
 *  Created on: Jun 5, 2018
 *      Author: sujiwo
 */

#ifndef MAPPER_H_
#define MAPPER_H_

#include <string>
#include <vector>
#include <memory>
#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>

#include "KeyFrame.h"
#include "MapPoint.h"


using namespace Eigen;
using std::vector;


struct DataItem {
	std::string imagePath;
	Vector3d position;
	Quaterniond orientation;
};

struct CameraPinholeParamsRead {
	double
		fx, fy,
		cx, cy;
	int width, height;
	Eigen::Matrix<double,3,4> toMatrix();
};


class Mapper
{
public:

	Mapper(const std::string &datasetDir);
	virtual ~Mapper();

	const CameraPinholeParamsRead& getCameraParams()
	{ return cparams; }

	bool run ();

private:
	std::vector<DataItem> dataset;
	CameraPinholeParamsRead cparams;

	cv::Mat mask;

//	Feature detector, descriptor and matcher
	cv::Ptr<cv::ORB> featureDetector;
	cv::Ptr<cv::BFMatcher> bfMatch;

	// Map Objects
	vector<KeyFrame*> frameList;
	vector<MapPoint*> pointList;

	void buildKeyFrames();

};

#endif /* MAPPER_H_ */
