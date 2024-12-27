#pragma once

#include "KC_ComponentManager.h"
#include "KC_EntityManager.h"

class KC_World final
{
public:
    const KC_MainComponentManager& GetComponentManager() const { return myComponentManager; };
    KC_MainComponentManager& GetComponentManager() { return myComponentManager; }
    
    const KC_EntityManager& GetEntityManager() const { return myEntityManager; }
    KC_EntityManager& GetEntityManager() { return myEntityManager; }

private:
    KC_MainComponentManager myComponentManager;
    KC_EntityManager myEntityManager;
};
