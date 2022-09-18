#include "componentSprite.h"
#include <math.h>
#include "glew.h"

ComponentSprite::ComponentSprite() : Component()
{
    a.setIdentity();
    setAnchor(0.5f, 0.5f);
}

void ComponentSprite::render(Matrix4 &vpMatrix, Transformation *tf)
{
    if (texture)
    {
        Matrix4 model, out;
        model.setIdentity();
        model.multiply(*transform.getModelMatrix());
        model.multiply(*tf->getModelMatrix());

        out.setIdentity();
        out.multiply(a);
        out.multiply(model);

        ShaderDescriptor *spShader;
        if (framesTotal == 1)
            spShader = shadersManager->getSpriteShader();
        else
            spShader = shadersManager->getSpriteFramedShader();

        shadersManager->switchShader(spShader);

        glUniformMatrix4fv(spShader->mViewProjectionLoc, 1, GL_FALSE, vpMatrix.values);
        glUniformMatrix4fv(spShader->mTransformLoc, 1, GL_FALSE, out.values);
        glUniform1f(spShader->fOpacityLoc, opacity);

        if (framesTotal)
        {
            glUniform2fv(spShader->v2TexCoordShift, 1, frameShift);
            glUniform2fv(spShader->v2TexCoordMul, 1, frameRenderSize);
        }

        glBindTexture(GL_TEXTURE_2D, texture->getGLTextureId());
        glBindVertexArray(spShader->vao);
        glDrawArrays(GL_QUADS, 0, 4);
    }
}

void ComponentSprite::setOpacity(float opacity)
{
    this->opacity = fmaxf(fminf(opacity, 1.0f), 0.0f);
}

float ComponentSprite::getOpacity()
{
    return opacity;
}

void ComponentSprite::setAnchor(float x, float y)
{
    a.setIdentity();
    a.setTranslation(-x, -y, 0.0f);
}

void ComponentSprite::setAnchor(Vector2 &anchor)
{
    a.setIdentity();
    a.setTranslation(-anchor.x, -anchor.y, 0.0f);
}

void ComponentSprite::setTexture(Texture *texture)
{
    this->texture = texture;
    if (texture && !texture->isLoaded())
        texture->reload();
    calcFrameRenderSize();
}

void ComponentSprite::setFramedRender(int framesInLine, int frameRows)
{
    if (framesInLine == 0)
        framesInLine = 1;
    if (frameRows == 0)
        frameRows = 1;

    this->framesInLine = framesInLine;
    framesTotal = framesInLine * frameRows;

    frameSize[0] = 1.0f / (float)framesInLine;
    frameSize[1] = 1.0f / (float)frameRows;

    setFrame(0);

    if (texture)
    {
        calcFrameRenderSize();
    }
}

void ComponentSprite::setFrame(int frame)
{
    frame = frame % framesTotal;
    frameShift[0] = frameSize[0] * (frame % framesInLine);
    frameShift[1] = frameSize[1] * (frame / framesInLine);
    currentFrame = frame;
}

Matrix4 ComponentSprite::getLocalspaceMatrix()
{
    Matrix4 model, out;
    model.setIdentity();
    model.multiply(*transform.getModelMatrix());

    out.setIdentity();
    out.multiply(a);
    out.multiply(model);

    return out;
}

void ComponentSprite::calcFrameRenderSize()
{
    if (texture)
    {
        transform.setScale((float)texture->getWidth() * frameSize[0], (float)texture->getHeight() * frameSize[1], 1.0f);
        float pxSize = 1.0f / (float)texture->getWidth();
        float pySize = 1.0f / (float)texture->getHeight();
        frameRenderSize[0] = frameSize[0] - pxSize;
        frameRenderSize[1] = frameSize[1] - pySize;
    }
    else
    {
        transform.setScale(1.0f, 1.0f, 1.0f);
        frameRenderSize[0] = 1.0f;
        frameRenderSize[1] = 1.0f;
    }
}