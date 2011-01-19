/*
 * CspGraph.hpp
 *
 *  Created on: 09-01-2011
 *      Author: mateusz
 */

#ifndef CSPGRAPH_HPP_
#define CSPGRAPH_HPP_

#include <deque>
#include <map>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/property_map/property_map.hpp>

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

// TODO: replace it with something reasonable
//class ShapeSegments;
typedef std::deque<AbstractShape *> ShapeSegments;
//typedef deque<AbstractShape *>::iterator ShapesQueueIterator;
//typedef vector<ShapeSegments *> ShapeSegmentsVector;


typedef std::vector<AbstractShape *> ShapeVector;

typedef std::vector <AbstractConstraint *> ConstraintVector;
typedef boost::adjacency_list <
		boost::listS,
		boost::vecS,
		boost::undirectedS,
		boost::property <
			boost::vertex_shape_t,
			AbstractShape*,
			boost::property <
				boost::vertex_status_t,
				int,
				boost::property <
					boost::vertex_type_t,
					int
				>
			>
		>,
		boost::property <
			boost::edge_constraint_t,
			ConstraintVector
		>
	> Graph;
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
typedef std::deque <AbstractShape *> ShapesStack;
typedef std::deque <AbstractShape *>::iterator ShapesStackIterator;
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

class CspGraph
{
public:
	CspGraph();
	virtual ~CspGraph();

	bool Init(int mandatoryVertexNum, int optionalVertex, bool empty);
	//przeszukiwanie grafu
	bool InitSearchGraph();
	//mapa krawedzi wychodzacych i przychodzacaych do wezla
	void InitInputOutputEdgeMap();
	bool NextSearchVector();
	void AddSearchVertex(Vertex numVertex, bool status);
	void ClearSearchVectors();
	VertexVector &GetVertex();
	VertexVector &GetVertex(Vertex u, AbstractConstraint *constraint);
	VertexVector &GetMandatoryVertex();
	VertexVector &GetMandatoryVertex(bool empty);
	VertexVector &GetMandatoryVertex(Vertex u, AbstractConstraint *constraint);
	VertexVector &GetOptionalVertex();
	VertexVector &GetOptionalVertex(bool empty);
	VertexVector &GetOptionalVertex(Vertex u, AbstractConstraint *constraint);
	bool AddEdge(Vertex u, Vertex v, AbstractConstraint*);
	ConstraintVector &GetConstraints(Edge e);
	void InsertVertexValue(AbstractShape *shape, Vertex v);
	AbstractShape *GetVertexValue(Vertex v);
	void SetVertexStatus(int status, Vertex v);
	int GetVertexStatus(Vertex v);
	void SetVertexType(Vertex v, int kind);
	int GetVertexType(Vertex v);
	Vertex GetSourceEdge(Edge e);
	Vertex GetTargetEdge(Edge e);
	EdgeVector &GetOutPutEdge(Vertex v);
	EdgeVector &GetInPutEdge(Vertex v);
	EdgeVector &GetOutPutEdge(Vertex v, AbstractConstraint *constraint);
	EdgeVector &GetInPutEdge(Vertex v, AbstractConstraint *constraint);
	bool FindCspShape(ShapeSegments *stack);
	bool FindCspShape(ShapeSegments *stack, SearchVector &searchVector);
	Vertex &getVertex(int vertexNum);
	void Clear();
	void ClearAll();
	void Clear(uint from, uint to);
	int NumEmptyMandatoryVertices();
	int NumMandatoryVertices();
	int NumEmptyOptionalVertices();
	int NumOptionalVertices();
	ShapeVector &GetUsedShapesVector();
	ShapeVector &GetAllUsedShapesVector();
	bool FindInMandatory(Vertex u);
	bool FindInOptional(Vertex u);
	bool checkConstraint(AbstractConstraint *constraint, Vertex u, AbstractShape *shape);
	VertexVector GetMandatoryVertexFrom(uint u, uint v);
	VertexQueue GetMandatoryVertexFrom(Vertex start, Vertex stop, AbstractConstraint *constraint);
	static bool ContainsElement(ShapesStack shapes, AbstractShape *);
	void SetEmpty(bool empty);
	bool GetEmpty();
	void Show();
	void ShowMandatory();
	void ShowOptional();
	void ReturnObjects(ShapeSegments *shapeSegments);
	int GetDomainParent();
	bool checkConstraints(Vertex u, AbstractShape *shape);
private:
	Graph g;
	VertexVector vertices;
	//wezy w grafie ktore sa wymagane
	VertexVector mandatoryVertices;
	//wezly w grafie ktore sa opcjonalne
	VertexVector optionalVertices;
	edge_constraint_map_t edgeConstraint;
	vertex_shape_map_t vertexShape;
	vertex_status_map_t vertexStatus;
	vertex_type_map_t vertexType;
	std::vector <Vertex>::iterator bracketStart;
	std::vector <Vertex>::iterator bracketStop;
	EdgeMap outputEdgeMap;
	EdgeMap inputEdgeMap;
	int numOfVertex;
	bool checkConstraint(Vertex u, Vertex v, Edge e);
	bool checkConstraint(AbstractShape *first, AbstractShape *second, Edge e);
	ShapeVector usedShapes;
	//zmienna informujac czy wstawiamy do grafu zaslepki
	bool empty;
	SearchVectors searchVectors;
	bool searchGraph(ShapeSegments *stack, SearchVector &searchVector);
	bool checkConstraints(Vertex src, EdgeVector &edgeVector);
	bool checkConstraints(Vertex src);
	bool goToVertex(Vertex from, Vertex to, AbstractConstraint *constraint, VertexQueue &vector);
	bool isVisited(VertexQueue &vector, Vertex u);
	EdgeVector outputEdgeVector;
	EdgeVector inputEdgeVector;
	VertexVector returnedMandatoryVertex;
	VertexVector returnedOptionalVertex;
	VertexVector returnedVertex;
};

}

}

#endif /* CSPGRAPH_HPP_ */
