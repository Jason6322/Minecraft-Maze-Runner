#include <iostream>

#include <mcpp/mcpp.h>

#include "menuUtils.h"
#include "Maze.h"
#include "LinkedList.h"
#include "Agent.h"

enum States{
    ST_Main,
    ST_GetMaze,
    ST_SolveMaze,
    ST_Creators,
    ST_Exit
};

int main(void){

    mcpp::MinecraftConnection mc;
    mcpp::Coordinate basePoint;

    mc.doCommand("time set day");
    States curState = ST_Main;
    std::vector<std::vector<char>> maze;
    unsigned int length = 0, width = 0;
    bool mazeGenerated = false, mazeClear = false;
    int option;
    LinkedList saveTerrain;

    //State machine for menu
    while (curState != ST_Exit)
    {
        printMainMenu();
        std::cin >> option;
        
        if (option == 1) {
            basePoint = mc.getPlayerPosition();
            curState = ST_GetMaze;
            while (curState == ST_GetMaze) {
                printGenerateMazeMenu();
                std::cin >> option;
                if (option == 1) {
                    std::cout << "Please enter the length and width:" << std::endl;
                    std::cin >> length >> width;
                    maze.resize(length, std::vector<char>(width));
                    std::cout << "Please enter the maze structure:" << std::endl;

                    for (int i = 0; i < length; ++i) {
                        for (int j = 0; j < width; ++j) {
                            std::cin >> maze[i][j];
                        }
                    }

                    std::vector<std::vector<bool>> visited(length, std::vector<bool>(width, false));
                    bool loopDetected = false;

                    for (int i = 0; i < length; ++i) {
                        for (int j = 0; j < width; ++j) {
                            // Only start DFS from unvisited open cells
                            if (maze[i][j] == '.' && !visited[i][j]) {
                                // Stack for DFS, each entry is (x, y, fromX, fromY)
                                std::vector<std::tuple<int, int, int, int>> stack;
                                stack.push_back({i, j, -1, -1});

                                while (!stack.empty()) {
                                    auto [x, y, fromX, fromY] = stack.back();
                                    stack.pop_back();

                                    // If visited, loop detected
                                    if (visited[y][x]) {
                                        loopDetected = true;
                                        break;
                                    }

                                    // Mark current cell as visited
                                    visited[y][x] = true;

                                    // Push neighbors to stack, avoiding the cell we came from
                                    if (x > 0 && !(x - 1 == fromX && y == fromY) && maze[y][x - 1] == '.') {
                                        stack.push_back({x - 1, y, x, y});
                                    }
                                    if (x < width - 1 && !(x + 1 == fromX && y == fromY) && maze[y][x + 1] == '.') {
                                        stack.push_back({x + 1, y, x, y});
                                    }
                                    if (y > 0 && !(x == fromX && y - 1 == fromY) && maze[y - 1][x] == '.') {
                                        stack.push_back({x, y - 1, x, y});
                                    }
                                    if (y < length - 1 && !(x == fromX && y + 1 == fromY) && maze[y + 1][x] == '.') {
                                        stack.push_back({x, y + 1, x, y});
                                    }
                                }

                                if (loopDetected) {
                                    break;
                                }
                            }
                        }
                        if (loopDetected) {
                            break;
                        }
                    }

                    if (loopDetected) {
                        std::cout << "Loop detected in the maze!" << std::endl;
                        std::cout << "Re-Enter a maze structure with NO loops." << std::endl;
                    } else {
                        std::cout << std::endl;
                        std::cout << "Maze generated successfully!" << std::endl;
                        std::cout << "** Printing Maze **" << std::endl;
                        std::cout << "BasePoint: " << "(" << basePoint.x << ", " << basePoint.y << ", " << basePoint.z << ")" << std::endl;
                        std::cout << "Structure: " << std::endl;

                        for (int i = 0; i < length; i++) {
                            for (int j = 0; j < width; j++) {
                                std::cout << maze[i][j];
                            }
                            std::cout << std::endl;
                        }
                        std::cout << "** End Printing Maze **" << std::endl;

                        mazeGenerated = true;
                        curState = ST_Main;
                    }
                }
                else if (option == 2) {
                    basePoint = mc.getPlayerPosition();
                    std::cout << "Please enter the length and width:" << std::endl;
                    std::cin >> length >> width;
                    maze.resize(length, std::vector<char>(width));

                    // Fill the array 2D array with all walls (x)
                    for (int i = 0; i < length; ++i) {
                        for (int j = 0; j < width; ++j) {
                            maze[i][j] = 'x';
                        }
                    }

                    // Create variables for player's position
                    int playerPos_x = 0;
                    int playerPos_y = 0;

                    // Select random entry point in maze (cannot be first or last row)
                    int entryPoint = rand() % (length - 2) + 1;
                    playerPos_y += entryPoint;

                    // Select random entry direction point (left or right)
                    int directionPoint = rand() % 2;
                    if (directionPoint == 1) {
                        playerPos_x = width - 1;
                    }

                    // Break the entry point
                    maze[playerPos_y][playerPos_x] = '.';

                    for (int i = 0; i < width*length; ++i) {
                        // Check top if valid
                        if (playerPos_y > 1
                        && maze[playerPos_y-1][playerPos_x] == 'x'
                        && maze[playerPos_y-1][playerPos_x-1] == 'x'
                        && maze[playerPos_y-1][playerPos_x+1] == 'x'
                        && maze[playerPos_y-2][playerPos_x] == 'x') {
                            --playerPos_y;
                            maze[playerPos_y][playerPos_x] = '.';
                        }
                        // Check bottom if valid
                        else if (playerPos_y < length - 2
                        && maze[playerPos_y+1][playerPos_x] == 'x'
                        && maze[playerPos_y+1][playerPos_x-1] == 'x'
                        && maze[playerPos_y+1][playerPos_x+1] == 'x'
                        && maze[playerPos_y+2][playerPos_x] == 'x') {
                            ++playerPos_y;
                            maze[playerPos_y][playerPos_x] = '.';
                        }
                        // Check right if valid
                        else if (playerPos_x < width - 2
                        && maze[playerPos_y][playerPos_x+1] == 'x'
                        && maze[playerPos_y-1][playerPos_x+1] == 'x'
                        && maze[playerPos_y+1][playerPos_x+1] == 'x'
                        && maze[playerPos_y][playerPos_x+2] == 'x') {
                            ++playerPos_x;
                            maze[playerPos_y][playerPos_x] = '.';
                        }
                        // Check left if valid
                        else if (playerPos_x > 1
                        && maze[playerPos_y][playerPos_x-1] == 'x'
                        && maze[playerPos_y-1][playerPos_x-1] == 'x'
                        && maze[playerPos_y+1][playerPos_x-1] == 'x'
                        && maze[playerPos_y][playerPos_x-2] == 'x') {
                            --playerPos_x;
                            maze[playerPos_y][playerPos_x] = '.';
                        }
                        // If path has reached a dead end then reset to starting point
                        else {
                            playerPos_x = 1;
                            playerPos_y = entryPoint;
                        }
                    }

                    std::cout << std::endl;
                    std::cout << "Maze generated successfully!" << std::endl;
                    std::cout << "** Printing Maze **" << std::endl;
                    std::cout << "Structure: " << std::endl;
                    std::cout << "BasePoint: " << "(" << basePoint.x << ", " << basePoint.y << ", " << basePoint.z << ")" << std::endl;

                    for (int a = 0; a < length; a++) {
                        for (int b = 0; b < width; b++) {
                            std::cout << maze[a][b];
                        }
                        std::cout << std::endl;
                    }
                    
                    mazeGenerated = true;
                } else if (option == 3) {
                    curState = ST_Main;
                } else {
                    std::cout << std::endl;
                    std::cout << "Input Error: Enter a number between 1 and 3 ..." << std::endl;
                }
            }
        } else if (option == 2) {
            if (mazeGenerated) {
                mcpp::Coordinate givenPoint = basePoint;
                int tempx = givenPoint.x;
                int tempz = givenPoint.z;

                // loop function to store the terrain before building the maze
                for (int layer = 0; layer < 5; layer++) {
                    for (int i = 0; i < length + 2; i++) {
                        for (int j = 0; j < width + 2; j++) {
                            mcpp::Coordinate coord(givenPoint.x - 1, givenPoint.y - 2, givenPoint.z - 1);
                            mcpp::BlockType originalBlock = mc.getBlock(coord);

                            if (originalBlock != mcpp::Blocks::AIR) {
                                saveTerrain.addBlock(coord, originalBlock);
                            }
                            
                            givenPoint.x +=1;
                        }
                    givenPoint.x = tempx;
                    givenPoint.z += 1;
                    }
                givenPoint.z = tempz;
                givenPoint.y += 1;
                }
                // mazeClear stays false to let maze.cpp to build the maze
                Maze(basePoint, maze, length, width, mazeClear);
            } else {
                std::cout << std::endl;
                std::cout << "Error: Please generate maze first ..." << std::endl;
            }
            
        } else if (option == 3) {
            curState = ST_SolveMaze;
            while (curState == ST_SolveMaze) {
                if (mazeGenerated) {
                    printSolveMazeMenu();

                    std::cin >> option;
                    if (option == 1 || option == 2 || option == 3) {
                        Agent(basePoint, maze, length, width, option);
                    } else if (option == 4) {
                        curState = ST_Main;
                    } else {
                        std::cout << std::endl;
                        std::cout << "Input Error: Enter a number between 1 and 4 ..." << std::endl;
                    }
                } else {
                    std::cout << std::endl;
                    std::cout << "Error: Please generate maze first ..." << std::endl;
                    curState = ST_Main;
                }
            }
        } else if (option == 4) {
            printTeamInfo();
        } else if (option == 5) {
            // mazeClear turns true to allow maze.cpp to remove the maze
            mazeClear = true;
            Maze(basePoint, maze, length, width, mazeClear);
            saveTerrain.restoreTerrain();
            curState = ST_Exit;
        } else {
            printExitMassage();
            std::cout << "Input Error: Enter a number between 1 and 5 ...";
        }
    }

    printExitMassage();


    return EXIT_SUCCESS;

}