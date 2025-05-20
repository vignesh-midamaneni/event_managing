#include"graph_algo_class.hpp"
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