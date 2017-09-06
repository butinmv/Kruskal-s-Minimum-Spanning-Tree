#include <iostream>
#include <vector>
#include <fstream>

#include <SFML/Graphics.hpp>
#include "ResourcePath.hpp"

using namespace sf;
using namespace std;

// FUNCTION PROTOTYPES
void inpFile(string fileName);          // reading the file
void showGraph();                       // display the graph to console
void sort();                            // edge sorting by weight
void min_span_tree();                   // find minimal spanning tree


int countEdges;                         // count of edges
int countVertex;                        // count of vertex
vector <vector <int> > edges;           // graph


int main()
{
    inpFile("input.txt");
    showGraph();
    
    cout << endl;
    min_span_tree();
    
    return 0;
}

//  Reading file
void inpFile(string fileName)
{
    // Clearing the vertices
    int size = edges.size();
    for ( int i = 0; i < size; i++)
        edges[i].clear();
    edges.clear();
    
    countEdges = 0;
    countVertex = 0;        // count of vertex
    ifstream inputFile(fileName);
    if (!inputFile.is_open())
        cout << "Error! File is not found!" << endl;
    else
        inputFile >> countVertex >> countEdges;
    
    // Init edges
    for (int i = 0; i < countEdges; i++)
        edges.push_back(*new vector<int>(countEdges));
    
    // Read start vertex, finish vertex and weight
    for (int i = 0; i < countEdges; i++)
        inputFile   >> edges[i][1]      // start vertex
                    >> edges[i][2]      // finish vertex
                    >> edges[i][0];     // weight
    
    // Close the file
    inputFile.close();
}

// Edge sorting by weight
void sort()
{
    for (int i = 0; i < countEdges - 1; i++)
        for (int j = i + 1; j < countEdges; j++)
            if (edges[i][0] > edges[j][0])
                for (int k = 0; k < 3; k++)
                {
                    int buf = edges[i][k];
                    edges[i][k] = edges[j][k];
                    edges[j][k] = buf;
                }
}

// Find minimal spanning tree
void min_span_tree()
{
    sort();

    vector<int> tree_id(countVertex);
    int size = tree_id.size();
    for (int i = 0; i < size; ++i)
        tree_id[i] = i;
    
    int sumWeight = 0;
    for (int i = 0; i < countEdges; i++)
    {
        int weight = edges[i][0];
        int start  = edges[i][1];
        int end    = edges[i][2];
        if (tree_id[start] != tree_id[end])
        {
            sumWeight += weight;
            int a = tree_id[start];
            int b = tree_id[end];
            for (int i = 0; i < size; i++)
                if (tree_id[i] == b)
                    tree_id[i] = a;
        }
    }
    cout << "Minimal weight: " << sumWeight;
}


// Display the graph to console
void showGraph ()
{
    cout << "Count of vertex: " << countVertex << endl;
    cout << "Count of edges: " << countEdges << endl;
    int sizeGraph = edges.size();
    for (int i = 0; i < sizeGraph; i++)
        cout    << "Vertex NO. " << edges[i][1]
                << " is connected with vertext NO. "  << edges[i][2]
                << ". Weight: " << edges[i][0] << endl;
}
