/*!
 * \file BlobResult.hpp
 * \brief Definition of the BlobResult class
 */

#ifndef BLOBRESULT_HPP_
#define BLOBRESULT_HPP_

#include <math.h>
#include <cxcore.h>
#include <vector>		// vectors de la STL

//! Vector of doubles
typedef std::vector<double> double_stl_vector;

#include <functional>

#include "Blob.hpp"
#include "BlobOperators.hpp"

#include "Drawable.hpp"


namespace Types {
namespace Blobs {


/*
 * Filters
 */

//! Actions performed by a filter (include or exclude blobs)
#define B_INCLUDE				1L
#define B_EXCLUDE				2L

//! Conditions to apply the filters
#define B_EQUAL					3L
#define B_NOT_EQUAL				4L
#define B_GREATER				5L
#define B_LESS					6L
#define B_GREATER_OR_EQUAL		7L
#define B_LESS_OR_EQUAL			8L
#define B_INSIDE			    9L
#define B_OUTSIDE			    10L


/*
 * Exceptions
 */

#define EXCEPTION_BLOB_OUT_OF_BOUNDS	1000
#define EXCEPCIO_CALCUL_BLOBS			1001

/*!
 * \brief Class to calculate the blobs of an image and calculate some properties
 * on them. Also, the class provides functions to filter the blobs using
 * some criteria.
 */
class BlobResult : public Drawable {
public:

	//! Standard constructor, it creates an empty set of blobs
	BlobResult();

	//! Image constructor, it creates an object with the blobs of the image
	BlobResult(const Blob_vector & blob_vector);

	//! Copy constructor
	BlobResult( const BlobResult &source );

	//! Destructor
	virtual ~BlobResult();

	//! Assigment operator
	BlobResult& operator=(const BlobResult& source);

	//! Addition operator to concatenate two sets of blobs
	BlobResult operator+( const BlobResult& source ) const;

	//! Adds a blob to the set of blobs
	void AddBlob( Blob *blob );

#ifdef MATRIXCV_ACTIU
	//! Computes some property on all the blobs of the class
	double_vector GetResult( funcio_calculBlob *evaluador ) const;
#endif
	//! Computes some property on all the blobs of the class
	double_stl_vector GetSTLResult( funcio_calculBlob *evaluador ) const;

	//! Computes some property on one blob of the class
	double GetNumber( int indexblob, funcio_calculBlob *evaluador ) const;

	//! Filters the blobs of the class using some property
	void Filter(BlobResult &dst,
				int filterAction, funcio_calculBlob *evaluador,
				int condition, double lowLimit, double highLimit = 0 );
	void Filter(BlobResult &dst,
				int filterAction, funcio_calculBlob *evaluador,
				int condition, double lowLimit, double highLimit = 0 ) const;

	//! Sorts the blobs of the class acording to some criteria and returns the n-th blob
	void GetNthBlob( funcio_calculBlob *criteri, int nBlob, Blob &dst ) const;

	//! Gets the n-th blob of the class ( without sorting )
	Blob GetBlob(int indexblob) const;
	Blob *GetBlob(int indexblob);

	//! Clears all the blobs of the class
	void ClearBlobs();

	//! Escriu els blobs a un fitxer
	//! Prints some features of all the blobs in a file
	void PrintBlobs( char *nom_fitxer ) const;

	void draw(cv::Mat & image, CvScalar color, int offsetx = 0, int offsety = 0);


	virtual Drawable * clone() {
		return new BlobResult(*this);
	}


//Metodes GET/SET

	//! Retorna el total de blobs
	//! Gets the total number of blobs
	int GetNumBlobs() const
	{
		return(m_blobs.size());
	}


private:

	//! Function to manage the errors
	void RaiseError(const int errorCode) const;

	//! Does the Filter method job
	void DoFilter(BlobResult &dst,
				int filterAction, funcio_calculBlob *evaluador,
				int condition, double lowLimit, double highLimit = 0) const;

protected:

	//! Vector with all the blobs
	Blob_vector		m_blobs;
};

} //: namespace Blobs
} //: namespace Types

#endif /* BLOBRESULT_HPP_ */
