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


int countEdges;                                                 // count of edges
int weight;                                                     // weight of edges
vector <vector <int> > edges;                                   // graph


int main()
{
    inpFile("input.txt");
    showGraph();
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
    
    
    int countVertex;        // count of vertex
    ifstream inputFile(fileName);
    if (!inputFile.is_open())
        cout << "Error! File is not found!" << endl;
    else
        inputFile >> countVertex >> countEdges;
    
    // Init edges
    for (int i = 0; i < countVertex; i++)
        edges.push_back(*new vector<int>(3));
    
    for (int i = 0; i < countEdges; i++)
        inputFile >> edges[i][0] >> edges[i][1] >> edges[i][2];
}

// Display the graph to console
void showGraph ()
{
    cout << "Count of vertex: " << edges.size() << endl;
    cout << "Count of edges: " << countEdges << endl;
    int sizeGraph = edges.size();
    for (int i = 0; i < sizeGraph; i++)
    {
        cout << "Vertex NO. " << edges[i][0] << " is connected with vertext NO. "  << edges[i][1] << " Weight: " << edges[i][2] << endl;
    }
}
