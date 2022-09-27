// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

// ComponentSprite are able to draw textures by fragments
// This way usualy spritesheet could be used to animate things
// Btw, Sonic belongs to Sega
class Sonic : public Actor
{
public:
    Sonic()
    {
        lifeSpan = (float)(rand() % 100) / 80.0f + 0.5f;
        transform.setPosition(-800, (rand() % 700) - 350);

        sprite = createComponent<ComponentSprite>();
        sprite->setTexture(sonicTexture);
        // Here we have 9 frames horizontally and 7 in verticaly.
        // Each frame sizes will be calculated automaticaly
        sprite->setFramedRender(9, 7);

        transform.setScale(4.0f);
        zDepth = 1;
    }

    void onProcess(float delta)
    {
        if (lifeSpan < 0.0f)
        {
            jump -= delta * 1200.0f;
            transform.translate(0, jump * delta);
            // here we set the frame of death
            sprite->setFrame(42);
            zDepth = 2;

            if (transform.getPosition().y < -800)
                destroy();
        }
        else
        {
            lifeSpan -= delta;
            transform.translate(delta * 800.0f, 0);
            frame += delta * 8;
            if ((int)frame > 9)
                frame -= 6.0f;
            // our animation loop
            sprite->setFrame((int)frame);
        }
    }

    static Texture *sonicTexture;

protected:
    float lifeSpan;
    ComponentSprite *sprite;
    float frame = 4;
    float jump = 500.0f;
};
Texture *Sonic::sonicTexture = nullptr;

int main()
{
    // Engine setup
    auto engine = RTEngine::createInstance();

    // View setup
    auto viewController = engine->getViewController();
    auto view = viewController->createView(
        "Example \"10. Hello Animation\"",
        viewController->getPrimaryScreenWidth() * 0.8f,
        viewController->getPrimaryScreenHeight() * 0.8f,
        false);

    // Stage setup
    auto stageController = engine->getStageController();
    auto stage = stageController->createStage("Hello Animation");

    // Layer with actors contain sprites and camera to view them
    // To make 2d games you should use ortographic camera that has no perspective distorsion
    // Second parameter index is being used to sort the actors
    auto layerActors = stage->createLayerActors("Hello Animation", 0);
    layerActors->enableSorting();

    // Layers and camera setup
    auto camera = layerActors->createActor<CameraOrto>();
    camera->setWidthBasedResolution(1280);

    // Textures setup
    auto resourceController = engine->getResourceController();
    Sonic::sonicTexture = resourceController->addTexture("./data/sonic.png");

    // Just a background for better look
    auto backgroundActor = layerActors->createActor<Actor>();
    backgroundActor->transform.setScale(2.0f);
    auto backgroundActorSprite = backgroundActor->createComponent<ComponentSprite>();
    backgroundActorSprite->setTexture(resourceController->addTexture("./data/background.jpg"));

    float spawnerCounter = 0.0f;

    while (!engine->isTerminationIntended())
    {
        float delta = engine->syncFrame();
        viewController->processEvents();

        spawnerCounter += delta;
        while (spawnerCounter > 0.1f)
        {
            spawnerCounter -= 0.1f;
            layerActors->createActor<Sonic>();
        }

        stage->process(delta);
        stage->present(view);
    }
}