#pragma once
#include <raylib.h>
#include <vector>
#include <string>
#include <iostream>

#include <queue>
#include <algorithm>

class Pathfinding
{
};

struct Node;

struct Edge
{
	Node* target;
	float cost;

	Edge() { target = nullptr; cost = 0; }

	Edge(Node* _target, float _cost) : target(_target), cost(_cost) {}
};

struct Node
{
	Vector2 position;
	std::vector<Edge> connections;

	int gScore;
	Node* parent;

	Node(float x, float y)
	{
		position.x = x;
		position.y = y;

		gScore = 0;
		parent = nullptr;
	}

	void ConnectTo(Node* other, float cost) { connections.push_back(Edge(other, cost)); }

	// A* implementation
	int fScore;	// Final score -- G + H score.
	int hScore; // Heuristic score
};

class NodeMap
{
public:
	// Define publicly-usable enums
	enum class DiagonalTypes
	{
		None,
		CutCorners,
		NoCutCorners
	};

private:
	int m_width, m_height;
	static float m_cellSize;

	Node** m_nodes;	// Dynamically-allocated 1D array of Node pointers.

	int (*m_heuristic_func)(Node* targetNode, Node* endNode);	// Store pointer of the heuristic function.

public:
	void Initialise(std::vector<std::string> asciiMap, int cellSize, DiagonalTypes diagonal);

	Node* GetNode(int x, int y) { return m_nodes[x + m_width * y]; }	// Returns a node of the given graph coordinates.
	void Draw();	// Draws the nodegraph: squares as walls and empty spaces as connected lines.
	void DrawPath(std::vector<Node*> nodeMapPath, Color lineColour);	// Draws the current calculated path as connected lines.

	static std::vector<Node*> DijkstraSearch(Node* startNode, Node* endNode);	// Creates a shortest path using Dijkstra's algorithm.

	// Creates a most-probably shortest path using the A* algorithm, given a heuristic.
	static std::vector<Node*> AStarSearch(Node* startNode, Node* endNode, float (*heuristic_func)(Node* targetNode, Node* endNode));

	static void SortScores(std::vector<Node*> list, bool AStar = false);	// Sorts the Open List by their G scores (or F scores if AStar == true).

	Node* GetClosestNode(Vector2 worldPos);	// Returns the node closest to the given world position.

	Vector2 NodeWPos(int x, int y);	// Node world position -- proportional to the m_cellsize and any offsets.
	static Vector2 NodeWPos(Node* node);	// Node world position -- proportional to the m_cellsize and any offsets.
	static float CellSize() { return m_cellSize; };	// Returns m_cellsize.

	static float Heu_Manhattan(Node* targetNode, Node* endNode);	// Sum of absolute X and Y difference.
	static float Heu_Euclidean(Node* targetNode, Node* endNode);	// Literal distance.
	static float Heu_Bogo(Node* targetNode, Node* endNode);	// Random.
};

class PathAgent
{
public:
	// Define enums publically first...
	enum class Heuristics
	{
		Manhattan,
		Euclidean,
		Bogo
	};

	enum class Algorithms
	{
		Dijkstra,
		AStar
	};

	enum class Shapes
	{
		Circle,
		Diamond,
		Square
	};

private:
	Vector2 m_position;

	float (*m_heuristic)(Node* targetNode, Node* endNode);	// Function pointer for pathing heuristic.
	Algorithms m_algorithm;	// The search algorithm this agent will use...


	std::vector<Node*> m_path;
	int m_currentIndex;
	Node* m_currentNode;

	float m_speed;
	
	NodeMap* m_nodeMap;	// What node map this agent is part of.

	Shapes m_shape;

public:

	PathAgent();	// Default constructor--assumes Euclidean heuristic.

	// Constructor defining heuristic, assigned nodemap, the spawn position and the chosen search algorithm...
	PathAgent(Algorithms algorithm, Heuristics heuristic, NodeMap* nodeMap, Vector2 position);

	Vector2 target = { GetMousePosition() };

	void Update(float deltaTime);
	void GoToNode(Node* node);	// Sets the agent on the shortest path to the given node.

	// Sets the agent on the shortest path to the given world position. If ratined, does not clear previous path if a new given path is invalid.
	void GoToPos(Vector2 worldPosition, bool retainPath);
	void Draw();
	void SetNode(Node* node) { m_currentNode = node; };
	void SetSpeed(int speed) { m_speed = speed; };
	std::vector<Node*> GetPath() { return m_path; };
	Vector2 GetPosition()  { return m_position; };
	void SetHeuristic(Heuristics heuristic);	// Set the pathing heuristic of the agent.
	void SetMap(NodeMap* nodeMap);	// Assigns the agent to the given node map.

	void SetPosition(Vector2 position) { m_position = position; };	// Sets the agent's position.

	void SetShape(Shapes shape) { m_shape = shape; };	// Sets shape of agent--debugging multiple agents...
};