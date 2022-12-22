// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "math/math.h"
#include "math/transformation.h"
#include "stage/layer.h"
#include <string>
#include <list>
#include <vector>

class Entity
{
public:
    EXPORT Entity();
    EXPORT virtual ~Entity();

    EXPORT void lookAt(Vector3 v);
    EXPORT void lookAt(float x, float y, float z);

    EXPORT bool isVisible();
    EXPORT void setVisible(bool state);

    EXPORT const std::string getName();
    EXPORT const std::string getClass();
    EXPORT bool is(std::string name);
    EXPORT bool implements(std::string name);
    EXPORT void setCurrentLayer(Layer *layer);

    EXPORT virtual void childUpdated();

    Transformation transform;
    int zDepth = 0;

protected:
    EXPORT void registerName(std::string name);

    bool bIsVisible = true;
    void *layer = nullptr;

    float restitution = 0.5f;
    float friction = 1.0f;
    float zLockedPosition = 0.0f;

private:
    std::string className = "";
    std::vector<std::string> classChierarchy;
};
