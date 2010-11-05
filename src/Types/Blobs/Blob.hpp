/*!
 * \file Blob.hpp
 * \brief Blob declaration
 * \author Ricard Borràs, Inspecta S.L.
 * \date
 */

#ifndef CBLOB_H_
#define CBLOB_H_

#include <cxcore.h>

#include "BlobContour.hpp"
#include "Drawable.hpp"

namespace Types {
namespace Blobs {


//! Type of labelled images
typedef unsigned int t_labelType;

//! Blob class
class Blob : public Drawable {
	typedef std::list<BlobContour> t_contourList;

public:



	Blob();
	Blob( t_labelType id, CvPoint startPoint, CvSize originalImageSize );
	virtual ~Blob();

	//! Copy constructor
	Blob( const Blob &src );
	Blob( const Blob *src );

	//! Operador d'assignaci�
	//! Assigment operator
	Blob& operator=(const Blob &src );

	//! Adds a new internal contour to the blob
	void AddInternalContour( const BlobContour &newContour );

	//! Retrieves contour in Freeman's chain code
	BlobContour *GetExternalContour()
	{
		return &m_externalContour;
	}

	//! Retrieves blob storage
	CvMemStorage *GetStorage()
	{
		return m_storage;
	}

	//! Get label ID
	t_labelType GetID()
	{
		return m_id;
	}

	/*!
	 * \brief Return true for extern blobs
	 * \param mask
	 * \param xBorder true to consider blobs touching horizontal borders as extern
	 * \param yBorder true to consider blobs touching vertical borders as extern
	 * \return
	 */
	int Exterior( IplImage *mask, bool xBorder = true, bool yBorder = true );

	//! Compute blob's area
	double Area();

	/*!
	 * \brief Get blob perimeter, ie. sum of the length of all the contours
	 * \return perimeter length
	 */
	double Perimeter();

	//! Compute blob's moment (p,q up to MAX_CALCULATED_MOMENTS)
	double Moment(int p, int q);

	/*!
	 * \brief Get extern perimeter (perimeter touching image borders)
	 *
	 * \note If BlobContour::GetContourPoints aproximates contours with a method different that NONE,
	 * this function will not give correct results
	 *
	 * \param mask if != NULL, counts maskImage black pixels as external pixels and contour points touching
	 * them are counted as external contour points.
	 * \param xBorder true to consider blobs touching horizontal borders as extern
	 * \param yBorder true to consider blobs touching vertical borders as extern
	 * \return
	 */
	double ExternPerimeter( IplImage *mask, bool xBorder  = true, bool yBorder = true );

	/*!
	 * \brief Get blob mean gray color in input image.
	 *
	 * This method calculates also standard deviation, which can be retrieved by StdDev method.
	 *
	 * \param image image from gray color are extracted
	 * \return mean color
	 */
	double Mean( IplImage *image );

	/*!
	 * \brief Get standard deviation grey color.
	 */
	double StdDev( IplImage *image );

	/*!
	 * \brief Shows if the blob has associated information
	 */
	bool IsEmpty();

	/*!
	 * \brief Calculates the convex hull polygon of the blob
	 * \return list of convex hull points
	 */
	BlobContour::t_PointList GetConvexHull();

	/*!
	 * \brief Fills the blob with a specified colour
	 * \param image where to paint
	 * \param color color to paint the blob
	 * \param offsetX
	 * \param offsetY
	 * \post modifies input image and returns the seed point used to fill the blob ??
	 */
	void draw( cv::Mat & image, CvScalar color, int offsetX = 0, int offsetY = 0 );

	/*!
	 * \brief Adds external contour to current external contour
	 * \param blob blob from which extract the added external contour
	 */
	void JoinBlob( Blob *blob );

	/*!
	 * \brief Get bounding box (without rotation) of a blob
	 * @return bounding box
	 */
	CvRect GetBoundingBox();

	/*!
	 * \brief Calculates bounding ellipse of external contour points
	 * \note Calculation is made using second order moment aproximation
	 * \return bounding ellipse
	 */
	CvBox2D GetEllipse();

	//! Minimum X
	double MinX()
	{
		return GetBoundingBox().x;
	}
	//! Minimum Y
	double MinY()
	{
		return GetBoundingBox().y;
	}
	//! Maximum X
	double MaxX()
	{
		return GetBoundingBox().x + GetBoundingBox().width;
	}
	//! Maximum Y
	double MaxY()
	{
		return GetBoundingBox().y + GetBoundingBox().height;
	}
private:

	//! Deallocates all contours
	void ClearContours();
	//////////////////////////////////////////////////////////////////////////
	// Blob contours
	//////////////////////////////////////////////////////////////////////////


	//! Contour storage memory
	CvMemStorage *m_storage;
	//! External contour of the blob (crack codes)
	BlobContour m_externalContour;
	//! Internal contours (crack codes)
	t_contourList m_internalContours;

	//////////////////////////////////////////////////////////////////////////
	// Blob features
	//////////////////////////////////////////////////////////////////////////

	//! Label number
	t_labelType m_id;
	//! Area
	double m_area;
	//! Perimeter
	double m_perimeter;
	//! Extern perimeter from blob
	double m_externPerimeter;
	//! Mean gray color
	double m_meanGray;
	//! Standard deviation from gray color blob distribution
	double m_stdDevGray;
	//! Bounding box
	CvRect m_boundingBox;
	//! Bounding ellipse
	CvBox2D m_ellipse;
	//! Sizes from image where blob is extracted
	CvSize m_originalImageSize;
};


//! vector of blobs
typedef std::vector<Blob*>	Blob_vector;

} //: namespace Blobs
} //: namespace Types

#endif /* CBLOB_H_ */
