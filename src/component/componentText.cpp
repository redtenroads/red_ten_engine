// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "math/glm/gtc/type_ptr.hpp"
#include "component/componentText.h"
#include "opengl/glew.h"
#include "common/commonShaders.h"
#include <math.h>
#include <SDL_ttf.h>
#include <SDL.h>

ComponentText::ComponentText()
{
    mAnchor = Matrix4(1.0f);
    setAnchor(0.5f, 0.5f);
    bUseBlendingPhase = true;
}

EXPORT ComponentText::~ComponentText()
{
    if (textureID != 0)
        glDeleteTextures(1, &textureID);
}

bool ComponentText::onRenderPrepare(Matrix4 &vpMatrix, Transformation *tf, bool isShadowStage)
{
    if (isStringDirty)
        rebuildString();

    if (textureID)
    {
        Matrix4 mTransform = *transform.getModelMatrix();
        Matrix4 mModelTransform = *tf->getModelMatrix() * mTransform;

        Matrix4 mOut = mModelTransform * mAnchor;

        auto shader = CommonShaders::spriteShader;
        shader->use(vpMatrix, mOut);
        shader->setOpacity(opacity);

        glBindTexture(GL_TEXTURE_2D, textureID);
        CommonShaders::spriteMesh->use();
        return true;
    }
    return false;
}

int  ComponentText::getVertexAmount()
{
    return 4;
}

void ComponentText::setOpacity(float opacity)
{
    this->opacity = fmaxf(fminf(opacity, 1.0f), 0.0f);
}

float ComponentText::getOpacity()
{
    return opacity;
}

void ComponentText::setAnchor(float x, float y)
{
    mAnchor = glm::translate(Matrix4(1.0f), Vector3(-x, -y, 0.0f));
}

void ComponentText::setAnchor(Vector2 &anchor)
{
    mAnchor = glm::translate(Matrix4(1.0f), Vector3(-anchor.x, -anchor.y, 0.0f));
}

void ComponentText::setFont(ResourceFont *font)
{
    if (this->font != font)
    {
        this->font = font;
        isStringDirty = true;
    }
}

void ComponentText::setColor(unsigned char r, unsigned char g, unsigned char b)
{
    if (this->r != r || this->g != g || this->b != b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        isStringDirty = true;
    }
}

void ComponentText::setText(std::string string)
{
    if (this->string != string)
    {
        this->string = string;
        isStringDirty = true;
    }
}

std::string ComponentText::getText()
{
    return string;
}

void ComponentText::rebuildString()
{
    isStringDirty = false;
    if (textureID != 0)
        glDeleteTextures(1, &textureID);
    textureID = 0;

    if (font && font->getFont() && string.length() > 0)
    {
        SDL_Color color = {r, g, b};
        SDL_Surface *surface = TTF_RenderUTF8_Blended((TTF_Font *)font->getFont(), string.c_str(), color);

        if (!surface)
        {
            logger->logff("Failed to render text into surface: %s", string.c_str());
            textTextureWidth = 0;
            textTextureHeight = 0;
        }
        else
        {
            transform.setScale(surface->w, surface->h);
            textTextureWidth = surface->w;
            textTextureHeight = surface->h;

            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
            glGenerateMipmap(GL_TEXTURE_2D);

            SDL_FreeSurface(surface);
        }
    }
}

int ComponentText::getWidth()
{
    return textTextureWidth;
}

int ComponentText::getHeight()
{
    return textTextureHeight;
}

Matrix4 ComponentText::getLocalspaceMatrix()
{
    Matrix4 mTransform = *transform.getModelMatrix();
    Matrix4 mOut(1.0f);
    mOut *= mAnchor;
    mOut *= mTransform;

    return mOut;
}