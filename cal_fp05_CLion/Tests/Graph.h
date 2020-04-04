/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include "MutablePriorityQueue.h"

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
	T info;						// content of the vertex
	vector<Edge<T> > adj;		// outgoing edges
	
	double dist = 0;
	Vertex<T> *path = NULL;
	int queueIndex = 0; 		// required by MutablePriorityQueue

	bool visited = false;		// auxiliary field
	bool processing = false;	// auxiliary field

	void addEdge(Vertex<T> *dest, double w);

public:
	Vertex(T in);
	T getInfo() const;
	double getDist() const;
	Vertex *getPath() const;

	bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};


template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
	return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
	return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
	return this->path;
}

/********************** Edge  ****************************/

template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


/*************************** Graph  **************************/

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set

public:
	Vertex<T> *findVertex(const T &in) const;
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	int getNumVertex() const;
	vector<Vertex<T> *> getVertexSet() const;

	// Fp05 - single source
	void unweightedShortestPath(const T &s);    //TODO...
	void dijkstraShortestPath(const T &s);      //TODO...
	void bellmanFordShortestPath(const T &s);   //TODO...
	vector<T> getPathTo(const T &dest) const;   //TODO...

	// Fp05 - all pairs
	void floydWarshallShortestPath();   //TODO...
	vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;   //TODO...
	//Stuff required for Floyd-Warshall
    int D[10][10];
    Vertex<T>* P[10][10];
    int getVertexIndex(Vertex<T>* vert);
};

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
	if ( findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex<T>(in));
	return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == NULL || v2 == NULL)
		return false;
	v1->addEdge(v2,w);
	return true;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::unweightedShortestPath(const T &orig) {
    Vertex<T>* start;

	for (auto i : vertexSet)
    {
	    if (i->info != orig) i->dist  = 99999;
	    else {i->dist = 0; start = i;}
	    i->path = NULL;
    }

	queue<Vertex<T>*> processing;
	//start->queueIndex = 0;
	processing.push(start);

	while (!processing.empty())
    {
	    Vertex<T>* current = processing.front();
	    processing.pop();
	    for (auto i: current->adj)
        {
	        if (i.dest->dist == 99999)
            {
	            processing.push(i.dest);
	            i.dest->dist = current->dist + 1;
	            i.dest->path = current;
            }
        }
    }
}


template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
    Vertex<T>* start;
    for (auto i : vertexSet)
    {
        if (i->info != origin) i->dist  = 99999;
        else {i->dist = 0; start = i;}
        i->path = NULL;
        i->queueIndex = 0;
    }

    MutablePriorityQueue<Vertex<T>> processing;
    processing.insert(start);
    while (!processing.empty())
    {
        auto current = processing.extractMin();
        for (auto i: current->adj)
        {
            if (i.dest->dist > (current->dist + i.weight))
            {
                i.dest->dist = (current->dist + i.weight);
                i.dest->path = current;
                if (i.dest->queueIndex == 0) processing.insert(i.dest);
                else processing.decreaseKey(i.dest);
            }
        }
    }

}


template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) {
	for (auto i : vertexSet)
    {
	    if (i->info == orig) i->dist = 0;
	    else i->dist = 99999;
	    i->path = NULL;
    }

	for (int i3 = 1; i3 < vertexSet.size(); i3++)
	{
	    for (auto i: vertexSet)
        {
            for (auto i2 : i->adj)
            {
                if (i2.dest->dist > (i->dist + i2.weight))
                {
                    i2.dest->dist = i->dist + i2.weight;
                    i2.dest->path = i;
                }
            }
        }
	}

	for (auto i : vertexSet)
    {
	    for (auto i2 : i->adj)
        {
	        if ((i->dist + i2.weight) < i2.dest->dist) break;
        }
    }
}


template<class T>
vector<T> Graph<T>::getPathTo(const T &dest) const{
	vector<T> res;

	for (auto i: vertexSet)
    {
	    if (i->info == dest)
        {
	        res.push_back(i->info);
	        while (i->path != NULL)
            {
                i = i->path;
	            res.push_back(i->info);
            }
            break;
        }

    }

	reverse(res.begin(),res.end());

	return res;
}



/**************** All Pairs Shortest Path  ***************/
template <class T>
int Graph<T>::getVertexIndex(Vertex<T>* vert)
{
    for (int i = 0; i < vertexSet.size(); ++i) {
        if (vertexSet[i] == vert) return i;
    }
}

template<class T>
void Graph<T>::floydWarshallShortestPath() {
    for (int vertex = 0; vertex < vertexSet.size(); ++vertex) {
        for (auto adjEdge : vertexSet[vertex]->adj)
        {
            auto currentAdjVertex = adjEdge.dest;
            int vertex2 = getVertexIndex(currentAdjVertex);
            D[vertex][vertex2] = adjEdge.weight;
        }
    }

}

template<class T>
vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const{
	vector<T> res;
	// TODO
	return res;
}


#endif /* GRAPH_H_ */
