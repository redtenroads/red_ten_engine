// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "stage/layer.h"
#include "actor/actor.h"
#include "os/view.h"
#include "camera/camera.h"
#include "controller/physicsController.h"
#include "math/math.h"
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
    EXPORT void enableCollisions();
    EXPORT void enablePhisics(Vector3 gravity);
    EXPORT void disablePhysics();
    EXPORT void enableSorting();
    EXPORT void disableSorting();

    EXPORT std::list<Actor *> castRayCollision(Vector3 v1, Vector3 v2, int channelId = 0);
    EXPORT std::list<Actor *> castSphereCollision(Vector3 v1, float radius, int channelId = 0);
    EXPORT std::list<Actor *> castPointCollision(Vector3 v1, int channelId = 0);

    EXPORT std::list<Actor *> *getActorsList();
    EXPORT void clear(bool destroyCameras = true);

    EXPORT void setVisible(bool state) { bIsVisible = state; }
    EXPORT bool isVisible() { return bIsVisible; }

    EXPORT void setProcessingEnabled(bool state) { bProcessingEnabled = state; }
    EXPORT bool isProcessingEnabled() { return bProcessingEnabled; }

    EXPORT void setAmbientColor(float r, float g, float b)
    {
        ambientColor[0] = r;
        ambientColor[1] = g;
        ambientColor[2] = b;
    }

protected:
    bool bIsVisible = true;
    bool bProcessingEnabled = true;
    float ambientColor[3] = {1.0f, 1.0f, 1.0f};

    std::list<Actor *> actors;
    PhysicsDescriptor *physicsSystem = nullptr;
    Vector3 gravity;
    bool bUseSorting = false;
    Camera *activeCamera = nullptr;
};
