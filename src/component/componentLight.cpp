// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "componentLight.h"
#include "common/commonShaders.h"
#include "common/utils.h"
#include "opengl/glew.h"
#include "opengl/wglew.h"
#include "math/glm/gtc/type_ptr.hpp"

ComponentLight::ComponentLight()
{
    bUseLightPhase = true;
}

void ComponentLight::setupSunLight(Vector3 position, Vector3 color, bool bCastShadows)
{
    type = LightType::Sun;
    auto normalLight = glm::normalize(position);

    this->normal[0] = normalLight.x;
    this->normal[1] = normalLight.y;
    this->normal[2] = normalLight.z;

    this->color[0] = color.x;
    this->color[1] = color.y;
    this->color[2] = color.z;

    bUseShadowPhase = bCastShadows;
    this->affectDistance = 32.0f;
}

void ComponentLight::setupOmniLight(float affectDistance, Vector3 color, bool bCastShadows)
{
    type = LightType::Omni;
    this->affectDistance = affectDistance;

    this->color[0] = color.x;
    this->color[1] = color.y;
    this->color[2] = color.z;

    bUseShadowPhase = bCastShadows;
}

bool ComponentLight::onRenderPrepare(Matrix4 &vpMatrix, Transformation *tf, bool isShadowStage)
{
    if (!isShadowStage)
        this->tf = tf;
    return false;
}

Matrix4 ComponentLight::preparePreShadowPhase(Vector3 cameraPosition)
{
    if (bUseShadowPhase)
    {
        if (type == LightType::Sun)
        {
            // View matrix is placement of sun
            Vector3 direction = glm::normalize(Vector3(normal[0], normal[1], normal[2]));
            Vector3 sunPosition = cameraPosition + direction * (affectDistance / 2.0f);
            Matrix4 rotation = glm::lookAt(sunPosition, cameraPosition, Vector3(0.0f, 1.0f, 0.0f));

            // Light is using orthogonal projection matrix
            float r = affectDistance / 2.0f;
            float l = -r;
            float b = -affectDistance / 2.0f;
            float t = -b;
            float f = affectDistance;
            float n = 0.0f;

            Matrix4 lightProjectionMatrix = glm::ortho(l, r, b, t, n, f);
            lightViewProjection = lightProjectionMatrix * rotation;

            /*
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                // Now we need to render scene to the depth buffer
                glViewport(0, 0, shadowMapWidth, shadowMapHeight);
                glBindFramebuffer(GL_FRAMEBUFFER, shadowFrameBuffer);
                glClear(GL_DEPTH_BUFFER_BIT);
            */
        }
    }
    return lightViewProjection;
}

void ComponentLight::renderLightPhase(Matrix4 &vpMatrix, unsigned int shadowMapTexture)
{
    if (type == LightType::Sun)
    {
        auto lightShader = bUseShadowPhase ? CommonShaders::sunWithShadowShader : CommonShaders::sunShader;
        lightShader->use(m1, m1);

        if (bUseShadowPhase)
        {
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
            lightShader->setLightSpaceMatrix(lightViewProjection);
        }

        lightShader->setLightDirection(normal);
        lightShader->setLightColor(color);

        CommonShaders::screenMesh->use();
        glBlendFunc(GL_ONE, GL_ONE);
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
        glBlendFunc(GL_ONE, GL_ONE);
        glDrawArrays(GL_QUADS, 0, 4);
    }
}

void ComponentLight::enableShadows()
{
    bUseShadowPhase = true;
}

void ComponentLight::disableShadows()
{
    bUseShadowPhase = false;
}
