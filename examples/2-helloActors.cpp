// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

// To create actor's logic you need to inherit actor into your class then you will get an access to Actor's life cycle
class Ball : public Actor
{
public:
    // Constuctor usually being used to create all of the components, but design of engine doesn't allow to
    // pass parameters here to your class. The usual way is to create setup function that will be called after the creation
    // and setting up your class initially by the engine
    Ball() : Actor()
    {
        moveDirection = (float)rand() / (float)RAND_MAX * 3.14f * 2.0f;
        moveSpeed = (float)(rand() % 100);
        life = 10.0f;

        // All actors has transform property inside them that provides ability to set position, scale and rotation in 3d or 2d space
        transform.setScale(0.5f);
        transform.setPosition(
            ((float)rand() / (float)RAND_MAX - 0.5f) * 400.0f,
            ((float)rand() / (float)RAND_MAX - 0.5f) * 400.0f);
        sprite = createComponent<ComponentSprite>();
        sprite->setTexture(ballTexture);
    }

    // This is the standart actor lifecycle function called on all stage updates
    void onProcess(float delta)
    {
        Actor::onProcess(delta);
        transform.translate(sinf(moveDirection) * moveSpeed * delta, cosf(moveDirection) * moveSpeed * delta);

        // Actors don't have transparancy because they are abstraction, they don't determine how the should be drawn,
        // only provide transformation to their components. But components can have a lot of parameters to control their presentation.
        // Such as opacity
        sprite->setOpacity(life);
        life -= delta;
        if (life < 0)
        {
            // Never delete actors manually, because Engine should handle creation and removing actors properly in specific stage of pipeline
            // After calling destroy your actor wont be immidietely removed, but will stop existing before rendering stage.
            destroy();
        }
    }

    // Common way of providing textures to actors is just to use static functions
    // or to provide resourceManager and load textures inside the constructor
    static Texture *ballTexture;

protected:
    float moveDirection, moveSpeed, life;
    ComponentSprite *sprite;
};
Texture *Ball::ballTexture = nullptr;

int main()
{
    // Engine setup
    auto engine = RTEngine::createInstance();

    // View setup
    auto viewController = engine->getViewController();
    auto view = viewController->createView("Example \"2. Hello Actors\"", 1280, 800, false);

    // Stage setup
    auto stageController = engine->getStageController();
    auto stage = stageController->createStage("Hello Actors");

    // Layers and camera setup
    auto layerActors = stage->createLayerActors("Hello Actors", 0);
    auto camera = layerActors->createActor<CameraOrto>();
    camera->setWidthBasedResolution(1280);

    // Textures setup
    auto resourceController = engine->getResourceController();
    Ball::ballTexture = resourceController->addTexture("./data/plate.png");

    float creationCounter = 0;
    while (!engine->isTerminationIntended())
    {
        float delta = engine->syncFrame();

        // This way we will create 200 balls per second
        // Still engine is fast and shouldn't slow down your PC
        creationCounter += delta * 200;
        while (creationCounter > 0.0f)
        {
            creationCounter -= 1.0f;
            layerActors->createActor<Ball>();
        }

        viewController->processEvents();
        stage->process(delta);
        stage->present(view);
    }
}