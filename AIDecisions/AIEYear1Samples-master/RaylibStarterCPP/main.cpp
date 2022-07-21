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

#include "FiniteStateMachine.h"
#include "Condition.h"
#include "State.h"

#include "UtilityAI.h"

#include "BH_Behaviour.h"
#include "BT_BehaviourCustoms.h"

#include "SoundEmitter.h"
#include "Blackboard.h"
#include "GameManager.h"

#include "Helper.h"

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
    //asciiMap.push_back("||||||||||||||||||||||||||||||");
    //asciiMap.push_back("||b...........||............||");
    //asciiMap.push_back("||.||||.|||||.||.|||||.||||.||");
    //asciiMap.push_back("||.||||.|||||.||.|||||.||||.||");
    //asciiMap.push_back("||.||||.|||||.||.|||||.||||.||");
    //asciiMap.push_back("||..........................||");
    //asciiMap.push_back("||.||||.||.||||||||.||.||||.||");
    //asciiMap.push_back("||.||||.||.||||||||.||.||||.||");
    //asciiMap.push_back("||......||....||....||......||");
    //asciiMap.push_back("|||||||.|||||.||.|||||.|||||||");
    //asciiMap.push_back("|||||||.|||||.||.|||||.|||||||");
    //asciiMap.push_back("|||||||.||..........||.|||||||");
    //asciiMap.push_back("|||||||.||.|||.||||.||.|||||||");
    //asciiMap.push_back("|||||||.||.|......|.||.|||||||");
    //asciiMap.push_back("a..........|......|..........a");
    //asciiMap.push_back("|||||||.||.|......|.||.|||||||");
    //asciiMap.push_back("|||||||.||.||||||||.||.|||||||");
    //asciiMap.push_back("|||||||.||..........||.|||||||");
    //asciiMap.push_back("|||||||.||.||||||||.||.|||||||");
    //asciiMap.push_back("|||||||.||.||||||||.||.|||||||");
    //asciiMap.push_back("||............||............||");
    //asciiMap.push_back("||.||||.|||||.||.|||||.||||.||");
    //asciiMap.push_back("||.||||.|||||.||.|||||.||||.||");
    //asciiMap.push_back("||...||................||...||");
    //asciiMap.push_back("||||.||.||.||||||||.||.||.||||");
    //asciiMap.push_back("||||.||.||.||||||||.||.||.||||");
    //asciiMap.push_back("||......||....||....||......||");
    //asciiMap.push_back("||.||||||||||.||.||||||||||.||");
    //asciiMap.push_back("||.||||||||||.||.||||||||||.||");
    //asciiMap.push_back("||.........................b||");
    //asciiMap.push_back("||||||||||||||||||||||||||||||");
    
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
                                                             // Y
    asciiMap.push_back("....|||||||||||||||||||||||||||");   // 0
    asciiMap.push_back("|........|...|......|.........|");   // 1
    asciiMap.push_back("|...|||..|...|......|.........|");   // 2
    asciiMap.push_back("|...|....|....................|");   // 3
    asciiMap.push_back("|...|....|....................|");   // 4
    asciiMap.push_back("|...|....|....................|");   // 5
    asciiMap.push_back("|...|....|...|||||....||||||..|");   // 6
    asciiMap.push_back("|...|....|.......|....|.|.....|");   // 7
    asciiMap.push_back("|...|||||||||....|....|.|.....|");   // 8
    asciiMap.push_back("|.....|.....|....||||.|.|.....|");   // 9
    asciiMap.push_back("|.....|.....|.......|...|...|||");   // 10
    asciiMap.push_back("|.....|.....|.......|...|...|.|");   // 11
    asciiMap.push_back("||||.....||||.......|...|...|.|");   // 12
    asciiMap.push_back("|........|..........|||||||.|.|");   // 13
    asciiMap.push_back("|........|.......|......|.....|");   // 14
    asciiMap.push_back("|..|||||||.......|......|.....|");   // 15
    asciiMap.push_back("|................|......|.....|");   // 16
    asciiMap.push_back("|.........||.....||||||||..||||");   // 17
    asciiMap.push_back("|.........||........|.........|");   // 18
    asciiMap.push_back("|.........||........|.........|");   // 19
    asciiMap.push_back("|||||.|||||||||||||||||||||...|");   // 20
    asciiMap.push_back("|.............................|");   // 21
    asciiMap.push_back("|.............................|");   // 22
    asciiMap.push_back("|.............................|");   // 23

    //                X 0123456789111111111122222222223
    //                  ..........012345678901234567890

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

    GameManager* g_GameManager = new GameManager();
    BlackBoard* g_BlackBoard = new BlackBoard(g_GameManager);

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

    Agent* agent1 = new Agent(&nodeMap, new GotoPointBehaviour(), g_BlackBoard);
    agent1->SetNode(start);
    agent1->SetPos({ 450, 450 });
    //agent1.SetPos({ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2});


    Agent* agent2 = new Agent(&nodeMap, new WanderBehaviour(), g_BlackBoard);
    agent2->SetNode(start);
    //agent2.SetPos({ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 });

    // FSM //
    // Set up a FSM--we're going to have 2 states with their own conditions
    DistanceCondition* closerThan5 = new DistanceCondition(5.0f * nodeMap.CellSize(), true);
    DistanceCondition* furtherThan7 = new DistanceCondition(7.0f * nodeMap.CellSize(), false);
    TimeCondition* after5 = new TimeCondition(20.0f);
    SoundCondition* hearSound = new SoundCondition(g_GameManager->GetSounds());

    // Register these states with the FSM, so it's responsible for deleting them now
    State* guardState = new State(new GuardBehaviour());
    State* followState = new State(new FollowerBehaviour());
    State* searchState = new State(new SearchBehaviour());
    guardState->AddTransition(closerThan5, followState);    // If target gets close, chase it.
    followState->AddTransition(furtherThan7, searchState);  // If target gets too far, start searching.
    searchState->AddTransition(after5, guardState);         // After 5 seconds of not seeing the target, go back to guarding
    searchState->AddTransition(closerThan5, followState);   // If target gets close, go back to chasing.
    guardState->AddTransition(hearSound, searchState);  // If the guard hears a sound, start searching it.
    searchState->AddTransition(hearSound, searchState); // If the guard hears a sound while searching, start a new search.

    // Make a FSM that starts off wandering.
    FiniteStateMachine* fsm = new FiniteStateMachine(guardState);
    fsm->AddState(guardState);
    fsm->AddState(followState);
    fsm->AddState(searchState);

    // UtilityAI //
    // Make a UtilityAI that evaluates between following or wandering.
    UtilityAI* utilityAI = new UtilityAI(true);
    utilityAI->AddBehaviour(new WanderBehaviour());
    utilityAI->AddBehaviour(new FollowerBehaviour());

    // BehaviourTree //
    // https://web.archive.org/web/20131209105717/http://www.altdevblogaday.com/2011/02/24/introduction-to-behavior-trees/ helped a lot understanding...
    BehaviourTreeBehaviour* BHTree = new BehaviourTreeBehaviour
    ({
        new BT_Selector
        ({
            new BT_Sequence
            ({
                new ConditionTargetClose(5, false), new ActionChase()
            }),
            new ActionWander()
        })
    });
    //BehaviourTreeBehaviour* BHTree = new BehaviourTreeBehaviour(std::initializer_list<BT_Behaviour*> {new BT_Selector(std::initializer_list<BT_Behaviour*> })});

    //Agent agent3(&nodeMap, new SelectorBehaviour(new FollowerBehaviour(), new WanderBehaviour()));
    Agent* agent3 = new Agent(&nodeMap, fsm, g_BlackBoard);
    //Agent agent3(&nodeMap, utilityAI);
    //Agent agent3(&nodeMap, BHTree);
    agent3->SetNode(start);
    //agent3.SetSpeed(50);
    agent3->SetTarget(agent1);
    agent3->SetGuardPoints({ nodeMap.GetNode(9, 10), nodeMap.GetNode(5, 13), nodeMap.GetNode(5, 17), nodeMap.GetNode(21, 15)});
    //agent3.SetPos({ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 });

    SoundSource* tsound = new SoundSource(5, 53, { 500, 500 }, SoundSource::SoundTypes::Steam);

    g_GameManager->AddAgent({ agent1, agent2, agent3 });
    g_GameManager->PlaySound(tsound);

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

        //agent1.Update(GetFrameTime());
        //agent2.Update(GetFrameTime());
        //agent3.Update(GetFrameTime());

        //tsound->Update(GetFrameTime());

        //agent1.SetPos(GetMousePosition());

        if (IsMouseButtonPressed(MouseButton::MOUSE_RIGHT_BUTTON))
        {
            g_GameManager->PlaySound(new SoundSource(2, 500, GetMousePosition(), SoundSource::SoundTypes::Rock));    // Play a rock sound where the mouse is on right click
        }

        g_GameManager->Update(GetFrameTime());

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


        //agent1.Draw();
        //agent2.Draw();
        //agent3.Draw();

        Helper::DrawCross(GetMousePosition(), 10, 1, WHITE, true);

        //tsound->Draw();

        g_GameManager->Draw();

        EndDrawing();   // Display to window.

        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}