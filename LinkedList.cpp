#include "LinkedList.h"
#include <ctime>
#include <chrono>
#include <thread>

// constructor
LinkedList::LinkedList(){
    head = nullptr;
}

// adds the coordinates and the block type of the terrain's block at the beginning of the list
void LinkedList::addBlock(const mcpp::Coordinate& coord, const mcpp::BlockType& block) {
    Node* newNode = new Node(coord, block);
    newNode->nextNode = head;
    head = newNode;
}

// restores the previous terrain's blocks
void LinkedList::restoreTerrain() {
    mcpp::MinecraftConnection mc;
    Node* current = head;

    if (!head) {
        return;
    }
    while (current) {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        mc.setBlock(current->coord, current->block);
        current = current->nextNode;
    }
}

// destructor
LinkedList::~LinkedList() {
    Node* current = head;
    while (current) {
        Node* toDelete = current;
        current = current->nextNode;
        delete toDelete;
    }
}