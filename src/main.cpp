#include "tree.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>

int numParticles = 10; 

int main() {
    Node* root = new Node(Vec2(0, 0), Vec2(100, 100));
    
    for (int i = 0; i < numParticles; ++i) {
        Particle* p = new Particle();
        p->position = Vec2(rand() % 100, rand() % 100);
        p->velocity = Vec2(0, 0);
        p->mass = 1.0;
        p->radius = 1.0;
        p->id = i;
        root->insert(p);
    }

    std::cout << "Test" << std::endl;
    std::cout << "Number of particles: " << root->get_all_particles().size() << std::endl;
    write_snapshot(root, 0);
    
    // Clean up
    delete root;

    return 0;
}

void write_snapshot(Node* root, int f) {
    std::ofstream file("out/frame_" + std::to_string(f) + ".csv");
    file << "t,particle_id,pos_x,pos_y,vel_x,vel_y,mass,radius" << std::endl;
    std::vector<Particle*> particles = root->get_all_particles();
    for (Particle* p : particles) {
        file << f << "," << p->id << "," << p->position.x << "," << p->position.y << ","
             << p->velocity.x << "," << p->velocity.y << ","
             << p->mass << "," << p->radius << std::endl;
    }
    file.close();
}