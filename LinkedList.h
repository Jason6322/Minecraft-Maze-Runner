#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <mcpp/mcpp.h>
#include <iostream>

// structure for a node in the linked list
class Node {
    public:
    mcpp::Coordinate coord;
    mcpp::BlockType block;
    Node* nextNode;

    // defining what values are in the node
    Node(const mcpp::Coordinate& coord, const mcpp::BlockType& block) {
        this->coord = coord;
        this->block = block;
        this->nextNode = nullptr;
    }
};

class LinkedList{
public:
    LinkedList();
    ~LinkedList();

    void addBlock(const mcpp::Coordinate& coord, const mcpp::BlockType& block);
    void restoreTerrain();

private:
    Node* head;
};

#endif //LINKEDLIST_H