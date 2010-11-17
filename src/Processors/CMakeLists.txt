SET(CMAKE_INCLUDE_CURRENT_DIR ON)

MESSAGE(STATUS "Building processors:")

ADD_PROCESSOR(Example)
ADD_PROCESSOR(BlobExtractor)
ADD_PROCESSOR(CvThreshold)
ADD_PROCESSOR(CvColorConv)
ADD_PROCESSOR(CvGaussianBlur)
ADD_PROCESSOR(CvSplit)
ADD_PROCESSOR(CvMorphology)
ADD_PROCESSOR(CvFilter2D)
ADD_PROCESSOR(CvPixelOperator)
ADD_PROCESSOR(ExecTimer)
ADD_PROCESSOR(Stereo)

# Task specific processors
ADD_PROCESSOR(MS_Sign_LUT)
ADD_PROCESSOR(MS_Sign_Decide)

ADD_PROCESSOR(MS_Barcode_Macro)
ADD_PROCESSOR(MS_Barcode_Prepare)
ADD_PROCESSOR(MS_Barcode_Decide)

ADD_PROCESSOR(CvFindChessboardCorners)
ADD_PROCESSOR(CvSolvePnP)
ADD_PROCESSOR(CvUndistort)
ADD_PROCESSOR(GrayClassification)

ADD_PROCESSOR(KW_Palm_LUT)
ADD_PROCESSOR(KW_Skin)
ADD_PROCESSOR(KW_Cov)
