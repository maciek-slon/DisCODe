/************************************************************************
  			BlobResult.h

FUNCIONALITAT: Definici� de la classe CBlobResult
AUTOR: Inspecta S.L.
MODIFICACIONS (Modificaci�, Autor, Data):

FUNCTIONALITY: Definition of the CBlobResult class
AUTHOR: Inspecta S.L.
MODIFICATIONS (Modification, Author, Date):

**************************************************************************/


#ifndef BLOBRESULT_HPP_
#define BLOBRESULT_HPP_

#include <math.h>
#include <cxcore.h>
#include <vector>		// vectors de la STL

//! Vector de doubles
typedef std::vector<double> double_stl_vector;

#include <functional>

#include "Types/Blob.hpp"
#include "BlobOperators.hpp"

#include "Drawable.hpp"

namespace Types {


/**************************************************************************
	Filtres / Filters
**************************************************************************/

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


/**************************************************************************
	Excepcions / Exceptions
**************************************************************************/

#define EXCEPTION_BLOB_OUT_OF_BOUNDS	1000
#define EXCEPCIO_CALCUL_BLOBS			1001

/**
	Class to calculate the blobs of an image and calculate some properties
	on them. Also, the class provides functions to filter the blobs using
	some criteria.
*/
class CBlobResult : public Drawable {
public:

	//! Standard constructor, it creates an empty set of blobs
	CBlobResult();

	//! Image constructor, it creates an object with the blobs of the image
	CBlobResult(const Types::Blob_vector & blob_vector);

	//! Copy constructor
	CBlobResult( const CBlobResult &source );

	//! Destructor
	virtual ~CBlobResult();

	//! Assigment operator
	CBlobResult& operator=(const CBlobResult& source);

	//! Addition operator to concatenate two sets of blobs
	CBlobResult operator+( const CBlobResult& source ) const;

	//! Adds a blob to the set of blobs
	void AddBlob( Types::Blob *blob );

#ifdef MATRIXCV_ACTIU
	//! Computes some property on all the blobs of the class
	double_vector GetResult( funcio_calculBlob *evaluador ) const;
#endif
	//! Computes some property on all the blobs of the class
	double_stl_vector GetSTLResult( funcio_calculBlob *evaluador ) const;

	//! Computes some property on one blob of the class
	double GetNumber( int indexblob, funcio_calculBlob *evaluador ) const;

	//! Filters the blobs of the class using some property
	void Filter(CBlobResult &dst,
				int filterAction, funcio_calculBlob *evaluador,
				int condition, double lowLimit, double highLimit = 0 );
	void Filter(CBlobResult &dst,
				int filterAction, funcio_calculBlob *evaluador,
				int condition, double lowLimit, double highLimit = 0 ) const;

	//! Sorts the blobs of the class acording to some criteria and returns the n-th blob
	void GetNthBlob( funcio_calculBlob *criteri, int nBlob, Types::Blob &dst ) const;

	//! Gets the n-th blob of the class ( without sorting )
	Types::Blob GetBlob(int indexblob) const;
	Types::Blob *GetBlob(int indexblob);

	//! Clears all the blobs of the class
	void ClearBlobs();

	//! Escriu els blobs a un fitxer
	//! Prints some features of all the blobs in a file
	void PrintBlobs( char *nom_fitxer ) const;

	void draw(IplImage * image, CvScalar color, int offsetx = 0, int offsety = 0);


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
	void DoFilter(CBlobResult &dst,
				int filterAction, funcio_calculBlob *evaluador,
				int condition, double lowLimit, double highLimit = 0) const;

protected:

	//! Vector with all the blobs
	Types::Blob_vector		m_blobs;
};

} //: namespace Types

#endif /* BLOBRESULT_HPP_ */
