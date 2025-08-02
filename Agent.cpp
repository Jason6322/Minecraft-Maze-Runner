#include "Agent.h"
#include <random>
#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <queue>
#include <vector>


Agent::Agent(mcpp::Coordinate startLoc, std::vector<std::vector<char>> maze, int length, int width, int option) {
    if (option == 1) {
        std::cout << "Running option 1\n";
    

        std::vector<mcpp::Coordinate> emptySpaces;
        mcpp::Coordinate empty;
        // If the vector has "." then push back the xyz location in vector empty
        for (int i = 0; i < length; ++i) {
            for (int j = 0; j < width; ++j) {
                if (maze.at(i).at(j) == '.') {
                    empty.x = startLoc.x + j;
                    empty.y = startLoc.y;
                    empty.z = startLoc.z + i;
                    emptySpaces.push_back(empty);
                }
            }
        }
        if (!emptySpaces.empty()) {
            // Initialize random seed
            std::srand(static_cast<unsigned int>(std::time(nullptr)));

            // Pick a random index from the emptySpaces vector
            int randomIndex = std::rand() % emptySpaces.size();
            mcpp::Coordinate randomLocation = emptySpaces.at(randomIndex);

            // Teleport player to the randomly selected location
            mc.setPlayerPosition(randomLocation);
            std::cout << "Player teleported to (" << randomLocation.x << ", " << randomLocation.y << ", " << randomLocation.z << ")" << std::endl;
        } else {
            std::cout << "No empty spaces found in the maze!" << std::endl;
        }

   } else if (option == 2) {
        std::cout << "Running option 2\n";

        mcpp::Coordinate currentPos = mc.getPlayerPosition();
        mcpp::Coordinate previousPos = currentPos;

        bool reachedExit = false;
        int orientation = 0;
        int i = 1;
        while (!reachedExit) {
            if (currentPos != previousPos) {
                std::cout << "Step[" << i << "]: (" << currentPos.x << ", " << currentPos.y << ", " << currentPos.z << ")" << std::endl;
                i++;  // Only increment i when currentPos changes
            }
            else {std::cout << "";}
            mc.setBlock(currentPos, mcpp::Blocks::LIME_CARPET);

            if (currentPos != previousPos) {
            mc.setBlock(previousPos, mcpp::Blocks::AIR);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            previousPos = currentPos;
            mcpp::Coordinate rightCheckPos = currentPos;
            mcpp::Coordinate forwardCheckPos = currentPos;

            

            if (orientation == 0) {  // North
                forwardCheckPos.z -= 1;
                rightCheckPos.x += 1;
            } else if (orientation == 1) { //East
                forwardCheckPos.x += 1;
                rightCheckPos.z += 1;
            } else if (orientation == 2) { //South
                forwardCheckPos.z += 1;
                rightCheckPos.x -= 1;
            } else if (orientation == 3) { //West
                forwardCheckPos.x -= 1;
                rightCheckPos.z -= 1;
            }

            bool rightWall = (mc.getHeight(rightCheckPos.x, rightCheckPos.z) - currentPos.y >= 2);
            bool frontWall = (mc.getHeight(forwardCheckPos.x,  forwardCheckPos.z) - currentPos.y >= 2);
            // If right wall isn't detected go right and change orientation
            if (!rightWall) {
                if (orientation == 0) {
                    currentPos.x += 1;
                    orientation = 1;
                } else if (orientation == 1) {
                    currentPos.z += 1;
                    orientation = 2;
                } else if (orientation == 2) {
                    currentPos.x -= 1;
                    orientation = 3;
                } else if (orientation == 3) {
                    currentPos.z -= 1;
                    orientation = 0;
                }
            // If front wall isn't detected go forward
            } else if (!frontWall) {
                if (orientation == 0) {
                    currentPos.z -= 1;
                } else if (orientation == 1) {
                    currentPos.x += 1;
                } else if (orientation == 2) {
                    currentPos.z += 1;
                } else if (orientation == 3) {
                    currentPos.x -= 1;
                }
            // If there is right and front wall change orientation
            } else if (rightWall && frontWall) {
                orientation = (orientation + 3) % 4;
            }

            if (mc.getBlock(currentPos) == mcpp::Blocks::BLUE_CARPET) {
                mc.setBlock(previousPos, mcpp::Blocks::AIR);
                std::cout << "Exit reached" << std::endl;           
                reachedExit = true;
            }




        }
    }

    else if (option == 3) {
        std::cout << "Running option 3\n";
        // Define the directions
        const int Directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
        mcpp::Coordinate start_pos = mc.getPlayerPosition();
        std::queue<std::vector<mcpp::Coordinate>> paths_queue;
        std::vector<std::vector<mcpp::Coordinate>> all_paths;
        std::vector<mcpp::Coordinate> visited;

        // Initialize BFS with the starting position as the first path
        paths_queue.push({start_pos});
        visited.push_back(start_pos);

        while (!paths_queue.empty()) {
            // Get the current path and the current position
            auto current_path = paths_queue.front();
            paths_queue.pop();
            mcpp::Coordinate current_pos = current_path.back();

            // If the current position is the exit, store the path and continue
            if (mc.getBlock(current_pos) == mcpp::Blocks::BLUE_CARPET) {
                all_paths.push_back(current_path);
            } else {
                // Explore neighbors
                for (const auto& direction : Directions) {
                    mcpp::Coordinate neighbor(current_pos.x + direction[0], current_pos.y, current_pos.z + direction[1]);

                    // Check if the neighbor is an air block and not yet visited in the current path
                    bool is_visited = std::find(visited.begin(), visited.end(), neighbor) != visited.end();
                    bool is_air_block = mc.getBlock(neighbor) == mcpp::Blocks::AIR || mc.getBlock(neighbor) == mcpp::Blocks::BLUE_CARPET;

                    if (!is_visited && is_air_block) {
                        // Mark as visited in this path and push the new path with neighbor
                        visited.push_back(neighbor);
                        auto new_path = current_path;
                        new_path.push_back(neighbor);
                        paths_queue.push(new_path);
                    }
                }
            }
        }

        // Find the shortest path
        if (!all_paths.empty()) {
            auto shortest_path = all_paths[0];
            for (const auto& path : all_paths) {
                if (path.size() < shortest_path.size()) {
                    shortest_path = path;
                }
            }

            // Output and display the shortest path
            int i = 1;
            for (const auto& step : shortest_path) {
                std::cout << "Step["<< i << "]: (" << step.x << ", " << step.y << ", " << step.z << ")\n";
                i++;
                // Only if neighbor isn't blue carpet place lime carpet
                if (mc.getBlock(step) != mcpp::Blocks::BLUE_CARPET) {
                    mc.setBlock(step, mcpp::Blocks::LIME_CARPET);
                    
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                if (mc.getBlock(step) == mcpp::Blocks::LIME_CARPET) {
                mc.setBlock(step, mcpp::Blocks::AIR);
                }
                else {mc.setBlock(step, mcpp::Blocks::BLUE_CARPET);}
            
            }
            std::cout << "Exit reached\n";
        } else {
            std::cout << "No path found\n";
        }




                
        
    }
}






Agent::~Agent() 
{
}


// References used for help
//https://andrewyong7338.medium.com/maze-escape-with-wall-following-algorithm-170c35b88e00