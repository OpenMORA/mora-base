
#include <mrpt/utils/CImage.h>

// OPENCV HEADERS
#define CV_NO_CVV_IMAGE // Avoid CImage name crash

#if MRPT_OPENCV_VERSION_NUM>=0x211
	#include <opencv2/core/core.hpp>
	#include <opencv2/objdetect/objdetect.hpp>
 	#include <opencv2/highgui/highgui.hpp>
 	#include <opencv2/imgproc/imgproc.hpp>
 	#include <opencv2/imgproc/imgproc_c.h>
	#include <opencv2/calib3d/calib3d.hpp>
	#include <opencv2/video/tracking.hpp>
#else
 	// For OpenCV <=2.1
  	#include <cv.h>
	#include <highgui.h>
  	#include <cvaux.h>
#endif
  
#ifdef CImage	// For old OpenCV versions (<=1.0.0)
#undef CImage
#endif
using mrpt::utils::CImage;
