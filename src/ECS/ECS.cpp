#include "ECS.h"

#include "Logger/Logger.h"
#include <algorithm>

// Initializes static methods from header
int IComponent::nextId = 0;

int Entity::GetId() const {
    return id;
}

void System::AddEntityToSystem(Entity entity) {
    entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity) {
    // Rely on Entity::operator==
    entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
}

// Returns a reference to the entities vector, not a copy of the vector
std::vector<Entity> System::GetSystemEntities() const {
    return entities;
}

const Signature& System::GetComponentSignature() const {
    return componentSignature;
}

Entity Registry::CreateEntity() {
    unsigned int entityId = numEntities++;

    // Creates new entity
    Entity entity(entityId);

    // Initialize registry
    entity.registry = this;

    // Insert new entity into the line
    entitiesToBeAdded.insert(entity);

    // Make sure the entityComponentSignatures can accommodate the new entity
    if (entityId >= entityComponentSignatures.size())
        entityComponentSignatures.resize(entityId + 1);

    Logger::Log("Entity created with id: {}", std::to_string(entityId));
    return entity;
}

void Registry::AddEntityToSystems(Entity entity) {
    // Get entity id
    const auto entityId = entity.GetId();

    // Match `entityComponentSignature` <----> `systemComponentSignature`
    const auto& entityComponentSignature = entityComponentSignatures[entityId];

    // Search all the systems within an unordered map
    for (auto& system : systems) {
        const auto& systemComponentSignature = system.second->GetComponentSignature();

        // Bitwise comparison between the entity and system signatures
        bool isInterested = (entityComponentSignature & systemComponentSignature)
                            == systemComponentSignature;

        // Add entity to the system
        if (isInterested)
            system.second->AddEntityToSystem(entity);
    }
}

void Registry::Update() {
    // Add entities from the creating waiting list to the active systems
    for (auto entity : entitiesToBeAdded)
        AddEntityToSystems(entity);
    entitiesToBeAdded.clear();

    // TODO: Remove entities from the deleting waiting list from the active systems
    // for (auto entity: entitiesToBeKilled) {
    //     KillEntity(entity);
    // }
    // entitiesToBeKilled.clear();
}