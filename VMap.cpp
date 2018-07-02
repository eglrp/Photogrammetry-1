/*
 * Map.cpp
 *
 *  Created on: Jun 13, 2018
 *      Author: sujiwo
 */

#include "VMap.h"
#include "KeyFrame.h"
#include "MapPoint.h"



using namespace Eigen;
using namespace std;


VMap::VMap(
	const cv::Mat &m,
	cv::Ptr<cv::FeatureDetector> fdetect,
	cv::Ptr<cv::DescriptorMatcher> dmatcher) :
		featureDetector(fdetect),
		descriptorMatcher(dmatcher)
{
	mask = m.clone();
}


VMap::~VMap()
{}


Matrix<double,3,4> CameraPinholeParams::toMatrix() const
{
	Matrix<double,3,4> K = Matrix<double,3,4>::Zero();
	K(2,2) = 1.0;
    K(0,0) = fx;
    K(1,1) = fy;
    K(0,2) = cx;
    K(1,2) = cy;
	return K;
}


kfid VMap::createKeyFrame(const cv::Mat &imgSrc,
		const Eigen::Vector3d &p, const Eigen::Quaterniond &o,
		KeyFrame **ptr)
{
	KeyFrame *nKf = new KeyFrame(imgSrc, p, o, mask, featureDetector, &camera);
	kfid nId = nKf->getId();
	keyframeInvIdx.insert(pair<kfid,KeyFrame*> (nId, nKf));

	if (ptr!=NULL)
		*ptr = nKf;
	return nId;
}


mpid VMap::createMapPoint(const Vector3d &p, MapPoint **ptr)
{
	MapPoint *mp = new MapPoint(p);
	mpid nId = mp->getId();
	mappointInvIdx.insert(pair<mpid,MapPoint*> (nId, mp));

	if (ptr!=NULL)
		*ptr = mp;
	return nId;
}


KeyFrame* VMap::getKeyFrameById (const kfid &i) const
{
	return keyframeInvIdx.at(i);
}


void VMap::estimateStructure(const kfid &kfid1, const kfid &kfid2)
{
	const KeyFrame
		*kf1 = getKeyFrameById(kfid1),
		*kf2 = getKeyFrameById(kfid2);

	vector<FeaturePair> featurePairs_1_2;
	KeyFrame::match(*kf1, *kf2, descriptorMatcher, featurePairs_1_2);

	framePoints[kfid1] = map<mpid, kpid>();
	framePoints[kfid2] = map<mpid, kpid>();

	vector<mpid> newMapPointList;
	KeyFrame::triangulate(kf1, kf2, newMapPointList, featurePairs_1_2,
		framePoints[kfid1], framePoints[kfid2],
		this);

	// Update visibility information
	for (mpid &mpidx: newMapPointList) {
		pointAppearances[mpidx].insert(kfid1);
		pointAppearances[mpidx].insert(kfid2);
	}
}


vector<kfid>
VMap::allKeyFrames () const
{
	vector<kfid> kfIdList(keyframeInvIdx.size());
	for (auto &key: keyframeInvIdx) {
		kfIdList.push_back(key.first);
	}
	return kfIdList;
}


vector<mpid>
VMap::allMapPoints () const
{
	vector<mpid> mpIdList(mappointInvIdx.size());
	for (auto &key: mappointInvIdx) {
		mpIdList.push_back(key.first);
	}
	return mpIdList;
}
