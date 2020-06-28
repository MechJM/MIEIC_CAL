template <class T>
void modifiedDijkstra(Vertex<T>* origin, Vertex<T>* dest)
{  
    for (auto i : vertexSet)
    {
        i->dist = INF;
        i->path = nullptr;
    }

    MutablePriorityQueue<Vertex<T>> q;
    q.insert(s);
    while( ! q.empty() ) {
        auto v = q.extractMin();
        if (v == dest) break;
        if (!v->visited) v->visited = true;
        for(auto e : v->adj) {
            auto oldDist = e.dest->dist;
            if (oldDist > (v->dist + e.weight))
            {
                e.dest->dist = (v->dist + e.weight);
                e.dest->path = v;
            }
            if (!e.dest->visited)
            {
                if (oldDist == INF) q.insert(e.dest);
                else q.decreaseKey(e.dest);
            }
        }
    }
}

void algorithm(Vertex<T>* origin, Vertex<T>* dest, Vertex<T>* mustPassThrough)
{
    modifiedDijkstra(origin,mustPassThrough);
    modifiedDijkstra(mustPassThrough,dest);
}