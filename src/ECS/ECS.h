#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

constexpr unsigned int MAX_COMPONENTS = 32;

//------------------------------------------------------------------------
// Signature
//------------------------------------------------------------------------
typedef std::bitset<MAX_COMPONENTS> Signature;

class Component {
    //
};

class Entity {
    int id;
public:
    Entity(int id): id(id) {}; // Constructor, initialize automatically
    int GetId() const;
};

//------------------------------------------------------------------------
// Systems
//------------------------------------------------------------------------
class System {
    Signature componentSignature;
    std::vector<Entity> entities;
public:
    System() = default;
    ~System() = default;

    void AddEntityToSystem(Entity entity);
    void RemoveEntityToSystem(Entity entity);
    std::vector<Entity> GetSystemEntities() const;
    Signature& GetComponentSignature() const;
};

class Registry {
    //
};

#endif  // ECS_H
