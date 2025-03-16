#ifndef ECS_H
#define ECS_H

#include <spdlog/spdlog.h>
#include <bitset>
#include <memory>
#include <set>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;

//------------------------------------------------------------------------
// Signature
//------------------------------------------------------------------------
// typedef std::bitset<MAX_COMPONENTS> Signature; // C style
using Signature = std::bitset<MAX_COMPONENTS>;  // Modern C++

// Class template. A parent class for components
struct IComponent {
protected:
    static int nextId;
};

//------------------------------------------------------------------------
// Components
//------------------------------------------------------------------------
// Class template that is used to assign a unique id to a component type
template <typename TComponent>
class Component : public IComponent {
public:
    // Returns the unique id of Component<T>
    static int GetId() {  // will exist for each component type separately
        static auto id = nextId++;
        return id;
    }
};

//------------------------------------------------------------------------
// Entity
//------------------------------------------------------------------------
class Entity {
    int id;

public:
    Entity(unsigned int id)
        : id(id){};  // Constructor, initialize automatically
    Entity(const Entity& entity) = default;
    int GetId() const;

    // Operator overloading
    Entity& operator=(const Entity& other) = default;

    bool operator==(const Entity& other) const {
        return id == other.id;
    }

    bool operator!=(const Entity& other) const {
        return id != other.id;
    }

    bool operator<(const Entity& other) const {
        return id < other.id;
    }

    bool operator>(const Entity& other) const {
        return id > other.id;
    }

    // Template functions for components management
    template <typename TComponent, typename... TArgs>
    void AddComponent(TArgs&&... args);

    template <typename TComponent>
    void RemoveComponent();

    template <typename TComponent>
    bool HasComponent() const;

    template <typename TComponent>
    TComponent& GetComponent() const;

    // Hold a pointer to the entity's owner registry
    class Registry* registry;  // forward declaration of `Registry` class
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
    void RemoveEntityFromSystem(Entity entity);
    std::vector<Entity> GetSystemEntities() const;
    const Signature& GetComponentSignature() const;

    // Defines the component type that entities must have to be considered by the system
    template <typename TComponent>
    void RequireComponent();
};

//------------------------------------------------------------------------
// Pool of components
//------------------------------------------------------------------------
class IPool {
public:
    virtual ~IPool(){};
};

// Collection of objects of type T
template <typename T>
class Pool : public IPool {
    std::vector<T> data;

public:
    Pool(int size = 50) {
        data.resize(size);
    }

    virtual ~Pool() = default;  // compiler will generate the default implementation of the destructor

    bool isEmpty() const {
        return data.empty();
    }

    int GetSize() const {
        return data.size();
    }

    void Resize(int n) {
        data.resize(n);
    }

    void Clear() {
        data.clear();
    }

    void Add(T object) {
        data.push_back(object);
    }

    void Set(int index, T object) {
        data[index] = object;
    }

    T& Get(int index) {
        return static_cast<T&>(data[index]);
    }

    // Operator overloading
    T& operator[](unsigned int index) {
        return data[index];
    }
};

//------------------------------------------------------------------------
// Registry
//------------------------------------------------------------------------
// Manages the creation/destruction of entities and adding/removing components and systems
class Registry {
    // Keep track of how many entities were added to the scene
    unsigned int numEntities = 0;

    // Vector of component pools.
    // Each pool contains all the data for a certain component type
    // [vector index = componentId], [pool index = entityId]
    std::vector<std::shared_ptr<IPool>> componentPools;

    // Vector of component signatures.
    // The signature lets us know which components are turned "on" for an entity
    // [ vector index = entity id ]
    std::vector<Signature> entityComponentSignatures;

    // Map of active systems [ index = system typeid ]
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

    // Set of entities awaiting creation/destruction in the next `Update()`
    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeKilled;

public:
    Registry() {
        spdlog::info("Registry constructor called.");
    }

    ~Registry() {
        spdlog::info("Registry destructor called.");
    }

    void Update();

    // Entity management
    Entity CreateEntity();

    // Checks the component signature of an entity
    // and add the entity to the systems that are interested in it
    void AddEntityToSystems(Entity entity);

    // void KillEntity(Entity entity);

    // Template functions for components management
    template <typename TComponent, typename... TArgs>
    void AddComponent(Entity entity, TArgs&&... args);

    template <typename TComponent>
    void RemoveComponent(Entity entity);

    template <typename TComponent>
    bool HasComponent(Entity entity) const;

    template <typename TComponent>
    TComponent& GetComponent(Entity entity) const;

    // Template functions for systems management
    template <typename TSystem, typename... TArgs>
    void AddSystem(TArgs&&... args);

    template <typename TSystem>
    void RemoveSystem();

    template <typename TSystem>
    bool HasSystem() const;

    template <typename TSystem>
    TSystem& GetSystem() const;
};

//------------------------------------------------------------------------
// Function templates
//------------------------------------------------------------------------
template <typename TComponent>
void System::RequireComponent() {
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

template <typename TSystem, typename... TArgs>
void Registry::AddSystem(TArgs&&... args) {
    // Create new system
    // TODO: replace a raw pointer with a smart pointer
    std::shared_ptr<TSystem> newSystem = std::make_shared<TSystem>(std::forward<TArgs>(args)...);

    // Add the system to an unordered map
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));  // pair(key, value)
}

template <typename TSystem>
void Registry::RemoveSystem() {
    if (HasSystem<TSystem>())
        systems.erase(std::type_index(typeid(TSystem)));
    // Get system id
    // auto system = systems.find(std::type_index(typeid(TSystem)));

    // Remove the system from the unordered map
    // systems.erase(system);
}

template <typename TSystem>
bool Registry::HasSystem() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem& Registry::GetSystem() const {
    // Get system id
    auto system = systems.find(std::type_index(typeid(TSystem)));

    // Returns the system with a correct type
    // '*->first' gets the key, '*->second' gets the value
    return *(std::static_pointer_cast<TSystem>(system->second));
}

template <typename TComponent, typename... TArgs>
void Registry::AddComponent(Entity entity, TArgs&&... args) {
    // Get a component/entity id
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    // Resize the pools of components
    if (componentId >= componentPools.size())
        componentPools.resize(componentId + 1, nullptr);

    // If nothing within the current position
    if (!componentPools[componentId]) {
        std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
        componentPools[componentId] = newComponentPool;  // assigns the current position
    }

    // Get the component pool
    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(
        componentPools[componentId]);

    // Resize current component pool
    if (entityId >= componentPool->GetSize())
        componentPool->Resize(numEntities);

    // Creating a new component
    TComponent newComponent(std::forward<TArgs>(args)...);

    // Set the position
    componentPool->Set(entityId, newComponent);

    // Enable the bitset signature
    entityComponentSignatures[entityId].set(componentId);

    spdlog::info("Component id = " + std::to_string(componentId) + " was added to entity id "
                + std::to_string(entityId));
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity) {
    // Get a component/entity id
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    // Disable the bitset signature
    entityComponentSignatures[entityId].set(componentId, false);

    spdlog::info("Component id = " + std::to_string(componentId) + " was removed from entity id "
                + std::to_string(entityId));
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) const {
    // Get a component/entity id
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    // Logger::Log("Entity id " + std::to_string(entityId)
    //             + " has a component id = " + std::to_string(componentId));

    // Return the component state
    return entityComponentSignatures[entityId].test(componentId);
}

template <typename TComponent>
TComponent& Registry::GetComponent(Entity entity) const {
    // Get a component/entity id
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    // Fetch the component object
    auto componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

    // Logger::Log("Get a component id = " + std::to_string(componentId) + " from Entity id " +
    // std::to_string(entityId));

    // Return the component by index
    return componentPool->Get(entityId);
}

// Entity's template functions

template <typename TComponent, typename... TArgs>
void Entity::AddComponent(TArgs&&... args) {
    // Ask registry to add a component
    registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
}

template <typename TComponent>
void Entity::RemoveComponent() {
    registry->RemoveComponent<TComponent>(*this);
}

template <typename TComponent>
bool Entity::HasComponent() const {
    return registry->HasComponent<TComponent>(*this);
}

template <typename TComponent>
TComponent& Entity::GetComponent() const {
    return registry->GetComponent<TComponent>(*this);
}

#endif  // ECS_H
