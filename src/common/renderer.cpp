// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "renderer.h"
#include "opengl/glew.h"
#include "opengl/wglew.h"
#include "common/commonShaders.h"

Renderer::Renderer(int width, int height, Config *config)
{
    this->width = width;
    this->height = height;

    shadowMapSize = 1024;
    if (config->getShadowQuality() == RenderQuality::High)
        shadowMapSize = 4096;
    if (config->getShadowQuality() == RenderQuality::Balanced)
        shadowMapSize = 2048;

    // Rendering images
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenTextures(1, &lightningPicture);
    glBindTexture(GL_TEXTURE_2D, lightningPicture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenTextures(1, &shadowPicture);
    glBindTexture(GL_TEXTURE_2D, shadowPicture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // G BUFFERS
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    // - color + specular color buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gAlbedoSpec, 0);

    // - normal color buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    // - position color buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gPosition, 0);

    // - light emission buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, lightningPicture, 0);

    // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(4, attachments);

    // Depth buffer
    glGenRenderbuffers(1, &depthbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Lightning buffer
    glGenFramebuffers(1, &lightningBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, lightningBuffer);

    // We draw to lightning picture only
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightningPicture, 0);

    // - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int lightningAttachments[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, lightningAttachments);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Shadow buffer
    glGenFramebuffers(1, &shadowBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);

    // Shadowmap
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowPicture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Renderer::~Renderer()
{
    // Deleting buffers
    glDeleteFramebuffers(1, &gBuffer);
    glDeleteFramebuffers(1, &lightningBuffer);
    glDeleteFramebuffers(1, &shadowBuffer);

    glDeleteTextures(1, &gAlbedoSpec);
    glDeleteTextures(1, &gNormal);
    glDeleteTextures(1, &gPosition);
    glDeleteTextures(1, &lightningPicture);
    glDeleteTextures(1, &shadowPicture);
}

unsigned int Renderer::getPositionTexture()
{
    return gPosition;
}

unsigned int Renderer::getNormalTexture()
{
    return gNormal;
}

unsigned int Renderer::getAlbedoTexture()
{
    return gAlbedoSpec;
}

unsigned int Renderer::getLightningTexture()
{
    return lightningPicture;
}

unsigned int Renderer::getShadowTexture()
{
    return shadowPicture;
}

void Renderer::setupNewFrame(bool clear)
{
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    if (clear)
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void Renderer::setupLightning(bool clear)
{
    glViewport(0, 0, width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, lightningBuffer);
    if (clear)
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void Renderer::setupShadowHQ(bool clear)
{
    glViewport(0, 0, shadowMapSize, shadowMapSize);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
    if (clear)
        glClear(GL_DEPTH_BUFFER_BIT);
}

int Renderer::getShadowMapSize()
{
    return shadowMapSize;
}