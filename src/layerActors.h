#pragma once
#include "utils.h"
#include "layer.h"
#include "actor.h"
#include "view.h"
#include "camera.h"
#include "physicsManager.h"
#include "vector3.h"
#include <list>

class LayerActors : public Layer
{
public:
    LayerActors(std::string name, int index);

    template <class T, typename std::enable_if<std::is_base_of<Actor, T>::value>::type * = nullptr>
    EXPORT T *createActor()
    {
        auto newActor = new T();
        prepareNewActor(newActor);
        return newActor;
    }

    EXPORT void process(float delta);
    EXPORT void render(View *view);
    EXPORT void prepareNewActor(Actor *actor);
    EXPORT void enablePhisics(Vector3 gravity);
    EXPORT void disablePhysics();
    EXPORT void enableSorting();
    EXPORT void disableSorting();

    EXPORT std::list<Actor *> castRayCollision(Vector3 v1, Vector3 v2);
    EXPORT std::list<Actor *> castSphereCollision(Vector3 v1, float radius);
    EXPORT std::list<Actor *> castPointCollision(Vector3 v1);

    EXPORT std::list<Actor *> *getActorsList();
    EXPORT void clear(bool destroyCameras = true);

    EXPORT void setVisible(bool state) { bIsVisible = state; }
    EXPORT bool isVisible() { return bIsVisible; }

    EXPORT void setProcessingEnabled(bool state) { bProcessingEnabled = state; }
    EXPORT bool isProcessingEnabled() { return bProcessingEnabled; }

protected:
    bool bIsVisible = true;
    bool bProcessingEnabled = true;

    std::list<Actor *> actors;
    PhysicsDescriptor *physicsSystem = nullptr;
    Vector3 gravity;
    bool bUseSorting = false;
    Camera *activeCamera = nullptr;
};
