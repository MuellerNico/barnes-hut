#ifndef TREE_H
#define TREE_H

#include "particle.h"
#include <vector>

#define MAX_PARTICLES 1 // particles per external node

struct Node {
    Vec3 position;
    Vec3 size;

    std::vector<Node*> children;
    std::vector<Particle*> particles;
    
    Vec3 center_of_mass = {0, 0, 0};
    double mass = 0;

    Node(const Vec3& position, const Vec3& size) : position(position), size(size) {}

    ~Node();

    bool contains(const Vec3& point) const;

    void insert(Particle* particle);

    void subdivide();

    bool is_leaf() const {
        return children.empty();
    }
};

// Destructor to clean up child nodes
Node::~Node() {
    for (Node* child : children) {
        delete child;
    }
    children.clear();
    particles.clear();
}

// Check if the node contains a point
bool Node::contains(const Vec3& point) const {
    return (point.x >= position.x && point.x < position.x + size.x &&
            point.y >= position.y && point.y < position.y + size.y &&
            point.z >= position.z && point.z < position.z + size.z);
}

// Insert a particle into the node
void Node::insert(Particle* particle) {
    // Update center of mass and total mass
    double total_mass = mass + particle->mass;
    center_of_mass = (center_of_mass * mass + particle->position * particle->mass) / total_mass;
    mass = total_mass;

    if (children.empty()) { // is external node
        particles.push_back(particle);
        if (particles.size() > MAX_PARTICLES) {
            subdivide();
        }
    } else {
        for (Node* child : children) {
            if (child->contains(particle->position)) {
                child->insert(particle);
                return;
            }
        }
    }
}

// Subdivide the node into eight
void Node::subdivide() {
    Vec3 half_size = size / 2;
    Vec3 offsets[] = {
        {0, 0, 0}, {half_size.x, 0, 0}, {0, half_size.y, 0}, {0, 0, half_size.z}, 
        {half_size.x, half_size.y, 0}, {half_size.x, 0, half_size.z}, {0, half_size.y, half_size.z}, 
        {half_size.x, half_size.y, half_size.z}
    };

    for (const Vec3& offset : offsets) {
        children.push_back(new Node(position + offset, half_size));
    }

    // Redistribute particles to children
    for (Particle* particle : particles) {
        for (Node* child : children) {
            if (child->contains(particle->position)) {
                child->insert(particle);
                break;
            }
        }
    }
    particles.clear();
}

#endif // TREE_H