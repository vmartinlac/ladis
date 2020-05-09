
#pragma once

#include <vector>

class UnionFind
{
public:

    void init(int num_nodes)
    {
        myParent.resize(num_nodes);

        for(int i=0; i<num_nodes; i++)
        {
            myParent[i] = i;
        }
    }

    void join(int i, int j)
    {
        myParent[find(i)] = find(j);
    }

    int getComponents(std::vector<int>& components)
    {
        components.assign(myParent.size(), -1);

        int N = 0;

        for(int i=0; i<myParent.size(); i++)
        {
            const int j = find(i);

            if(components[j] < 0)
            {
                components[j] = N;
                N++;
            }

            components[i] = components[j];
        }

        return N;
    }

protected:

    int find(int i)
    {
        while( myParent[myParent[i]] != myParent[i] )
        {
            myParent[i] = myParent[myParent[i]];
        }

        return myParent[i];
    }

protected:

    std::vector<int> myParent;
};
