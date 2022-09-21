// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "actor/actorGUIElement.h"
#include "math/math.h"
#include <SDL.h>

std::list<ActorGUIElement *> ActorGUIElement::selectives;
std::list<int> ActorGUIElement::pressIDStack;
Camera *ActorGUIElement::camera = nullptr;

ActorGUIElement::ActorGUIElement()
{
    auto inputPress = registerButtonCallback<ActorGUIElement>(&ActorGUIElement::onPressCheck);
    inputPress->addInputBinding(InputType::MOUSE, (int)InputTypeMouse::LEFT_BUTTON, 1.0f);
}

ActorGUIElement::~ActorGUIElement()
{
    if (bAddedToSelectives)
    {
        auto it = selectives.begin();
        while (it != selectives.end())
            if ((*it) == this)
            {
                selectives.erase(it);
                break;
            }
    }
}

void ActorGUIElement::setActiveArea(float width, float height)
{
    activeAreaLeft = -width / 2.0f;
    activeAreaRight = width / 2.0f;
    activeAreaTop = -height / 2.0f;
    activeAreaBottom = height / 2.0f;
}

void ActorGUIElement::setActiveArea(float l, float r, float t, float b)
{
    activeAreaLeft = l;
    activeAreaRight = r;
    activeAreaTop = t;
    activeAreaBottom = b;
}

bool ActorGUIElement::isFocues()
{
    return bIsFocused;
}

void ActorGUIElement::onProcess(float delta)
{
    if (!camera)
        return;

    int x, y;
    SDL_GetMouseState(&x, &y);

    float mouseX = (float)x * camera->getWidthViewProportion() - ((float)camera->getWidth() / 2.0f);
    float mouseY = -((float)y * camera->getHeightViewProportion() - ((float)camera->getHeight() / 2.0f));

    Vector4 position(0, 0, 0, 1.0f);
    Vector4 screenPosition = *transform.getModelMatrix() * position;

    bool bHover = screenPosition.x + activeAreaLeft < mouseX &&
                  screenPosition.x + activeAreaRight > mouseX &&
                  screenPosition.y + activeAreaTop < mouseY &&
                  screenPosition.y + activeAreaBottom > mouseY;

    if (bHover && !bIsHovered)
    {
        bIsHovered = true;
        onHover();
    }
    if (!bHover && bIsHovered)
    {
        bIsHovered = false;
        onBlur();
    }
}

void ActorGUIElement::onHover()
{
}

void ActorGUIElement::onFocus()
{
}

void ActorGUIElement::onBlur()
{
}

void ActorGUIElement::onPress()
{
    if (pressID)
        pressIDStack.push_back(pressID);
}

void ActorGUIElement::onRelease()
{
}

void ActorGUIElement::setSelectiveIndex(int selectiveIndex)
{
    this->selectiveIndex = selectiveIndex;
    if (!bAddedToSelectives)
    {
        bAddedToSelectives = true;
        selectives.push_back(this);
    }
}

void ActorGUIElement::setPressID(int pressID)
{
    this->pressID = pressID;
}

void ActorGUIElement::clearPressIDStack()
{
    pressIDStack.clear();
}

int ActorGUIElement::getFirstPressID()
{
    if (pressIDStack.size() > 0)
    {
        int out = pressIDStack.front();
        pressIDStack.pop_front();
        return out;
    }
    return 0;
}

void ActorGUIElement::onPressCheck(InputType type, int deviceIndex, int index, bool state)
{
    if (bIsHovered && state)
        onPress();

    if (bIsHovered && !state)
        onRelease();
}

void ActorGUIElement::sortSelectives()
{
}