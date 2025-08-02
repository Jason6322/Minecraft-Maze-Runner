#include "Maze.h"
#include "LinkedList.h"
#include <random>
#include <ctime>
#include <chrono>
#include <thread>
#include <string>
#include <sstream>

Maze::Maze(mcpp::Coordinate basePoint, std::vector<std::vector<char>> maze, int xlen, int zlen, bool clearMaze)
{
    build_length = xlen;
    build_width = zlen;
    mcpp::MinecraftConnection mc;
    mcpp::Coordinate givePoint = basePoint;
    int tempx = givePoint.x;
    int tempz = givePoint.z;

    // flattens terrain
    if (!clearMaze) {
        givePoint = basePoint;
        // clears anything above ground floor to build the maze
        for (int layer = 0; layer < 3; layer++) {
            for (int i = 0; i < build_length + 2; i++) {
                for (int j = 0; j < build_width + 2; j++) {
                    mcpp::Coordinate coord(givePoint.x-1, givePoint.y, givePoint.z-1);
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    mc.setBlock(coord, mcpp::Blocks::AIR);
                    givePoint.x +=1;
                }
                givePoint.x = tempx;
                givePoint.z += 1;
            }
            givePoint.z = tempz;
            givePoint.y += 1;
        }


        givePoint = mcpp::Coordinate(basePoint.x, basePoint.y, basePoint.z);
        mcpp::BlockType givenBlock = mc.getBlock(mcpp::Coordinate(basePoint.x, basePoint.y-1, basePoint.z));
        // creates 2 layers of flooring for the ground
        for (int layer = 0; layer < 2; layer++) {
            for (int i = 0; i < build_length + 2; i++) {
                for (int j = 0; j < build_width + 2; j++) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    mc.setBlock(mcpp::Coordinate(givePoint.x-1, givePoint.y-2, givePoint.z-1),
                    givenBlock);
                    givePoint.x += 1;
                }
            givePoint.x = tempx;
            givePoint.z += 1;
            }
        givePoint.z = tempz;
        givePoint.y += 1;
        }

    // building the maze
        givePoint = basePoint;
        for (int layer = 0; layer < 3; layer++) {
            for (int i = 0; i < build_length; i++) {
                for (int j = 0; j < build_width; j++) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                    // checks where to build a wall
                    if (maze[i][j] == 'x') {
                        mc.setBlock(mcpp::Coordinate(givePoint.x, givePoint.y, givePoint.z),
                        mcpp::Blocks::ACACIA_WOOD_PLANK);
                    }
                    // checks for the exit and places a blue carpet
                    else if (maze[i][j] == '.' && layer == 0) {
                        if (i == 0 || j == 0 || (i == build_length - 1) || (j == build_width - 1)) {
                            mc.setBlock(mcpp::Coordinate(givePoint.x, givePoint.y, givePoint.z),
                            mcpp::Blocks::BLUE_CARPET);
                        }
                    }
                    givePoint.x += 1;
                }
                givePoint.x = tempx;
                givePoint.z += 1;
            }
            givePoint.z = tempz;
            givePoint.y += 1;
        }
    }

// removes the maze
    if (clearMaze) {
        givePoint = basePoint;
        for (int layer = 0; layer < 3; layer++) {
        for (int i = 0; i < build_length; i++) {
            for (int j = 0; j < build_width; j++) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                mc.setBlock(mcpp::Coordinate(givePoint.x, givePoint.y, givePoint.z),
                mcpp::Blocks::AIR);
                givePoint.x += 1;
            }
            givePoint.x = tempx;
            givePoint.z += 1;
        }
        givePoint.z = tempz;
        givePoint.y += 1;
        }
    }
}

Maze::~Maze()
{
}