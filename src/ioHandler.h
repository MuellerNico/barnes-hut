#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include "node.h"


// TODO use binary files instead of csv
struct IOHandler {

    std::ofstream position_file;
    std::ofstream tree_file;
    
    IOHandler() {
        position_file.open("out/data.csv");
        if (!position_file.is_open()) {
            std::cerr << "Error opening file" << std::endl;
        }
        position_file << "frame,time,particle_id,pos_x,pos_y,pos_z,vel_x,vel_y,vel_z,mass,radius" << std::endl;
        position_file.flush();
        tree_file.open("out/tree.csv");
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
            position_file << frame << "," << t << "," << p.id << "," << p.position.x << "," << p.position.y << "," << p.position.z << ","
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

    // std::vector<Particle> read_input(const std::string& filename) {
    //     std::ifstream file(filename);
    //     std::vector<Particle> particles;
    //     if (!file.is_open()) {
    //         std::cerr << "Error opening file for reading input data." << std::endl;
    //         return particles;
    //     }
    //     std::string line;
    //     std::getline(file, line); // skip header
    //     while (std::getline(file, line)) {
    //         std::istringstream iss(line);
    //         Particle p;
    //         p.id = particles.size(); // assign ID based on order of reading
    //         char comma; // to ignore commas
    //         iss >> p.position.x >> comma
    //             >> p.position.y >> comma
    //             >> p.velocity.x >> comma
    //             >> p.velocity.y >> comma
    //             >> p.mass >> comma
    //             >> p.radius;
    //         particles.push_back(p);
    //     }
    //     file.close();

    //     // scale positions to fit in the range [10, 90] while keeping the aspect ratio consistent
    //     double min_x = particles[0].position.x;
    //     double min_y = particles[0].position.y;
    //     double max_x = particles[0].position.x;
    //     double max_y = particles[0].position.y;
    //     for (const Particle& p : particles) {
    //         if (p.position.x < min_x) min_x = p.position.x;
    //         if (p.position.y < min_y) min_y = p.position.y;
    //         if (p.position.x > max_x) max_x = p.position.x;
    //         if (p.position.y > max_y) max_y = p.position.y;
    //     }
    //     double range_x = max_x - min_x;
    //     double range_y = max_y - min_y;
    //     double scale = 80.0 / std::max(range_x, range_y); // use the larger range to keep aspect ratio
    //     double offset_x = 10 + (80.0 - scale * range_x) / 2.0; // center in the range [10, 90]
    //     double offset_y = 10 + (80.0 - scale * range_y) / 2.0; // center in the range [10, 90]
    //     for (Particle& p : particles) {
    //         p.position.x = offset_x + (p.position.x - min_x) * scale;
    //         p.position.y = offset_y + (p.position.y - min_y) * scale;
    //         p.velocity.x *= scale;
    //         p.velocity.y *= scale;
    //     }
    //     // scale masses and radii
    //     double max_mass = 0;
    //     for (const Particle& p : particles) {
    //         if (p.mass > max_mass) max_mass = p.mass;
    //     }
    //     for (Particle& p : particles) {
    //         p.mass /= max_mass; // normalize mass
    //         p.radius = p.mass; // set radius equal to mass for simplicity
    //     }
        
    //     // print particles for debugging
    //     for (const Particle& p : particles) {
    //         std::cout << "Particle ID: " << p.id << ", Position: (" << p.position.x << ", " << p.position.y << "), "
    //                   << "Velocity: (" << p.velocity.x << ", " << p.velocity.y << "), "
    //                   << "Mass: " << p.mass << ", Radius: " << p.radius << std::endl;
    //     }
    //     return particles;
    // }
};

#endif // IOHANDLER_H