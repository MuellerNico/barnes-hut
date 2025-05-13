#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <stdexcept>
#include "node.h"


// TODO use binary files instead of csv
struct IOHandler {

    std::ofstream position_file;
    std::ofstream tree_file;
    
    IOHandler() {
        position_file.open("output/data.csv");
        if (!position_file.is_open()) {
            std::cerr << "Error opening file" << std::endl;
        }
        position_file << "frame,time,pos_x,pos_y,pos_z,vel_x,vel_y,vel_z,mass,radius" << std::endl;
        position_file.flush();
        tree_file.open("output/tree.csv");
        if (!tree_file.is_open()) {
            std::cerr << "Error opening file" << std::endl;
        }
        tree_file << "frame,pos_x,pos_y,pos_z,size_x,size_y,size_z,center_of_mass_x,center_of_mass_y,mass,center_of_mass_z" << std::endl;
        tree_file.flush();
    }

    ~IOHandler() {
        position_file.close();
        tree_file.close();
    }

    void write_snapshot(int frame, const std::vector<Particle>& particles, double t) {
        for (const Particle& p : particles) {
            position_file << frame << "," << t << "," << p.position.x << "," << p.position.y << "," << p.position.z << ","
                    << p.velocity.x << "," << p.velocity.y << "," << p.velocity.z << ","
                    << p.mass << "," << p.radius << std::endl;
        }
        position_file.flush();
    }

    void write_tree(Node* root, int frame) {
        std::queue<Node*> queue;
        queue.push(root);
        while (!queue.empty()) {
            Node* node = queue.front();
            queue.pop();
            tree_file << frame << "," << node->position.x << "," << node->position.y << "," << node->position.z << ","
                    << node->size.x << "," << node->size.y << "," << node->size.z << ","
                    << node->center_of_mass.x << "," << node->center_of_mass.y << "," << node->center_of_mass.z << ","
                    << node->mass << std::endl;
            // enqueue children
            for (Node* child : node->children) {
                queue.push(child);
            }
        }
        tree_file.flush();    
    }   

    std::vector<Particle> read_snapshot(const std::string& filename) {
        std::vector<Particle> particles;

        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return particles;
        }

        uint32_t count = 0;
        file.read(reinterpret_cast<char*>(&count), sizeof(uint32_t));

        particles.resize(count);
        file.read(reinterpret_cast<char*>(particles.data()), count * sizeof(Particle));

        return particles;
    }

};

#endif // IOHANDLER_H