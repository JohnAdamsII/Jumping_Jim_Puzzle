/* 

Programmed by: John Adams
Filename: maze.cpp 
Build: g++ maze.cpp
Run: ./a.out

IMPORTANT: Program Looks for file named "input.txt" to read in data

Output is printed to console and "output.txt"

Information:

My solution reads in the N x N puzzle matrix and converts it to an adjacency (implemented as Hash Map)
then preforms BFS to find shortest path to goal

*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <string>

using namespace std;

// Globals
vector<vector<int>> puzzle; //2D vector to read input file into
int start = 11;             // Upper-left corner of input matrix designated as index 11
int goal;                   // goal index obtained from file

// Utility Function
void Find_moves(vector<int> path);
int File_reader();

int File_reader()
{
    std::vector<std::string> strVecs;
    std::string fileName = "input.txt";

    // Open the File
    std::ifstream in(fileName.c_str());

    // Check if object is valid
    if (!in)
    {
        std::cerr << "Cannot open the File : " << fileName << std::endl;
        return false;
    }

    std::string str;
    // Read the next line from File untill it reaches the end.
    while (std::getline(in, str))
    {
        // Line contains string of length > 0 then save it in vector
        if (str.size() > 0)
            strVecs.push_back(str);
    }
    //Close The File
    in.close();

    const std::size_t size = strVecs[0].size();
    // resulting 2D vector
    puzzle.reserve(size);

    for (const std::string &strInteger : strVecs)
    {
        std::vector<int> rawVec;
        rawVec.reserve(size);
        for (const char Char : strInteger)
            if (isdigit(Char))
                rawVec.emplace_back(static_cast<int>(Char - '0'));
        // save the row to the 2D vector
        puzzle.emplace_back(rawVec);
    }

    goal = (puzzle[0][0] * 10) + puzzle[0][0]; // get index of goal from file
    puzzle.erase(puzzle.begin());
    std::cout << std::endl;
}

class Graph
{
  public:
    void build_adj_list();
    void addEdge(int vertex1, int vertex2);
    unordered_set<int> &GetNeighbors(int vertex);

  private:
    unordered_map<int, unordered_set<int>> Adj_List; // Adjecency List graph representation as Hash Map
};

void Graph::build_adj_list()
{
    for (int i = 0; i < puzzle.size(); i++)
    {

        for (int j = 0; j < puzzle[i].size(); j++)
        {
            int key = ((i + 1) * 10) + j + 1; // calculates key for adj list such that matrix at [0][0] has key 11 etc.

            if (puzzle[i][j] + j <= puzzle[i].size() - 1) // check for E neighbor and push to map

            {
                int value = ((i + 1) * 10) + (j + puzzle[i][j] + 1);
                addEdge(key, value);
            }

            if (puzzle[i][j] + i <= puzzle[i].size() - 1) // check for S neighbor and push to map
            {
                int value = ((i + puzzle[i][j] + 1) * 10) + j + 1;
                addEdge(key, value);
            }

            if (j - puzzle[i][j] >= 0) // check for W neighbor and push to map
            {
                int value = ((i + 1) * 10) + (j - puzzle[i][j] + 1);
                addEdge(key, value);
            }

            if (i - puzzle[i][j] >= 0) // check for N neighbor and push to map
            {
                int value = ((i + 1 - puzzle[i][j]) * 10) + j + 1;
                addEdge(key, value);
            }
        }
    }
}

void Graph::addEdge(int vertex1, int vertex2)
{
    Adj_List[vertex1].insert(vertex2);
}

unordered_set<int> &Graph::GetNeighbors(int vertex)
{
    return Adj_List[vertex];
}

unordered_map<int, int *>
BFS(Graph &graph, int Vertex)
{
    unordered_map<int, int *> Traversal_Tree = {{Vertex, nullptr}};
    queue<int> q;
    q.push(Vertex);

    while (!q.empty())
    {
        int CurrentVertex = q.front();
        q.pop();
        int *parentNode = new int{CurrentVertex};

        for (int childNode : graph.GetNeighbors(CurrentVertex))
        {
            if (Traversal_Tree.find(childNode) == Traversal_Tree.end())
            {
                Traversal_Tree[childNode] = parentNode;
                q.push(childNode);
            }
        }
    }

    return Traversal_Tree;
}

vector<int> GetPredecessorPath(int Goal,
                               unordered_map<int, int *> &BFSPathTree)
{
    vector<int> path;
    int CurrentVertex = Goal;
    int *NextVertex = BFSPathTree[CurrentVertex];

    while (true)
    {
        path.push_back(CurrentVertex);
        NextVertex = BFSPathTree[CurrentVertex];

        if (NextVertex == nullptr)
            break;

        CurrentVertex = *NextVertex;
    }

    reverse(path.begin(), path.end());
    return path;
}

void Find_moves(vector<int> path)
{
    ofstream outputs ("outputs.txt");

    for (int i = 0; i <= path.size() - 2; ++i)
    {
        if (path[i] % 10 < path[i + 1] % 10)
        {
            cout << "E" << " ";
            outputs << "E ";
            continue;
        }
        if (path[i] % 10 > path[i + 1] % 10)
        {
            cout << "W" << " ";
            outputs << "W ";
            continue;
        }
        if (path[i] / 10 % 10 < path[i + 1] / 10 % 10)
        {
            cout << "S" << " ";
            outputs << "S ";
            continue;
        }

        else
        {
            cout << "N" << " ";
            outputs << "N ";
            continue;
        }
    }
}

int main()
{
    int readfile = File_reader();

    Graph graph;
    graph.build_adj_list();

    unordered_map<int, int *> BFSPathTree = BFS(graph, start);

    std::cout << "Shortest path from " << start << " to " << goal << ": ";

    vector<int> path = GetPredecessorPath(goal, BFSPathTree);

    for (auto i = path.begin(); i != path.end(); ++i)
        cout << *i << ' ';

    cout << endl;
    cout << "Number of Moves: ";
    cout << path.size() - 1 << endl; // -1 because start isn't a move

    cout << "Moves Taken: ";
    std::cout << "\n";

    Find_moves(path);
}