/*!
 * \file
 * \brief
 * \author Tomek Kornuta,,,
 */

#include <memory>
#include <string>

#include "CvSIFT.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvSIFT {

CvSIFT::CvSIFT(const std::string & name) :
		Base::Component(name)  {

}

CvSIFT::~CvSIFT() {
}

void CvSIFT::prepareInterface() {
	// Register handlers with their dependencies.
	h_onNewImage.setup(this, &CvSIFT::onNewImage);
	registerHandler("onNewImage", &h_onNewImage);
	addDependency("onNewImage", &in_img);

	// Input and output data streams.
	registerStream("in_img", &in_img);
	registerStream("out_features", &out_features);
	registerStream("out_descriptors", &out_descriptors);
}

bool CvSIFT::onInit() {

	return true;
}

bool CvSIFT::onFinish() {
	return true;
}

bool CvSIFT::onStop() {
	return true;
}

bool CvSIFT::onStart() {
	return true;
}

void CvSIFT::onNewImage()
{
	LOG(LTRACE) << "CvSIFT::onNewImage\n";
	try {
		// Input: a grayscale image.
		cv::Mat input = in_img.read();

		//-- Step 1: Detect the keypoints.
	    cv::SiftFeatureDetector detector;
	    std::vector<cv::KeyPoint> keypoints;
	    detector.detect(input, keypoints);

		//-- Step 2: Calculate descriptors (feature vectors).
		cv::SiftDescriptorExtractor extractor;
		Mat descriptors;
		extractor.compute( input, keypoints, descriptors);

		// Write results to outputs.
	    Types::Features features(keypoints);
		out_features.write(features);
		out_descriptors.write(descriptors);
	} catch (...) {
		LOG(LERROR) << "CvSIFT::onNewImage failed\n";
	}
}


} //: namespace CvSIFT
} //: namespace Processors
