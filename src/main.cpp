#include "tree.h"
#include <vector>
#include <iostream>

int numParticles = 10; 

int main() {
    Node* root = new Node(Vec2(0, 0), Vec2(100, 100));
    
    for (int i = 0; i < numParticles; ++i) {
        Particle* p = new Particle();
        p->position = Vec2(rand() % 100, rand() % 100);
        p->velocity = Vec2(0, 0);
        p->mass = 1.0;
        p->radius = 1.0;
        root->insert(p);
    }

    std::cout << "Test" << std::endl;

    // Clean up
    delete root;

    return 0;
}