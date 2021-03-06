#include "ResizeProcess.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <glog/logging.h>

ResizeProcess::ResizeProcess(size_t height, bool forceIntergerScaling)
	: d_height(height)
	, d_initialized(false)
	, d_forceIntegerScaling(forceIntergerScaling) {
}

ResizeProcess::~ResizeProcess() {}


std::vector<ProcessFunction> ResizeProcess::Prepare(size_t nbProcess, const cv::Size & size) {
	if (d_initialized == false) {
		if (d_forceIntegerScaling == true ) {
			d_height = std::round(size.height / std::round(size.height/d_height));
			if ( d_height % 2 == 1 ) {
				--d_height;
			}
		}

		size_t width = (size.width * d_height) / size.height ;
		if ( width %2 == 1 ) {
			--width;
		}
		d_resized = cv::Mat(d_height,width,CV_8U);
		d_initialized = true;
	}

	return {
		[this](const Frame::Ptr & frame,
		       const cv::Mat & upstream,
		       fort::hermes::FrameReadout & readout,
		       cv::Mat & result) {
			if ( upstream.size() == cv::Size(0,0) ) {
				cv::resize(frame->ToCV(),d_resized,d_resized.size());
			} else {
				cv::resize(upstream,d_resized,d_resized.size());
			}

			cv::Scalar mean,dev;
			cv::meanStdDev(d_resized,mean,dev);
			DLOG(INFO) << "Mean is now : " << mean;
			cv::cvtColor(d_resized,result,cv::COLOR_GRAY2RGB);
		}
	};
}
