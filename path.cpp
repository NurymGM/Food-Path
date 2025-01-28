#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;

const int gridHeight = 10;
const int gridWidth = 10;

vector < vector <char> > grid(gridHeight, vector <char> (gridWidth, '+'));

void defineObstacles() 
{
    grid[2][0] = '#';
    grid[2][1] = '#';
    grid[2][2] = '#';
    grid[2][3] = '#';

    grid[4][2] = '#';
    grid[4][3] = '#';
    grid[4][4] = '#';
    grid[4][5] = '#';

    grid[8][9] = '#';
    grid[8][8] = '#';
    grid[8][7] = '#';
    grid[8][6] = '#';
    grid[8][5] = '#';
}

pair <int, int> location;

struct Node 
{
    int x, y;
    int g; 
    int h; 
    int f;
    Node* parent;
    Node(int x, int y, int g, int h, Node* parent)
        : x(x), y(y), g(g), h(h), f(g + h), parent(parent) {}
};

struct CompareNode 
{
    bool operator()(const Node* a, const Node* b) 
    {
        return a->f > b->f;
    }
};

double heuristic(int x1, int y1, int x2, int y2) 
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void aStar(pair <int, int> start, pair <int, int> goal) 
{
    priority_queue <Node*, vector <Node*>, CompareNode> openSet;
    vector < vector <bool> > closedSet(gridHeight, vector <bool> (gridWidth, false));
    
    Node* startNode = new Node(start.first, start.second, 0, heuristic(start.first, start.second, goal.first, goal.second), nullptr);
    
    openSet.push(startNode);
    
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};
    
    while (!openSet.empty()) 
    {
        Node* current = openSet.top();
        openSet.pop();
        
        closedSet[current->x][current->y] = true;
        
        if (current->x == goal.first && current->y == goal.second) 
        {
            vector < pair <int, int> > path;
            while (current != nullptr) 
            {
                path.push_back({current->x, current->y});
                current = current->parent;
            }
            
            cout << "Shortest Path:" << endl;
            for(int i = path.size() - 1; i >= 0; --i) 
            {
                grid[path[i].first][path[i].second] = '.';
                cout << "(" << path[i].first << ", " << path[i].second << ")";
                if (i > 0) cout << " -> ";
            }
            cout << endl;
            
            while (!openSet.empty()) 
            {
                delete openSet.top();
                openSet.pop();
            }
            return;
        }
        
        for (int i = 0; i < 4; i++) 
        {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];
            
            if (newX >= 0 && newX < gridHeight && newY >= 0 && newY < gridWidth && grid[newX][newY] != '#' && !closedSet[newX][newY]) 
            {
                int tentativeG = current->g + 1;
                
                bool isBetter = false;
                Node* neighborNode = nullptr;

                bool found = false;
                vector<Node*> openSetCopy;
                while (!openSet.empty()) 
                {
                    openSetCopy.push_back(openSet.top());
                    openSet.pop();
                }

                for (Node* node : openSetCopy) 
                {
                    if (node->x == newX && node->y == newY) 
                    {
                        neighborNode = node;
                        found = true;
                        break;
                    }
                }

                for (Node* node : openSetCopy) 
                {
                    openSet.push(node);
                }

                if (!found || tentativeG < neighborNode->g) 
                {
                    if (!found) 
                    {
                        neighborNode = new Node(newX, newY, tentativeG, heuristic(newX, newY, goal.first, goal.second), current);
                    } 
                    else 
                    {
                        neighborNode->g = tentativeG;
                        neighborNode->f = tentativeG + neighborNode->h;
                        neighborNode->parent = current;
                    }
                    isBetter = true;
                }
                
                if (isBetter) 
                {
                    openSet.push(neighborNode);
                }
            }
        }
    }
    
    cout << "No path found." << endl;
}

int main()
{
    defineObstacles();
    int curX = 0, curY = 0;

    while(1)
    {      
        while(1)
        {
            cout << "Enter X coordinate of Food: ";
            cin >> location.first;
            if(location.first >= 0 && location.first < gridHeight)
            {
                break;
            }   
            else
            {
                cout << "Invalid Input, try again!\n";
            }
        }
        while(1)
        {
            cout << "Enter Y coordinate of Food: ";
            cin >> location.second;
            if(location.second >= 0 && location.second < gridWidth)
            {
                break;
            }   
            else
            {
                cout << "Invalid Input, try again!\n";
            }
        }

        grid[curX][curY] = 'X';
        grid[location.first][location.second] = '0';

        for(int i = 0; i < 10; i++)
        {
            for(int j = 0; j < 10; j++)
            {
                cout << grid[i][j] << " ";
            }   cout << "\n";
        }

        aStar({curX, curY}, location);

        for(int i = 0; i < 10; i++)
        {
            for(int j = 0; j < 10; j++)
            {
                cout << grid[i][j] << " ";
            }   cout << "\n";
        }

        char again;
        cout << "Do you have something to buy? (y/n): ";
        cin >> again;
        if(again != 'y' && again != 'Y')
        {
            break;
        }
        else
        {
            for(int i = 0; i < 10; i++)
            {
                for(int j = 0; j < 10; j++)
                {
                    if(grid[i][j] == '.')
                    {
                        grid[i][j] = '+';
                    }
                }
            }
            curX = location.first;
            curY = location.second;
        }
    }

    return 0;
}
