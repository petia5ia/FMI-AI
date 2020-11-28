#include <iostream>
#include <set>
#include <string>
#include <vector>
using namespace std;

set<vector<vector<int> > > visited;
// botton, left, top, right for finding child nodes
int row[4] = { 1, 0, -1, 0 };
int col[4] = { 0, -1, 0, 1 };
// to store the visited configurations, if a repeated configuration is observed, ignore
int final[3][3] = { { 1, 2, 3 },
					{ 4, 5, 6 },
					{ 7, 8, 0 } };

// Structure of the node to be used in space search
struct Node
{
	vector<vector<int> > conf; // stores configuration of the tiles in each node
	int x, y;                  // stores blank tile(0) cordinates
	int h;                     // stores the heuristic cost to reach the goal state
	int g;                     // stores the actual cost to reach the tile from the start tile
	Node* parent;              // stores parent node of current node to print the sequence of steps or the path in the space search tree in the end
};

bool isGoal(vector<vector<int> > conf) {
	for (int i = 0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			if (conf[i][j] != final[i][j]) {
				return false;
			}
		}
	}
	return true;
}

int calculateManhattanCost(vector<vector<int> > initial)
{
	int dist = 0;
	for (int i = 0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			if (initial[i][j] != 0) {
				for (int k = 0; k<3; k++) {
					for (int l = 0; l<3; l++) {
						if (initial[i][j] == final[k][l]) dist += abs(i - k) + abs(j - l);
					}
				}
			}
		}
	}
	return dist;
}

// print path from initial state to goal state
void printPath(Node* node)
{
	int count = 0;
	int prevX, currX, prevY, currY;
	vector<string> vec;
	while (node != NULL) {
		count++;
		prevX = node->x;
		prevY= node->y;
		node = node->parent;
		
		if (node != NULL) {
			currX = node->x;
			currY = node->y;

			if (prevX - currX == 1) {
				vec.push_back("up");
			}
			else if (prevX - currX == -1) {
				vec.push_back("down");
			}
			else if (prevY - currY == 1) {
				vec.push_back("left");
			}
			else if (prevY - currY == -1) {
				vec.push_back("right");
			}
		}
	}

	cout << "\n" << count - 1 << " steps" << endl;

	for (int i = vec.size() - 1; i >= 0; i--) {
		cout << vec[i] << endl;
	}
}

// Function to expand or create a new node
Node* expandNode(vector<vector<int> > conf, int x, int y, int newX, int newY, int g, Node* parent)
{
	Node* node = new Node;
	node->parent = parent;
	node->conf = conf;
	// move blank tile by 1 postion
	swap(node->conf[x][y], node->conf[newX][newY]);
	node->h = INT_MAX;
	node->g = g;
	// update new blank tile cordinates
	node->x = newX;
	node->y = newY;
	return node;
}

// Helper recursive function for IDAStar algorithm, prunes the branches with higher costs than threshold.
// If an already searched node is observed in an iteration, it is skipped
int search(vector<Node*> path, set<vector<vector<int> > > path1, int g, int threshold) {
	Node* node = path.back();
	int f = g + calculateManhattanCost(node->conf);
	if (f>threshold) {
        return f;
    }
	if (isGoal(node->conf)) {
		printPath(node);
		return -1;
	}
	int min = INT_MAX;
	// do for each child of min node
	for (int i = 0; i < 4; i++) {
		Node* child = NULL;
		if (node->x + row[i] >= 0 && node->x + row[i] < 3 && node->y + col[i] >= 0 && node->y + col[i] < 3) {
			child = expandNode(node->conf, node->x, node->y, 
                            node->x + row[i], node->y + col[i], node->g + 1, node);
		}
		else {
            continue;
        }
		set<vector<vector<int> > >::iterator it = path1.find(child->conf);
		if (it == path1.end()) {
			path.push_back(child);
			pair< set<vector<vector<int> > >::iterator, bool> it1 = path1.insert(child->conf);
			int t = search(path, path1, g + 1, threshold);
			if (t == -1) {
                return -1;
            }
			if (t<min) {
                min = t;
            }
			path.pop_back();
			path1.erase(it1.first);
		}
	}
	return min;
}

// IDAStar search
// visited stores the nodes expanded in the most recent iteration
void IDAStar(vector<vector<int> > initial, int x, int y) {
	Node* root = expandNode(initial, x, y, x, y, 0, NULL);  // create a root node and calculate its h cost
	int threshold = calculateManhattanCost(root->conf);
	vector<Node*> path;
	set<vector<vector<int> > > path1;
	path.push_back(root);
	path1.insert(root->conf);
	while (1) {
		int t = search(path, path1, 0, threshold);
		if (t == -1) { // Solution found
			return;
		} else if (t == INT_MAX) { // No solution
			cout << "No Solution\n";
			return;
		}
		threshold = t;
	}
}

int main()
{
    int N, I;
    cin >> N >> I;
    
	N = sqrt(N + 1);

	int x = 0, y = 0; // stores blank tile(0) cordinates from the input/start state
	vector<vector<int> > initial(N, vector<int>(N, 0));
	
	for (int i = 0; i < initial.size(); i++) {
		for (int j = 0; j < initial.size(); j++) {
			cin >> initial[i][j];
			
			if (initial[i][j] == 0) {
				x = i;
				y = j;
			}
		}
	}

	IDAStar(initial, x, y);

	return 0;
}
