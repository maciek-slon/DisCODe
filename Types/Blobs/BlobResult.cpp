/*!
 * \file BlobResult.cpp
 * \brief Implementation of the BlobResult class
 */

#include "BlobResult.hpp"

#include <limits.h>
#include <stdio.h>
#include <iostream>
#include <functional>
#include <algorithm>


namespace Types {
namespace Blobs {


/**************************************************************************
		Constructors / Destructors
**************************************************************************/

/*!
 * Standard constructor. Creates an empty set of blobs.
 */
BlobResult::BlobResult()
{
	m_blobs = Blob_vector();
}


/* This comment is related to method that doesn't exist. Hmmm...
 * Constructor from an image. Fills an object with all the blobs in	the image.
 * Result is object with all the blobs in the image. It throws an EXCEPCIO_CALCUL_BLOBS
 * if some error appears in the BlobAnalysis function
 *
 * \param source      image to extract the blobs from
 * \param mask        optional mask to apply. The blobs will be extracted where the mask is
 *                    not 0. All the neighbouring blobs where the mask is 0 will be extern blobs
 * \param threshold   threshold level to apply to the image before computing blobs
 * \param findmoments true to calculate the blob moments (slower) (needed to calculate elipses!)
 * \param blackBlobs  true to search for black blobs in the binarization (it will join all extern white blobs).
 *                    false to search for white blobs in the binarization (it will join all extern black blobs).
 */

/*!
 *
 * @param blob_vector
 */
BlobResult::BlobResult(const Blob_vector & blob_vector)
{
	m_blobs = blob_vector;
}
/*!
 * Copy constructor
 * @param source object to copy
 */
BlobResult::BlobResult( const BlobResult &source )
{
	m_blobs = Blob_vector( source.GetNumBlobs() );

	// creem el nou a partir del passat com a par�metre
	m_blobs = Blob_vector( source.GetNumBlobs() );
	// copiem els blobs de l'origen a l'actual
	Blob_vector::const_iterator pBlobsSrc = source.m_blobs.begin();
	Blob_vector::iterator pBlobsDst = m_blobs.begin();

	while( pBlobsSrc != source.m_blobs.end() )
	{
		// no podem cridar a l'operador = ja que Blob_vector �s un
		// vector de Blob*. Per tant, creem un blob nou a partir del
		// blob original
		*pBlobsDst = new Blob(**pBlobsSrc);
		pBlobsSrc++;
		pBlobsDst++;
	}
}


/*!
 * Destructor
 */
BlobResult::~BlobResult()
{
	ClearBlobs();
}

/* *************************************************************************
 * Operators
 * *************************************************************************/

/*!
 * Assigment operator
 */
BlobResult& BlobResult::operator=(const BlobResult& source)
{
	// check for X=X
	if (this != &source)
	{
		// alliberem el conjunt de blobs antic
		for( int i = 0; i < GetNumBlobs(); i++ )
		{
			delete m_blobs[i];
		}
		m_blobs.clear();
		// creem el nou a partir del passat com a par�metre
		m_blobs = Blob_vector( source.GetNumBlobs() );
		// copiem els blobs de l'origen a l'actual
		Blob_vector::const_iterator pBlobsSrc = source.m_blobs.begin();
		Blob_vector::iterator pBlobsDst = m_blobs.begin();

		while( pBlobsSrc != source.m_blobs.end() )
		{
			// no podem cridar a l'operador = ja que Blob_vector �s un
			// vector de Blob*. Per tant, creem un blob nou a partir del
			// blob original
			*pBlobsDst = new Blob(**pBlobsSrc);
			pBlobsSrc++;
			pBlobsDst++;
		}
	}
	return *this;
}


/**
- FUNCI�: operador +
- FUNCIONALITAT: Concatena els blobs de dos BlobResult
- PAR�METRES:
	- source: d'on s'agafaran els blobs afegits a l'actual
- RESULTAT:
	- retorna un nou BlobResult amb els dos BlobResult concatenats
- RESTRICCIONS:
- AUTOR: Ricard Borr�s
- DATA DE CREACI�: 25-05-2005.
- NOTA: per la implementaci�, els blobs del par�metre es posen en ordre invers
- MODIFICACI�: Data. Autor. Descripci�.
*/
/**
- FUNCTION: + operator
- FUNCTIONALITY: Joins the blobs in source with the current ones
- PARAMETERS:
	- source: object to copy the blobs
- RESULT:
	- object with the actual blobs and the source blobs
- RESTRICTIONS:
- AUTHOR: Ricard Borr�s
- CREATION DATE: 25-05-2005.
- MODIFICATION: Date. Author. Description.
*/
BlobResult BlobResult::operator+( const BlobResult& source ) const
{
	//creem el resultat a partir dels blobs actuals
	BlobResult resultat( *this );

	// reservem mem�ria per als nous blobs
	resultat.m_blobs.resize( resultat.GetNumBlobs() + source.GetNumBlobs() );

	// declarem els iterador per rec�rrer els blobs d'origen i desti
	Blob_vector::const_iterator pBlobsSrc = source.m_blobs.begin();
	Blob_vector::iterator pBlobsDst = resultat.m_blobs.end();

	// insertem els blobs de l'origen a l'actual
	while( pBlobsSrc != source.m_blobs.end() )
	{
		pBlobsDst--;
		*pBlobsDst = new Blob(**pBlobsSrc);
		pBlobsSrc++;
	}

	return resultat;
}

/**************************************************************************
		Operacions / Operations
**************************************************************************/

/**
- FUNCI�: AddBlob
- FUNCIONALITAT: Afegeix un blob al conjunt
- PAR�METRES:
	- blob: blob a afegir
- RESULTAT:
	- modifica el conjunt de blobs actual
- RESTRICCIONS:
- AUTOR: Ricard Borr�s
- DATA DE CREACI�: 2006/03/01
- MODIFICACI�: Data. Autor. Descripci�.
*/
void BlobResult::AddBlob( Blob *blob )
{
	if( blob != NULL )
		m_blobs.push_back( new Blob( blob ) );
}


#ifdef MATRIXCV_ACTIU

/**
- FUNCI�: GetResult
- FUNCIONALITAT: Calcula el resultat especificat sobre tots els blobs de la classe
- PAR�METRES:
	- evaluador: Qualsevol objecte derivat de BlobOperator
- RESULTAT:
	- Retorna un array de double's amb el resultat per cada blob
- RESTRICCIONS:
- AUTOR: Ricard Borr�s
- DATA DE CREACI�: 25-05-2005.
- MODIFICACI�: Data. Autor. Descripci�.
*/
/**
- FUNCTION: GetResult
- FUNCTIONALITY: Computes the function evaluador on all the blobs of the class
				 and returns a vector with the result
- PARAMETERS:
	- evaluador: function to apply to each blob (any object derived from the
				 BlobOperator class )
- RESULT:
	- vector with all the results in the same order as the blobs
- RESTRICTIONS:
- AUTHOR: Ricard Borr�s
- CREATION DATE: 25-05-2005.
- MODIFICATION: Date. Author. Description.
*/
double_vector BlobResult::GetResult( funcio_calculBlob *evaluador ) const
{
	if( GetNumBlobs() <= 0 )
	{
		return double_vector();
	}

	// definim el resultat
	double_vector result = double_vector( GetNumBlobs() );
	// i iteradors sobre els blobs i el resultat
	double_vector::iterator itResult = result.GetIterator();
	Blob_vector::const_iterator itBlobs = m_blobs.begin();

	// avaluem la funci� en tots els blobs
	while( itBlobs != m_blobs.end() )
	{
		*itResult = (*evaluador)(**itBlobs);
		itBlobs++;
		itResult++;
	}
	return result;
}
#endif

/**
- FUNCI�: GetSTLResult
- FUNCIONALITAT: Calcula el resultat especificat sobre tots els blobs de la classe
- PAR�METRES:
	- evaluador: Qualsevol objecte derivat de BlobOperator
- RESULTAT:
	- Retorna un array de double's STL amb el resultat per cada blob
- RESTRICCIONS:
- AUTOR: Ricard Borr�s
- DATA DE CREACI�: 25-05-2005.
- MODIFICACI�: Data. Autor. Descripci�.
*/
/**
- FUNCTION: GetResult
- FUNCTIONALITY: Computes the function evaluador on all the blobs of the class
				 and returns a vector with the result
- PARAMETERS:
	- evaluador: function to apply to each blob (any object derived from the
				 BlobOperator class )
- RESULT:
	- vector with all the results in the same order as the blobs
- RESTRICTIONS:
- AUTHOR: Ricard Borr�s
- CREATION DATE: 25-05-2005.
- MODIFICATION: Date. Author. Description.
*/
double_stl_vector BlobResult::GetSTLResult( funcio_calculBlob *evaluador ) const
{
	if( GetNumBlobs() <= 0 )
	{
		return double_stl_vector();
	}

	// definim el resultat
	double_stl_vector result = double_stl_vector( GetNumBlobs() );
	// i iteradors sobre els blobs i el resultat
	double_stl_vector::iterator itResult = result.begin();
	Blob_vector::const_iterator itBlobs = m_blobs.begin();

	// avaluem la funci� en tots els blobs
	while( itBlobs != m_blobs.end() )
	{
		*itResult = (*evaluador)(**itBlobs);
		itBlobs++;
		itResult++;
	}
	return result;
}

/**
- FUNCI�: GetNumber
- FUNCIONALITAT: Calcula el resultat especificat sobre un �nic blob de la classe
- PAR�METRES:
	- evaluador: Qualsevol objecte derivat de BlobOperator
	- indexblob: n�mero de blob del que volem calcular el resultat.
- RESULTAT:
	- Retorna un double amb el resultat
- RESTRICCIONS:
- AUTOR: Ricard Borr�s
- DATA DE CREACI�: 25-05-2005.
- MODIFICACI�: Data. Autor. Descripci�.
*/
/**
- FUNCTION: GetNumber
- FUNCTIONALITY: Computes the function evaluador on a blob of the class
- PARAMETERS:
	- indexBlob: index of the blob to compute the function
	- evaluador: function to apply to each blob (any object derived from the
				 BlobOperator class )
- RESULT:
- RESTRICTIONS:
- AUTHOR: Ricard Borr�s
- CREATION DATE: 25-05-2005.
- MODIFICATION: Date. Author. Description.
*/
double BlobResult::GetNumber( int indexBlob, funcio_calculBlob *evaluador ) const
{
	if( indexBlob < 0 || indexBlob >= GetNumBlobs() )
		RaiseError( EXCEPTION_BLOB_OUT_OF_BOUNDS );
	return (*evaluador)( *m_blobs[indexBlob] );
}

/////////////////////////// FILTRAT DE BLOBS ////////////////////////////////////

/**
- FUNCI�: Filter
- FUNCIONALITAT: Filtra els blobs de la classe i deixa el resultat amb nom�s
			   els blobs que han passat el filtre.
			   El filtrat es basa en especificar condicions sobre un resultat dels blobs
			   i seleccionar (o excloure) aquells blobs que no compleixen una determinada
			   condicio
- PAR�METRES:
	- dst: variable per deixar els blobs filtrats
	- filterAction:	acci� de filtrat. Incloure els blobs trobats (B_INCLUDE),
				    o excloure els blobs trobats (B_EXCLUDE)
	- evaluador: Funci� per evaluar els blobs (qualsevol objecte derivat de BlobOperator
	- Condition: tipus de condici� que ha de superar la mesura (FilterType)
				 sobre cada blob per a ser considerat.
				    B_EQUAL,B_NOT_EQUAL,B_GREATER,B_LESS,B_GREATER_OR_EQUAL,
				    B_LESS_OR_EQUAL,B_INSIDE,B_OUTSIDE
	- LowLimit:  valor num�ric per a la comparaci� (Condition) de la mesura (FilterType)
	- HighLimit: valor num�ric per a la comparaci� (Condition) de la mesura (FilterType)
				 (nom�s t� sentit per a aquelles condicions que tenen dos valors
				 (B_INSIDE, per exemple).
- RESULTAT:
	- Deixa els blobs resultants del filtrat a destination
- RESTRICCIONS:
- AUTOR: Ricard Borr�s
- DATA DE CREACI�: 25-05-2005.
- MODIFICACI�: Data. Autor. Descripci�.
*/
/**
- FUNCTION: Filter
- FUNCTIONALITY: Get some blobs from the class based on conditions on measures
				 of the blobs.
- PARAMETERS:
	- dst: where to store the selected blobs
	- filterAction:	B_INCLUDE: include the blobs which pass the filter in the result
				    B_EXCLUDE: exclude the blobs which pass the filter in the result
	- evaluador: Object to evaluate the blob
	- Condition: How to decide if  the result returned by evaluador on each blob
				 is included or not. It can be:
				    B_EQUAL,B_NOT_EQUAL,B_GREATER,B_LESS,B_GREATER_OR_EQUAL,
				    B_LESS_OR_EQUAL,B_INSIDE,B_OUTSIDE
	- LowLimit:  numerical value to evaluate the Condition on evaluador(blob)
	- HighLimit: numerical value to evaluate the Condition on evaluador(blob).
				 Only useful for B_INSIDE and B_OUTSIDE
- RESULT:
	- It returns on dst the blobs that accomplish (B_INCLUDE) or discards (B_EXCLUDE)
	  the Condition on the result returned by evaluador on each blob
- RESTRICTIONS:
- AUTHOR: Ricard Borr�s
- CREATION DATE: 25-05-2005.
- MODIFICATION: Date. Author. Description.
*/
void BlobResult::Filter(BlobResult &dst,
						 int filterAction,
						 funcio_calculBlob *evaluador,
						 int condition,
						 double lowLimit, double highLimit /*=0*/) const

{
	// do the job
	DoFilter(dst, filterAction, evaluador, condition, lowLimit, highLimit );
}

/**
- FUNCI�: Filter (const version)
- FUNCIONALITAT: Filtra els blobs de la classe i deixa el resultat amb nom�s
			   els blobs que han passat el filtre.
			   El filtrat es basa en especificar condicions sobre un resultat dels blobs
			   i seleccionar (o excloure) aquells blobs que no compleixen una determinada
			   condicio
- PAR�METRES:
	- dst: variable per deixar els blobs filtrats
	- filterAction:	acci� de filtrat. Incloure els blobs trobats (B_INCLUDE),
				    o excloure els blobs trobats (B_EXCLUDE)
	- evaluador: Funci� per evaluar els blobs (qualsevol objecte derivat de BlobOperator
	- Condition: tipus de condici� que ha de superar la mesura (FilterType)
				 sobre cada blob per a ser considerat.
				    B_EQUAL,B_NOT_EQUAL,B_GREATER,B_LESS,B_GREATER_OR_EQUAL,
				    B_LESS_OR_EQUAL,B_INSIDE,B_OUTSIDE
	- LowLimit:  valor num�ric per a la comparaci� (Condition) de la mesura (FilterType)
	- HighLimit: valor num�ric per a la comparaci� (Condition) de la mesura (FilterType)
				 (nom�s t� sentit per a aquelles condicions que tenen dos valors
				 (B_INSIDE, per exemple).
- RESULTAT:
	- Deixa els blobs resultants del filtrat a destination
- RESTRICCIONS:
- AUTOR: Ricard Borr�s
- DATA DE CREACI�: 25-05-2005.
- MODIFICACI�: Data. Autor. Descripci�.
*/
/**
- FUNCTION: Filter (const version)
- FUNCTIONALITY: Get some blobs from the class based on conditions on measures
				 of the blobs.
- PARAMETERS:
	- dst: where to store the selected blobs
	- filterAction:	B_INCLUDE: include the blobs which pass the filter in the result
				    B_EXCLUDE: exclude the blobs which pass the filter in the result
	- evaluador: Object to evaluate the blob
	- Condition: How to decide if  the result returned by evaluador on each blob
				 is included or not. It can be:
				    B_EQUAL,B_NOT_EQUAL,B_GREATER,B_LESS,B_GREATER_OR_EQUAL,
				    B_LESS_OR_EQUAL,B_INSIDE,B_OUTSIDE
	- LowLimit:  numerical value to evaluate the Condition on evaluador(blob)
	- HighLimit: numerical value to evaluate the Condition on evaluador(blob).
				 Only useful for B_INSIDE and B_OUTSIDE
- RESULT:
	- It returns on dst the blobs that accomplish (B_INCLUDE) or discards (B_EXCLUDE)
	  the Condition on the result returned by evaluador on each blob
- RESTRICTIONS:
- AUTHOR: Ricard Borr�s
- CREATION DATE: 25-05-2005.
- MODIFICATION: Date. Author. Description.
*/
void BlobResult::Filter(BlobResult &dst,
						 int filterAction,
						 funcio_calculBlob *evaluador,
						 int condition,
						 double lowLimit, double highLimit /*=0*/)

{
	int numBlobs = GetNumBlobs();

	// do the job
	DoFilter(dst, filterAction, evaluador, condition, lowLimit, highLimit );

	// inline operation: remove previous blobs
	if( &dst == this )
	{
		// esborrem els primers blobs ( que s�n els originals )
		// ja que els tindrem replicats al final si passen el filtre
		Blob_vector::iterator itBlobs = m_blobs.begin();
		for( int i = 0; i < numBlobs; i++ )
		{
			delete *itBlobs;
			itBlobs++;
		}
		m_blobs.erase( m_blobs.begin(), itBlobs );
	}
}


//! Does the Filter method job
void BlobResult::DoFilter(BlobResult &dst, int filterAction, funcio_calculBlob *evaluador,
						   int condition, double lowLimit, double highLimit/* = 0*/) const
{
	int i, numBlobs;
	bool resultavaluacio;
	double_stl_vector avaluacioBlobs;
	double_stl_vector::iterator itavaluacioBlobs;

	if( GetNumBlobs() <= 0 ) return;
	if( !evaluador ) return;
	//avaluem els blobs amb la funci� pertinent
	avaluacioBlobs = GetSTLResult(evaluador);
	itavaluacioBlobs = avaluacioBlobs.begin();
	numBlobs = GetNumBlobs();
	switch(condition)
	{
		case B_EQUAL:
			for(i=0;i<numBlobs;i++, itavaluacioBlobs++)
			{
				resultavaluacio= *itavaluacioBlobs == lowLimit;
				if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
					( !resultavaluacio && filterAction == B_EXCLUDE ))
				{
					dst.m_blobs.push_back( new Blob( GetBlob( i ) ));
				}
			}
			break;
		case B_NOT_EQUAL:
			for(i=0;i<numBlobs;i++, itavaluacioBlobs++)
			{
				resultavaluacio = *itavaluacioBlobs != lowLimit;
				if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
					( !resultavaluacio && filterAction == B_EXCLUDE ))
				{
					dst.m_blobs.push_back( new Blob( GetBlob( i ) ));
				}
			}
			break;
		case B_GREATER:
			for(i=0;i<numBlobs;i++, itavaluacioBlobs++)
			{
				resultavaluacio= *itavaluacioBlobs > lowLimit;
				if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
					( !resultavaluacio && filterAction == B_EXCLUDE ))
				{
					dst.m_blobs.push_back( new Blob( GetBlob( i ) ));
				}
			}
			break;
		case B_LESS:
			for(i=0;i<numBlobs;i++, itavaluacioBlobs++)
			{
				resultavaluacio= *itavaluacioBlobs < lowLimit;
				if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
					( !resultavaluacio && filterAction == B_EXCLUDE ))
				{
					dst.m_blobs.push_back( new Blob( GetBlob( i ) ));
				}
			}
			break;
		case B_GREATER_OR_EQUAL:
			for(i=0;i<numBlobs;i++, itavaluacioBlobs++)
			{
				resultavaluacio= *itavaluacioBlobs>= lowLimit;
				if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
					( !resultavaluacio && filterAction == B_EXCLUDE ))
				{
					dst.m_blobs.push_back( new Blob( GetBlob( i ) ));
				}
			}
			break;
		case B_LESS_OR_EQUAL:
			for(i=0;i<numBlobs;i++, itavaluacioBlobs++)
			{
				resultavaluacio= *itavaluacioBlobs <= lowLimit;
				if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
					( !resultavaluacio && filterAction == B_EXCLUDE ))
				{
					dst.m_blobs.push_back( new Blob( GetBlob( i ) ));
				}
			}
			break;
		case B_INSIDE:
			for(i=0;i<numBlobs;i++, itavaluacioBlobs++)
			{
				resultavaluacio=( *itavaluacioBlobs >= lowLimit) && ( *itavaluacioBlobs <= highLimit);
				if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
					( !resultavaluacio && filterAction == B_EXCLUDE ))
				{
					dst.m_blobs.push_back( new Blob( GetBlob( i ) ));
				}
			}
			break;
		case B_OUTSIDE:
			for(i=0;i<numBlobs;i++, itavaluacioBlobs++)
			{
				resultavaluacio=( *itavaluacioBlobs < lowLimit) || ( *itavaluacioBlobs > highLimit);
				if( ( resultavaluacio && filterAction == B_INCLUDE ) ||
					( !resultavaluacio && filterAction == B_EXCLUDE ))
				{
					dst.m_blobs.push_back( new Blob( GetBlob( i ) ));
				}
			}
			break;
	}
}
/**
- FUNCI�: GetBlob
- FUNCIONALITAT: Retorna un blob si aquest existeix (index != -1)
- PAR�METRES:
	- indexblob: index del blob a retornar
- RESULTAT:
- RESTRICCIONS:
- AUTOR: Ricard Borr�s
- DATA DE CREACI�: 25-05-2005.
- MODIFICACI�: Data. Autor. Descripci�.
*/
/*
- FUNCTION: GetBlob
- FUNCTIONALITY: Gets the n-th blob (without ordering the blobs)
- PARAMETERS:
	- indexblob: index in the blob array
- RESULT:
- RESTRICTIONS:
- AUTHOR: Ricard Borr�s
- CREATION DATE: 25-05-2005.
- MODIFICATION: Date. Author. Description.
*/
Blob BlobResult::GetBlob(int indexblob) const
{
	if( indexblob < 0 || indexblob >= GetNumBlobs() )
		RaiseError( EXCEPTION_BLOB_OUT_OF_BOUNDS );

	return *m_blobs[indexblob];
}
Blob *BlobResult::GetBlob(int indexblob)
{
	if( indexblob < 0 || indexblob >= GetNumBlobs() )
		RaiseError( EXCEPTION_BLOB_OUT_OF_BOUNDS );

	return m_blobs[indexblob];
}

/**
- FUNCI�: GetNthBlob
- FUNCIONALITAT: Retorna l'en�ssim blob segons un determinat criteri
- PAR�METRES:
	- criteri: criteri per ordenar els blobs (objectes derivats de BlobOperator)
	- nBlob: index del blob a retornar
	- dst: on es retorna el resultat
- RESULTAT:
	- retorna el blob nBlob a dst ordenant els blobs de la classe segons el criteri
	  en ordre DESCENDENT. Per exemple, per obtenir el blob major:
		GetNthBlob( BlobGetArea(), 0, blobMajor );
		GetNthBlob( BlobGetArea(), 1, blobMajor ); (segon blob m�s gran)
- RESTRICCIONS:
- AUTOR: Ricard Borr�s
- DATA DE CREACI�: 25-05-2005.
- MODIFICACI�: Data. Autor. Descripci�.
*/
/*
- FUNCTION: GetNthBlob
- FUNCTIONALITY: Gets the n-th blob ordering first the blobs with some criteria
- PARAMETERS:
	- criteri: criteria to order the blob array
	- nBlob: index of the returned blob in the ordered blob array
	- dst: where to store the result
- RESULT:
- RESTRICTIONS:
- AUTHOR: Ricard Borr�s
- CREATION DATE: 25-05-2005.
- MODIFICATION: Date. Author. Description.
*/
void BlobResult::GetNthBlob( funcio_calculBlob *criteri, int nBlob, Blob &dst ) const
{
	// verifiquem que no estem accedint fora el vector de blobs
	if( nBlob < 0 || nBlob >= GetNumBlobs() )
	{
		//RaiseError( EXCEPTION_BLOB_OUT_OF_BOUNDS );
		dst = Blob();
		return;
	}

	double_stl_vector avaluacioBlobs, avaluacioBlobsOrdenat;
	double valorEnessim;

	//avaluem els blobs amb la funci� pertinent
	avaluacioBlobs = GetSTLResult(criteri);

	avaluacioBlobsOrdenat = double_stl_vector( GetNumBlobs() );

	// obtenim els nBlob primers resultats (en ordre descendent)
	std::partial_sort_copy( avaluacioBlobs.begin(),
						    avaluacioBlobs.end(),
						    avaluacioBlobsOrdenat.begin(),
						    avaluacioBlobsOrdenat.end(),
						    std::greater<double>() );

	valorEnessim = avaluacioBlobsOrdenat[nBlob];

	// busquem el primer blob que t� el valor n-ssim
	double_stl_vector::const_iterator itAvaluacio = avaluacioBlobs.begin();

	bool trobatBlob = false;
	int indexBlob = 0;
	while( itAvaluacio != avaluacioBlobs.end() && !trobatBlob )
	{
		if( *itAvaluacio == valorEnessim )
		{
			trobatBlob = true;
			dst = Blob( GetBlob(indexBlob));
		}
		itAvaluacio++;
		indexBlob++;
	}
}

/**
- FUNCI�: ClearBlobs
- FUNCIONALITAT: Elimina tots els blobs de l'objecte
- PAR�METRES:
- RESULTAT:
	- Allibera tota la mem�ria dels blobs
- RESTRICCIONS:
- AUTOR: Ricard Borr�s Navarra
- DATA DE CREACI�: 25-05-2005.
- MODIFICACI�: Data. Autor. Descripci�.
*/
/*
- FUNCTION: ClearBlobs
- FUNCTIONALITY: Clears all the blobs from the object and releases all its memory
- PARAMETERS:
- RESULT:
- RESTRICTIONS:
- AUTHOR: Ricard Borr�s
- CREATION DATE: 25-05-2005.
- MODIFICATION: Date. Author. Description.
*/
void BlobResult::ClearBlobs()
{
	Blob_vector::iterator itBlobs = m_blobs.begin();
	while( itBlobs != m_blobs.end() )
	{
		delete *itBlobs;
		itBlobs++;
	}

	m_blobs.clear();
}

/*!
 * Error handling function.
 * In _SHOW_ERRORS version, prints error. In release is silent.
 * In both cases throws an exception with the error.
 *
 * @param errorCode reason of the error
 */
void BlobResult::RaiseError(const int errorCode) const
{
//! Do we need to show errors?
#ifdef _SHOW_ERRORS
	switch (errorCode)
	{
	case EXCEPTION_BLOB_OUT_OF_BOUNDS:
		std::cout << "Error en BlobResult: Intentant accedir a un blob no existent\n";
		break;
	default:
		std::cout << "Error en BlobResult: Codi d'error desconegut\n";
		break;
	}

#endif
	throw errorCode;
}



/* *************************************************************************
 * Auxiliary functions
 * *************************************************************************/

/*!
 * Prints some blob features in an ASCII file
 * @param nom_fitxer full path + filename to generate
 */
void BlobResult::PrintBlobs( char *nom_fitxer ) const
{
	double_stl_vector area, /*perimetre,*/ exterior, compacitat, longitud,
					  externPerimeter, perimetreConvex, perimetre;
	int i;
	FILE *fitxer_sortida;

 	area      = GetSTLResult( BlobGetArea());
	perimetre = GetSTLResult( BlobGetPerimeter());
	exterior  = GetSTLResult( BlobGetExterior());
	compacitat = GetSTLResult(BlobGetCompactness());
	longitud  = GetSTLResult( BlobGetLength());
	externPerimeter = GetSTLResult( BlobGetExternPerimeter());
	perimetreConvex = GetSTLResult( BlobGetHullPerimeter());

	fitxer_sortida = fopen( nom_fitxer, "w" );

	for(i=0; i<GetNumBlobs(); i++)
	{
		fprintf( fitxer_sortida, "blob %d ->\t a=%7.0f\t p=%8.2f (%8.2f extern)\t pconvex=%8.2f\t ext=%.0f\t c=%3.2f\t l=%8.2f\n",
				 i, area[i], perimetre[i], externPerimeter[i], perimetreConvex[i], exterior[i], compacitat[i], longitud[i] );
	}
	fclose( fitxer_sortida );

}

/*!
 * Draw all blobs onto given image.
 */
void BlobResult::draw(cv::Mat & image, CvScalar color, int offsetx, int offsety) {
	for(int i=0; i<GetNumBlobs(); i++)
	{
		m_blobs[i]->draw(image, color, offsetx, offsety);
	}
}

} //: namespace Blobs
} //: namespace Types
