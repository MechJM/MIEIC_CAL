/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;


/****************** Provided structures  ********************/

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T> > adj;  // list of outgoing edges
	bool visited;          // auxiliary field used by dfs and bfs
	int indegree;          // auxiliary field used by topsort
	bool processing;       // auxiliary field used by isDAG

	void addEdge(Vertex<T> *dest, double w);
	bool removeEdgeTo(Vertex<T> *d);
public:
	Vertex(T in);
	friend class Graph<T>;
	//My additions
	bool operator==(const Vertex<T> &right);
	T getInfo();
	void setVisited(bool visited);
	bool getVisited();
};

template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;
	//My additions
	Vertex<T>* getDest();
};

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set

	void dfsVisit(Vertex<T> *v,  vector<T> & res) const;
	Vertex<T> *findVertex(const T &in) const;
	bool dfsIsDAG(Vertex<T> *v) const;
public:
	int getNumVertex() const;
	bool addVertex(const T &in);
	bool removeVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	bool removeEdge(const T &sourc, const T &dest);
	vector<T> dfs() const;
	vector<T> bfs(const T &source) const;
	vector<T> topsort() const;
	int maxNewChildren(const T &source, T &inf) const;
	bool isDAG() const;
};

/****************** Provided constructors and functions ********************/

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
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

/****************** 1a) addVertex ********************/

/*
 *  Adds a vertex with a given content/info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
	if (findVertex(in) == NULL)
    {
	    vertexSet.push_back(new Vertex<T>(in));
	    return true;
    }
	return false;
}

/****************** 1b) addEdge ********************/

/*
 * Adds an edge to a graph (this), given the contents of the source (sourc) and
 * destination (dest) vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	Vertex<T>* source = findVertex(sourc);
	Vertex<T>* destination = findVertex(dest);
	if (source != NULL && destination != NULL)
    {
	    source->addEdge(destination,w);
	    return true;
    }
	return false;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d,w));
}


/****************** 1c) removeEdge ********************/

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
	Vertex<T> *source = findVertex(sourc);
	Vertex<T> *destination = findVertex(dest);
	if (source != NULL && destination != NULL && source->removeEdgeTo(destination)) return true;
	return false;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
	for (auto it = adj.begin();  it != adj.end(); it++)
    {
	    if ((*it->getDest()) == *d)
        {
	        adj.erase(it);
	        return true;
        }
    }
	return false;
}


/****************** 1d) removeVertex ********************/

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template <class T>
bool Graph<T>::removeVertex(const T &in) {
	Vertex<T> copy(in);
	bool found = false;

	for (auto it = vertexSet.begin(); it != vertexSet.end(); it++)
    {
	    if ((*it)->getInfo() == in)
        {
	        it = vertexSet.erase(it);
	        it--;
	        found = true;
        }
	    else
        {
            (*it)->removeEdgeTo(&copy);
        }
    }

	if (found) return true;

	return false;
}


/****************** 2a) dfs ********************/

/*
 * Performs a depth-first search (dfs) in a graph (this).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
vector<T> Graph<T>::dfs() const {
	vector<T> res;

	for (auto i : vertexSet)
    {
        i->visited = false;
    }

	for (auto i : vertexSet)
    {
	    if (!i->visited) dfsVisit(i,res);
    }

	return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Updates a parameter with the list of visited node contents.
 */
template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v, vector<T> & res) const {
	v->visited = true;
	res.push_back(v->info);
	for (auto i : v->adj)
    {
	    if (!i.dest->visited) dfsVisit(i.dest,res);
    }
}

/****************** 2b) bfs ********************/

/*
 * Performs a breadth-first search (bfs) in a graph (this), starting
 * from the vertex with the given source contents (source).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
vector<T> Graph<T>::bfs(const T & source) const {
	vector<T> res;
    Vertex<T> *start;
	for (auto i : vertexSet)
	{
        i->visited = false;
        if (i->info == source) {start = i; i->visited = true;}
    }

    queue<Vertex<T>*> processLine;
    processLine.push(start);

    while(!processLine.empty())
    {
        Vertex<T> *currentVertex = processLine.front();
        processLine.pop();
        res.push_back(currentVertex->info);

        for (auto i : currentVertex->adj)
        {
            auto currentAdj = i.dest;
            if (!currentAdj->visited)
            {
                processLine.push(currentAdj);
                currentAdj->visited = true;
            }
        }
    }


	return res;
}

/****************** 2c) toposort ********************/

/*
 * Performs a topological sorting of the vertices of a graph (this).
 * Returns a vector with the contents of the vertices by topological order.
 * If the graph has cycles, returns an empty vector.
 * Follows the algorithm described in theoretical classes.
 */

template<class T>
vector<T> Graph<T>::topsort() const {
	vector<T> res;


    for (auto i : vertexSet) i->indegree = 0;
	for (auto i : vertexSet)
	{
	    for (auto i2 : i->adj)
        {
	        i2.dest->indegree++;
        }
	}
    queue<Vertex<T> *> processLine;
	for (auto i : vertexSet) if (i->indegree == 0) processLine.push(i);

	while(!processLine.empty())
    {
	    auto currentVertex = processLine.front();
	    processLine.pop();
	    res.push_back(currentVertex->info);
	    for (auto i : currentVertex->adj)
        {
	        i.dest->indegree--;
	        if (i.dest->indegree == 0) processLine.push(i.dest);
        }
    }

    if (res.size() != vertexSet.size()) res.clear();

	return res;
}

/****************** 3a) maxNewChildren (HOME WORK)  ********************/

/*
 * Performs a breadth-first search in a graph (this), starting
 * from the vertex with the given source contents (source).
 * During the search, determines the vertex that has a maximum number
 * of new children (adjacent not previously visited), and returns the
 * contents of that vertex (inf) and the number of new children (return value).
 */

template <class T>
int Graph<T>::maxNewChildren(const T & source, T &inf) const {
    int res = -99999;

    //indegree is used as a counter of new children for each node

    Vertex<T> *start;
    for (auto i : vertexSet)
    {
        i->visited = false;
        i->indegree = 0;
        if (i->info == source) {start = i; i->visited = true;}
    }

    queue<Vertex<T>*> processLine;
    processLine.push(start);

    while(!processLine.empty())
    {
        Vertex<T> *currentVertex = processLine.front();
        processLine.pop();

        for (auto i : currentVertex->adj)
        {
            auto currentAdj = i.dest;
            if (!currentAdj->visited)
            {
                currentVertex->indegree++;
                processLine.push(currentAdj);
                currentAdj->visited = true;
            }
        }
    }

    for (auto i : vertexSet)
    {
        if (i->indegree > res)
        {
            res = i->indegree;
            inf = i->info;
        }
    }

    return res;
}

/****************** 3b) isDAG   (HOME WORK)  ********************/

/*
 * Performs a depth-first search in a graph (this), to determine if the graph
 * is acyclic (acyclic directed graph or DAG).
 * During the search, a cycle is found if an edge connects to a vertex
 * that is being processed in the the stack of recursive calls (see theoretical classes).
 * Returns true if the graph is acyclic, and false otherwise.
 */

template <class T>
bool Graph<T>::isDAG() const {
	// TODO (9 lines, mostly reused)
	// HINT: use the auxiliary field "processing" to mark the vertices in the stack.
	return true;
}

/**
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Returns false (not acyclic) if an edge to a vertex in the stack is found.
 */
template <class T>
bool Graph<T>::dfsIsDAG(Vertex<T> *v) const {
	// TODO (12 lines, mostly reused)
	return true;
}

//My additions

template<class T>
bool Vertex<T>::operator==(const Vertex<T> &right)
{
    return info == right.info;
}

template<class T>
Vertex<T>* Edge<T>::getDest()
{
    return dest;
}
template<class T>
T Vertex<T>::getInfo()
{
    return info;
}

template<class T>
void Vertex<T>::setVisited(bool visited)
{
    this->visited = visited;
}

template<class T>
bool Vertex<T>::getVisited()
{
    return visited;
}


#endif /* GRAPH_H_ */
