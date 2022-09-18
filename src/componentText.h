#pragma once
#include "utils.h"
#include "component.h"
#include "texture.h"
#include "font.h"

class ComponentText : public Component
{
public:
    EXPORT ComponentText();

    EXPORT void render(Matrix4 &vpMatrix, Transformation *tf);
    EXPORT void destroy();

    EXPORT void setOpacity(float opacity);
    EXPORT float getOpacity();
    EXPORT void setAnchor(float x, float y);
    EXPORT void setAnchor(Vector2 &anchor);

    EXPORT void setFont(Font *font);
    EXPORT void setColor(unsigned char r, unsigned char g, unsigned char b);
    EXPORT void setText(std::string string);
    EXPORT std::string getText();
    EXPORT void rebuildString();

    EXPORT Matrix4 getLocalspaceMatrix();

protected:
    unsigned char r = 255, g = 255, b = 255;
    std::string string = "";
    bool isStringDirty = true;
    float opacity = 1.0f;
    unsigned int textureID = 0;
    Matrix4 a;
    Font *font = nullptr;
};
