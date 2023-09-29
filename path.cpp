#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;

// size of the supermarket
const int gridHeight = 10;
const int gridWidth = 10;

vector < vector <char> > grid(gridHeight, vector <char> (gridWidth, '+'));

// Define obstacle locations (for example, walls represented as '#')
void defineObstacles() 
{
    // Add walls to the store layout
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
    // Add more obstacles as needed
}

// Define the location of the product (onions, represented as 'O')

pair <int, int> location;

// Define node structure
struct Node 
{
    int x, y;
    int g; // Cost from start to current node
    int h; // Heuristic (estimated) cost from current node to goal
    int f; // Total cost (g + h)
    Node* parent;
    Node(int x, int y, int g, int h, Node* parent)
        : x(x), y(y), g(g), h(h), f(g + h), parent(parent) {}
};

// Define a custom comparison function for the priority queue
struct CompareNode 
{
    bool operator()(const Node* a, const Node* b) 
    {
        return a->f > b->f;
    }
};

// Define the heuristic function (Euclidean distance)
double heuristic(int x1, int y1, int x2, int y2) 
{
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// Implement A* algorithm
void aStar(pair <int, int> start, pair <int, int> goal) 
{
    // Create the open and closed sets
    priority_queue <Node*, vector <Node*>, CompareNode> openSet;
    vector < vector <bool> > closedSet(gridHeight, vector <bool> (gridWidth, false));
    
    // Create the start node
    Node* startNode = new Node(start.first, start.second, 0, heuristic(start.first, start.second, goal.first, goal.second), nullptr);
    
    // Add the start node to the open set
    openSet.push(startNode);
    
    // Define possible moves (up, down, left, right)
    const int dx[] = {-1, 1, 0, 0};
    const int dy[] = {0, 0, -1, 1};
    
    while (!openSet.empty()) 
    {
        // Get the node with the lowest f value from the open set
        Node* current = openSet.top();
        openSet.pop();
        
        // Mark the current node as visited
        closedSet[current->x][current->y] = true;
        
        // Check if we've reached the goal
        if (current->x == goal.first && current->y == goal.second) 
        {
            // We found the goal, reconstruct the path
            vector < pair <int, int> > path;
            while (current != nullptr) 
            {
                path.push_back({current->x, current->y});
                current = current->parent;
            }
            
            // Print the path in reverse order (from start to goal)
            cout << "Shortest Path:" << endl;
            for(int i = path.size() - 1; i >= 0; --i) 
            {
                grid[path[i].first][path[i].second] = '.';
                cout << "(" << path[i].first << ", " << path[i].second << ")";
                if (i > 0) cout << " -> ";
            }
            cout << endl;
            
            // Clean up memory (delete nodes)
            while (!openSet.empty()) 
            {
                delete openSet.top();
                openSet.pop();
            }
            return;
        }
        
        // Generate neighbors
        for (int i = 0; i < 4; i++) 
        {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];
            
            // Check if the neighbor is within bounds and not an obstacle
            if (newX >= 0 && newX < gridHeight && newY >= 0 && newY < gridWidth && grid[newX][newY] != '#' && !closedSet[newX][newY]) 
            {
                // Calculate tentative g value
                int tentativeG = current->g + 1; // Assuming uniform cost for movement
                
                // Check if this path to the neighbor is better
                bool isBetter = false;
                Node* neighborNode = nullptr;

                bool found = false;
                // Convert the priority_queue to a vector for searching
                vector<Node*> openSetCopy;
                while (!openSet.empty()) 
                {
                    openSetCopy.push_back(openSet.top());
                    openSet.pop();
                }

                // Search for the neighbor node
                for (Node* node : openSetCopy) 
                {
                    if (node->x == newX && node->y == newY) 
                    {
                        neighborNode = node;
                        found = true;
                        break;
                    }
                }

                // Restore the priority_queue
                for (Node* node : openSetCopy) 
                {
                    openSet.push(node);
                }

                if (!found || tentativeG < neighborNode->g) 
                {
                    if (!found) 
                    {
                        // Create a new neighbor node
                        neighborNode = new Node(newX, newY, tentativeG, heuristic(newX, newY, goal.first, goal.second), current);
                    } 
                    else 
                    {
                        // Update the existing neighbor node
                        neighborNode->g = tentativeG;
                        neighborNode->f = tentativeG + neighborNode->h;
                        neighborNode->parent = current;
                    }
                    isBetter = true;
                }
                
                // Add the neighbor to the open set
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

        // Call the A* algorithm to find the shortest path
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