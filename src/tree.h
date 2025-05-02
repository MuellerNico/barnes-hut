#include "particle.h"
#include <vector>

#define MAX_PARTICLES 1

struct Node {
    Vec2 position;
    Vec2 size;

    std::vector<Node*> children;
    std::vector<Particle*> particles;
    
    Vec2 center_of_mass = {0, 0};
    double mass = 0;

    Node(const Vec2& position, const Vec2& size) : position(position), size(size) {}

    ~Node();

    bool contains(const Vec2& point) const;

    void insert(Particle* particle);

    void subdivide();

    std::vector<Particle*> get_all_particles();
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
bool Node::contains(const Vec2& point) const {
    return (point.x >= position.x && point.x < position.x + size.x &&
            point.y >= position.y && point.y < position.y + size.y);
}

// Insert a particle into the node
void Node::insert(Particle* particle) {
    // Update center of mass and total mass
    double total_mass = mass + particle->mass;
    center_of_mass = (center_of_mass * mass + particle->position * particle->mass) / total_mass;
    mass = total_mass;

    if (children.empty()) {
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

// Subdivide the node into four children
void Node::subdivide() {
    Vec2 half_size = size / 2;
    children.push_back(new Node(position, half_size));
    children.push_back(new Node(position + Vec2(half_size.x, 0), half_size));
    children.push_back(new Node(position + Vec2(0, half_size.y), half_size));
    children.push_back(new Node(position + half_size, half_size));
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

// Get all particles in the node and its children recursively
std::vector<Particle*> Node::get_all_particles() {
    std::vector<Particle*> all_particles = particles;
    for (Node* child : children) {
        std::vector<Particle*> child_particles = child->get_all_particles();
        all_particles.insert(all_particles.end(), child_particles.begin(), child_particles.end());
    }
    return all_particles;
}
