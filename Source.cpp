

#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include<queue>
#include "d_matrix.h"
#include "graph.h"
#include<stack>

typedef pair<int, int> iPair;
using namespace std;
class maze
{
public:
    maze(ifstream& fin);
    void print(int, int, int, int);
    bool isLegal(int i, int j);
    void setMap(int i, int j, int n);
    int getMap(int i, int j) const;
    void mapMazeToGraph(maze& m, graph& g);
    bool isNode(int i, int j);
    void setNodeRow(int nodeNum, int row);
    int getNodeRow(int nodeNum);
    void getNodeColumn(int nodeNum, int col);
    int getNodeCol(int nodeNum);
    void findEdges(int row, int col, graph& g);
    vector<int> getNeighbors(int v, graph& g);
    //bool findPathRecursive(graph& g, int node);
    
    void findShortestPath2(graph& g, int startVertex, int endVertex);
    void printPath();
    void printMap();
    void printStack(graph& g);
    bool findShortestPath1(graph& g, int frontNode);
    void printShortestPath1(graph& g);
private:
    int rows; // number of rows in the maze
    int cols; // number of columns in the maze
    matrix<bool> value;
    matrix<int> map;      // Mapping from maze (i,j) values to node index values
    int nodeRow;
    int nodeColumn;
    bool pathFound1;
    vector<int>neighbors;
    stack<int>path;
    vector<int>shortestPath1;


};
void maze::setMap(int i, int j, int n)
// Set mapping from maze cell (i,j) to graph node n.
{
}
/*int maze::getMap(int i, int j) const
// Return mapping of maze cell (i,j) in the graph.
{

}*/
maze::maze(ifstream& fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
    fin >> rows;
    fin >> cols;
    char x;
    value.resize(rows, cols);
    for (int i = 0; i <= rows - 1; i++)
        for (int j = 0; j <= cols - 1; j++)
        {
            fin >> x;
            if (x == 'O')
                value[i][j] = true;
            else
                value[i][j] = false;
        }
    map.resize(rows, cols);
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{

    cout << endl;
    if (goalI < 0 || goalI > rows - 1 || goalJ < 0 || goalJ > cols - 1)
        throw rangeError("Bad value in maze::print");
    if (currI < 0 || currI > rows - 1 || currJ < 0 || currJ > cols - 1)
        throw rangeError("Bad value in maze::print");
    for (int i = 0; i <= rows - 1; i++)
    {
        for (int j = 0; j <= cols - 1; j++)
        {
            if (i == goalI && j == goalJ)
                cout << "*";
            else
                if (i == currI && j == currJ)
                    cout << "+";
                else
                    if (value[i][j])
                        cout << " ";
                    else
                        cout << "X";
        }
        cout << endl;
    }
    cout << endl << endl;

    /*for (int i = 0;i < rows;i++)
    {
        for (int j = 0;j < cols;j++)
        {
            if (map[i][j] == 0)
                cout << "X  ";
            else if (map[i][j] <= 10)
                cout << map[i][j] - 1 << "  ";
            else
                cout << map[i][j] - 1 << " ";
        }
        cout << endl;
    }
    cout << endl;*/
}
void maze::printMap()
{
    for (int i = 0;i < rows;i++)
    {
        for (int j = 0;j < cols;j++)
        {
            if (map[i][j] == 0)
                cout << "X  ";
            else if (map[i][j] <= 10)
                cout << map[i][j] - 1 << "  ";
            else
                cout << map[i][j] - 1 << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze.
{
    if (i < 0 || i > rows - 1 || j < 0 || j > cols - 1)
        throw rangeError("Bad value in maze::isLegal");
    return value[i][j];
}
bool maze::isNode(int i, int j)
//Check if the position in the maze is a node
{
    if (value[i][j] == true)
    {

        //Center cases
        if (i != 0 && j != 0 && i != rows - 1 && j != cols - 1)
        {
            if (value[i][j + 1] == true && value[i][j - 1] == true && value[i - 1][j] == false && value[i + 1][j] == false)
                return false;
            if (value[i][j + 1] == false && value[i][j - 1] == false && value[i - 1][j] == true && value[i + 1][j] == true)
                return false;
            else
                return true;

        }
        //Corners
        if ((i == 0 || i == rows - 1) && (j == 0 || j == cols - 1))
            return true;
        //top cases
        if (i == 0)
        {
            if ((value[i][j - 1] == true && value[i][j + 1] == true) && value[i + 1][j] == false)
                return false;
            else
                return true;
        }
        //bottom cases
        if (i == rows - 1)
        {
            if ((value[i][j - 1] == true && value[i][j + 1] == true) && value[i - 1][j] == false)
                return false;
            else
                return true;
        }
        //left cases
        if (j == 0)
        {
            if ((value[i - 1][j] == true && value[i + 1][j] == true) && value[i][j + 1] == false)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        //right cases
        if (j == cols - 1)
        {
            if ((value[i - 1][j] == true && value[i + 1][j] == true) && value[i][j - 1] == false)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

    }
    return false;
}
void maze::findEdges(int row, int  col, graph& g)
//Find edges
{
    int prev, prevk, previ;

    //Go through rows to see if two nodes are connected. If so, make an edge between them
    for (int i = 0;i < rows;i++)
    {
        prev = 0;
        prevk = -1;
        for (int k = 0;k < cols;k++)
        {
            if (map[i][k] > 0 && prevk < 0)
            {
                prev = map[i][k];
                prevk = k;
            }
            if (map[i][k] > 0 && prevk >= 0)
            {
                if (prevk != k)
                {
                    //cout << "Edge added at " << map[i][prevk] << " " << map[i][k] << " " << abs(k - prevk) << endl;
                    g.addEdge(map[i][prevk] - 1, map[i][k] - 1, abs(k - prevk));
                }
                if ((k == cols - 1) == false)
                {
                    if (value[i][k + 1] == true)
                    {
                        prev = map[i][k];
                        prevk = k;
                    }
                    else
                    {
                        prev = 0;
                        prevk = -1;
                    }
                }



            }

        }
    }

    //Go through columns to see if two nodes are connected. If so, make an edge between them
    for (int k = 0;k < cols;k++)
    {
        prev = 0;
        previ = -1;
        for (int i = 0;i < rows;i++)
        {
            if (map[i][k] > 0 && previ < 0)
            {
                prev = map[i][k];
                previ = i;
            }
            if (map[i][k] > 0 && previ >= 0)
            {
                if (previ != i)
                {
                    //cout << "Edge added " << map[previ][k] << " " << map[i][k] << " " << endl;
                    g.addEdge(map[previ][k] - 1, map[i][k] - 1, abs(i - previ));
                }
                if ((i == rows - 1) == false)
                {
                    if (value[i + 1][k] == true)
                    {
                        prev = map[i][k];
                        previ = i;
                    }
                    else
                    {
                        prev = 0;
                        previ = -1;
                    }
                }



            }

        }
    }
}

int maze::getNodeCol(int nodeNum)
//gets node row for given node number
{

    for (int i = 0;i < rows;i++)
    {
        for (int j = 0;j < cols;j++)
        {
            if (nodeNum == map[i][j])
            {
                return j;
            }
        }
    }
    return 0;
}
int maze::getNodeRow(int nodeNum)
//returns row indez for given node
{
    for (int i = 0;i < rows;i++)
    {
        for (int j = 0;j < cols;j++)
        {
            if (nodeNum == map[i][j])
            {

                return i;
            }
        }
    }
    return 0;
}


void maze::mapMazeToGraph(maze& m, graph& g)
// Create a graph g that represents the legal moves in the maze m.
{
    //first node is equal to zero
    int nodeNum = 0;
    //go through each point and see if they are a node, if so, add node to graph and to map
    for (int i = 0;i < rows;i++)
    {
        for (int j = 0;j < cols;j++)
        {
            if (isNode(i, j) == true)
            {
                nodeNum++;
                g.addNode(0);
                map[i][j] = nodeNum;


            }
        }
    }
    //Find edges between nodes
    findEdges(0, 0, g);
}

vector<int> maze::getNeighbors(int v, graph& g)
//gets all neigbors of given vertex point and puts in vector
{
    for (int i = 0; i < g.numNodes();i++)
    {
        if (g.isEdge(v, i) == true)
            neighbors.push_back(i);
    }
    return neighbors;
}

/* bool maze::findPathRecursive(graph& g, int node)
//function to find a path recursively
{
    if (node == g.numNodes()-1)
    {
        path.push(node);
        return true;
    }
    g.unVisit(node);
    //cout << "Node value " << node << endl;
    //g.unVisit(node);
    for (int i = 0; i <= 15;i++)
    {
        if ((g.isEdge(node, i) == true||g.isEdge(i,node)==true )&& g.isVisited(i) == false)
        {
            //print(6, 9, getNodeRow(i), getNodeCol(i));

            g.visit(node);
            if (findPathRecursive(g, i) == true)
            {
                //cout << "True returned" << endl;
                path.push(node);
                return true;
            }

        }
    }
    return false;

} */

bool maze::findShortestPath1(graph& g, int frontNode)
//finds shortest path through BFS
{
    cout << "Checking for shortest path through queue based BFS" << endl;


    pathFound1 = false;  //sets pathFound boolean as false to begin with
    const int size = g.numNodes();
    vector<int> pred(size, 0); //predeccesor array

    list<int> nodeQueue;  //queue for bfs
    g.clearVisit();  //marks all nodes unvisited
    nodeQueue.push_back(frontNode); //push start node to queue
    g.visit(frontNode);  //mark start node as visited


    cout << neighbors.size() << " Neighbor size" << endl;
    //runs while queue is not empty
    while (!nodeQueue.empty() && nodeQueue.front() <= 15)
    {
        frontNode = nodeQueue.front();  //sets start node as top of stack
        getNeighbors(frontNode, g);    //gets neighbors of startnode as vector

        //for loop checks all neighbors of start node
        for (int i = 0; i <neighbors.size();i++)
        {
            cout << "Neighbors.size() " << neighbors.size() << endl;
            //if statement checks if node is unvisited
            cout << "Neighbors[i] " << neighbors[i] << endl;
            if (g.isVisited(neighbors[i]) == false)
            {
                g.visit(neighbors[i]);   //sets neighbor node as visited
                nodeQueue.push_back(neighbors[i]);  //adds neighbors to queue
                cout << "I " << i << endl;
                cout << pred.size() << " pred.size()" << endl;
                pred[neighbors[i]] = frontNode;   //sets the current front node of queue as predecessor of its neighbors
            }
        }
        //neighbor is dead end if it's been visited and is popped from stack
        nodeQueue.pop_front();
    }

    int curr = g.numNodes() - 1;   //variable for current predecessor array position
    shortestPath1.push_back(curr);  //pushes final node into predeccor array

   //while loop runs until the predeccor is the first node
    cout << "Curr " << curr << endl;
    while (pred[curr] != 0)
    {
        shortestPath1.push_back(pred[curr]);   //pushes the predecessor of each node into the shortest path array
        curr = pred[curr];                      //sets the predeccor as the current node

        //if first current value is predecessor of final node, the shortest path has been found and the variable is set to true
        if (curr == 14)
            pathFound1 = true;

        cout << "Curr " << curr << endl;
    }
    shortestPath1.push_back(0);
    printShortestPath1(g);
    return pathFound1;
}

void maze::printShortestPath1(graph& g)
//function to print whether the shortest path was found through BFS and if it was, prints the shortest path
{
    //if path exists, the path is printed out
    if (pathFound1 == true)
    {
        cout << "Shortest Path Found" << endl;
        for (int i = shortestPath1.size() - 1;i >= 0;i--)
        {
            cout << "Next element in path is node " << shortestPath1[i] << endl;
            print(6, 9, getNodeRow(shortestPath1[i] + 1), getNodeCol(shortestPath1[i] + 1));  //prints maze at each step
        }
    }
    //if path is not found, the message below prints
    else
        cout << "Shortest Path Not Found" << endl;
}

void maze::findShortestPath2(graph& g, int startVertex, int endVertex)
//finds shortest path through Dijkstra's algorithm
{
    int numV = g.numNodes(); //Number of nodes
   
    priority_queue< iPair, vector <iPair>, greater<iPair> > PQ; //Set up a integer pair
    
    getNeighbors(startVertex, g); //Get neighbors


    for (int j = 1;j < numV; j++)
    {
        g.setNodeWeight(j, INT16_MAX);//Set all nodes to max value
        PQ.push(make_pair(INT16_MAX, j));//Create a pair in the priority queue
    }
    g.setNodeWeight(0, 0);//Set the first node to zero
    PQ.push(make_pair(0,0));//Add first node to queue
    
    while (PQ.size() != 0)
    {
        int curr = PQ.top().second;//Current is equal to the node number
        cout << "Current node is " << curr << endl;
        PQ.pop();//Remove the top node in queue
        for (int i = 0; i < g.numNodes();i++)
        //Go through each possible neighbor
        {
            if (g.isEdge(curr, i) == true)//Check if neighbors
            {
                
                if ((g.getEdgeWeight(curr, i) + g.getNodeWeight(curr)) < g.getNodeWeight(i))
                    //If this path is shorter, push to priority queue
                {
                    g.setNodeWeight(i, g.getEdgeWeight(curr, i) + g.getNodeWeight(curr));//Set node weight
                    PQ.push(make_pair(g.getEdgeWeight(curr, i) + g.getNodeWeight(curr), i));//Add to priority queue
                    g.setNodeParent(i, curr);//Set the parent to the previous
                }
            }
            else if (g.isEdge(i, curr) == true) //Repeat for the opposite edge orientation
            {
                
                if ((g.getEdgeWeight(i, curr) + g.getNodeWeight(curr) )< g.getNodeWeight(i))
                {
                    g.setNodeWeight(i, g.getEdgeWeight(i, curr) + g.getNodeWeight(curr));
                    PQ.push(make_pair(g.getEdgeWeight(i, curr) + g.getNodeWeight(curr), i));
                    g.setNodeParent(i, curr);
                   
                }
            }
        }
    }
    printStack(g);//Print the path
}

void maze::printPath()
//Print path taken to get from start to finish
{
    //Identify and print the starting node
    cout << "Start at node " << path.top() << endl;

    //Print current position and end goal position
    print(rows - 1, cols - 1, getNodeRow(path.top()), getNodeCol(path.top()));
    path.pop();

    //Print every other position until the final position is reached
    while (path.size() != 0)
    {
        cout << "Then go to node " << path.top() << endl;
        print(rows - 1, cols - 1, getNodeRow(path.top()), getNodeCol(path.top()));
        path.pop();

    }
}

void maze::printStack(graph& g)
//Takes a graph as a parameter and prints the solution
{
    cout << "Printing solution..." << endl<<endl; 
    int n=g.numNodes()-1; //Node number starting at the highest
    stack<int> stackParents;

    //Start at the end, go to the beginning
    while (n != 0)
    {
        stackParents.push(n);//Add the node number to the stack
        n = g.getNodeParent(n);//Find the parent of the node, set n equal to the parent to conintue going back
    }

    stackParents.push(n);//Add the zero node
    while (stackParents.size() != 0)
    {
        cout << "Then go to node " << stackParents.top();
        print(6, 9, getNodeRow(stackParents.top()+1), getNodeCol(stackParents.top()+1));//{rint the map at positino of node
        stackParents.pop();//Remove the node
    }
}



int main()
{
    char x;
    ifstream fin;
    // Read the maze from the file.
    string fileName = "graph.txt";
    fin.open(fileName.c_str());
    int empty = 0;
    




    if (!fin)
    {
        cerr << "Cannot open " << fileName << endl;
        exit(1);
    }
    try
    {
        graph g(empty);
        maze m(fin);

        //Map the maze to the graph
        m.mapMazeToGraph(m, g);

        //Print initial graph
        m.print(6, 9, 0, 0);


        g.clearVisit();

        //Find and print the shortest path using BFS
        m.findShortestPath1(g, 0);

        //Find and print the shortest path using Dijkstra's algorithm
        m.findShortestPath2(g, 0, 15);
        
   
       

    }
    catch (indexRangeError& ex)
    {
        cout << ex.what() << endl; exit(1);
    }
    catch (rangeError& ex)
    {
        cout << ex.what() << endl; exit(1);
    }
}



