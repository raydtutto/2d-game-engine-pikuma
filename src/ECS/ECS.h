#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>

constexpr unsigned int MAX_COMPONENTS = 32;

//------------------------------------------------------------------------
// Signature
//------------------------------------------------------------------------
typedef std::bitset<MAX_COMPONENTS> Signature;

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
    // Returns the unique id of Component<T>
    static int GetId() { // will exist for each component type separately
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
};

//------------------------------------------------------------------------
// Systems
//------------------------------------------------------------------------
class System {
    Signature componentSignature;
    std::vector<Entity> entities;

public:
    System() = default;
    virtual ~System() = default;

    void AddEntityToSystem(Entity entity);
    void RemoveEntityToSystem(Entity entity);
    [[nodiscard]] const std::vector<Entity>& GetSystemEntities() const;
    [[nodiscard]] const Signature& GetComponentSignature() const;

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
    Pool(int size = 100) {
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

    // Set of entities awaiting creation/destruction in the next `Update()`
    std::set<Entity> entitiesToBeAdded;
    std::set<Entity> entitiesToBeKilled;

    // Vector of component pools.
    // Each pool contains all the data for a certain component type
    // [vector index = componentId], [pool index = entityId]
    std::vector<IPool*> componentPools;

    // Vector of component signatures.
    // The signature lets us know which components are turned "on" for an entity
    // [ vector index = entity id ]
    std::vector<Signature> entityComponentSignatures;

    // Map of active systems [ index = system typeid ]
    std::unordered_map<std::type_index, System*> systems;

public:
    Registry() = default;

    void Update();

    // Entity management
    Entity CreateEntity();

    // Checks the component signature of an entity
    // and add the entity to the systems that are interested in it
    void AddEntityToSystems(Entity entity);

    // void KillEntity(Entity entity);

    // Component management
    template <typename TComponent, typename... TArgs>
    void AddComponent(Entity entity, TArgs&&... args);

    template <typename T>
    void RemoveComponent(Entity entity);

    template <typename T>
    bool HasComponent(Entity entity) const;

    // template <typename T>
    // void GetComponent(Entity entity);

    // System management
    template <typename TSystem, typename... TArgs>
    void AddSystem(TArgs&&... args);

    template <typename TSystem>
    void RemoveSystem();

    template <typename TSystem>
    bool HasSystem() const;

    template <typename TSystem>
    TSystem GetSystem() const;
};

//------------------------------------------------------------------------
// Function templates
//------------------------------------------------------------------------
template <typename TComponent>
void System::RequireComponent() {
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
};

template <typename TSystem, typename... TArgs>
void Registry::AddSystem(TArgs&&... args) {
    // Create new system
    // TODO: replace a raw pointer with a smart pointer
    TSystem* newSystem(new TSystem(std::forward<TArgs>(args)...));

    // Add the system to an unordered map
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem)); // pair(key, value)
}

template <typename TSystem>
void Registry::RemoveSystem() {
    // Get system id
    const auto system = systems.find(std::type_index(typeid(TSystem)));

    // Remove the system from the unordered map
    systems.erase(system);
}

template <typename TSystem>
bool Registry::HasSystem() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template <typename TSystem>
TSystem Registry::GetSystem() const {
    // Get system id
    const auto system = systems.find(std::type_index(typeid(TSystem)));

    // Returns the system with a correct type
    // '*->first' gets the key, '*->second' gets the value
    return std::static_pointer_cast<TSystem>(system->second);
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
        // TODO: replace a raw pointer with a smart pointer
        Pool<TComponent>* newComponentPool = new Pool<TComponent>();
        componentPools[componentId] = newComponentPool;  // assigns the current position
    }

    // Get the component pool
    Pool<TComponent>* componentPool = componentPools[componentId];

    // Resize current component pool
    if (entityId >= componentPool->GetSize())
        componentPool->Resize(numEntities);

    // Creating a new component
    TComponent newComponent(std::forward<TArgs>(args)...);

    // Set the position
    componentPool->Set(entityId, newComponent);

    // Enable the bitset signature
    entityComponentSignatures[entityId].set(componentId);
}

template <typename T>
void Registry::RemoveComponent(Entity entity) {
    // Get a component/entity id
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    // Disable the bitset signature
    entityComponentSignatures[entityId].set(componentId, false);
}

template <typename T>
bool Registry::HasComponent (Entity entity) const {
    // Get a component/entity id
    const auto componentId = Component<T>::GetId();
    const auto entityId = entity.GetId();

    // Return the component state
    return entityComponentSignatures[entityId].test(componentId);
}

#endif  // ECS_H
