#ifndef COMPONENTLABELING_HPP_
#define COMPONENTLABELING_HPP_

#include "Types/BlobContour.hpp"
#include "Types/Blob.hpp"

#include <vector>

bool ComponentLabeling(	IplImage* inputImage,
						IplImage* maskImage,
						unsigned char backgroundColor,
						Types::Blob_vector &blobs );


void contourTracing( IplImage *image,
					 IplImage *mask,
					 CvPoint contourStart,
					 Types::Blob::t_labelType *labels,
					 bool *visitedPoints,
					 Types::Blob::t_labelType label,
					 bool internalContour,
					 unsigned char backgroundColor,
					 Types::BlobContour *currentBlobContour );

CvPoint tracer( IplImage *image,
				IplImage *mask,
				CvPoint P,
				bool *visitedPoints,
				short initialMovement,
				unsigned char backgroundColor,
				short &movement );


#endif	/* COMPONENTLABELING_HPP_ */
