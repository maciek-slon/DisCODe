/*!
 * \file BlobOperators.hpp
 * \brief Helper classes to perform operations on Blobs
 * \author Ricard Borràs, Inspecta S.L.
 */

#ifndef BLOBOPERATORS_HPP_
#define BLOBOPERATORS_HPP_

#include "Blob.hpp"

namespace Types {
namespace Blobs {

#define DEGREE2RAD		(CV_PI / 180.0)

/*!
 * \brief Interface to derive all Blob operations
 */
class BlobOperator
{
public:
	virtual ~BlobOperator()
	{
	}

	//! Apply operator to Blob
	virtual double operator()(Blob &Blob) = 0;

	//! Get operator name
	virtual const char *name() = 0;

	operator BlobOperator*()
	{
		return (BlobOperator*) this;
	}
};

typedef BlobOperator funcio_calculBlob;

/*!
 * \brief Class to get ID of a Blob
 */
class BlobGetID: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		return Blob.GetID();
	}

	const char *name()
	{
		return "BlobGetID";
	}
};

/*!
 * \brief Class to get the area of a Blob
 */
class BlobGetArea: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		return Blob.Area();
	}

	const char *name()
	{
		return "BlobGetArea";
	}
};

/*!
 * \brief Class to get the perimeter of a Blob
 */
class BlobGetPerimeter: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		return Blob.Perimeter();
	}
	const char *name()
	{
		return "BlobGetPerimeter";
	}
};

/*!
 * \brief Class to get the extern flag of a Blob
 */
class BlobGetExterior: public BlobOperator
{
public:
	BlobGetExterior()
	{
		m_mask = NULL;
		m_xBorder = false;
		m_yBorder = false;
	}
	BlobGetExterior(IplImage *mask, bool xBorder = true, bool yBorder = true)
	{
		m_mask = mask;
		m_xBorder = xBorder;
		m_yBorder = yBorder;
	}
	double operator()(Blob &Blob)
	{
		return Blob.Exterior(m_mask, m_xBorder, m_yBorder);
	}
	const char *name()
	{
		return "BlobGetExterior";
	}
private:
	IplImage *m_mask;
	bool m_xBorder, m_yBorder;
};

/*!
 * \brief Class to get the mean grey level of a Blob
 */
class BlobGetMean: public BlobOperator
{
public:
	BlobGetMean()
	{
		m_image = NULL;
	}
	BlobGetMean(IplImage *image)
	{
		m_image = image;
	}

	double operator()(Blob &Blob)
	{
		return Blob.Mean(m_image);
	}
	const char *name()
	{
		return "BlobGetMean";
	}
private:

	IplImage *m_image;
};

/*!
 * \brief Class to get the standard deviation of the grey level values of a Blob
 */
class BlobGetStdDev: public BlobOperator
{
public:
	BlobGetStdDev()
	{
		m_image = NULL;
	}
	BlobGetStdDev(IplImage *image)
	{
		m_image = image;
	}

	double operator()(Blob &Blob)
	{
		return Blob.StdDev(m_image);
	}
	const char *name()
	{
		return "BlobGetStdDev";
	}
private:

	IplImage *m_image;

};

/*!
 * \brief Class to calculate the compactness of a Blob
 */
class BlobGetCompactness: public BlobOperator
{
public:
	double operator()(Blob &Blob);
	const char *name()
	{
		return "BlobGetCompactness";
	}
};

/*!
 * \brief Class to calculate the length of a Blob
 * \note The length is an approximation to the real length
 */
class BlobGetLength: public BlobOperator
{
public:
	double operator()(Blob &Blob);
	const char *name()
	{
		return "BlobGetLength";
	}
};

/*!
 * \brief Class to calculate the breadth of a Blob (the smallest axis of the blob)
 * \brief The breadth is an approximation to the real breadth
 */
class BlobGetBreadth: public BlobOperator
{
public:
	double operator()(Blob &Blob);
	const char *name()
	{
		return "BlobGetBreadth";
	}
};

/*!
 * \brief Class to calculate the difference in the blob x
 */
class BlobGetDiffX: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		return Blob.GetBoundingBox().width;
	}
	const char *name()
	{
		return "BlobGetDiffX";
	}
};

/*!
 * \brief Class to calculate the difference in the blob y
 */
class BlobGetDiffY: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		return Blob.GetBoundingBox().height;
	}
	const char *name()
	{
		return "BlobGetDiffY";
	}
};

/*!
 * \brief Class to calculate the P,Q moment of a Blob
 */
class BlobGetMoment: public BlobOperator
{
public:
	//! Standard constructor (gets the 00 moment)
	BlobGetMoment()
	{
		m_p = m_q = 0;
	}

	//! Constructor: gets the PQ moment
	BlobGetMoment(int p, int q)
	{
		m_p = p;
		m_q = q;
	}

	/*!
	 * \brief Returns the pq moment or 0 if the moment it is not implemented
	 * \note Currently, implemented moments up to 3
	 */
	double operator()(Blob &Blob);

	const char *name()
	{
		return "BlobGetMoment";
	}

private:
	//! moment que volem calcular
	int m_p, m_q;
};

/*!
 * \brief Class to calculate the convex hull perimeter of a Blob
 */
class BlobGetHullPerimeter: public BlobOperator
{
public:
	double operator()(Blob &Blob);
	/*!
	 * \brief Returns the convex hull perimeter of the blob or the perimeter if the
	 * blob edges could not be retrieved
	 */
	const char *name()
	{
		return "BlobGetHullPerimeter";
	}
};

/*!
 * \brief Class to calculate the convex hull area of a Blob
 */
class BlobGetHullArea: public BlobOperator
{
public:
	double operator()(Blob &Blob);
	const char *name()
	{
		return "BlobGetHullArea";
	}
};

/*!
 * \brief Class to calculate the minimum x on the minimum y
 */
class BlobGetMinXatMinY: public BlobOperator
{
public:
	double operator()(Blob &Blob);
	const char *name()
	{
		return "BlobGetMinXatMinY";
	}
};

/*!
 * \brief Class to calculate the minimum y on the maximum x
 */
class BlobGetMinYatMaxX: public BlobOperator
{
public:
	double operator()(Blob &Blob);
	const char *name()
	{
		return "BlobGetMinYatMaxX";
	}
};

/*!
 * \brief Class to calculate the maximum x on the maximum y
 */
class BlobGetMaxXatMaxY: public BlobOperator
{
public:
	double operator()(Blob &Blob);
	const char *name()
	{
		return "BlobGetMaxXatMaxY";
	}
};

/*!
 * \brief Class to calculate the maximum y on the minimum y
 */
class BlobGetMaxYatMinX: public BlobOperator
{
public:
	double operator()(Blob &Blob);
	const char *name()
	{
		return "BlobGetMaxYatMinX";
	}
};

/*!
 * \brief Class to get the minimum x
 */
class BlobGetMinX: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		return Blob.MinX();
	}
	const char *name()
	{
		return "BlobGetMinX";
	}
};

/*!
 * \brief Class to get the maximum x
 */
class BlobGetMaxX: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		return Blob.MaxX();
	}
	const char *name()
	{
		return "BlobGetMaxX";
	}
};

/*!
 * \brief Class to get the minimum y
 */
class BlobGetMinY: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		return Blob.MinY();
	}
	const char *name()
	{
		return "BlobGetMinY";
	}
};

/*!
 * \brief Class to get the maximum y
 */
class BlobGetMaxY: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		return Blob.MaxY();
	}
	const char *name()
	{
		return "BlobGetMaxY";
	}
};

/*!
 * \brief Class to calculate the elongation of the Blob (length/breadth)
 */
class BlobGetElongation: public BlobOperator
{
public:
	double operator()(Blob &Blob);
	const char *name()
	{
		return "BlobGetElongation";
	}
};

/*!
 * \brief Class to calculate the roughness of the Blob ( ratio between perimeter and convex hull perimeter)
 */
class BlobGetRoughness: public BlobOperator
{
public:
	double operator()(Blob &Blob);
	const char *name()
	{
		return "BlobGetRoughness";
	}
};

/*!
 * \brief Class to calculate the euclidean distance between the center of a Blob and a given point
 */
class BlobGetDistanceFromPoint: public BlobOperator
{
public:
	//! Standard constructor (distance to point 0,0)
	BlobGetDistanceFromPoint()
	{
		m_x = m_y = 0.0;
	}
	//! Constructor (distance to point x,y)
	BlobGetDistanceFromPoint(const double x, const double y)
	{
		m_x = x;
		m_y = y;
	}

	double operator()(Blob &Blob);
	const char *name()
	{
		return "BlobGetDistanceFromPoint";
	}

private:
	// coordenades del punt on volem calcular la dist�ncia
	double m_x, m_y;
};

/*!
 * \brief Class to get the number of extern pixels of a Blob
 */
class BlobGetExternPerimeter: public BlobOperator
{
public:
	BlobGetExternPerimeter()
	{
		m_mask = NULL;
		m_xBorder = false;
		m_yBorder = false;
	}
	BlobGetExternPerimeter(IplImage *mask, bool xBorder = true, bool yBorder = true)
	{
		m_mask = mask;
		m_xBorder = xBorder;
		m_yBorder = yBorder;
	}
	double operator()(Blob &Blob)
	{
		return Blob.ExternPerimeter(m_mask, m_xBorder, m_yBorder);
	}
	const char *name()
	{
		return "BlobGetExternPerimeter";
	}
private:
	IplImage *m_mask;
	bool m_xBorder, m_yBorder;
};

/*!
 * \brief Class to calculate the ratio between the perimeter and the number of extern pixels
 */
class BlobGetExternPerimeterRatio: public BlobOperator
{
public:
	BlobGetExternPerimeterRatio()
	{
		m_mask = NULL;
		m_xBorder = false;
		m_yBorder = false;
	}
	BlobGetExternPerimeterRatio(IplImage *mask, bool xBorder = true, bool yBorder = true)
	{
		m_mask = mask;
		m_xBorder = xBorder;
		m_yBorder = yBorder;
	}
	double operator()(Blob &Blob)
	{
		if (Blob.Perimeter() != 0)
			return Blob.ExternPerimeter(m_mask, m_xBorder, m_yBorder) / Blob.Perimeter();
		else
			return Blob.ExternPerimeter(m_mask, m_xBorder, m_yBorder);
	}
	const char *name()
	{
		return "BlobGetExternPerimeterRatio";
	}
private:
	IplImage *m_mask;
	bool m_xBorder, m_yBorder;
};

/*!
 * \brief Class to calculate the ratio between the perimeter and the number of extern pixels
 */
class BlobGetExternHullPerimeterRatio: public BlobOperator
{
public:
	BlobGetExternHullPerimeterRatio()
	{
		m_mask = NULL;
		m_xBorder = false;
		m_yBorder = false;
	}
	BlobGetExternHullPerimeterRatio(IplImage *mask, bool xBorder = true, bool yBorder = true)
	{
		m_mask = mask;
		m_xBorder = xBorder;
		m_yBorder = yBorder;
	}
	double operator()(Blob &Blob)
	{
		BlobGetHullPerimeter getHullPerimeter;
		double hullPerimeter;

		if ((hullPerimeter = getHullPerimeter(Blob)) != 0)
			return Blob.ExternPerimeter(m_mask, m_xBorder, m_yBorder) / hullPerimeter;
		else
			return Blob.ExternPerimeter(m_mask, m_xBorder, m_yBorder);
	}
	const char *name()
	{
		return "BlobGetExternHullPerimeterRatio";
	}
private:
	IplImage *m_mask;
	bool m_xBorder, m_yBorder;

};

/*!
 * \brief Class to calculate the center in the X direction
 */
class BlobGetXCenter: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		return Blob.MinX() + ((Blob.MaxX() - Blob.MinX()) / 2.0);
	}
	const char *name()
	{
		return "BlobGetXCenter";
	}
};

/*!
 * \brief Class to calculate the center in the Y direction
 */
class BlobGetYCenter: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		return Blob.MinY() + ((Blob.MaxY() - Blob.MinY()) / 2.0);
	}
	const char *name()
	{
		return "BlobGetYCenter";
	}
};

/*!
 * \brief Class to calculate the length of the major axis of the ellipse that fits the Blob edges
 */
class BlobGetMajorAxisLength: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		CvBox2D elipse = Blob.GetEllipse();

		return elipse.size.width;
	}
	const char *name()
	{
		return "BlobGetMajorAxisLength";
	}
};

/*!
 * \brief Class to calculate the ratio between the area of the
 * bounding ellipse and the area of the blob.
 */
class BlobGetAreaElipseRatio: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		if (Blob.Area() == 0.0)
			return 0.0;

		CvBox2D elipse = Blob.GetEllipse();
		double ratioAreaElipseAreaTaca = ((elipse.size.width / 2.0) * (elipse.size.height / 2.0) * CV_PI) / Blob.Area();

		return ratioAreaElipseAreaTaca;
	}
	const char *name()
	{
		return "BlobGetAreaElipseRatio";
	}
};

/*!
 * \brief Class to calculate the length of the minor axis of the ellipse that fits the Blob edges
 */
class BlobGetMinorAxisLength: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		CvBox2D elipse = Blob.GetEllipse();

		return elipse.size.height;
	}
	const char *name()
	{
		return "BlobGetMinorAxisLength";
	}
};

/*!
 * \brief Class to calculate the orientation of the ellipse that fits the Blob edges in radians
 */
class BlobGetOrientation: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		CvBox2D elipse = Blob.GetEllipse();
		/*
		 if( elipse.angle > 180.0 )
		 return (( elipse.angle - 180.0 )* DEGREE2RAD);
		 else
		 return ( elipse.angle * DEGREE2RAD);
		 */
		return elipse.angle;
	}
	const char *name()
	{
		return "BlobGetOrientation";
	}
};

/*!
 * \brief Class to calculate the cosinus of the orientation of the ellipse that fits the Blob edges
 */
class BlobGetOrientationCos: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		BlobGetOrientation getOrientation;
		return fabs(cos(getOrientation(Blob) * DEGREE2RAD));
	}
	const char *name()
	{
		return "BlobGetOrientationCos";
	}
};

/*!
 * \brief Class to calculate the ratio between both axes of the ellipse
 */
class BlobGetAxisRatio: public BlobOperator
{
public:
	double operator()(Blob &Blob)
	{
		double major, minor;
		BlobGetMajorAxisLength getMajor;
		BlobGetMinorAxisLength getMinor;

		major = getMajor(Blob);
		minor = getMinor(Blob);

		if (major != 0)
			return minor / major;
		else
			return 0;
	}
	const char *name()
	{
		return "BlobGetAxisRatio";
	}
};

/*!
 * \brief Class to calculate whether a point is inside a Blob
 */
class BlobGetXYInside: public BlobOperator
{
public:

	//! Standard constructor
	BlobGetXYInside()
	{
		m_p.x = 0;
		m_p.y = 0;
	}

	//! Constructor: sets the point
	BlobGetXYInside(CvPoint2D32f p)
	{
		m_p = p;
	}
	;
	double operator()(Blob &Blob);
	const char *name()
	{
		return "BlobGetXYInside";
	}

private:
	//! point to be considered
	CvPoint2D32f m_p;
};

} //: namespace Blobs
} //: namespace Types

#endif	/* BLOBOPERATORS_HPP_ */
