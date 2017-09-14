// system libs
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>

//local libs
#include "ResourcePath.hpp"
#include "button.hpp"
#include "programmObject.hpp"

using namespace sf;
using namespace std;

// ---------------------------------------------------------------------
// ITOA
void reverse(char str[], int length)
{
    int start = 0;
    int end = length -1;
    while (start < end)
    {
        swap(*(str+start), *(str+end));
        start++;
        end--;
    }
}

char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
    
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
    
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
    
    if (isNegative)
        str[i++] = '-';
    str[i] = '\0';
    reverse(str, i);
    return str;
}
// ---------------------------------------------------------------------

// FUNCTION PROTOTYPES
void inpFile(string fileName);          // reading the file
void showGraph();                       // display the graph to console
void sort();                            // edge sorting by weight
int min_span_tree();                    // find minimal spanning tree
int work();                             // all function


// GLOBAL VARIABLES
int countEdges;                         // count of edges
int countVertex;                        // count of vertex
vector <vector <int> * > edges;         // graph
vector<int> way;

// INTERFACE
vector<int> x, y;                       // coordinates of vertex
int r = 700;                            // radius of cycle
// Color
Color white = Color(255, 255, 255);
Color white_100 = Color(255, 255, 255, 100);
Color white_255 = Color(255, 255, 255, 255);
Color background = Color(35, 95, 165);
Color line_color = Color(255, 255, 255, 50);
Color noCycle = Color(24, 67, 117);

// FLAG BUTTONS
bool isPressU = 0;
bool isPressO = 0;

int main()
{
    // Setting window
    ContextSettings settings;
    settings.antialiasingLevel = 100;
    VideoMode videoMode;
    videoMode.width = 2440;
    videoMode.height = 2160;
    string name = "Minimal spanning tree";
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
    
    
    // найстройки кнопок
    vector<Button> button;
    string str = "";
    char textInfo[1000] = "";
    
    Button openFile(40, videoMode.height - 400, 560, 80, white_100, Color(0,0,0), "Open File");
    button.push_back(openFile);
    
    Button update(640, videoMode.height - 400, 560, 80, white_100, Color(0,0,0), "Update");
    button.push_back(update);
    
    Button infouser(40, videoMode.height - 280, 1160, 200, white_100, Color(0,0,0), "Info User\nOPEN FILE - Key O  UPDATE - Key U\n", 0);
    button.push_back(infouser);
    
    Button info(1280, videoMode.height - 400, 1120, 320, white_100, Color(0,0,0), "Minimal weight: ", 0);
    button.push_back(info);
    
    
    int sumWeight = work();
    /*
    text.setPosition(1280, videoMode.height - 400);
    itoa(sumWeight, textInfo, 10);
    str = str + textInfo + "\nWay: ";
    for (int i = 0; i < way.size(); i++)
    {
        itoa(way[i], textInfo, 10);
        str = str + " " + textInfo;
    }
    text.setString(str);
    window.draw(text);
    */

    
    // задаем круг для вершины
    int r = 30;
    
    // белый круг, используется в цикле
    CircleShape shape(r);
    shape.setOutlineColor(Color::Red);
    shape.setOutlineThickness(5);
    shape.setFillColor(Color(0, 0, 0, 0));
    
    CircleShape shape2(r);
    shape2.setOutlineColor(white_255);
    shape2.setOutlineThickness(5);
    shape2.setFillColor(Color(0, 0, 0, 0));
    
    
    
    
    while (window.isOpen())
    {
        
        if (update.update(&window) || (Keyboard::isKeyPressed(Keyboard::U) && !isPressU))
        {
           
            
            isPressU = 1;
        }
        
        if (openFile.update(&window) || (Keyboard::isKeyPressed(Keyboard::O) && !isPressO))
        {
            system("open -a TextEdit input.txt");
            isPressO = 1;
        }
        
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
            
            // move vertex
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                for (int i = 0; i < countVertex; i++)
                {
                    if (x[i] <= (Mouse::getPosition(window)).x && x[i] >= (Mouse::getPosition(window)).x - 2 * r &&
                        y[i] <= (Mouse::getPosition(window)).y && y[i] >= (Mouse::getPosition(window)).y - 2 * r)
                    {
                        x[i] = (Mouse::getPosition(window)).x - r;
                        y[i] = (Mouse::getPosition(window)).y - r;
                        break;
                    }
                }
            }
            
            
            
            window.clear(background);
            
            
            // display edge
            for (int i = 0; i < countEdges; i++)
            {
                Vertex line[] =
                {
                    Vertex(Vector2f(x[(*edges[i])[1] - 1] + r, y[(*edges[i])[1] - 1] + r), white_255),
                    Vertex(Vector2f(x[(*edges[i])[2] - 1] + r, y[(*edges[i])[2] - 1] + r), white_255)
                };
                
                
                char s[5] = "";
                itoa((*edges[i])[0] - 1, s, 10);
                text.setString(s);
                text.setPosition((x[(*edges[i])[1] - 1] * 2 + x[(*edges[i])[2] - 1]) / 3 + r, (y[(*edges[i])[1] - 1] * 2 + y[(*edges[i])[2] - 1]) / 3);
                window.draw(line, 2, Lines);
                window.draw(text);
            }
            
            // display way
            for (int i = 0; i < way.size(); i = i + 2)
            {
                Vertex line[] =
                {
                    Vertex(Vector2f(x[way[i] - 1] + r, y[way[i] - 1] + r), Color::Red),
                    Vertex(Vector2f(x[way[i + 1] - 1] + r, y[way[i + 1] - 1] + r), Color::Red)
                };
                window.draw(line, 2, Lines);
            }
            
            // display vertex
            for (int i = 0; i < countVertex; i++)
            {
                shape.setPosition(x[i], y[i]);
                char s[5] = "";
                itoa(i + 1, s, 10);
                text.setString(s);
                text.setPosition(x[i] - 30, y[i] - 30);
                window.draw(shape);
                window.draw(text);
            }
            


            
            
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
            
            
            // вывод кнопок
            size = button.size();
            for(int i = 0; i < size; i++)
                button[i].draw(window);
            
            
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
    
    // вывод по кругу
    for (int i = 0; i < countVertex; i++)
    {
        int xx = 1220 - 30 + r * cos(i * 360 / countVertex * M_PI / 180);
        int yy = 900 - 30 + r * sin(i * 360 / countVertex * M_PI / 180);
        x.push_back(xx);
        y.push_back(yy);
    }
    
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
int min_span_tree()
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
    return sumWeight;
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

int work()
{
    inpFile("input.txt");
    showGraph();
    cout << endl;
    int sumWeight = min_span_tree();
    return sumWeight;
}
