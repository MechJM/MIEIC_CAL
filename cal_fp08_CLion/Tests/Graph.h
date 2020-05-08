/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <limits>
#include <cmath>

using namespace std;

template <class T> class Edge;
template <class T> class Graph;

constexpr auto INF = std::numeric_limits<double>::max();

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */
template <class T>
class Vertex {
	T info;
	vector<Edge<T> *> outgoing;  // adj
	vector<Edge<T> *> incoming;
	Edge<T> * addEdge(Vertex<T> *dest, double c, double f);
	Vertex(T in);

	bool visited;  // for path finding
	Edge<T> *path; // for path finding

public:
	T getInfo() const;
	vector<Edge<T> *> getAdj() const;
	friend class Graph<T>;
};


template <class T>
Vertex<T>::Vertex(T in): info(in) {
}

template <class T>
Edge<T> *Vertex<T>::addEdge(Vertex<T> *dest, double c, double f) {
	Edge<T> * e = new Edge<T>(this, dest, c, f);
	this->outgoing.push_back(e);
	dest->incoming.push_back(e);
	return e;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
vector<Edge<T> *> Vertex<T>::getAdj() const {
	return this->outgoing;
}


/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */
template <class T>
class Edge {
	Vertex<T> * orig;
	Vertex<T> * dest;
	double capacity;
	double flow;
	Edge(Vertex<T> *o, Vertex<T> *d, double c, double f=0);

public:
	double getFlow() const;
	Vertex<T> *getDest() const;

	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *o, Vertex<T> *d, double w, double f): orig(o), dest(d), capacity(w), flow(f){}

template <class T>
double Edge<T>::getFlow() const {
	return flow;
}

template <class T>
Vertex<T>* Edge<T>::getDest() const {
	return dest;
}


/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */
template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;
	Vertex<T>* findVertex(const T &inf) const;
public:
	vector<Vertex<T> *> getVertexSet() const;
	Vertex<T> *addVertex(const T &in);
	Edge<T> *addEdge(const T &sourc, const T &dest, double c, double f=0);
	void fordFulkerson(T source, T target);
    ///Auxiliary functions
    bool findAugmentationPath(T source, T target);
    void testAndVisit(queue<Vertex<T>*> &Q, Edge<T>* edge, Vertex<T>* dest, double residual);
    double findMinResidualPath(T source, T target);
    void augmentFlowPath(T source, T target, double flow);
};

template <class T>
Vertex<T> * Graph<T>::addVertex(const T &in) {
	Vertex<T> *v = findVertex(in);
	if (v != nullptr)
		return v;
	v = new Vertex<T>(in);
	vertexSet.push_back(v);
	return v;
}

template <class T>
Edge<T> * Graph<T>::addEdge(const T &sourc, const T &dest, double c, double f) {
	auto s = findVertex(sourc);
	auto d = findVertex(dest);
	if (s == nullptr || d == nullptr)
		return nullptr;
	else
		return s->addEdge(d, c, f);
}

template <class T>
Vertex<T>* Graph<T>::findVertex(const T & inf) const {
	for (auto v : vertexSet)
		if (v->info == inf)
			return v;
	return nullptr;
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}


/**
 * Finds the maximum flow in a graph using the Ford Fulkerson algorithm
 * (with the improvement of Edmonds-Karp).
 * Assumes that the graph forms a flow network from source vertex 's'
 * to sink vertex 't' (distinct vertices).
 * Receives as arguments the source and target vertices (identified by their contents).
 * The result is defined by the "flow" field of each edge.
 */
template <class T>
void Graph<T>::fordFulkerson(T source, T target) {
    //resetFlows
    for (auto vert : vertexSet)
    {
        for (auto edge : vert->outgoing)
        {
            edge->flow = 0;
        }
    }

    double flow;
    while (findAugmentationPath(source,target))
    {
        flow = findMinResidualPath(source,target);
        augmentFlowPath(source,target,flow);
    }
}

template <class T>
bool Graph<T>::findAugmentationPath(T source, T target)
{
    Vertex<T> *start,*end;
    for (auto vert : vertexSet)
    {
        if (vert->info == source) start = vert;
        if (vert->info == target) end = vert;
        vert->visited = false;
    }
    start->visited = true;

    queue<Vertex<T>*> processingLine;
    processingLine.push(start);

    while (!processingLine.empty() && !(end->visited))
    {
        Vertex<T>* currentVert = processingLine.front();
        processingLine.pop();

        for (auto outEdge : currentVert->outgoing)
        {
            testAndVisit(processingLine,outEdge,outEdge->dest,outEdge->capacity - outEdge->flow);
        }
        for (auto inEdge : currentVert->incoming)
        {
            testAndVisit(processingLine,inEdge,inEdge->orig,inEdge->flow);
        }
    }
    return end->visited;
}

template <class T>
void Graph<T>::testAndVisit(queue<Vertex<T>*> &Q, Edge<T>* edge, Vertex<T>* dest, double residual)
{
    if (!dest->visited && residual > 0)
    {
        dest->visited = true;
        dest->path = edge;
        Q.push(dest);
    }
}

template <class T>
double Graph<T>::findMinResidualPath(T source, T target)
{
    double flow = 9999999;
    Vertex<T>* start,*end;
    for (auto vert : vertexSet)
    {
        if (vert->info == source) start = vert;
        if (vert->info == target) end = vert;
    }

    Vertex<T>* currentVert = end;
    while (currentVert != start)
    {
        Edge<T>* currentEdge = currentVert->path;
        if (currentEdge->dest == currentVert)
        {
            flow = min(flow,currentEdge->capacity - currentEdge->flow);
            currentVert = currentEdge->orig;
        }
        else
        {
            flow = min(flow,currentEdge->flow);
            currentVert = currentEdge->dest;
        }
    }
    return flow;
}

template <class T>
void Graph<T>::augmentFlowPath(T source, T target, double flow)
{
    Vertex<T> *start,*end;
    for (auto vert : vertexSet)
    {
        if (vert->info == source) start = vert;
        if (vert->info == target) end = vert;
    }
    Vertex<T>* currentVert = end;
    while (currentVert != start)
    {
        Edge<T>* currentEdge = currentVert->path;
        if (currentEdge->dest == currentVert)
        {
            currentEdge->flow += flow;
            currentVert = currentEdge->orig;
        }
        else
        {
            currentEdge->flow -= flow;
            currentVert = currentEdge->dest;
        }
    }
}



#endif /* GRAPH_H_ */
