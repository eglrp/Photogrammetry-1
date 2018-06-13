/*
 * KeyFrame.h
 *
 *  Created on: Jun 5, 2018
 *      Author: sujiwo
 */

#ifndef KEYFRAME_H_
#define KEYFRAME_H_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <Eigen/Eigen>
#include <string>
#include <memory>
#include <tuple>

#include "MapPoint.h"


//typedef std::tuple<int64, cv::Point2f, int64, cv::Point2f> FeaturePair;
struct FeaturePair {
	int64 id1;
	cv::Point2f keypoint1;
	int64 id2;
	cv::Point2f keypoint2;
};


class KeyFrame {
public:
	KeyFrame(const std::string &path,
			const Eigen::Vector3d &p, const Eigen::Quaterniond &o,
			cv::Mat &mask,
			cv::Ptr<cv::FeatureDetector> fdetector);
	virtual ~KeyFrame();

	inline std::vector<cv::KeyPoint> getKeypoints()
	{ return keypoints; }

	inline cv::Mat getDescriptors()
	{ return descriptors; }

	static void match (const KeyFrame &k1, const KeyFrame &k2,
		cv::Ptr<cv::DescriptorMatcher> matcher,
		std::vector<FeaturePair> &featurePairs
	);

	static void triangulate (
		KeyFrame &k1, KeyFrame &k2,
		std::vector<MapPoint*> ptsList
	);

	int64 getId () const
	{ return id; }

	static int64 nextId;


private:
	int64 id;
	cv::Mat image;
	std::vector<cv::KeyPoint> keypoints;
	cv::Mat descriptors;

	Eigen::Vector3d position;
	Eigen::Quaterniond orientation;

	KeyFrame* prev;
};

#endif /* KEYFRAME_H_ */
