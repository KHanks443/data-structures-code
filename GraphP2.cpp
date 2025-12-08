#include "graph.h"
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>
using namespace std;

// This constructor initializes all matrix values, visited array,
// and prepares Union-Find parent and rank arrays.
Graph::Graph(int maxV)
{
    maxVertices = maxV;
    numVertices = 0;
    vertices = nullptr;
    visited = new bool[maxVertices];

    adjMatrix = new int*[maxVertices];
    for(int i = 0; i < maxVertices; i++)
    {
        adjMatrix[i] = new int[maxVertices];
        for(int j = 0; j < maxVertices; j++)
            adjMatrix[i][j] = 0;
    }

    parentUF = new int[maxVertices];
    rankUF = new int[maxVertices];
    for(int i = 0; i < maxVertices; i++)
    {
        parentUF[i] = i;
        rankUF[i] = 0;
    }
}

// This destructor frees all dynamic memory used by the graph.
Graph::~Graph()
{
    for(int i = 0; i < maxVertices; i++)
        delete[] adjMatrix[i];
    delete[] adjMatrix;
    delete[] visited;

    delete[] parentUF;
    delete[] rankUF;

    while(vertices)
    {
        vertex* temp = vertices;
        while(temp->aHead)
        {
            edge* e = temp->aHead;
            temp->aHead = temp->aHead->eNext;
            delete e;
        }
        vertices = vertices->vNext;
        delete temp;
    }
}

// This function finds a vertex node based on its character label.
vertex* Graph::getVertex(char d)
{
    vertex* temp = vertices;
    while(temp)
    {
        if(temp->data == d) return temp;
        temp = temp->vNext;
    }
    return nullptr;
}

// This function resets all visitation states to false.
void Graph::resetVisited()
{
    for(int i = 0; i < maxVertices; i++)
        visited[i] = false;
}

// This function adds a new vertex to the linked list of vertices.
void Graph::addVertex(char data)
{
    if(numVertices >= maxVertices) return;
    vertex* v = new vertex;
    v->data = data;
    v->aHead = nullptr;
    v->vNext = vertices;
    vertices = v;
    numVertices++;
}

// This function adds an undirected edge between two vertices.
void Graph::addEdge(char src, char dest, int weight)
{
    vertex* s = getVertex(src);
    vertex* d = getVertex(dest);
    if(!s || !d) return;

    edge* e = new edge;
    e->dest = d;
    e->weight = weight;
    e->eNext = s->aHead;
    s->aHead = e;

    edge* f = new edge;
    f->dest = s;
    f->weight = weight;
    f->eNext = d->aHead;
    d->aHead = f;

    int i = src - 'A';
    int j = dest - 'A';
    adjMatrix[i][j] = weight;
    adjMatrix[j][i] = weight;
}

// This function removes an undirected edge between two vertices.
void Graph::removeEdge(char src, char dest)
{
    vertex* s = getVertex(src);
    if(!s) return;

    edge* prev = nullptr;
    edge* curr = s->aHead;

    while(curr)
    {
        if(curr->dest->data == dest)
        {
            if(prev) prev->eNext = curr->eNext;
            else s->aHead = curr->eNext;
            delete curr;
            break;
        }
        prev = curr;
        curr = curr->eNext;
    }

    vertex* d = getVertex(dest);
    if(!d) return;

    prev = nullptr;
    curr = d->aHead;

    while(curr)
    {
        if(curr->dest->data == src)
        {
            if(prev) prev->eNext = curr->eNext;
            else d->aHead = curr->eNext;
            delete curr;
            break;
        }
        prev = curr;
        curr = curr->eNext;
    }

    int i = src - 'A';
    int j = dest - 'A';
    adjMatrix[i][j] = 0;
    adjMatrix[j][i] = 0;
}

// This function performs BFS using adjacency lists.
void Graph::bfsL(char start)
{
    resetVisited();
    queue<vertex*> q;

    vertex* s = getVertex(start);
    if(!s) return;

    q.push(s);
    visited[start - 'A'] = true;

    while(!q.empty())
    {
        vertex* v = q.front();
        q.pop();
        cout << v->data << " ";

        edge* e = v->aHead;
        while(e)
        {
            int idx = e->dest->data - 'A';
            if(!visited[idx])
            {
                visited[idx] = true;
                q.push(e->dest);
            }
            e = e->eNext;
        }
    }
}

// This function performs BFS using the adjacency matrix.
void Graph::bfsM(char start)
{
    resetVisited();
    queue<int> q;

    int s = start - 'A';
    visited[s] = true;
    q.push(s);

    while(!q.empty())
    {
        int u = q.front();
        q.pop();
        cout << char(u + 'A') << " ";

        for(int v = 0; v < maxVertices; v++)
        {
            if(adjMatrix[u][v] != 0 && !visited[v])
            {
                visited[v] = true;
                q.push(v);
            }
        }
    }
}

// This function performs DFS using adjacency lists.
void Graph::dfsL(char start)
{
    resetVisited();
    stack<vertex*> st;

    vertex* s = getVertex(start);
    if(!s) return;

    st.push(s);

    while(!st.empty())
    {
        vertex* v = st.top();
        st.pop();

        int idx = v->data - 'A';
        if(visited[idx]) continue;

        visited[idx] = true;
        cout << v->data << " ";

        edge* e = v->aHead;
        while(e)
        {
            int di = e->dest->data - 'A';
            if(!visited[di]) st.push(e->dest);
            e = e->eNext;
        }
    }
}

// This function performs DFS using the adjacency matrix.
void Graph::dfsM(char start)
{
    resetVisited();
    stack<int> st;

    int s = start - 'A';
    st.push(s);

    while(!st.empty())
    {
        int u = st.top();
        st.pop();

        if(visited[u]) continue;
        visited[u] = true;
        cout << char(u + 'A') << " ";

        for(int v = maxVertices - 1; v >= 0; v--)
        {
            if(adjMatrix[u][v] != 0 && !visited[v])
                st.push(v);
        }
    }
}

// This helper recursively checks for cycles in an undirected graph.
bool Graph::detectCycleUtil(vertex* v, vertex* parent)
{
    visited[v->data - 'A'] = true;

    edge* e = v->aHead;
    while(e)
    {
        int idx = e->dest->data - 'A'];
        if(!visited[idx])
        {
            if(detectCycleUtil(e->dest, v)) return true;
        }
        else if(e->dest != parent)
            return true;

        e = e->eNext;
    }
    return false;
}

// This function detects cycles in the graph.
bool Graph::detectCycle()
{
    resetVisited();

    vertex* v = vertices;
    while(v)
    {
        if(!visited[v->data - 'A'])
            if(detectCycleUtil(v, nullptr))
                return true;
        v = v->vNext;
    }
    return false;
}

// This function finds the root of a set in Union-Find.
int Graph::findUF(int v)
{
    if(parentUF[v] != v)
        parentUF[v] = findUF(parentUF[v]);
    return parentUF[v];
}

// This function merges two sets in Union-Find.
void Graph::unionUF(int a, int b)
{
    int ra = findUF(a);
    int rb = findUF(b);
    if(ra == rb) return;

    if(rankUF[ra] < rankUF[rb])
        parentUF[ra] = rb;
    else if(rankUF[rb] < rankUF[ra])
        parentUF[rb] = ra;
    else
    {
        parentUF[rb] = ra;
        rankUF[ra]++;
    }
}

// This function performs Dijkstra’s algorithm using a min-heap.
void Graph::dijkstra(char start)
{
    vector<int> dist(maxVertices, numeric_limits<int>::max());
    int s = start - 'A';
    dist[s] = 0;

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({0, s});

    while(!pq.empty())
    {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();
        if(d != dist[u]) continue;

        for(int v = 0; v < maxVertices; v++)
        {
            if(adjMatrix[u][v] != 0)
            {
                int w = adjMatrix[u][v];
                if(dist[u] + w < dist[v])
                {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
    }

    cout << "Dijkstra shortest paths from " << start << ":" << endl;
    for(int i = 0; i < maxVertices; i++)
    {
        cout << start << " -> " << char(i + 'A') << ": ";
        if(dist[i] == numeric_limits<int>::max())
            cout << "INF";
        else
            cout << dist[i];
        cout << endl;
    }
}

// This function performs Prim’s algorithm to compute the Minimum Spanning Tree.
void Graph::prim(char start)
{
    vector<int> key(maxVertices, numeric_limits<int>::max());
    vector<bool> inMST(maxVertices, false);
    vector<int> parent(maxVertices, -1);

    int s = start - 'A';
    key[s] = 0;

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({0, s});

    while(!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();
        inMST[u] = true;

        for(int v = 0; v < maxVertices; v++)
        {
            if(adjMatrix[u][v] != 0 && !inMST[v] && adjMatrix[u][v] < key[v])
            {
                key[v] = adjMatrix[u][v];
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }

    int total = 0;
    cout << "Prim MST:" << endl;
    for(int v = 0; v < maxVertices; v++)
    {
        if(parent[v] != -1)
        {
            cout << char(parent[v] + 'A') << " - " << char(v + 'A')
                 << " : " << adjMatrix[parent[v]][v] << endl;
            total += adjMatrix[parent[v]][v];
        }
    }
    cout << "Total cost: " << total << endl;
}

// This function performs Kruskal’s algorithm to compute the Minimum Spanning Tree.
void Graph::kruskal()
{
    struct Edge { int u, v, w; };
    vector<Edge> edges;

    for(int i = 0; i < maxVertices; i++)
    {
        for(int j = i + 1; j < maxVertices; j++)
        {
            if(adjMatrix[i][j] != 0)
                edges.push_back({i, j, adjMatrix[i][j]});
        }
    }

    sort(edges.begin(), edges.end(), [](Edge a, Edge b){ return a.w < b.w; });

    int total = 0;
    cout << "Kruskal MST:" << endl;

    for(auto& e : edges)
    {
        int ru = findUF(e.u);
        int rv = findUF(e.v);
        if(ru != rv)
        {
            cout << char(e.u + 'A') << " - "
                 << char(e.v + 'A') << " : " << e.w << endl;
            total += e.w;
            unionUF(ru, rv);
        }
    }

    cout << "Total cost: " << total << endl;
}
