/*!
 * \file CspGraph.hpp
 * \brief
 * \author mboryn
 * \date 2011-01-22
 */

#ifndef CSPGRAPH_HPP_
#define CSPGRAPH_HPP_

#include <deque>
#include <map>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/shared_ptr.hpp>

#include "AbstractConstraint.hpp"
#include "AbstractShape.hpp"

/**
 * Realizacja grafu dla rozwiazywania problemów csp
 */
//ograniczenia
namespace boost {
enum edge_constraint_t
{
	edge_constraint = 222
};
BOOST_INSTALL_PROPERTY(edge,constraint);
}
//ksztalty przechowywane w wezle
namespace boost {
enum vertex_shape_t
{
	vertex_shape = 333
};
BOOST_INSTALL_PROPERTY(vertex,shape);
}
//status wezla , opcjonalny czy nie
namespace boost {
enum vertex_status_t
{
	vertex_status = 444
};
BOOST_INSTALL_PROPERTY(vertex,status);
}

//rodzaj ksztaltu jaki moze byc przechowywany w danym wezle
namespace boost {
enum vertex_type_t
{
	vertex_type = 555
};
BOOST_INSTALL_PROPERTY(vertex,type);
}

namespace Processors {
namespace CspObjectRecognize {

class AbstractConstraint;
class AbstractShape;

typedef std::deque <boost::shared_ptr <AbstractShape> > ShapeSegments;

typedef std::vector <boost::shared_ptr <AbstractConstraint> > ConstraintVector;
typedef boost::adjacency_list <boost::listS, boost::vecS, boost::undirectedS, boost::property <boost::vertex_shape_t,
		boost::shared_ptr <AbstractShape>, boost::property <boost::vertex_status_t, int, boost::property <
				boost::vertex_type_t, int> > >, boost::property <boost::edge_constraint_t, ConstraintVector> > Graph;
typedef boost::graph_traits <Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits <Graph>::vertex_iterator Iterator;
typedef boost::graph_traits <Graph>::edge_descriptor Edge;
typedef boost::graph_traits <Graph>::edge_iterator EdgeIterator;
typedef boost::graph_traits <Graph>::out_edge_iterator OutEdgeIterator;
typedef boost::graph_traits <Graph>::in_edge_iterator InEdgeIterator;
typedef std::vector <Vertex> VertexVector;
typedef std::deque <Vertex> VertexQueue;
typedef std::deque <Vertex>::iterator VertexQueueIterator;
typedef std::vector <Edge> EdgeVector;
typedef boost::property_map <Graph, boost::edge_constraint_t>::type edge_constraint_map_t;
typedef boost::property_map <Graph, boost::vertex_shape_t>::type vertex_shape_map_t;
typedef boost::property_map <Graph, boost::vertex_status_t>::type vertex_status_map_t;
typedef boost::property_map <Graph, boost::vertex_type_t>::type vertex_type_map_t;
typedef std::deque <boost::shared_ptr <AbstractShape> > ShapesStack;
typedef std::deque <boost::shared_ptr <AbstractShape> >::iterator ShapesStackIterator;
typedef std::vector <ShapesStack> ShapesStackVector;

//przeszukiwanie grafu
typedef std::pair <Vertex, bool> PairSearch;
typedef std::deque <PairSearch> SearchVector;
typedef std::vector <SearchVector> SearchVectors;
typedef std::map <Vertex, EdgeVector> EdgeMap;

enum VertexStatusEnum
{
	MANDATORY, OPTIONAL
};

/**
 * Searches for object defined by a graph.
 * Graph consists of vertices (for example line segments)
 * and edges (constraints between them, for example two line segments have common end).
 */
class CspGraph
{
public:
	CspGraph();
	virtual ~CspGraph();

	/**
	 * Initialize graph.
	 * @param mandatoryVertexNum number of mandatory vertices.
	 * @param optionalVertex number of optional vertices.
	 * @param empty  czy graf ma wstawiac zaslepki
	 * @return
	 */
	bool init(int mandatoryVertexNum, int optionalVertex, bool empty);

	/**
	 * metoda inizjalizujaca mape krawedzi przychodzacych i wychodzacych do wezlow.
	 * mapa krawedzi wychodzacych i przychodzacaych do wezla
	 */
	void InitInputOutputEdgeMap();

	/**
	 * Inicjalizacja wektora przeszukiwan grafu
	 * @return
	 */
	bool InitSearchGraph();

	/**
	 *
	 * @param numVertex
	 * @param status
	 */
	void AddSearchVertex(Vertex numVertex, bool status);

	/**
	 * Insert edge to the graph with given constraint.
	 * @param u from-vertex
	 * @param v to-vertex
	 * @param constraint constraint
	 * @return true if inserted
	 */
	bool AddEdge(Vertex u, Vertex v, boost::shared_ptr <AbstractConstraint> constraint);

	/**
	 * Metoda przeszukuje graf oraz wstawia do niego znalezione ksztalty, gdy model jest uzupelniony poprawnie.
	 * zwracane jest informacja ze znaleziono obiekt
	 *
	 * @param stack
	 * @return
	 */
	bool FindCspShape(ShapeSegments *stack);

	/**
	 * Metoda zwraca wszytskie ksztalty uzyte w grafie, w wezlach opcjonalnych oraz wymaganych.
	 * @return
	 */
	ShapeVector &GetAllUsedShapesVector();

	/**
	 * Get all vertices.
	 * @return
	 */
	VertexVector &GetVertex();
private:
	/**
	 *
	 */
	void Clear();

	/**
	 * Metoda czysci caly graf.
	 */
	void ClearAll();

	/**
	 * Utworzenie nastepnego vektora przeszukiwan wezlow w grafie
	 * @return
	 */
	bool NextSearchVector();

	/**
	 * Get vertices which satisfy given constraint.
	 * @param u
	 * @param constraint
	 * @return
	 */
	VertexVector &GetMandatoryVertex(Vertex u, boost::shared_ptr <AbstractConstraint> constraint);

	/**
	 * Wstawienie wartosci dla wybranego wezla
	 * @param shape
	 * @param v
	 */
	void InsertVertexValue(boost::shared_ptr <AbstractShape> shape, Vertex v);

	/**
	 * Get Vertex Value.
	 * @param v
	 * @return
	 */
	boost::shared_ptr <AbstractShape> GetVertexValue(Vertex v);
	void SetVertexStatus(int status, Vertex v);

	/**
	 * Set Vertex Type.
	 * @param v
	 * @param kind
	 */
	void SetVertexType(Vertex v, int kind);

	/**
	 * Get Vertex Type.
	 * @param v
	 * @return
	 */
	int GetVertexType(Vertex v);

	/**
	 * Metoda pobiera wezel zrodlowy dla danej krawedzi.
	 * @param e
	 * @return
	 */
	Vertex GetSourceEdge(Edge e);

	/**
	 * Metoda zwraca krawedzie polaczone z danym wezlem, jako wychodzace polaczenia
	 * @param v
	 * @return
	 */
	EdgeVector &GetOutPutEdge(Vertex v);

	/**
	 * Metoda zwraca krawedzie polaczone z danym wezlem, jako wychodzace polaczenia
	 * spelniajace ograniczenie
	 * @param v
	 * @param constraint
	 * @return
	 */
	EdgeVector &GetOutPutEdge(Vertex v, boost::shared_ptr <AbstractConstraint> constraint);

	/**
	 * Metoda sprawdza czy wezel jest wsrod wymaganych wezlow
	 * @param u
	 * @return
	 */
	bool FindInMandatory(Vertex u);

	/**
	 * Metoda sprawdza czy ksztalt shape wstawiony do wezla u spelnia ograniczenie constraint.
	 * @param constraint
	 * @param u
	 * @param shape
	 * @return
	 */
	bool checkConstraint(boost::shared_ptr <AbstractConstraint> constraint, Vertex u, boost::shared_ptr <
					AbstractShape> shape);

	/**
	 * Metoda sprawdza czy dany wezel spelnia ograniczenia.
	 * @param u
	 * @param shape
	 * @return
	 */
	bool checkConstraints(Vertex u, boost::shared_ptr <AbstractShape> shape);

	/**
	 * Metoda sprawdza ograniczenia
	 * @param src wezel zrodlowy
	 * @param edgeVector
	 * @return
	 */
	bool checkConstraints(Vertex src, EdgeVector &edgeVector);

	Graph g;
	VertexVector vertices;
	//wezy w grafie ktore sa wymagane
	VertexVector mandatoryVertices;
	//wezly w grafie ktore sa opcjonalne
	VertexVector optionalVertices;
	edge_constraint_map_t edgeConstraint;
	vertex_shape_map_t vertexShape;
	vertex_type_map_t vertexType;
	EdgeMap outputEdgeMap;
	EdgeMap inputEdgeMap;
	ShapeVector usedShapes;
	//zmienna informujac czy wstawiamy do grafu zaslepki
	bool empty;
	SearchVectors searchVectors;

	/**
	 * Metoda sprawdza czy wszystkie ograniczenia sa spelnione dla dwoch wezlow.
	 * @param u
	 * @param v
	 * @param e
	 * @return
	 */
	bool checkConstraint(Vertex u, Vertex v, Edge e);

	/**
	 * Metoda sprawdza czy wszystkie ograniczenia sa spelnione dla dwoch wezlow.
	 * @param first
	 * @param second
	 * @param e
	 * @return
	 */
	bool checkConstraint(boost::shared_ptr <AbstractShape> first, boost::shared_ptr <AbstractShape> second, Edge e);

	/**
	 * Metoda wywolywana rekurencyjnie do szukania obiektu,
	 * searchVector- vektor przeszukiwania grafu
	 * @param stack
	 * @param searchVector
	 * @return
	 */
	bool searchGraph(ShapeSegments *stack, SearchVector &searchVector);

	/**
	 * Metoda wyszukuje polaczen spelniajacych podane ograniczeni i probuje przejsc do wezla koncowego to.
	 * @param from
	 * @param to
	 * @param constraint
	 * @param vector
	 * @return
	 */
	bool goToVertex(Vertex from, Vertex to, boost::shared_ptr <AbstractConstraint> constraint, VertexQueue &vector);

	/**
	 * Metoda sprawdza czy podany wezel znajduje sie w kolejce
	 * @param vector
	 * @param u
	 * @return
	 */
	bool isVisited(VertexQueue &vector, Vertex u);

	/** */
	EdgeVector outputEdgeVector;

	/** */
	VertexVector returnedMandatoryVertex;

	//	void ClearSearchVectors();
	//	VertexVector &GetVertex(Vertex u, boost::shared_ptr <AbstractConstraint> constraint);
	//	VertexVector &GetMandatoryVertex();
	//	VertexVector &GetMandatoryVertex(bool empty);
	//	VertexVector &GetOptionalVertex();
	//	VertexVector &GetOptionalVertex(bool empty);
	//	VertexVector &GetOptionalVertex(Vertex u, boost::shared_ptr <AbstractConstraint> constraint);
	//	ConstraintVector &GetConstraints(Edge e);
	//	int GetVertexStatus(Vertex v);
	//	Vertex GetTargetEdge(Edge e);
	//	EdgeVector &GetInPutEdge(Vertex v);
	//	EdgeVector &GetInPutEdge(Vertex v, boost::shared_ptr <AbstractConstraint> constraint);
	//	bool FindCspShape(ShapeSegments *stack, SearchVector &searchVector);
	//	Vertex &getVertex(int vertexNum);
	//	void Clear(uint from, uint to);
	//	int NumEmptyMandatoryVertices();
	//	int NumMandatoryVertices();
	//	int NumEmptyOptionalVertices();
	//	int NumOptionalVertices();
	//	ShapeVector &GetUsedShapesVector();
	//	bool FindInOptional(Vertex u);
	//	VertexVector GetMandatoryVertexFrom(uint u, uint v);
	//	VertexQueue GetMandatoryVertexFrom(Vertex start, Vertex stop, boost::shared_ptr <AbstractConstraint> constraint);
	//	static bool ContainsElement(ShapesStack shapes, boost::shared_ptr <AbstractShape>);
	//	void SetEmpty(bool empty);
	//	bool GetEmpty();
	//	void Show();
	//	void ShowMandatory();
	//	void ReturnObjects(ShapeSegments *shapeSegments);
	//	int GetDomainParent();
	//	std::vector <Vertex>::iterator bracketStart;
	//	std::vector <Vertex>::iterator bracketStop;
	//	bool checkConstraints(Vertex src);
	//	void ShowOptional();

	//	vertex_status_map_t vertexStatus;
	//	EdgeVector inputEdgeVector;
	//	VertexVector returnedOptionalVertex;
	//	VertexVector returnedVertex;
};

} // namespace Processors
} // namespace CspObjectRecognize

#endif /* CSPGRAPH_HPP_ */
