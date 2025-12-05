#include "graph.h"

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
}

Graph::~Graph()
{
    for(int i = 0; i < maxVertices; i++)
        delete[] adjMatrix[i];
    delete[] adjMatrix;
    delete[] visited;

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

void Graph::resetVisited()
{
    for(int i = 0; i < maxVertices; i++)
        visited[i] = false;
}

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

    int i = src - 'A';
    int j = dest - 'A';
    adjMatrix[i][j] = weight;
}

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

    int i = src - 'A';
    int j = dest - 'A';
    adjMatrix[i][j] = 0;
}

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

bool Graph::detectCycleUtil(vertex* v, vertex* parent)
{
    visited[v->data - 'A'] = true;

    edge* e = v->aHead;
    while(e)
    {
        int idx = e->dest->data - 'A';
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

void Graph::displayList()
{
    vertex* v = vertices;
    while(v)
    {
        cout << v->data << ": ";
        edge* e = v->aHead;
        while(e)
        {
            cout << e->dest->data << "(" << e->weight << ") ";
            e = e->eNext;
        }
        cout << endl;
        v = v->vNext;
    }
}

void Graph::displayMatrix()
{
    for(int i = 0; i < maxVertices; i++)
    {
        for(int j = 0; j < maxVertices; j++)
            cout << adjMatrix[i][j] << " ";
        cout << endl;
    }
}
