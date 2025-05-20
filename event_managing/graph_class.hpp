#include"event_class.hpp"

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

