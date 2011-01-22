/*
 * CspGraph.cpp
 *
 *  Created on: 09-01-2011
 *      Author: mateusz
 */

#include <iostream>

#include "CspGraph.hpp"
#include "Logger.hpp"

namespace Processors {
namespace CspObjectRecognize {

using namespace std;
using boost::get;

namespace Settings {
enum Settings
{
	NOT_DEFINED = 3456
};
}

CspGraph::CspGraph()
{
	g.clear();
	edgeConstraint = get(boost::edge_constraint, g);
	vertexShape = get(boost::vertex_shape, g);
	//	vertexStatus = get(boost::vertex_status, g);
	vertexType = get(boost::vertex_type, g);

	usedShapes.clear();
	empty = false;
}

CspGraph::~CspGraph()
{
	g.clear();
}

bool CspGraph::init(int numMandatory, int numOptional, bool empty)
{
	ClearAll();
	//dodawanie wezlow wymaganych
	for (int i = 0; i < numMandatory; i++) {
		Vertex tmp = add_vertex(g);
		InsertVertexValue(boost::shared_ptr <AbstractShape>(), tmp);
		//		SetVertexStatus(MANDATORY, tmp);
		SetVertexType(tmp, Settings::NOT_DEFINED);
		vertices.push_back(tmp);
		mandatoryVertices.push_back(tmp);
	}
	//dodawanie wezlow opcjonalnych
	for (int i = 0; i < numOptional; i++) {
		Vertex tmp = add_vertex(g);
		InsertVertexValue(boost::shared_ptr <AbstractShape>(), tmp);
		//		SetVertexStatus(OPTIONAL, tmp);
		SetVertexType(tmp, Settings::NOT_DEFINED);
		vertices.push_back(tmp);
		optionalVertices.push_back(tmp);
	}
	this->empty = empty;
	return true;
}

VertexVector &CspGraph::GetVertex()
{
	return vertices;
}

VertexVector &CspGraph::GetMandatoryVertex(Vertex u, boost::shared_ptr <AbstractConstraint> constraint)
{
	returnedMandatoryVertex.clear();
	//pobieramy krawedzie wychodzace od danego wezla
	EdgeVector edgeOutPutVector = GetOutPutEdge(u, constraint);

	for (uint i = 0; i < edgeOutPutVector.size(); i++) {
		Edge edge = edgeOutPutVector[i];
		Vertex dst = GetSourceEdge(edge); //docelowe wezly
		if (dst != u && FindInMandatory(dst)) {
			ConstraintVector constraintVector = edgeConstraint[edge];
			for (uint k = 0; k < constraintVector.size(); k++) {
				if (constraintVector[k] == constraint) {
					returnedMandatoryVertex.push_back(dst);
					break;
				}
			}
		}
	}
	return returnedMandatoryVertex;
}

bool CspGraph::AddEdge(Vertex u, Vertex v, boost::shared_ptr <AbstractConstraint> constraint)
{
	bool inserted = false;
	ConstraintVector constraintVector;
	Edge e;
	tie(e, inserted) = edge(u, v, g);
	if (inserted) {
		edgeConstraint[e].push_back(constraint);
	} else {
		tie(e, inserted) = add_edge(u, v, g);
		put(edgeConstraint, e, constraintVector);
		edgeConstraint[e].push_back(constraint);

	}

	return inserted;
}

void CspGraph::InsertVertexValue(boost::shared_ptr <AbstractShape> shape, Vertex v)
{
	vertexShape[v] = shape;
}

boost::shared_ptr <AbstractShape> CspGraph::GetVertexValue(Vertex v)
{
	return vertexShape[v];
}

void CspGraph::SetVertexType(Vertex v, int kind)
{
	vertexType[v] = kind;
}

int CspGraph::GetVertexType(Vertex v)
{
	return vertexType[v];
}

Vertex CspGraph::GetSourceEdge(Edge e)
{
	return target(e, g);
}

EdgeVector &CspGraph::GetOutPutEdge(Vertex v)
{
	if (outputEdgeMap.size() > 0) {
		return outputEdgeMap[v];
	} else {
		OutEdgeIterator i, end;
		outputEdgeVector.clear();
		for (tie(i, end) = out_edges(v, g); i != end; ++i) {
			outputEdgeVector.push_back(*i);
		}
		return outputEdgeVector;
	}
}

EdgeVector &CspGraph::GetOutPutEdge(Vertex v, boost::shared_ptr <AbstractConstraint> constraint)
{
	OutEdgeIterator i, end;
	outputEdgeVector.clear();
	for (tie(i, end) = out_edges(v, g); i != end; ++i) {
		ConstraintVector constraintVector = edgeConstraint[*i];
		for (uint j = 0; j < constraintVector.size(); j++) {
			if (constraintVector[j] == constraint) {
				outputEdgeVector.push_back(*i);
			}
		}

	}
	return outputEdgeVector;
}

void CspGraph::InitInputOutputEdgeMap()
{
	//wezly przychodzace
	InEdgeIterator in_i, in_end;

	OutEdgeIterator out_i, out_end;

	Vertex v;

	for (uint counter = 0; counter < vertices.size(); counter++) {
		EdgeVector input;
		EdgeVector output;

		v = vertices[counter];
		//krawedzie przychodzace do wezla
		for (tie(in_i, in_end) = in_edges(v, g); in_i != in_end; ++in_i) {
			input.push_back(*in_i);
		}
		inputEdgeMap.insert(make_pair(v, input));
		//krawedzie wychodzace z wezla
		for (tie(out_i, out_end) = out_edges(v, g); out_i != out_end; ++out_i) {
			output.push_back(*out_i);
		}
		outputEdgeMap.insert(make_pair(v, output));
	}
}

bool CspGraph::FindCspShape(ShapeSegments *stack)
{
	int numSearch = -1;

	bool returnedValue = false;
	uint stop;
	Clear();
	stop = mandatoryVertices.size();
	SearchVector searchVector;

	if (!stack->empty() && searchVectors.size() == 0) {
		cout << "Pusty sciezka przeszukiwania" << endl;
		exit(1);
	}
	if (!stack->empty() && searchVectors.size() > 0) {
		for (uint i = 0; i < searchVectors.size(); i++) {
			if (searchVectors[i].size() > 0) {
				searchVector = searchVectors[i];
				if ((returnedValue = searchGraph(stack, searchVector)) == true) {
					numSearch = i;
					break;
				}
			}
		}
	}
	return returnedValue;
}

bool CspGraph::searchGraph(ShapeSegments *stack, SearchVector &searchVector)
{
	boost::shared_ptr <AbstractShape> shape;
	PairSearch pairSearch;
	bool returnedValue = false;
	bool status;
	bool typeVerify = true;
	int type;

	if (!stack->empty() && searchVector.size() > 0)//iterator<stop pobranie obiektu ze stosu
	{
		//pobieramy kolejny element do przeszukiwania
		pairSearch = searchVector.front();
		searchVector.pop_front();

		Vertex src = pairSearch.first;
		status = pairSearch.second;
		type = GetVertexType(src);
		//w pierwszej czesci wstawiamy ksztalty a gdy to sie nie uda to bierzemy zaslepke
		EdgeVector edgeVector = GetOutPutEdge(src);
		for (uint i = 0; i < stack->size(); i++) {
			shape = stack->front();
			stack->pop_front();
			if (type != Settings::NOT_DEFINED) {
				if (type != shape->getType()) {
					//brak zgodnosci typów w wezle
					typeVerify = false;
				} else {
					typeVerify = true;
				}
			} else {
				typeVerify = true;
			}

			if (typeVerify) {
				InsertVertexValue(shape, src);

				if (checkConstraints(src, edgeVector)) {
					returnedValue = searchGraph(stack, searchVector);
					//znaleziono warunek stopu
					if (returnedValue == true) {
						break;
					}
				} else {
					returnedValue = false;
				}
				InsertVertexValue(boost::shared_ptr <AbstractShape>(), src);
			}
			stack->push_back(shape);
		}
		//próbujemy wstawic zaslepke
		if (!returnedValue) {

			if (!status) {
				//wstaawiamy zaslepke
				InsertVertexValue(boost::shared_ptr <AbstractShape>(), src);
				returnedValue = searchGraph(stack, searchVector);
			} else//nie moznaq wstawic zaslepki
			{
				returnedValue = false;
			}
		}
		searchVector.push_front(pairSearch);

	} else if (stack->empty() && searchVector.size() > 0)//jest za malo elementów niż wezlow w grafie
	{
		//		sprawdzamy czy mamy jakies wymagane czy nie
		returnedValue = true;
		for (uint i = 0; i < searchVector.size(); i++) {
			PairSearch sv = searchVector[i];
			//jezeli nie ma wymaganych to uznajemy ze model jest pelny
			if (sv.second == true) {
				returnedValue = false;
				break;
			}
		}
	} else//graf zostal wypelniony
	{
		returnedValue = true;
	}
	return returnedValue;
}

bool CspGraph::checkConstraints(Vertex src, EdgeVector &edgeVector)
{
	boost::shared_ptr <AbstractShape> first;
	boost::shared_ptr <AbstractShape> second;
	first = GetVertexValue(src);
	for (uint j = 0; j < edgeVector.size(); j++) {
		Edge edge = edgeVector[j];

		Vertex dst = GetSourceEdge(edge); //docelowe wezly
		second = GetVertexValue(dst);
		if (first != NULL && second != NULL) {
			if (!checkConstraint(first, second, edge)) {
				return false;
			}
		}
	}
	return true;
}

bool CspGraph::checkConstraint(Vertex u, Vertex v, Edge e)
{
	boost::shared_ptr <AbstractShape> src = GetVertexValue(v);

	boost::shared_ptr <AbstractShape> dst = GetVertexValue(u);

	return checkConstraint(src, dst, e);
}

bool CspGraph::checkConstraint(boost::shared_ptr <AbstractShape> first, boost::shared_ptr <AbstractShape> second, Edge e)
{
	ConstraintVector constraintVector = edgeConstraint[e];
	if (first != NULL && second != NULL)//jak ktores jest nullem to zakladamy ze ograniczenia sa spelnione
	{
		for (uint i = 0; i < constraintVector.size(); i++) {
			if (!constraintVector[i]->isSatisifed(first, second)) {
				return false;
			}
		}
	}
	return true;
}
void CspGraph::Clear()
{
	for (uint i = 0; i < vertices.size(); i++) {
		vertexShape[vertices[i]] = boost::shared_ptr <AbstractShape>();
	}
}

void CspGraph::ClearAll()
{
	vertices.clear();
	mandatoryVertices.clear();
	optionalVertices.clear();
	//	edgeConstraint.clear();
	//	vertexShape.clear();
	//	vertexStatus.clear();
	usedShapes.clear();
	searchVectors.clear();
	g.clear();
}

bool CspGraph::goToVertex(Vertex from, Vertex to, boost::shared_ptr <AbstractConstraint> constraint, VertexQueue &vertexQueue)
{
	VertexVector vector = GetMandatoryVertex(from, constraint);
	bool status = false;
	for (uint i = 0; i < vector.size(); i++) {
		if (from == to) {
			//osiagnieto warunek stopu
			vertexQueue.push_back(vector[i]);
			status = true;
			break;
		} else if (vector[i] != from) {

			if (isVisited(vertexQueue, vector[i])) {
				status = false;
				break;
			} else {
				vertexQueue.push_back(vector[i]);
				status = goToVertex(vector[i], to, constraint, vertexQueue);
				if (!status) {//zdejmujemy wezel ostatni
					vertexQueue.pop_back();
				} else {//znaleziono warunek stopu
					break;
				}
			}
		}
	}
	return status;
}

bool CspGraph::isVisited(VertexQueue &queue, Vertex u)
{
	VertexQueueIterator iterator = find(queue.begin(), queue.end(), u);
	if (iterator != queue.end()) {
		return true;
	}
	return false;
}

ShapeVector &CspGraph::GetAllUsedShapesVector()
{
	//	LOG(LFATAL) << "\n\nCspGraph::GetAllUsedShapesVector() begin";
	boost::shared_ptr <AbstractShape> tmp;
	usedShapes.clear();

	for (uint i = 0; i < mandatoryVertices.size(); i++) {
		//		LOG(LFATAL) << "mandatoryVertices";
		if ((tmp = GetVertexValue(mandatoryVertices[i])) != NULL) {
			//			LOG(LFATAL) << "mandatoryVertices ....";
			usedShapes.push_back(tmp);
		}
	}
	for (uint i = 0; i < optionalVertices.size(); i++) {
		//		LOG(LFATAL) << "optionalVertices";
		if ((tmp = GetVertexValue(optionalVertices[i])) != NULL) {
			//			LOG(LFATAL) << "optionalVertices ....";
			usedShapes.push_back(tmp);
		}
	}
	//	LOG(LFATAL) << "CspGraph::GetAllUsedShapesVector() end\n\n";
	return usedShapes;
}

bool CspGraph::FindInMandatory(Vertex u)
{
	for (uint i = 0; i < mandatoryVertices.size(); i++) {
		if (mandatoryVertices[i] == u) {
			return true;
		}
	}
	return false;
}

bool CspGraph::checkConstraint(boost::shared_ptr <AbstractConstraint> constraint, Vertex u, boost::shared_ptr <
		AbstractShape> shape)
{
	EdgeVector edgeVector = GetOutPutEdge(u, constraint);

	for (uint i = 0; i < edgeVector.size(); i++) {
		Vertex v = GetSourceEdge(edgeVector[i]);
		if (checkConstraint(u, v, edgeVector[i])) {
			return true;
		}
	}
	return false;
}

bool CspGraph::InitSearchGraph()
{
	searchVectors.clear();
	return true;
}

bool CspGraph::NextSearchVector()
{
	SearchVector searchVector;
	searchVectors.push_back(searchVector);
	return true;
}
void CspGraph::AddSearchVertex(Vertex numVertex, bool status)
{
	PairSearch pair;
	pair.first = numVertex;
	pair.second = status;
	if (searchVectors.size() > 0) {
		searchVectors[searchVectors.size() - 1].push_back(pair);
	} else {
		NextSearchVector();
		searchVectors[searchVectors.size() - 1].push_back(pair);
	}
}

bool CspGraph::checkConstraints(Vertex src, boost::shared_ptr <AbstractShape> shape)
{
	InsertVertexValue(shape, src);
	EdgeVector edgeVector = GetOutPutEdge(src);
	bool passed = true;
	for (uint j = 0; j < edgeVector.size(); j++) {
		Edge edge = edgeVector[j];
		Vertex dst = GetSourceEdge(edge); //docelowe wezly

		//sprawdzanie ograniczen
		if (!checkConstraint(src, dst, edge)) {
			passed = false;
			InsertVertexValue(boost::shared_ptr <AbstractShape>(), src);
			break;
		}
	}
	return passed;
}

/**
 * Metoda zwraca wszystkie wezly spelniajace ograniczenie wzgledem wezla u
 */
//VertexVector &CspGraph::GetVertex(Vertex u, boost::shared_ptr <AbstractConstraint> constraint)
//{
//	returnedVertex.clear();
//	//pobieramy krawedzie wychodzace od danego wezla
//	EdgeVector edgeOutPutVector = GetOutPutEdge(u, constraint);
//
//	for (uint i = 0; i < edgeOutPutVector.size(); i++) {
//		Edge edge = edgeOutPutVector[i];
//		Vertex dst = GetSourceEdge(edge); //docelowe wezly
//
//		if (dst != u) {
//			ConstraintVector constraintVector = edgeConstraint[edge];
//			for (uint k = 0; k < constraintVector.size(); k++) {
//				if (constraintVector[k] == constraint) {
//					boost::shared_ptr <AbstractShape> abstractShape = GetVertexValue(dst);
//					if (abstractShape != NULL) {
//						returnedVertex.push_back(dst);
//						break;
//					}
//				}
//			}
//		}
//	}
//	return returnedVertex;
//}
/**
 * Metoda zwraca wymagane wezly
 */
//VertexVector &CspGraph::GetMandatoryVertex()
//{
//	return mandatoryVertices;
//}
/**
 * Metoda zwraca wezly wymagane w zaleznosci od flagi puste albo wypelnione
 */
//VertexVector &CspGraph::GetMandatoryVertex(bool empty)
//{
//	returnedMandatoryVertex.clear();
//	for (uint i = 0; i < mandatoryVertices.size(); i++) {
//		boost::shared_ptr <AbstractShape> shape = GetVertexValue(mandatoryVertices[i]);
//		if (empty && shape == NULL) {
//			returnedMandatoryVertex.push_back(mandatoryVertices[i]);
//		} else if (!empty && shape != NULL) {
//			returnedMandatoryVertex.push_back(mandatoryVertices[i]);
//		}
//	}
//	return returnedMandatoryVertex;
//}
/**
 * Metoda zwraca opcjonalne wezly
 */
//VertexVector &CspGraph::GetOptionalVertex()
//{
//	return optionalVertices;
//}

/**
 * Metoda zwraca wezly opcjonalne w zaleznosci od flagi puste albo wypelnione
 */
//VertexVector &CspGraph::GetOptionalVertex(bool empty)
//{
//	returnedOptionalVertex.clear();
//	for (uint i = 0; i < optionalVertices.size(); i++) {
//		boost::shared_ptr <AbstractShape> shape = GetVertexValue(optionalVertices[i]);
//		if (empty && shape == NULL) {
//			returnedOptionalVertex.push_back(optionalVertices[i]);
//		} else if (!empty && shape != NULL) {
//			returnedOptionalVertex.push_back(optionalVertices[i]);
//		}
//	}
//	return returnedOptionalVertex;
//}

/**
 * Metoda zwraca opcjonalne wezly spelniajace podane ograniczenie
 */
//VertexVector &CspGraph::GetOptionalVertex(Vertex u, boost::shared_ptr <AbstractConstraint> constraint)
//{
//	returnedOptionalVertex.clear();
//	//pobieramy krawedzie wychodzace od danego wezla
//	EdgeVector edgeOutPutVector = GetOutPutEdge(u, constraint);
//
//	for (uint i = 0; i < edgeOutPutVector.size(); i++) {
//		Edge edge = edgeOutPutVector[i];
//		Vertex dst = GetSourceEdge(edge); //docelowe wezly
//		if (FindInOptional(dst)) {
//			if (dst != u) {
//				ConstraintVector constraintVector = edgeConstraint[edge];
//				for (uint k = 0; k < constraintVector.size(); k++) {
//					if (constraintVector[k] == constraint) {
//						returnedOptionalVertex.push_back(dst);
//						break;
//					}
//				}
//			}
//		}
//	}
//	return returnedOptionalVertex;
//}
/**
 * Metoda ustawia status wezla
 */
//void CspGraph::SetVertexStatus(int status, Vertex v)
//{
//	vertexStatus[v] = status;
//}
/**
 * Metoda pobiera status wezla
 */
//int CspGraph::GetVertexStatus(Vertex v)
//{
//	return vertexStatus[v];
//}
/**
 * Pobieranie ograniczen dla danej krawedzi
 */
//ConstraintVector &CspGraph::GetConstraints(Edge e)
//{
//	return edgeConstraint[e];
//}
/**
 * Metodoa pobiera wezel docelowy dla danej krawedzi
 */
//Vertex CspGraph::GetTargetEdge(Edge e)
//{
//	return source(e, g);
//}
/**
 * Metoda zwraca krawedzie polaczone z danym wezlem jako przychodzace polaczenia
 */
//EdgeVector &CspGraph::GetInPutEdge(Vertex v, boost::shared_ptr <AbstractConstraint> constraint)
//{
//	InEdgeIterator i, end;
//	inputEdgeVector.clear();
//	for (tie(i, end) = in_edges(v, g); i != end; ++i) {
//		ConstraintVector constraintVector = edgeConstraint[*i];
//		for (uint j = 0; j < constraintVector.size(); j++) {
//			if (constraintVector[j] == constraint) {
//				inputEdgeVector.push_back(*i);
//			}
//		}
//	}
//	return inputEdgeVector;
//}
/**
 * Metoda zwraca krawedzie polaczone z danym wezlem jako przychodzace polaczenia
 * i spelniajace ograniczenie
 */
//EdgeVector &CspGraph::GetInPutEdge(Vertex v)
//{
//	if (inputEdgeMap.size() > 0) {
//		return inputEdgeMap[v];
//	} else {
//		InEdgeIterator i, end;
//		inputEdgeVector.clear();
//		for (tie(i, end) = in_edges(v, g); i != end; ++i) {
//			inputEdgeVector.push_back(*i);
//		}
//		return inputEdgeVector;
//	}
//}
/**
 * Metoda do przeszukiwania grafu ,
 * SearchVector-vectorprzeszukiwania wezlow
 */
//bool CspGraph::FindCspShape(ShapeSegments *stack, SearchVector &searchVector)
//{
//	return searchGraph(stack, searchVector);
//}
/**
 * Metoda sprawdza ograniczenia
 * src -wezel zrodlowy
 */
//bool CspGraph::checkConstraints(Vertex src)
//{
//	EdgeVector edgeVector = GetOutPutEdge(src);
//	for (uint j = 0; j < edgeVector.size(); j++) {
//		Edge edge = edgeVector[j];
//		Vertex dst = GetSourceEdge(edge); //docelowe wezly
//
//		//sprawdzanie ograniczen
//		if (!checkConstraint(src, dst, edge)) {
//			return false;
//		}
//	}
//	return true;
//}
//Vertex &CspGraph::getVertex(int numVertex)
//{
//	return vertices[numVertex];
//}
//void CspGraph::Clear(uint from, uint to)
//{
//	for (uint i = from; i < to; i++) {
//		InsertVertexValue(boost::shared_ptr <AbstractShape>(), vertices[i]);
//	}
//}

//VertexVector CspGraph::GetMandatoryVertexFrom(uint u, uint v)
//{
//	returnedMandatoryVertex.clear();
//	for (uint i = u; i < v; i++) {
//		returnedMandatoryVertex.push_back(i);
//	}
//	return returnedMandatoryVertex;
//}

/**
 * Metoda przechodzi od wezla start do stop tworzac sciezke, wybierane wezly sa polaczone krawedziami zawierajacymi ograniczenie cosntraint
 */
//VertexQueue CspGraph::GetMandatoryVertexFrom(Vertex start, Vertex stop, boost::shared_ptr <AbstractConstraint> constraint)
//{
//	//TODO zle dziala nie mozna iterowac od poczatku w kolejnych zaglebieniach poniewaz powstanie blad za szybko sie skonczy
//	VertexQueue vector;
//	goToVertex(start, stop, constraint, vector);
//	return vector;
//}
/**
 * Metoda zwraca liczbe wezlow pustych
 */
//int CspGraph::NumEmptyMandatoryVertices()
//{
//	int count = 0;
//	boost::shared_ptr <AbstractShape> tmp;
//	for (uint i = 0; i < mandatoryVertices.size(); i++) {
//		tmp = GetVertexValue(mandatoryVertices[i]);
//		if (tmp == NULL) {
//			count++;
//		}
//	}
//	return count;
//}
/**
 * Metoda zwraca liczbe wszystkich wezlow wymaganych
 */
//int CspGraph::NumMandatoryVertices()
//{
//	return mandatoryVertices.size();
//}
/**
 * Metoda zwraca liczbe wezlow pustych
 */
//int CspGraph::NumEmptyOptionalVertices()
//{
//	int count = 0;
//	boost::shared_ptr <AbstractShape> tmp;
//	for (uint i = 0; i < optionalVertices.size(); i++) {
//		tmp = GetVertexValue(optionalVertices[i]);
//		if (tmp == NULL) {
//			count++;
//		}
//	}
//	return count;
//}
/**
 * Metoda zwraca liczbe wszystkich wezlow opcjonalnych
 */
//int CspGraph::NumOptionalVertices()
//{
//	return optionalVertices.size();
//}
/**
 * Metoda zwraca liste ksztaltow uzytych podaczas detekcji obiektu
 */
//ShapeVector &CspGraph::GetUsedShapesVector()
//{
//	boost::shared_ptr <AbstractShape> tmp;
//	usedShapes.clear();
//
//	for (uint i = 0; i < mandatoryVertices.size(); i++) {
//		if ((tmp = GetVertexValue(mandatoryVertices[i])) != NULL) {
//			usedShapes.push_back(tmp);
//		}
//	}
//	return usedShapes;
//}
/**
 * Metoda sprawdza czy wezel jest wsrod opcjonalnych wezlow
 */
//bool CspGraph::FindInOptional(Vertex u)
//{
//	for (uint i = 0; i < optionalVertices.size(); i++) {
//		if (optionalVertices[i] == u) {
//			return true;
//		}
//	}
//	return false;
//}
/**
 * Metoda sprawdza czy podsany element jest na liscie
 */
//bool CspGraph::ContainsElement(ShapesStack shapes, boost::shared_ptr <AbstractShape> shape)
//{
//	ShapesStackIterator iter = find(shapes.begin(), shapes.end(), shape);
//
//	if (iter != shapes.end()) {
//		return true;
//	}
//	return false;
//}

//void CspGraph::SetEmpty(bool empty)
//{
//	this->empty = empty;
//}

//bool CspGraph::GetEmpty()
//{
//	return empty;
//}

//void CspGraph::Show()
//{
//	cout << "CSPGraph Show:" << endl;
//	for (uint i = 0; i < vertices.size(); i++) {
//		cout << "Wezel :" << i << " Wartosc :" << GetVertexValue(vertices[i]) << endl;
//	}
//}
//void CspGraph::ShowMandatory()
//{
//	cout << "Wezly wymagane:" << endl;
//	for (uint i = 0; i < mandatoryVertices.size(); i++) {
//		cout << "Wezel :" << i << " Wartosc :" << GetVertexValue(mandatoryVertices[i]) << endl;
//	}
//}
//void CspGraph::ShowOptional()
//{
//	cout << "Wszytskie opcjonalne :" << endl;
//	for (uint i = 0; i < optionalVertices.size(); i++) {
//		cout << "Wezel :" << i << " Wartosc :" << GetVertexValue(optionalVertices[i]) << endl;
//	}
//}
//void CspGraph::ClearSearchVectors()
//{
//	searchVectors.clear();
//}
/**
 * Metoda zwraca objekty na liste
 */
//void CspGraph::ReturnObjects(ShapeSegments *shapeSegments)
//{
//
//}

} // namespace Processors
} // namespace CspObjectRecognize
