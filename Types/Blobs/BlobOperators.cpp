/*!
 * \file BlobOperators.hpp
 * \brief Helper classes to perform operations on Blobs - implementation
 * \author Ricard Borràs, Inspecta S.L.
 */

#include <limits.h>

#include "BlobOperators.hpp"

namespace Types {
namespace Blobs {

double BlobGetMoment::operator()(Blob &blob)
{
	return blob.Moment(m_p, m_q);
}

double BlobGetHullPerimeter::operator()(Blob &blob)
{
	CvSeq *convexHull;
	double perimeter;

	convexHull = blob.GetConvexHull();

	if( convexHull )
		perimeter = fabs(cvArcLength(convexHull,CV_WHOLE_SEQ,1));
	else
		return 0;

	cvClearSeq(convexHull);

	return perimeter;
}

double BlobGetHullArea::operator()(Blob &blob)
{
	CvSeq *convexHull;
	double area;

	convexHull = blob.GetConvexHull();

	if( convexHull )
		area = fabs(cvContourArea(convexHull));
	else
		return 0;

	cvClearSeq(convexHull);

	return area;
}

double BlobGetMinXatMinY::operator()(Blob &blob)
{
	double result = LONG_MAX;

	CvSeqReader reader;
	CvPoint actualPoint;
	BlobContour::t_PointList externContour;

	externContour = blob.GetExternalContour()->GetContourPoints();
	if( !externContour ) return result;
	cvStartReadSeq( externContour, &reader);

	for( int i=0; i< externContour->total; i++)
	{
		CV_READ_SEQ_ELEM( actualPoint, reader);

		if( (actualPoint.y == blob.MinY()) && (actualPoint.x < result) )
		{
			result = actualPoint.x;
		}
	}

	return result;
}

double BlobGetMinYatMaxX::operator()(Blob &blob)
{
	double result = LONG_MAX;

	CvSeqReader reader;
	CvPoint actualPoint;
	BlobContour::t_PointList externContour;

	externContour = blob.GetExternalContour()->GetContourPoints();
	if( !externContour ) return result;
	cvStartReadSeq( externContour, &reader);

	for( int i=0; i< externContour->total; i++)
	{
		CV_READ_SEQ_ELEM( actualPoint, reader);

		if( (actualPoint.x == blob.MaxX()) && (actualPoint.y < result) )
		{
			result = actualPoint.y;
		}
	}

	return result;
}

double BlobGetMaxXatMaxY::operator()(Blob &blob)
{
	double result = LONG_MIN;

	CvSeqReader reader;
	CvPoint actualPoint;
	BlobContour::t_PointList externContour;

	externContour = blob.GetExternalContour()->GetContourPoints();
	if( !externContour ) return result;

	cvStartReadSeq( externContour, &reader);

	for( int i=0; i< externContour->total; i++)
	{
		CV_READ_SEQ_ELEM( actualPoint, reader);

		if( (actualPoint.y == blob.MaxY()) && (actualPoint.x > result) )
		{
			result = actualPoint.x;
		}
	}

	return result;
}

double BlobGetMaxYatMinX::operator()(Blob &blob)
{
	double result = LONG_MIN;

	CvSeqReader reader;
	CvPoint actualPoint;
	BlobContour::t_PointList externContour;

	externContour = blob.GetExternalContour()->GetContourPoints();
	if( !externContour ) return result;

	cvStartReadSeq( externContour, &reader);


	for( int i=0; i< externContour->total; i++)
	{
		CV_READ_SEQ_ELEM( actualPoint, reader);

		if( (actualPoint.x == blob.MinX()) && (actualPoint.y > result) )
		{
			result = actualPoint.y;
		}
	}

	return result;
}

double BlobGetElongation::operator()(Blob &blob)
{
	double ampladaC,longitudC,amplada,longitud;

	double tmp;

	tmp = blob.Perimeter()*blob.Perimeter() - 16*blob.Area();

	if( tmp > 0.0 )
		ampladaC = (double) (blob.Perimeter()+sqrt(tmp))/4;
	// error intr�nsec en els c�lculs de l'�rea i el per�metre
	else
		ampladaC = (double) (blob.Perimeter())/4;

	if(ampladaC<=0.0) return 0;
	longitudC=(double) blob.Area()/ampladaC;

	longitud=MAX( longitudC , ampladaC );
	amplada=MIN( longitudC , ampladaC );

	return (double) longitud/amplada;
}

double BlobGetCompactness::operator()(Blob &blob)
{
	if( blob.Area() != 0.0 )
		return (double) pow(blob.Perimeter(),2)/(4*CV_PI*blob.Area());
	else
		return 0.0;
}

double BlobGetRoughness::operator()(Blob &blob)
{
	BlobGetHullPerimeter getHullPerimeter = BlobGetHullPerimeter();

	double hullPerimeter = getHullPerimeter(blob);

	if( hullPerimeter != 0.0 )
		return blob.Perimeter() / hullPerimeter;//HullPerimeter();

	return 0.0;
}

double BlobGetLength::operator()(Blob &blob)
{
	double ampladaC,longitudC;
	double tmp;

	tmp = blob.Perimeter()*blob.Perimeter() - 16*blob.Area();

	if( tmp > 0.0 )
		ampladaC = (double) (blob.Perimeter()+sqrt(tmp))/4;
	// error intr�nsec en els c�lculs de l'�rea i el per�metre
	else
		ampladaC = (double) (blob.Perimeter())/4;

	if(ampladaC<=0.0) return 0;
	longitudC=(double) blob.Area()/ampladaC;

	return MAX( longitudC , ampladaC );
}

double BlobGetBreadth::operator()(Blob &blob)
{
	double ampladaC,longitudC;
	double tmp;

	tmp = blob.Perimeter()*blob.Perimeter() - 16*blob.Area();

	if( tmp > 0.0 )
		ampladaC = (double) (blob.Perimeter()+sqrt(tmp))/4;
	// error intr�nsec en els c�lculs de l'�rea i el per�metre
	else
		ampladaC = (double) (blob.Perimeter())/4;

	if(ampladaC<=0.0) return 0;
	longitudC = (double) blob.Area()/ampladaC;

	return MIN( longitudC , ampladaC );
}

double BlobGetDistanceFromPoint::operator()(Blob &blob)
{
	double xmitjana, ymitjana;
	BlobGetXCenter getXCenter;
	BlobGetYCenter getYCenter;

	xmitjana = m_x - getXCenter( blob );
	ymitjana = m_y - getYCenter( blob );

	return sqrt((xmitjana*xmitjana)+(ymitjana*ymitjana));
}

double BlobGetXYInside::operator()(Blob &blob)
{
	if( blob.GetExternalContour()->GetContourPoints() )
	{
		return cvPointPolygonTest( blob.GetExternalContour()->GetContourPoints(), m_p,0) >= 0;
	}

	return 0;
}

} //: namespace Blobs
} //: namespace Types

