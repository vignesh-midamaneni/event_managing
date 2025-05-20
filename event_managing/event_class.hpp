#include <bits/stdc++.h>
using namespace std;

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

