/*
================================================================================
                             GRAPH ANALYSIS TOOL
================================================================================

Author: Vignesh Midamaneni

This C++ program performs analysis on a directed graph representing events with
associated "hype scores". It supports four types of queries related to the graph's
structure and dynamics:

    1. Cycle Detection: Determines whether the graph contains a cycle.
    2. Strongly Connected Components (SCC): Computes the number of SCCs and the
       size of the largest SCC.
    3. Valid Topological Order: If the graph is a DAG (Directed Acyclic Graph),
       prints a valid topological order of events.
    4. Maximum Hype Score Path: Computes the maximum cumulative hype score that
       can be collected along a path in the SCC condensed graph.

Key Features:
-------------
- Uses Depth First Search (DFS) for:
  - Cycle detection
  - Topological sorting
  - Discovery and finishing times
  - Strongly Connected Component (SCC) detection using Kosaraju's algorithm
- Uses Kahn's Algorithm (BFS-based) for a topological sort
- Computes SCCs and condenses them into a DAG for efficient analysis
- Calculates the maximum hype score path in the SCC DAG using dynamic programming

Data Structures:
----------------
- event class stores metadata for each node: index, hype score, SCC index, colors, etc.
- Adjacency lists represent the original and transposed graphs.
- Topological sort is maintained in separate vectors for different methods.
- SCCs are stored as a vector of vectors.

How to Use:
-----------
- Input:
    - First line: N (number of nodes), M (number of edges)
    - Second line: N hype scores (1-based indexing)
    - Next M lines: Edges (u -> v)
    - Q queries follow; each query is a single integer:
        - 1: Check for cycle
        - 2: Report number and max size of SCCs
        - 3: Output a valid topological order if acyclic
        - 4: Output maximum cumulative hype score across any path

- Output:
    - Answers to the queries listed above

================================================================================
*/
#include <bits/stdc++.h>
using namespace std;

// Class to represent each event in the graph
class event
{
public:
    long long int index;
    long long int hype_score; // Hype score of the event
    char color;               // Used during DFS: 'w' = white, 'g' = gray, 'b' = black
    long long int d, f;       // Discovery and finish time
    long long int scc_index;  // Index of the strongly connected component this node belongs to
    long long int indeg;      // In-degree for topological sort
};

// Comparator to compare events based on their hype scores
class Comparator
{
public:
    bool operator()(event &a, event &b)
    {
        return a.hype_score < b.hype_score;
    }
};

// Comparator to sort based on finish time (for topological order)
class comparatortopo
{
public:
    bool operator()(event &a, event &b)
    {
        return a.f > b.f;
    }
};

// Base abstract class for graph algorithms
class GraphAlgorithm
{
public:
    virtual void Query() = 0;
};

// Graph class to store graph data and perform computations
class graph
{
public:
    vector<set<long long int>> adj_list; // Original graph adjacency list
    vector<event> events;                // Events data
    vector<event> topsort;               // Topological sort (based on DFS)
    vector<event> topsort1;              // Topological sort (based on indegree method)
    vector<event> topsort_scc;           // Topo sort of SCC graph
    vector<vector<long long int>> sccs;  // Strongly connected components
    long long int maxScore;              // Max hype score for Query 4
    long long int cycle = -1;            // Cycle detection flag

    void call(); // Function to trigger preprocessing
};

graph g;

// Function declarations (moved outside)
void dfs(event &start, vector<set<long long int>> &adj_list, vector<event> &events, long long int &p);
void dfsvisit(event &node, vector<set<long long int>> &adj_list, vector<event> &events, long long int &t, long long int &p);
void dfs_scc(event &node, vector<event> &topsort, vector<set<long long int>> &adj_listt, vector<vector<long long int>> &sccs);
void dfsvisit_scc(event &node, vector<set<long long int>> &adj_listt, vector<long long int> &v, vector<char> &colors);
vector<event> findtopsort(vector<event> events1, vector<set<long long int>> adj_list1);

// Query classes implementation
class isCycle : public GraphAlgorithm
{
public:
    void Query()
    {
        if (g.cycle == 1)
            cout << "YES\n";
        else
            cout << "NO\n";
    }
};

class indepComponent : public GraphAlgorithm
{
public:
    void Query()
    {
        cout << g.sccs.size() << " ";
        long long int max_size = 0;
        for (auto &scc : g.sccs)
        {
            max_size = max(max_size, (long long int)scc.size());
        }
        cout << max_size << endl;
    }
};

class validOrder : public GraphAlgorithm
{
public:
    void Query()
    {
        if (g.cycle == 1)
            cout << "NO\n";
        else
        {
            for (auto &e : g.topsort1)
            {
                cout << e.index << " ";
            }
            cout << endl;
        }
    }
};

class maxHype : public GraphAlgorithm
{
public:
    void Query()
    {
        cout << g.maxScore << endl;
    }
};

// Perform topological sort using in-degree (Kahn's algorithm)
vector<event> findtopsort(vector<event> events1, vector<set<long long int>> adj_list1)
{
    set<long long int> s;
    vector<event> result;
    for (long long int i = 1; i < (long long int)(events1.size()); i++)
    {
        if (events1[i].indeg == 0)
            s.insert(i);
    }
    while (!s.empty())
    {
        auto l = *s.begin();
        result.push_back(events1[l]);
        for (auto &neigh : adj_list1[l])
        {
            events1[neigh].indeg--;
            if (events1[neigh].indeg == 0)
                s.insert(neigh);
        }
        s.erase(l);
    }
    return result;
}

// Standard DFS over graph
void dfs(event &start, vector<set<long long int>> &adj_list, vector<event> &events, long long int &p)
{
    long long int n = adj_list.size() - 1;
    for (int i = 1; i <= n; i++)
        events[i].color = 'w';
    long long int time = 0;
    for (int i = 1; i <= n; i++)
    {
        if (events[i].color == 'w')
        {
            dfsvisit(events[i], adj_list, events, time, p);
        }
    }
}

// DFS helper with time stamping and cycle detection
void dfsvisit(event &node, vector<set<long long int>> &adj_list, vector<event> &events, long long int &t, long long int &p)
{
    t += 1;
    node.d = t;
    node.color = 'g';
    for (auto &v : adj_list[node.index])
    {
        if (events[v].color == 'w')
            dfsvisit(events[v], adj_list, events, t, p);
        else if (events[v].color == 'g' && p == 0)
            g.cycle = 1;
    }
    t += 1;
    node.f = t;
    node.color = 'b';
    if (p == 0)
        g.topsort.push_back(node);
    else
        g.topsort_scc.push_back(node);
}

// DFS to find strongly connected components
void dfs_scc(event &node, vector<event> &topsort, vector<set<long long int>> &adj_listt, vector<vector<long long int>> &sccs)
{
    long long int n = g.adj_list.size() - 1;
    vector<char> colors(n + 1, 'w');
    for (auto &node : topsort)
    {
        if (colors[node.index] == 'w')
        {
            vector<long long int> scc;
            dfsvisit_scc(node, adj_listt, scc, colors);
            sccs.push_back(scc);
        }
    }
}

// DFS helper to collect SCC
void dfsvisit_scc(event &node, vector<set<long long int>> &adj_listt, vector<long long int> &v, vector<char> &colors)
{
    colors[node.index] = 'g';
    v.push_back(node.index);
    for (auto &v1 : adj_listt[node.index])
    {
        if (colors[v1] == 'w')
            dfsvisit_scc(g.events[v1], adj_listt, v, colors);
    }
    colors[node.index] = 'b';
}

// Main preprocessing function to compute all graph properties
void graph::call()
{
    long long int p = 0;

    // Step 1: DFS to get finish times and detect cycles
    dfs(events[1], adj_list, events, p);
    reverse(topsort.begin(), topsort.end());

    // Step 2: Kahn's algorithm for topological sort (used in Query 3)
    topsort1 = findtopsort(events, adj_list);

    // Step 3: Reverse the graph for SCCs
    vector<set<long long int>> adj_listt(adj_list.size());
    for (long long int i = 1; i < adj_list.size(); i++)
    {
        for (auto &v : adj_list[i])
        {
            adj_listt[v].insert(i);
        }
    }

    // Step 4: Get SCCs using reversed graph and DFS
    dfs_scc(events[topsort[0].index], topsort, adj_listt, sccs);
    for (long long int i = 0; i < sccs.size(); i++)
    {
        for (auto &j : sccs[i])
        {
            events[j].scc_index = i + 1;
        }
    }

    // Step 5: Build graph of SCCs
    vector<set<long long int>> adj_list_scc(sccs.size() + 1);
    for (long long int i = 1; i < adj_list.size(); i++)
    {
        for (auto &j : adj_list[i])
        {
            if (events[i].scc_index != events[j].scc_index)
            {
                adj_list_scc[events[i].scc_index].insert(events[j].scc_index);
            }
        }
    }

    // Step 6: Create new SCC vertices and assign cumulative hype scores
    vector<event> scc_vertices(sccs.size() + 1);
    for (long long int i = 1; i < scc_vertices.size(); i++)
    {
        scc_vertices[i].index = i;
        scc_vertices[i].color = 'w';
        scc_vertices[i].hype_score = 0;
        for (auto &j : sccs[i - 1])
        {
            scc_vertices[i].hype_score += events[j].hype_score;
        }
    }

    // Step 7: DFS on SCC graph to compute max hype scores
    p = 1;
    dfs(scc_vertices[1], adj_list_scc, scc_vertices, p);
    reverse(topsort_scc.begin(), topsort_scc.end());

    vector<long long> dp(scc_vertices.size(), 0);
    for (long long int i = 1; i < scc_vertices.size(); i++)
    {
        dp[i] = scc_vertices[i].hype_score;
    }

    for (auto &e : topsort_scc)
    {
        long long int j = e.index;
        for (auto &k : adj_list_scc[j])
        {
            dp[k] = max(dp[k], dp[j] + scc_vertices[k].hype_score);
        }
        maxScore = max(maxScore, dp[j]);
    }
}

// Driver code
int main()
{
    long long int N, M;
    cin >> N >> M;

    // Create query handler objects
    GraphAlgorithm *q1 = new isCycle();
    GraphAlgorithm *q2 = new indepComponent();
    GraphAlgorithm *q3 = new validOrder();
    GraphAlgorithm *q4 = new maxHype();

    g.events.resize(N + 1);

    // Read hype scores
    for (long long int i = 1; i <= N; i++)
    {
        g.events[i].index = i;
        g.events[i].indeg = 0;
        cin >> g.events[i].hype_score;
    }

    // Read edges
    g.adj_list.resize(N + 1);
    for (long long int i = 0; i < M; i++)
    {
        long long int u, v;
        cin >> u >> v;
        g.adj_list[u].insert(v);
        g.events[v].indeg++;
    }

    // Precompute all graph data
    g.call();

    long long int Q;
    cin >> Q;
    while (Q--)
    {
        long long int Q1;
        cin >> Q1;
        if (Q1 == 1)
            q1->Query();
        else if (Q1 == 2)
            q2->Query();
        else if (Q1 == 3)
            q3->Query();
        else if (Q1 == 4)
            q4->Query();
    }

    return 0;
}