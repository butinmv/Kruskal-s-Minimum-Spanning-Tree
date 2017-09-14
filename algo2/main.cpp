// system libs
#include <iostream>
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>

//local libs
#include "ResourcePath.hpp"

using namespace sf;
using namespace std;


// FUNCTION PROTOTYPES
void inpFile(string fileName);          // reading the file
void showGraph();                       // display the graph to console
void sort();                            // edge sorting by weight
void min_span_tree();                   // find minimal spanning tree
void work();                            // all function


// GLOBAL VARIABLES
int countEdges;                         // count of edges
int countVertex;                        // count of vertex
vector <vector <int> * > edges;         // graph
vector<int> way;

// INTERFACE COLOR
Color white = Color(255, 255, 255);
Color white_100 = Color(255, 255, 255, 100);
Color white_255 = Color(255, 255, 255, 255);
Color blue_background = Color(35, 95, 165);
Color line_color = Color(255, 255, 255, 50);
Color noCycle = Color(24, 67, 117);

int main()
{
    // Setting window
    ContextSettings settings;
    settings.antialiasingLevel = 100;
    VideoMode videoMode;
    videoMode.width = 2440;
    videoMode.height = 2160;
    string name = "Symple cycles";
    RenderWindow window(videoMode, name, Style::Close, settings);
    
    // Settings font
    Font font;
    if (!font.loadFromFile("cour.ttf"))
    {
        return EXIT_FAILURE;
    }
    Text text("", font, 35);
    text.setFillColor(white);
    text.setStyle(Text::Bold);
    
    // Setting icon
    Image icon;
    if (!icon.loadFromFile("icon.png"))
    {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    
    work();
    
    
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                window.close();
            }
            
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                window.close();
            }
            window.clear(blue_background);
            
            
            // Draw the grid
            int v = 40;
            int size = videoMode.width / v + 1;
            for(int j = 0; j < size; j++)
            {
                Vertex line[] =
                {
                    Vertex(Vector2f(j * v, 0), line_color),
                    Vertex(Vector2f(j * v, videoMode.height), line_color)
                };
                window.draw(line, 2, Lines);
            }
            
            size = videoMode.height / v + 1;
            for(int j = 0; j < size; j++)
            {
                Vertex line[] =
                {
                    Vertex(Vector2f(0, j * v), line_color),
                    Vertex(Vector2f(videoMode.width, j * v), line_color)
                };
                window.draw(line, 2, Lines);
            }
            
            
            window.display();
        }
    }
    
    return 0;
}

//  Reading file
void inpFile(string fileName)
{
    // Clearing the vertices
    int size = edges.size();
    for ( int i = 0; i < size; i++)
        edges[i]->clear();
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
        edges.push_back(new vector<int>(3));
    
   
    // Read start vertex, finish vertex and weight
    for (int i = 0; i < countEdges; i++)
        inputFile   >> (*edges[i])[1]      // start vertex
                    >> (*edges[i])[2]      // finish vertex
                    >> (*edges[i])[0];     // weight
    
    // Close the file
    inputFile.close();
}

// Edge sorting by weight
void sort()
{
    for (int i = 0; i < countEdges - 1; i++)
        for (int j = i + 1; j < countEdges; j++)
            if ((*edges[i])[0] > (*edges[j])[0])
                for (int k = 0; k < 3; k++)
                {
                    int buf = (*edges[i])[k];
                    (*edges[i])[k] = (*edges[j])[k];
                    (*edges[j])[k] = buf;
                }
}

// Find minimal spanning tree
void min_span_tree()
{
    sort();
    cout << "After sort" << endl;
    int sizeGraph = edges.size();
    for (int i = 0; i < sizeGraph; i++)
        cout    << "Vertex NO. " << (*edges[i])[1]
                << " is connected with vertext NO. "  << (*edges[i])[2]
                << ". Weight: " << (*edges[i])[0] << endl;
    
    vector<int> tree_id(countVertex);
    int size = tree_id.size();
    for (int i = 0; i < size; ++i)
        tree_id[i] = i;
    
    int sumWeight = 0;
    for (int i = 0; i < countEdges; i++)
    {
        int weight = (*edges[i])[0];
        int start  = (*edges[i])[1] - 1;
        int end    = (*edges[i])[2] - 1;
        if (tree_id[start] != tree_id[end])
        {
            sumWeight += weight;
            way.push_back(start + 1);
            way.push_back(end + 1);
            int a = tree_id[start];
            int b = tree_id[end];
            for (int j = 0; j < size; j++)
                if (tree_id[j] == b)
                    tree_id[j] = a;
        }
    }
    cout << "Minimal weight: " << sumWeight << endl;
    cout << "Our prograssive: ";
    for (int i = 0; i < way.size(); i++)
        cout << way[i] << " ";
}


// Display the graph to console
void showGraph()
{
    cout << "Count of vertex: " << countVertex << endl;
    cout << "Count of edges: " << countEdges << endl;
    int sizeGraph = edges.size();
    for (int i = 0; i < sizeGraph; i++)
        cout    << "Vertex NO. " << (*edges[i])[1]
                << " is connected with vertext NO. "  << (*edges[i])[2]
                << ". Weight: " << (*edges[i])[0] << endl;
}

void work()
{
    inpFile("input.txt");
    showGraph();
    cout << endl;
    min_span_tree();
}
