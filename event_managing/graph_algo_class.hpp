#include"graph_class.hpp"
// Base abstract class for graph algorithms
class GraphAlgorithm
{
public:
    virtual void Query() = 0;
};
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
