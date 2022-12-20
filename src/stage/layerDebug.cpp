// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "layerDebug.h"

ResourceFont *LayerDebug::font = nullptr;
DebugController *LayerDebug::debugController = nullptr;

class LayerDebugActor : public Actor
{
public:
    void setup(ResourceFont *font, int entryIndex, std::string str, Color *color)
    {
        this->entryIndex = entryIndex;

        ComponentText *text;
        text = createComponent<ComponentText>();
        text->setFont(font);
        text->setText(str);
        text->setColor(10, 10, 10);
        text->setAnchor(0.0f, 1.0f);
        text->transform.setPosition(-1.0f, -1.0f);

        text = createComponent<ComponentText>();
        text->setFont(font);
        text->setText(str);
        text->setColor(10, 10, 10);
        text->setAnchor(0.0f, 1.0f);
        text->transform.setPosition(1.0f, 1.0f);

        text = createComponent<ComponentText>();
        text->setFont(font);
        text->setText(str);
        text->setColor(10, 10, 10);
        text->setAnchor(0.0f, 1.0f);
        text->transform.setPosition(-1.0f, 1.0f);

        text = createComponent<ComponentText>();
        text->setFont(font);
        text->setText(str);
        text->setColor(10, 10, 10);
        text->setAnchor(0.0f, 1.0f);
        text->transform.setPosition(1.0f, -1.0f);

        text = createComponent<ComponentText>();
        text->setFont(font);
        text->setText(str);
        text->setColor(color->r * 255, color->g * 255, color->b * 255);
        text->setAnchor(0.0f, 1.0f);
    }

    int entryIndex;
};

LayerDebug::LayerDebug(int index) : LayerActors("debug", index)
{
    camera = createActor<CameraOrto>();
    camera->setWidthBasedResolution(1280);
}

void LayerDebug::process(float delta)
{
    LayerActors::process(delta);

    if (!debugController->isVisible())
    {
        setVisible(false);
        return;
    }

    auto entries = debugController->getEntries();
    for (auto it = entries->begin(); it != entries->end(); it++)
        if (!hasIndex((*it)->index))
        {
            auto actor = createActor<LayerDebugActor>();
            actor->setup(font, (*it)->index, (*it)->str, &(*it)->color);
            debugActors.push_back(actor);
        }

    if (debugActors.size() > 0 && camera->view && camera->view->getWidth() > 0)
    {
        camera->setWidthBasedResolution(camera->view->getWidth());

        float shift = 0.0f;
        auto it = debugActors.begin();
        float sPositionX = -camera->getWidth() / 2.0f + 5.0f;
        float sPositionY = camera->getHeight() / 2.0f - 5.0f;
        while (it != debugActors.end())
        {
            LayerDebugActor *actor = (LayerDebugActor *)(*it);
            actor->transform.setPosition(sPositionX, sPositionY - shift);
            shift += 32.0f;
            if (!debugController->hasIndex(actor->entryIndex))
            {
                actor->destroy();
                it = debugActors.erase(it);
            }
            else
                ++it;
        }
    }
}

bool LayerDebug::hasIndex(int index)
{
    for (auto it = debugActors.begin(); it != debugActors.end(); it++)
    {
        if (((LayerDebugActor *)(*it))->entryIndex == index)
            return true;
    }
    return false;
}

void LayerDebug::setFont(ResourceFont *font)
{
    LayerDebug::font = font;
}

void LayerDebug::setDebugController(DebugController *debugController)
{
    LayerDebug::debugController = debugController;
}