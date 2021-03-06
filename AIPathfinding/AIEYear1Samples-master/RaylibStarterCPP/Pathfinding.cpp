#include "Pathfinding.h"

// Declare static members to keep the linker happy
//std::vector<Node*> NodeMap::DijkstraSearch(Node* startNode, Node* endNode);
float NodeMap::m_cellSize;

void NodeMap::Initialise(std::vector<std::string> asciiMap, int cellSize, DiagonalTypes diagonal)
{
	m_cellSize = cellSize;
	const char emptySquare = '|';

	// Assume all strings are the same length, so we'll size the map according to the num of strings AND the length of the first string.
	m_height = asciiMap.size();
	m_width = asciiMap[0].size();

	m_nodes = new Node * [m_width * m_height];

	// Loop over the strings, creating Node entries as we go
	for (int y = 0; y < m_height; y++)
	{
		std::string& line = asciiMap[y];
		// Report to the user that you've a mis-matched string length

		if (line.size() != m_width)
			std::cout << "Mismatched line #" << y << " in ASCII map (" << line.size() << " instead of " << m_width << ")" << std::endl;

		for (int x = 0; x < m_width; x++)
		{
			// Get the x-th char, or return an empty node if the string isn't long enough
			char tile = x < line.size() ? line[x] : emptySquare;

			// Create a node for anything but a "." character
			m_nodes[x + m_width * y] = tile == emptySquare ? nullptr : new Node(x, y);
			
			
		}
	}

	// Now loop over the nodes, creating connections between each node and its neigbour to the West and South on the grid. This will link up all nodes.
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			Node* node = GetNode(x, y);
			if (node)
			{
				// See if there's a node to our West, checking for array overruns first if we're on the west-most edge
				Node* nodeWest = x == 0 ? nullptr : GetNode(x - 1, y);

				if (nodeWest)
				{
					node->ConnectTo(nodeWest, 10);	// TODO weights
					nodeWest->ConnectTo(node, 10);
				}

				// See if there's a node North of us, checking for array index overruns again.
				Node* nodeNorth = y == 0 ? nullptr : GetNode(x, y - 1);
				if (nodeNorth)
				{
					node->ConnectTo(nodeNorth, 10);	// TODO weights
					nodeNorth->ConnectTo(node, 10);
				}
				
				// DIAGONAL LINKING
				// Only attempt diagonal linking if specified in graph initiation.
				if (diagonal != DiagonalTypes::None)
				{
					// Only bother trying to get the southern node if we aren't already on the bottom-edge of the graph
					// This node is purely used for testing South-West corner cutting
					Node* nodeSouth = y >= m_height - 1 ? nullptr : GetNode(x, y + 1);

					// See if there's a node North-West available
					if (x != 0 && y != 0)
					{
						Node* nodeNW = GetNode(x - 1, y - 1);	// Get the north-west node
						
						// Check if the N-W node exists (and if we're not cutting corners, there are neighbouring nodes)
						if ((nodeNW && diagonal == DiagonalTypes::CutCorners) || (nodeNW && diagonal == DiagonalTypes::NoCutCorners && nodeWest && nodeNorth))
						{
							node->ConnectTo(nodeNW, 14);
							nodeNW->ConnectTo(node, 14);
						}
					}

					// See if there's a node South-West available
					if (x != 0 && y != m_height - 1)
					{
						Node* nodeSW = GetNode(x - 1, y + 1);	// Get the south-west node

						// Check if the S-W node exists (and if we're not cutting corners, there are neighbouring nodes)
						if ((nodeSW && diagonal == DiagonalTypes::CutCorners) || (nodeSW && diagonal == DiagonalTypes::NoCutCorners && nodeWest && nodeSouth))
						{
							node->ConnectTo(nodeSW, 14);
							nodeSW->ConnectTo(node, 14);
						}
					}
				}
			}
		}
	}

	// Pac-Man warps...
	//GetNode(0, 14)->ConnectTo(GetNode(m_width - 1, 14), 1);
	//GetNode(m_width - 1, 14)->ConnectTo(GetNode(0, 14), 1);
}

void NodeMap::Draw()
{
	// Red colour for the blocks
	Color cellColor = RED;
	Color lineColor = { 0, 121, 241, 50 };

	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			Node* node = GetNode(x, y);
			if (node == nullptr)
			{
				// Draw a solid block in empty squares w/o a navigation node
				DrawRectangle((int)(x * m_cellSize), (int)(y * m_cellSize), (int)m_cellSize - 1, (int)m_cellSize - 1, cellColor);	// the -1 means we'll have a 1-pixel gap between walls so we can visualise the grid better.
			}
			else
			{
				// Draw the connections between the node and its neighbours
				for (int i = 0; i < node->connections.size(); i++)
				{
					Node* other = node->connections[i].target;
	
					DrawLine((x + 0.5f) * m_cellSize, (y + 0.5f) * m_cellSize,
						(other->position.x + 0.5f) * m_cellSize,
						(other->position.y + 0.5f) * m_cellSize,
						lineColor);
				}

				DrawCircle(x * m_cellSize + m_cellSize / 2, y * m_cellSize + m_cellSize / 2, 2, lineColor);	// Draw a dot per node.
			}
		}
	}
}

void NodeMap::DrawPath(std::vector<Node*> nodeMapPath, Color lineColour)
{
	// First check that there's even a path to draw...
	if (nodeMapPath.size() > 0)
	{
		for (int i = 0; i < nodeMapPath.size() - 1; i++)
		{
			DrawLine((nodeMapPath[i]->position.x + 0.5f) * m_cellSize, (nodeMapPath[i]->position.y + 0.5f) * m_cellSize,
				(nodeMapPath[i + 1]->position.x + 0.5f) * m_cellSize,
				(nodeMapPath[i + 1]->position.y + 0.5f) * m_cellSize,
				lineColour);
		}
	}
}

std::vector<Node*> NodeMap::DijkstraSearch(Node* startNode, Node* endNode)
{
	std::cout << "==Dijkstra search==" << std::endl;

	std::vector<Node*> Path;

	// Validate input
	if (!startNode || !endNode || startNode == endNode)
	{
		std::cout << "Input is invalid/start == end" << std::endl;
	}
	else
	{
		// Initialise starting node
		startNode->gScore = 0;
		startNode->parent = nullptr;

		endNode->gScore = 0;
		endNode->parent = nullptr;

		// Create temp lists for storing nodes we're visiting/visited.
		std::vector<Node*> openList;
		std::vector<Node*> closedList;

		openList.push_back(startNode);

		while (!openList.empty())
		{
			//std::sort(openList.begin(), openList.end(), [](const int& a, const int& b) { return a < b; });
			// TODO: create custom sorting algo for the openList -- for each node's gScore in ascending order (lowest-highest)
			SortScores(openList, false);

			Node* currentNode = openList[0];

			// If we visit the endNode, then we can exit early.
			// Sorting the openList above guarantees the shortest path is found,
			// given no negative costs (a prerequisite of the algorithm).
			// This is an optional optimisation that improves performance,
			// but doesn't always guarantee the shortest path!
			if (currentNode == endNode)
				break;

			openList.erase(openList.begin());
			closedList.push_back(currentNode);

			for (int i = 0; i < currentNode->connections.size(); i++)
			{
				if (std::find(closedList.begin(), closedList.end(), currentNode->connections[i].target) == closedList.end())
				{
					int gScore = currentNode->gScore + currentNode->connections[i].cost;

					//std::cout << "G = " << gScore << std::endl;

					// Have not yet visited the node
					// So, calculate the Score and update its parent.
					// Also add it to the openList
					if (std::find(openList.begin(), openList.end(), currentNode->connections[i].target) == openList.end())
					{
						currentNode->connections[i].target->gScore = gScore;
						currentNode->connections[i].target->parent = currentNode;
						openList.push_back(currentNode->connections[i].target);
					}

					// Node is already in the openList with a valid Score.
					// So, compare the calculated Score with the existing to find the shorter path.
					else if (gScore < currentNode->connections[i].target->gScore)
					{
						currentNode->connections[i].target->gScore = gScore;
						currentNode->connections[i].target->parent = currentNode;
					}
				}
			}

		}

		if (!endNode->parent)
		{
			std::cout << "Could not find path!" << std::endl;
			return Path;	// Return the empty path now, otherwise the endNode is still returned as the path, making the agent fly over to it.
		}

		// Create a Path in reverse from endNode to startNode
		Node* currentNode = endNode;

		while (currentNode)
		{
			Path.push_back(currentNode);
			currentNode = currentNode->parent;
		}
	}

	return Path;
}

std::vector<Node*> NodeMap::AStarSearch(Node* startNode, Node* endNode, float (*heuristic_func)(Node* targetNode, Node* endNode))
{
	std::cout << "==A* search==" << std::endl;
	std::vector<Node*> Path;	// The final path to add to and return later. Returns empty for and  invalid input/impossible path.

	// If either of the given nodes are invalid or the same, we can't path-find.
	if (!startNode || !endNode || startNode == endNode)
	{
		std::cout << "Input is invalid/start == end" << std::endl;	// Error feedback.
	}
	else
	{
		// Initialise starting node
		startNode->gScore = 0;
		startNode->parent = nullptr;

		endNode->gScore = 0;
		endNode->parent = nullptr;

		// Create temp lists for storing nodes we're visiting/visited.
		std::vector<Node*> openList;
		std::vector<Node*> closedList;

		openList.push_back(startNode);

		while (!openList.empty())
		{
			//std::sort(openList.begin(), openList.end(), [](const int& a, const int& b) { return a < b; });
			// TODO: create custom sorting algo for the openList -- for each node's gScore in ascending order (lowest-highest)
			SortScores(openList, true);

			Node* currentNode = openList[0];

			// If we visit the endNode, then we can exit early.
			// Sorting the openList above guarantees the shortest path is found,
			// given no negative costs (a prerequisite of the algorithm).
			// This is an optional optimisation that improves performance,
			// but doesn't always guarantee the shortest path!
			if (currentNode == endNode)
				break;

			openList.erase(openList.begin());
			closedList.push_back(currentNode);

			// Loop through each of this node's connected nodes via its Edges.
			for (int i = 0; i < currentNode->connections.size(); i++)
			{
				int hScore = (*heuristic_func)(currentNode->connections[i].target, endNode);
				int gScore = currentNode->gScore + currentNode->connections[i].cost * (*heuristic_func)(currentNode->connections[i].target, endNode);
				// Although it says the heuristic should be allistic; not overestimate; multiplying by the heuristic function returns much better 
				// results while retaining unique pathfinding for different heuristics and algorithms.
				int fScore = gScore + hScore;

				// Is the connected node in the open list?
				if (std::find(openList.begin(), openList.end(), currentNode->connections[i].target) != openList.end())
				{
					if (currentNode->connections[i].target->gScore <= gScore)
						continue;
					//int gScore = currentNode->gScore + currentNode->connections[i].cost;

					//int hScore = (*heuristic_func)(currentNode->connections[i].target, endNode);


					// Use the given heuristic function to calculate the hScore.
					//int hScore = (*heuristic_func)(currentNode->connections[i].target, endNode);
					//int fScore = gScore + hScore;	// Final score based on G and H score.

					//std::cout << "H F = " << hScore << " | " << fScore << std::endl;
				}
					// Have not yet visited the node
					// So, calculate the Score and update its parent.
					// Also add it to the openList
				else if (std::find(closedList.begin(), closedList.end(), currentNode->connections[i].target) != closedList.end())
				{
					if (currentNode->connections[i].target->gScore <= gScore)
						continue;
					else
					{
						openList.push_back(currentNode->connections[i].target);
						closedList.erase(std::find(closedList.begin(), closedList.end(), currentNode->connections[i].target));
					}

					//currentNode->connections[i].target->gScore = gScore;
					////currentNode->connections[i].target->fScore = fScore;
					//currentNode->connections[i].target->parent = currentNode;
					//openList.push_back(currentNode->connections[i].target);
				}
				else
				{
					openList.push_back(currentNode->connections[i].target);
					currentNode->connections[i].target->hScore = hScore;
					//currentNode->connections[i].target->fScore = fScore;
				}

				currentNode->connections[i].target->gScore = gScore;
				currentNode->connections[i].target->parent = currentNode;
			}
		}

		if (openList[0] != endNode)
		{
			std::cout << "Could not find path!" << std::endl;
			return Path;	// Return the empty path now, otherwise the endNode is still returned as the path, making the agent fly over to it.
		}

		// Create a Path in reverse from endNode to startNode
		Node* currentNode = endNode;

		while (currentNode)
		{
			Path.push_back(currentNode);
			currentNode = currentNode->parent;
		}
	}

	std::cout << "---" << std::endl;
	return Path;
}

void NodeMap::SortScores(std::vector<Node*> list, bool AStar)
{
	bool isSorted = false;

	while (!isSorted)
	{
		isSorted = true;

		for (int i = 0; i < list.size() - 1; i++)
		{
			// Compare either the G scores or F scores depending on the sort type given...
			if (AStar)
			{
				if (list[i]->fScore > list[i + 1]->fScore)
				{
					std::swap(list[i], list[i + 1]);	// Swap the two members
					isSorted = false;
				}
			}
			else
			{
				if (list[i]->gScore > list[i + 1]->gScore)
				{
					std::swap(list[i], list[i + 1]);	// Swap the two members
					isSorted = false;
				}
			}
		}
	}
}

Node* NodeMap::GetClosestNode(Vector2 worldPos)
{
	int x = (int)(worldPos.x / m_cellSize);
	int y = (int)(worldPos.y / m_cellSize);

	if (x < 0 || x >= m_width || y < 0 || y >= m_height)
		return nullptr;
	else
		return GetNode(x, y);
}

Vector2 NodeMap::NodeWPos(int x, int y)
{
	Node* node = GetNode(x, y);

	Vector2 wPos = { (node->position.x * m_cellSize) + 0.5f, (node->position.y * m_cellSize) + 0.5f };
	return wPos;
}

Vector2 NodeMap::NodeWPos(Node* node)
{
	Vector2 wPos = { (node->position.x * m_cellSize) + 0.5f, (node->position.y * m_cellSize) + 0.5f };
	return wPos;
}

float NodeMap::Heu_Euclidean(Node* targetNode, Node* endNode)
{
	// Return the literal distance of the two given nodes. Square-root operation omitted for faster calculation--proportions compared will all be the same.
	return (
		(endNode->position.x - targetNode->position.x) * (endNode->position.x - targetNode->position.x) +
		(endNode->position.y - targetNode->position.y) * (endNode->position.y - targetNode->position.y)
	);
}

float NodeMap::Heu_Bogo(Node* targetNode, Node* endNode)
{
	// Seemingly random based on the given nodes...
	float dist = (
		(endNode->position.x - targetNode->position.x) * (endNode->position.x - targetNode->position.x) +
		(endNode->position.y - targetNode->position.y) * (endNode->position.y - targetNode->position.y)
		);

	float random = rand() % 5 + 1 * dist;

	return random;
}

float NodeMap::Heu_Manhattan(Node* targetNode, Node* endNode)
{
	// Accumilate absolute X and Y distances between the given nodes. 
	return (
		abs(endNode->position.x - targetNode->position.x) + abs(endNode->position.x - targetNode->position.x) +
		abs(endNode->position.y - targetNode->position.y) + abs(endNode->position.y - targetNode->position.y)
		);
}

///////////

PathAgent::PathAgent()
{
	SetHeuristic(Heuristics::Euclidean);
}

PathAgent::PathAgent(Algorithms algorithm, Heuristics heuristic, NodeMap* nodeMap, Vector2 position)
{
	m_algorithm = algorithm;
	SetHeuristic(heuristic);
	SetMap(nodeMap);
	SetPosition(position);
}

void PathAgent::Update(float deltaTime)
{
	// Only do anything if we have a path to move along.
	if (!m_path.empty())
	{
		//Record current and next node to reduce headaches...
		Vector2 currNode = { (m_path[m_currentIndex]->position.x + 0.5f) * NodeMap::CellSize(), (m_path[m_currentIndex]->position.y + 0.5f) * NodeMap::CellSize()};
		//Vector2 nextNode = { (m_path[m_currentIndex + 1]->position.x + 0.5f) * 50, (m_path[m_currentIndex + 1]->position.y + 0.5f) * 50};

		// Calculate the distance to the next node
		float magnitude = sqrtf(
			(currNode.x - m_position.x) * (currNode.x - m_position.x) +
			(currNode.y - m_position.y) * (currNode.y - m_position.y)
		);

		// Calculate a vector between the agent and the current node
		Vector2 vecDelta = { (currNode.x - m_position.x), (currNode.y - m_position.y) };

		// Calculate the unit vector to the next node (normalise it)
		Vector2 vecUnit = { vecDelta.x / magnitude, vecDelta.y / magnitude };
		
		magnitude -= (m_speed * deltaTime);

		// If the distance is more than 0, we're just heading towards the node.
		// Otherwise, we've overshot it this frame

		if (magnitude > 0)
		{
			m_position.x += m_speed * deltaTime * vecUnit.x;
			m_position.y += m_speed * deltaTime * vecUnit.y;
		}
		else
		{
			// Overshot
			m_currentIndex += 1;

			//if (m_currentIndex > 1)
			//	GoToPos(GetMousePosition(), true);
			//std::cout << "Index = " << m_currentIndex << std::endl;
			

			// Is this the end?
			if (m_currentIndex - 1 == m_path.size() - 1)
			{
				// Yes, this is the final node.
				m_position = currNode;	// Set agent's position to the last node's position.
				m_path.clear();	// Clear path to stop future pathing. We're done here.
			}
			else
			{
				// No, there is another.
				// Compensate for overshot-ness by using it for the next node...
				
			}

		}

		// Brain no work--simple solution for now

		//if (m_path.size() > 1)
		//{
		//	if (m_position.x < (m_path[m_currentIndex + 1]->position.x + 0.5f) * 50)
		//		m_position.x += m_speed;
		//	else if (m_position.x > (m_path[m_currentIndex + 1]->position.x + 0.5f) * 50)
		//		m_position.x -= m_speed;
		//
		//
		//	if (m_position.y < (m_path[m_currentIndex + 1]->position.y + 0.5f) * 50)
		//		m_position.y += m_speed;
		//	else if (m_position.y > (m_path[m_currentIndex + 1]->position.y + 0.5f) * 50)
		//		m_position.y -= m_speed;
		//
		//	if (m_position.x == (m_path[m_currentIndex + 1]->position.x + 0.5f) * 50 && m_position.y == (m_path[m_currentIndex + 1]->position.y + 0.5f) * 50)
		//	{
		//		m_currentIndex++;
		//
		//		if (m_currentIndex >= m_path.size() - 1)
		//		{
		//			// finished
		//			m_path.clear();
		//		}
		//		else
		//		{
		//			m_currentNode = m_path[m_currentIndex];
		//		}
		//	}
		//}
	}
}

void PathAgent::GoToNode(Node* node)
{
	// TODO: check if our agent was already pathing somewhere. If so, choose that path's currNode as the starting point.

	if (m_nodeMap)
		SetNode(m_nodeMap->GetClosestNode(m_position));


	// Use the defined searching algorithm for this agent..

	switch (m_algorithm)
	{
		case Algorithms::AStar:
			m_path = NodeMap::AStarSearch(m_currentNode, node, m_heuristic);	// Uses A* with the heuristic defined for this agent.
			break;

		case Algorithms::Dijkstra:
			m_path = NodeMap::DijkstraSearch(m_currentNode, node);
			break;
	}

	// Since the algorithms return a list bottom-up, we want to reverse it so we're given a path from start-to-finish.
	std::reverse(m_path.begin(), m_path.end());
	m_currentIndex = 0;	// Reset the current index...
}

void PathAgent::GoToPos(Vector2 worldPosition, bool retainPath)
{
	if (m_nodeMap)
	{
		Node* endNode = m_nodeMap->GetClosestNode(worldPosition);

		// If we aren't retaining the current path, attempt to go to the node regardless if it's valid or not (clears current path).
		// If we are retaining the current path, make sure the end node is valid, otherwise don't try pathing to the new location.
		if ((retainPath && endNode != nullptr) || !retainPath)
			GoToNode(endNode);
	}
}

void PathAgent::Draw()
{
	// Draw the agent itself, based on what shape we gave it...

	switch (m_shape)
	{
	case Shapes::Circle:
		DrawCircle((int)m_position.x, (int)m_position.y, 8, YELLOW);
		break;

	case Shapes::Diamond:
		DrawRectanglePro({ m_position.x, m_position.y - (NodeMap::CellSize() / 1.5f), 20, 20 }, { 0, 0 }, 45, PINK);
		break;

	case Shapes::Square:
		DrawRectanglePro({ 0, 0, 20, 20 }, { -m_position.x + NodeMap::CellSize() / 2, -m_position.y + NodeMap::CellSize() / 2 }, 0, DARKGREEN);
		break;
	}


	// Draw some debug circles to tell us the position of the current (purple) and next (green) nodes.
	if (!m_path.empty())
	{
		DrawCircleLines((int)(m_path[m_currentIndex]->position.x + 0.5f) * NodeMap::CellSize() + (NodeMap::CellSize() / 2), (int)(m_path[m_currentIndex]->position.y + 0.5f) * NodeMap::CellSize() + (NodeMap::CellSize() / 2), 5, PURPLE);
		
		if (m_currentIndex < m_path.size() - 1)
			DrawCircleLines((int)(m_path[m_currentIndex + 1]->position.x + 0.5f) * NodeMap::CellSize() + (NodeMap::CellSize() / 2), (int)(m_path[m_currentIndex + 1]->position.y + 0.5f) * NodeMap::CellSize() + (NodeMap::CellSize() / 2), 5, GREEN);
			
	}
	
}

void PathAgent::SetHeuristic(Heuristics heuristic)
{
	switch (heuristic)
	{
	case Heuristics::Manhattan:
		m_heuristic = &NodeMap::Heu_Manhattan;	// Set the agent's heuristic to Manhattan
		break;

	case Heuristics::Euclidean:
		m_heuristic = &NodeMap::Heu_Euclidean;	// Set the agent's heuristic to Euclidean
		break;

	case Heuristics::Bogo:
		m_heuristic = &NodeMap::Heu_Bogo;	// Set the agent's heuristic to Bogo; somewhat random
		break;
	}
}

void PathAgent::SetMap(NodeMap* nodeMap)
{
	m_nodeMap = nodeMap;
}
