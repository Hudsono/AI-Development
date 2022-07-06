/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"
#include <vector>
#include <string>
#include "Pathfinding.h"

#include "Agent.h"
#include "Behaviour.h"

int main(int argc, char* argv[])
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 800;
    int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    std::vector<std::string> asciiMap;  // The node graph denoted by ASCII characters. '.' = empty space; '|' = wall.

    // Below are node graphs created by adding a string of characters; each string is one row of nodes in the node graph.

    // Pac-Man node graph.
    asciiMap.push_back("||||||||||||||||||||||||||||||");
    asciiMap.push_back("||b...........||............||");
    asciiMap.push_back("||.||||.|||||.||.|||||.||||.||");
    asciiMap.push_back("||.||||.|||||.||.|||||.||||.||");
    asciiMap.push_back("||.||||.|||||.||.|||||.||||.||");
    asciiMap.push_back("||..........................||");
    asciiMap.push_back("||.||||.||.||||||||.||.||||.||");
    asciiMap.push_back("||.||||.||.||||||||.||.||||.||");
    asciiMap.push_back("||......||....||....||......||");
    asciiMap.push_back("|||||||.|||||.||.|||||.|||||||");
    asciiMap.push_back("|||||||.|||||.||.|||||.|||||||");
    asciiMap.push_back("|||||||.||..........||.|||||||");
    asciiMap.push_back("|||||||.||.|||.||||.||.|||||||");
    asciiMap.push_back("|||||||.||.|......|.||.|||||||");
    asciiMap.push_back("a..........|......|..........a");
    asciiMap.push_back("|||||||.||.|......|.||.|||||||");
    asciiMap.push_back("|||||||.||.||||||||.||.|||||||");
    asciiMap.push_back("|||||||.||..........||.|||||||");
    asciiMap.push_back("|||||||.||.||||||||.||.|||||||");
    asciiMap.push_back("|||||||.||.||||||||.||.|||||||");
    asciiMap.push_back("||............||............||");
    asciiMap.push_back("||.||||.|||||.||.|||||.||||.||");
    asciiMap.push_back("||.||||.|||||.||.|||||.||||.||");
    asciiMap.push_back("||...||................||...||");
    asciiMap.push_back("||||.||.||.||||||||.||.||.||||");
    asciiMap.push_back("||||.||.||.||||||||.||.||.||||");
    asciiMap.push_back("||......||....||....||......||");
    asciiMap.push_back("||.||||||||||.||.||||||||||.||");
    asciiMap.push_back("||.||||||||||.||.||||||||||.||");
    asciiMap.push_back("||.........................b||");
    asciiMap.push_back("||||||||||||||||||||||||||||||");
    
    // Small example node graph.
    //asciiMap.push_back("||||||||||||");
    //asciiMap.push_back("|.|...|...||");
    //asciiMap.push_back("|.|.|...|..|");
    //asciiMap.push_back("|...||||||.|");
    //asciiMap.push_back("|.|........|");
    //asciiMap.push_back("|.||||||.|||");
    //asciiMap.push_back("|..........|");
    //asciiMap.push_back("||||||||||||");

    // Large, maze-like node graph.
    //asciiMap.push_back("....|||||||||||||||||||||||||||");
    //asciiMap.push_back("|........|...|......|.........|");
    //asciiMap.push_back("|...|||..|...|......|.........|");
    //asciiMap.push_back("|...|....|....................|");
    //asciiMap.push_back("|...|....|....................|");
    //asciiMap.push_back("|...|....|....................|");
    //asciiMap.push_back("|...|....|...|||||....||||||..|");
    //asciiMap.push_back("|...|....|.......|....|.|.....|");
    //asciiMap.push_back("|...|||||||||....|....|.|.....|");
    //asciiMap.push_back("|.....|.....|....||||.|.|.....|");
    //asciiMap.push_back("|.....|.....|.......|...|...|||");
    //asciiMap.push_back("|.....|.....|.......|...|...|.|");
    //asciiMap.push_back("||||.....||||.......|...|...|.|");
    //asciiMap.push_back("|........|..........|||||||.|.|");
    //asciiMap.push_back("|........|.......|......|.....|");
    //asciiMap.push_back("|..|||||||.......|......|.....|");
    //asciiMap.push_back("|................|......|.....|");
    //asciiMap.push_back("|.........||.....||||||||..||||");
    //asciiMap.push_back("|.........||........|.........|");
    //asciiMap.push_back("|.........||........|.........|");
    //asciiMap.push_back("|||||||||||||||||||||||||||...|");
    //asciiMap.push_back("|.............................|");
    //asciiMap.push_back("|.............................|");
    //asciiMap.push_back("|.............................|");

    // Cityblock-esque node graph.
    //asciiMap.push_back("|||||||||||||||||||||||");
    //asciiMap.push_back("|.....................|");
    //asciiMap.push_back("|.||.|.||.||||.||.||.||");
    //asciiMap.push_back("|.||.|.||.||||.||.||.||");
    //asciiMap.push_back("|.||.|.||.............|");
    //asciiMap.push_back("|.||.|.||.||||.||.||.||");
    //asciiMap.push_back("|.||.|....||||.||.||.||");
    //asciiMap.push_back("|.||||.||.||||.||.||.||");
    //asciiMap.push_back("|.....|||............||");
    //asciiMap.push_back("|.|||||...||||.||.||.||");
    //asciiMap.push_back("|.||....|.||||.||.||.||");
    //asciiMap.push_back("|.|||||.||||||.||.||.||");
    //asciiMap.push_back("|.|||||..............||");
    //asciiMap.push_back("|.......||||||||||||.||");
    //asciiMap.push_back("|||||||||||||||||||||||");

    // Large, empty node graph.
    //asciiMap.push_back(".......................");
    //asciiMap.push_back(".......................");
    //asciiMap.push_back(".......................");
    //asciiMap.push_back(".......................");
    //asciiMap.push_back(".......................");
    //asciiMap.push_back(".......................");
    //asciiMap.push_back(".......................");
    //asciiMap.push_back(".......................");
    //asciiMap.push_back(".......................");
    //asciiMap.push_back(".......................");
    //asciiMap.push_back(".......................");
    //asciiMap.push_back(".......................");
    //asciiMap.push_back(".......................");
    //asciiMap.push_back(".......................");

    // Create the node map and initialise it with our ASCII string vector.
    // Tile size is 20 units; has diagonal connections but does not cross them with walls.
    NodeMap nodeMap;
    nodeMap.Initialise(asciiMap, 20, NodeMap::DiagonalTypes::NoCutCorners);

    // Initial starting path nodes.
    Node* start = nodeMap.GetNode(1, 1);
    Node* end = nodeMap.GetNode(1, 1);

    // Create 3 path agents: #1 using Dijkstra, #2 using A* with a Euclidean heuristic, and #3 using A* with a Manhattan heuristic. 
    // We assign each to the node map we created, all spawned at the world position of the node at X, Y of 1, 1.
    //PathAgent agent1(PathAgent::Algorithms::Dijkstra, PathAgent::Heuristics::Euclidean, &nodeMap, { nodeMap.NodeWPos(1, 1)});    // Doesn't use heuristic.
    //PathAgent agent2(PathAgent::Algorithms::AStar, PathAgent::Heuristics::Euclidean, &nodeMap, { nodeMap.NodeWPos(1, 1)});
    //PathAgent agent3(PathAgent::Algorithms::AStar, PathAgent::Heuristics::Manhattan, &nodeMap, { nodeMap.NodeWPos(1, 1)});

    // All agents have the same movement speed for fair comparison.
    //agent1.SetSpeed(100);
    //agent2.SetSpeed(100);
    //agent3.SetSpeed(100);
    //
    //// Each agent has a unique shape (and therefore colour) to visually differentiate and identify them.
    //agent1.SetShape(PathAgent::Shapes::Circle);
    //agent2.SetShape(PathAgent::Shapes::Diamond);
    //agent3.SetShape(PathAgent::Shapes::Square);

    Agent agent1(&nodeMap, new GotoPointBehaviour());
    agent1.SetNode(start);
    agent1.SetPos({ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2});


    Agent agent2(&nodeMap, new WanderBehaviour());
    agent2.SetNode(start);
    agent2.SetPos({ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 });

    Agent agent3(&nodeMap, new SelectorBehaviour(new FollowerBehaviour(), new WanderBehaviour()));
    agent3.SetNode(start);
    agent3.SetSpeed(50);
    agent3.SetTarget(&agent1);
    agent3.SetPos({ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 });

    // Main game update loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Left-clicking an empty node will create an end point and make the 3 agents path to it.
        //if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        //{
        //    system("cls");  // Clear console each time an end point is made.
        //    Vector2 mousePos = GetMousePosition();  // Record the mouse position once.
        //
        //    // Here, we tell each of the 3 agents to path find to the empty node closest to the mouse position.
        //    // We record the time taken to calculate each path, used to compare computation cost/complexity per algorithm/heuristic.
        //    // Often, Dijkstra takes slightly longer than any A* implementation; however, Euclidean A* sometimes takes longer.
        //
        //    double timeNow = GetTime(); // Start a timer right before calculating the path
        //    agent1.GoToPos(mousePos, false); // Tell agent1 to pathfind to the given end point (mouse position).
        //    std::cout << "TTC: " << GetTime() - timeNow << std::endl;   // Stop the timer right after; print time taken to calculate.
        //
        //    timeNow = GetTime();
        //    agent2.GoToPos(mousePos, false);
        //    std::cout << "TTC: " << GetTime() - timeNow << std::endl;
        //
        //    timeNow = GetTime();
        //    agent3.GoToPos(mousePos, false);
        //    std::cout << "TTC: " << GetTime() - timeNow << std::endl;
        //
        //    // Print the length of the calculated path for each agent.
        //    std::cout << "\nPath length Dijkstra =\t\t " << agent1.GetPath().size() << std::endl;
        //    std::cout << "Path length A* Manhattan =\t " << agent2.GetPath().size() << std::endl;
        //    std::cout << "Path length A* Euclidean =\t " << agent3.GetPath().size() << std::endl;
        //}

        // Update the agents so they move along their path, if given one. The delta time is the frame time.
        //agent1.Update(GetFrameTime());
        //agent2.Update(GetFrameTime());
        //agent3.Update(GetFrameTime());

        agent1.Update(GetFrameTime());
        //agent2.Update(GetFrameTime());
        //agent3.Update(GetFrameTime());

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK); // Black background



        nodeMap.Draw(); // Draw the node graph.

        // Draw the calculated path for each agent.
        //nodeMap.DrawPath(agent2.GetPath(), PINK);
        //nodeMap.DrawPath(agent3.GetPath(), DARKGREEN);
        //nodeMap.DrawPath(agent1.GetPath(), YELLOW);

        // Draw each of the agents (in an order relative to their shapes so they're all visible).
        //agent2.Draw();
        //agent3.Draw();
        //agent1.Draw();

        // Instructions
        //DrawText("Left click on an empty node to calculate a path.", 10, GetScreenHeight() - 60, 20, WHITE);
        //
        //// Colour-Icon legend: draw the icon, then the label.
        //DrawRectangle(10, GetScreenHeight() - 40, 20, 20, RED);
        //DrawText("Wall", 40, GetScreenHeight() - 40, 20, RED);
        //
        //DrawCircle(100, GetScreenHeight() - 30, 5, BLUE);
        //DrawText("Node", 115, GetScreenHeight() - 40, 20, BLUE);
        //
        //DrawLine(180, GetScreenHeight() - 30, 200, GetScreenHeight() - 30, BLUE);
        //DrawText("Edge", 210, GetScreenHeight() - 40, 20, BLUE);
        //
        //DrawCircle(290, GetScreenHeight() - 30, 10, YELLOW);
        //DrawText("Dijkstra", 310, GetScreenHeight() - 40, 20, YELLOW);
        //
        //DrawRectanglePro({ 430, (float)GetScreenHeight() - 42, 20, 20}, {0, 0}, 45, PINK);
        //DrawText("A* Euclidean", 450, GetScreenHeight() - 40, 20, PINK);
        //
        //DrawRectangle(590, GetScreenHeight() - 40, 20, 20, DARKGREEN);
        //DrawText("A* Manhattan", 620, GetScreenHeight() - 40, 20, DARKGREEN);


        agent1.Draw();
        agent2.Draw();
        agent3.Draw();

        EndDrawing();   // Display to window.

        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}