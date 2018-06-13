/*
 * KeyFrame.cpp
 *
 *  Created on: Jun 5, 2018
 *      Author: sujiwo
 */

#include "KeyFrame.h"
#include <exception>

using namespace std;


int64 KeyFrame::nextId = 0;


KeyFrame::KeyFrame(const string &path,
		const Eigen::Vector3d &p, const Eigen::Quaterniond &o,
		cv::Mat &mask,
		cv::Ptr<cv::FeatureDetector> fdetector):
	orientation(o),
	position(p),
	prev(NULL),
	id (nextId++)
{
	image = cv::imread(path, cv::IMREAD_GRAYSCALE);
	if (image.empty()==true)
		throw runtime_error("Unable to open image file");

	fdetector->detectAndCompute(image, mask, keypoints, descriptors, false);
}

KeyFrame::~KeyFrame()
{
	// TODO Auto-generated destructor stub
}


void KeyFrame::match(const KeyFrame &k1, const KeyFrame &k2,
	cv::Ptr<cv::DescriptorMatcher> matcher,
	vector<FeaturePair> &featurePairs
)
{
	vector<cv::DMatch> k12matches;
	matcher->match(k1.descriptors, k2.descriptors, k12matches);

	for (auto &m: k12matches) {
		if (m.trainIdx < k1.keypoints.size() and m.queryIdx < k2.keypoints.size()) {
			FeaturePair fp = {k1.id, k1.keypoints[m.trainIdx].pt, k2.id, k2.keypoints[m.queryIdx].pt};
			featurePairs.push_back (fp);
		}
	}
}
