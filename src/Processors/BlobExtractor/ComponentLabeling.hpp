#ifndef COMPONENTLABELING_HPP_
#define COMPONENTLABELING_HPP_

#include "Types/Blobs/BlobContour.hpp"
#include "Types/Blobs/Blob.hpp"

#include <vector>

namespace tb = Types::Blobs;

bool ComponentLabeling(	IplImage* inputImage,
						IplImage* maskImage,
						unsigned char backgroundColor,
						tb::Blob_vector &blobs );


void contourTracing( IplImage *image,
					 IplImage *mask,
					 CvPoint contourStart,
					 tb::t_labelType *labels,
					 bool *visitedPoints,
					 tb::t_labelType label,
					 bool internalContour,
					 unsigned char backgroundColor,
					 tb::BlobContour *currentBlobContour );

CvPoint tracer( IplImage *image,
				IplImage *mask,
				CvPoint P,
				bool *visitedPoints,
				short initialMovement,
				unsigned char backgroundColor,
				short &movement );


#endif	/* COMPONENTLABELING_HPP_ */
