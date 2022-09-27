// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "componentLight.h"
#include "common/commonShaders.h"
#include "opengl/glew.h"
#include "opengl/wglew.h"
#include "math/glm/gtc/type_ptr.hpp"

ComponentLight::ComponentLight()
{
    bUseLightPhase = true;
}

void ComponentLight::setupSunLight(Vector3 normal, Vector3 color, bool bCastShadows)
{
    type = LightType::Sun;
    auto normalLight = glm::normalize(normal * -1.0f);

    this->normal[0] = normalLight.x;
    this->normal[1] = normalLight.y;
    this->normal[2] = normalLight.z;

    this->color[0] = color.x;
    this->color[1] = color.y;
    this->color[2] = color.z;

    this->bCastShadows = bCastShadows;
}

void ComponentLight::setupOmniLight(float affectDistance, Vector3 color, bool bCastShadows)
{
    type = LightType::Omni;
    this->affectDistance = affectDistance;

    this->color[0] = color.x;
    this->color[1] = color.y;
    this->color[2] = color.z;

    this->bCastShadows = bCastShadows;
}

void ComponentLight::renderLightPhase(Matrix4 &vpMatrix, Transformation *tf)
{
    if (type == LightType::Sun)
    {
        auto lightShader = CommonShaders::sunShader;

        Matrix4 m;
        lightShader->use(m, m);
        lightShader->setLightDirection(normal);
        lightShader->setLightColor(color);

        CommonShaders::screenMesh->use();
        glDrawArrays(GL_QUADS, 0, 4);
    }

    if (type == LightType::Omni)
    {
        Matrix4 mModelTransform = *tf->getModelMatrix() * *transform.getModelMatrix();
        auto lightShader = CommonShaders::omniShader;

        lightShader->use(mModelTransform, mModelTransform);
        lightShader->setAffectDistance(affectDistance);
        lightShader->setLightColor(color);

        CommonShaders::screenMesh->use();
        glDrawArrays(GL_QUADS, 0, 4);
    }
}