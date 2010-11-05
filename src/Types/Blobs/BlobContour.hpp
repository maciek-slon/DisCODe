/*!
 * \file BlobContour.hpp
 * \brief BlobContour declaration
 * \author Ricard Borràs, Inspecta S.L.
 * \date
 */

#ifndef BLOBCONTOUR_H_
#define BLOBCONTOUR_H_

#include <list>

#include <cv.h>

namespace Types {
namespace Blobs {

//! Max order of calculated moments
#define MAX_MOMENTS_ORDER		3

//! Blob contour class (in crack code)
class BlobContour
{
	friend class Blob;

public:

	//! Type of chain codes
	typedef unsigned char t_chainCode;
	//! Type of list of chain codes
	typedef CvSeq* t_chainCodeList;
	//! Type of list of points
	typedef CvSeq* t_PointList;


	//! Constructors
	BlobContour();
	BlobContour(CvPoint startPoint, CvMemStorage *storage);
	//! Copy constructor
	BlobContour(BlobContour *source);

	~BlobContour();
	//! Assigment operator
	BlobContour& operator=(const BlobContour &source);

	/*!
	 * \brief Add chain code to contour
	 * \param code chain code to be added
	 */
	void AddChainCode(t_chainCode code);

	/*!
	 * \brief Return freeman chain coded contour
	 * @return
	 */
	t_chainCodeList GetChainCode()
	{
		return m_contour;
	}


	bool IsEmpty()
	{
		return m_contour == NULL || m_contour->total == 0;
	}

	/*!
	 * \brief Calculate contour points from crack codes
	 * @return
	 */
	t_chainCodeList GetContourPoints();

protected:

	CvPoint GetStartPoint() const
	{
		return m_startPoint;
	}

	/*!
	 * \brief Clears chain code contour and points
	 */
	void ResetChainCode();

	/*!
	 * \brief Computes area from chain code
	 * \note Algorithm derived from "Properties of contour codes", G.R. Wilson
	 * \note May give negative areas for clock wise contours
	 * @return
	 */
	double GetArea();

	/*!
	 * \brief Get perimeter from chain code. Diagonals sum sqrt(2) and horizontal and vertical codes 1
	 * \note Algorithm derived from "Methods to estimate area and perimeters of blob-like objects: A comparison", L.Yang
	 * \return perimeter value
	 */
	double GetPerimeter();

	/*!
	 * \brief Get contour moment (p,q up to MAX_CALCULATED_MOMENTS)
	 * @param p
	 * @param q
	 * @return calculated moment
	 */
	double GetMoment(int p, int q);

	//! Crack code list
	t_chainCodeList m_contour;

private:
	//! Starting point of the contour
	CvPoint m_startPoint;
	//! All points from the contour
	t_PointList m_contourPoints;

	//! Computed area from contour
	double m_area;
	//! Computed perimeter from contour
	double m_perimeter;
	//! Computed moments from contour
	CvMoments m_moments;

	//! Pointer to storage
	CvMemStorage *m_parentStorage;
};

} //: namespace Blobs
} //: namespace Types

#endif	/* BLOBCONTOUR_H_ */
