#include<iostream>
#include<fstream>
#include<limits>
#include<vector>
#include<map>

using namespace std;

class Maze
{
    public:

    struct Node
    {
        int x{}, y{};
        Node(int xin, int yin) : x(xin), y(yin) {};

        bool operator<(const Node& n) const
        {
                return (x < n.x) || (x == n.x && y < n.y);
        }
    };

    int grid_X , grid_Y;

    bool** grid; 
    char** graph;
    std::vector<Node> nodeList;
    std::map<Node, int> indexMap;
    size_t totNodes;
    int** adjacencyMatrix;

    Maze(const char* buff)
    {
        std::string filename(buff);
        std::ifstream file(filename);

        if(!file.is_open())
        {
            std::cerr << "Error opening maze file" << std::endl;
        }

        //get x and y
        file >> grid_X >> grid_Y;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        //grid alloc
        grid = new bool* [grid_X];

        for(int i = 0; i<grid_X; i++)
        {
            grid[i] = new bool [grid_Y]();
        }

        std::string line;

        for(int i = 0; i<grid_X; i++)
        {
            std::getline(file, line);
            for(int j = 0; j<grid_Y; j++)
                grid[i][j] = (line[j] == '1');
        }

        file.close();
    }

    void print()
    {
        for(int i = 0; i<grid_X; i++)
        {
            for(int j = 0; j<grid_Y; j++)
                std::cout<< grid[i][j];
            std::cout << std::endl;
        }
    }

    bool isNode(int i,int j )
    {
        
        //if path in these directions
        int pL{};
        int pR{};
        int pU{};
        int pD{};

        int Left = j-1;
        int Right = j+1;
        int Up = i-1;
        int Down = i+1;
        
        if(Left >= 0)
            if(grid[i][Left] == true)
                pL = 1;
        if(Right < grid_Y) 
            if(grid[i][Right] == true)
                pR = 1;
        if(Up >= 0) 
            if(grid[Up][j] == true)
                pU = 1;
        if(Down < grid_X)
            if(grid[Down][j] == true)
                pD = 1;

        bool horizontalCorridor = (pL && pR) && !(pU || pD);
        bool verticalCorridor = (pU && pD) && !(pL || pR);
        int totalPaths = pL + pR + pU + pD;

        if(totalPaths >= 2 && !(horizontalCorridor || verticalCorridor))
            return true;
        else return false;
    }
            
    void plotGraph()
    {
        graph = new char* [grid_X];

        for(int i = 0; i<grid_X; i++)
        {
            graph[i] = new char [grid_Y];
            for(int j = 0; j<grid_Y; j++)
            {
                if(grid[i][j] == true)
                {
                    if(isNode(i,j))
                    {
                        Node node(i,j);
                        graph[i][j] = 'N';
                        nodeList.push_back(node);
                        indexMap[node] = nodeList.size()-1;

                    }else
                    {
                        graph[i][j] = '1';
                    }
                }
                else graph[i][j] = '0';
            }
        }

        totNodes = nodeList.size();
    }

    void printGraph()
    {
        for(int i = 0; i<grid_X; i++)
        {
            for(int j = 0; j<grid_Y; j++)
                std::cout << graph[i][j];
            std::cout<< std::endl;
        }
    }

    void printNodeList()
    {
        size_t c{};
        cout << "Total Nodes: " << totNodes << endl;
        for(auto node : nodeList)
        {
            cout << c++ << ":  (" << node.x << "," << node.y << ")" << endl;
        }
    }

    int getIndex(int x, int y)
    {
        int index = indexMap[Node(x,y)]; 
        cout << "index of node at: " << x << " y: " << y << " is: " << index << endl;

        return index;
    }

   
    int* getEdgeList(Node& node)
    {
        int* edgeList = new int[totNodes]();

        int Rcount{};
        int Lcount{};
        int Ucount{};
        int Dcount{};

        int row = node.x;
        int col = node.y;

        //check above
        
        for( int i = row-1; i>=0; i--)
        {
            Ucount++;
            if(grid[i][col] == false)
                    break;
            if(graph[i][col] == 'N')
            {
                int index = getIndex(i, col);
                edgeList[index] = Ucount;
                break;
            }
        }

        //check down
        
        for(int i = row+1; i<= grid_X; i++)
        {
            Dcount++;
            if(grid[i][col] == false)
                    break;
            if(graph[i][col] == 'N')
            {
                int index = getIndex(i, col);
                edgeList[index] = Dcount;
                break;
            }
        }

        //check left
        
        for(int i = col-1; i>=0; i--)
        {
            Lcount++;
            if(grid[row][i] == false)
                break;
            if(graph[row][i] == 'N')
            {
                int index = getIndex(row, i);
                edgeList[index] = Lcount;
                break;
            }
        }

        //check right

        for(int i = col+1; i<= grid_Y; i++)
        {
            Rcount++;
            if(grid[row][i] == false)
                break;
            if(graph[row][i] == 'N')
            {
                int index = indexMap[Node(row,i)];
                edgeList[index] = Rcount;
                break;
            }
        }

        return edgeList;
    }

    void makeAdjacencyMatrix()
    {
        for(int i = 0; i<totNodes; i++)
        {
            adjacencyMatrix[i] = getEdgeList(nodeList[i]);
        }
    }

    void printDistanceMatrix()
    {
        for(int i = 0; i< totNodes; i++)
        {
            for(int j = 0; j< totNodes; j++)
                cout << adjacencyMatrix[i][j] << " ";
            cout << endl;
        }
    }

    void dfs_util()
    {

    }

    void dfs()
    {

    }

};

int main()
{

   Maze maze1("maze1.txt");
   std::cout<< "maze1: " << std::endl;

   maze1.print();
   maze1.plotGraph();

   std::cout << "\n\n" << "Ploting nodes on maze:" << std::endl;
   maze1.printGraph();
   
   cout << endl;
   maze1.printNodeList();

   maze1.makeAdjacencyMatrix();

   cout << endl << endl;

   maze1.printDistanceMatrix();
}

