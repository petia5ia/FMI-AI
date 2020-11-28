#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class State
{
private:
	vector<int> state;
	int N;

public:
	State(int n) : N(n)
	{
		state.resize(N*N);
		for (unsigned int i = 0; i < state.size(); i++)
		{
			state[i] = i;
		}
	}

	State(int n, const vector<int>& arr) : N(n)
	{
		state = arr;
	}

	//copy constructor
	State(const State& other)
	{
		N = other.N;
		state = other.state;
	}

	//assignment operator
	State& operator = (const State& other)
	{
		if (this != &other)
		{
			N = other.N;
			state = other.state;
		}
		return *this;
	}

	//equal to operator
	friend bool operator == (const State& a, const State& b)
	{
		return (a.state == b.state);
	}

	//not equal to operator
	friend bool operator != (const State& a, const State& b)
	{
		return (a.state != b.state);
	}

	//find the index of the empty slot
	inline int FindEmptyTileIndex() const
	{
		for (unsigned int i = 0; i < state.size(); i++)
        {
			if (state[i] == 0) 
            {
				return i;
			}
		}
		return -1; //no empty tile
	}

	//swap the values of the indices
	inline void SwapWithEmpty(int i0, int i1)
	{
		int tmp = state[i1];
		state[i1] = state[i0];
		state[i0] = tmp;
	}

	inline const vector<int>& GetArray() const
	{
		return state;
	}

	inline int GetNumOfRows() const
	{
		return N;
	}
};

class Neighbours
{
public:
	map<int,vector<int>> edges;

	Neighbours()
	{
		CreateNeighbours();
	}

    //key is the index to the empty tile
	const vector<int>& GetNeighbours(int key) const
	{
		map<int,vector<int>>::const_iterator itr(edges.find(key));
		if (itr != edges.end())
        {
            return itr->second;
		}
        static vector<int> s;
		return s;
	}

private:
	void CreateNeighbours()
	{
		edges.insert(make_pair(0, vector<int>{1, 3}));
		edges.insert(make_pair(1, vector<int>{0, 2, 4}));
		edges.insert(make_pair(2, vector<int>{1, 5}));
		edges.insert(make_pair(3, vector<int>{4, 0, 6}));
		edges.insert(make_pair(4, vector<int>{3, 5, 1, 7}));
		edges.insert(make_pair(5, vector<int>{4, 2, 8}));
		edges.insert(make_pair(6, vector<int>{7, 3}));
		edges.insert(make_pair(7, vector<int>{6, 8, 4}));
		edges.insert(make_pair(8, vector<int>{7, 5}));
	}
};

class Node
{
private:
	State state;
	shared_ptr<Node> parent;
	int depth;

public:
	Node(const State& s, shared_ptr<Node> node, int d = 0) : state(s), depth(d)
	{
		parent = node;
	}

	void SetParent(shared_ptr<Node> node)
	{
		parent = node;
	}

	const shared_ptr<Node> GetParent() const
	{
		return parent;
	}

	const State& GetState() const
	{
		return state;
	}

	int GetDepth() const
	{
		return depth;
	}	
};

inline int GetManhattanCost(const State& st)
{
	int cost = 0;
	const vector<int>& state = st.GetArray();
	int rows = st.GetNumOfRows();
	
    for (unsigned int i = 0; i < state.size(); ++i)
	{
		int v = state[i];
		if (v == 0) continue;

		//actual index of v should be v-1
		v = v - 1;
		int gx = v % rows;
		int gy = v / rows;

		int x = i % rows;
		int y = i / rows;

		int manhattanCost = abs(x - gx) + abs(y - gy);
		cost += manhattanCost;
	}
	
    return cost;
}

class CompareFunctor
{
public:
	bool operator()( const shared_ptr<Node>& n1, const shared_ptr<Node>& n2) const
	{
		const State& state1 = n1->GetState();
		int cost1 = GetManhattanCost(state1) + n1->GetDepth();
		const State& state2 = n2->GetState();
		int cost2 = GetManhattanCost(state2) + n2->GetDepth();

		return cost1 < cost2;
	}
};

inline bool isInArray(const State& state, const vector<shared_ptr<Node> >& values)
{
	for (unsigned int i = 0; i < values.size(); ++i)
	{
		if (state == values[i]->GetState())
			return true;
	}
	return false;
}

class Solver
{
private:
    //holds all the nodes that need to be explored or visited
	vector< shared_ptr<Node> > openlist;
    //any node that had already been visited will be removed from openlist and added in the closedlist
	vector< shared_ptr<Node> > closedlist;
	const State& goal;
	bool solved;

public:
	Solver(const State& start, const State& new_goal) : goal(new_goal), solved(false)
	{
		shared_ptr<Node> root(new Node(start, 0, 0));
		openlist.push_back(root);
	}

	inline bool isSolved() const
	{
		return solved;
	}

	//Returns next node in the search
	shared_ptr<Node> GetNextNode()
	{
		if (openlist.empty()) 
        {
            return 0;
		}
        
        shared_ptr<Node> current;

		vector< shared_ptr<Node> >::iterator current_itr( min_element(
			openlist.begin(),
			openlist.end(),
			CompareFunctor() ) );

		if (current_itr == openlist.end())
        {
            return 0;
        }
        
		//copy the value first to a shared pointer and then erase from the open list
		current = *current_itr;

		//erase from the open list
		openlist.erase(current_itr);
		closedlist.push_back(current);

		return current;
	}

	//expand the graph by looking into the neighbours for the given node
	void ExpandNode(shared_ptr<Node> current, const Neighbours& graph)
	{
		if (current->GetState() == goal)
		{
			solved = true;
			return;
		}

		int zero = current->GetState().FindEmptyTileIndex();
		const vector<int>& neighbours = graph.GetNeighbours(zero);

		for (unsigned int i = 0; i < neighbours.size(); i++)
		{
			State state = current->GetState();
			state.SwapWithEmpty(zero, neighbours[i]);

			if (!isInArray(state, closedlist))
			{
				shared_ptr<Node> n(new Node(state, current, current->GetDepth() + 1));
				openlist.push_back(n);
			}
		}
	}
};

int main()
{
	int N, I, tile;
	vector<int> input;
	vector<int> goal_vector;

	cin >> N >> I;

	for (int i = 0; i < N + 1; i++)
	{
		cin >> tile;
		input.push_back(tile);
	}

	if ((I <= -1) || (I >= 8)) //invalid index
	{
		goal_vector = { 1, 2, 3, 4, 5, 6, 7, 8, 0 };
	}
	else
	{
		for (int i = 1; i < N + 1; i++)
		{
			if (i - 1 == I)
			{
				goal_vector.push_back(0);
			}
			goal_vector.push_back(i);
		}
	}

	N = sqrt(N + 1);

	Neighbours g;

	State goal(N, goal_vector);

	State start(N, input);

	shared_ptr<Node> node;
	Solver solver(start, goal);
	int count = 0;
	while (!solver.isSolved())
	{
		node = solver.GetNextNode();
		solver.ExpandNode(node, g);
		count++;
	}

	//accumulate the nodes for the solution.
	vector< shared_ptr<Node> > solution;
	shared_ptr<Node> s = node;
	do
	{
		solution.push_back(s);
		s = s->GetParent();
	} while (s != NULL);
	
	//Print the solution:
	int prev, curr, move;
	cout << solution.size() - 1 << endl;
	for (int i = (int)solution.size() - 2; i >= 0; i--)
	{
		prev = solution[i + 1]->GetState().FindEmptyTileIndex();
		curr = solution[i]->GetState().FindEmptyTileIndex();
		move = prev - curr;
		if (move == 1)
		{
			cout << "right" << endl;
		}
		else if (move == -1)
		{
			cout << "left" << endl;
		}
		else if (move > 1)
		{
			cout << "down" << endl;
		}
		else // move < -1
		{
			cout << "up" << endl;
		}
	}
	
	return 0;
}
