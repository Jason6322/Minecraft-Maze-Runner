#ifndef ASSIGN_MAZE_H
#define ASSIGN_MAZE_H

#include <vector>
#include <mcpp/mcpp.h>

class Maze
{

public:
    Maze(mcpp::Coordinate basePoint, std::vector<std::vector<char>> maze, int xlen, int zlen, bool clearMaze);
    ~Maze();

private:
    /* data */
    int build_length;
    int build_width;
};




#endif //ASSIGN_MAZE_H
