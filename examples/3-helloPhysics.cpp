// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

// This actor represents circle in physics
class Ball : public Actor
{
public:
    Ball() : Actor()
    {
        registerClassName("Ball");
        setPhysicsMotionType(MotionType::Dynamic);
        transform.setScale(0.1f);
        sprite = createComponent<ComponentSprite>();
        sprite->setTexture(ballTexture);

        // Phisics shapes are added to components and then built in overall complex shape for actor
        sprite->addPhysics2dCircle(120.0f);
        // Friction and restitution should be in range of 0 to 1
        setFrictionAndRestitution(0.85f, 0.4f);
        // Since we're in 2d space this command blocks Z movement leaving only X(left-right) and Y(top-bottom) movement
        setZAxisLocked(true);
    }
    void onProcess(float delta)
    {
        // If entity fall too low under the screen destroy it
        if (transform.getPosition().y < -300)
            destroy();
    }

    static Texture *ballTexture;

protected:
    ComponentSprite *sprite;
};
Texture *Ball::ballTexture = nullptr;

// This actor represents box shape
class Crate : public Actor
{
public:
    Crate() : Actor()
    {
        registerClassName("Crate");
        setPhysicsMotionType(MotionType::Dynamic);
        transform.setScale(0.1f);
        sprite = createComponent<ComponentSprite>();
        sprite->setTexture(crateTexture);
        sprite->addPhysics2dBox(256.0f, 256.0f);
        setFrictionAndRestitution(0.9f, 0.1f);
        setZAxisLocked(true);
    }
    void onProcess(float delta)
    {
        if (transform.getPosition().y < -300)
            destroy();
    }

    static Texture *crateTexture;

protected:
    ComponentSprite *sprite;
};
Texture *Crate::crateTexture = nullptr;

// This actor represents more complex box shape consist of 2 boxes
class DoubleCrate : public Actor
{
public:
    DoubleCrate() : Actor()
    {
        registerClassName("DoubleCrate");
        setPhysicsMotionType(MotionType::Dynamic);
        transform.setScale(0.1f);
        spriteLeft = createComponent<ComponentSprite>();
        spriteLeft->setTexture(crateTexture);

        spriteRight = createComponent<ComponentSprite>();
        spriteRight->setTexture(crateTexture);
        spriteRight->transform.setPosition(256.0f, 0.0f, 0.0f);

        spriteLeft->addPhysics2dBox(256.0f, 256.0f);
        spriteRight->addPhysics2dBox(256.0f, 256.0f);

        setFrictionAndRestitution(1.0f, 0.1f);
        setZAxisLocked(true);
    }
    void onProcess(float delta)
    {
        if (transform.getPosition().y < -300)
            destroy();
    }

    static Texture *crateTexture;

protected:
    ComponentSprite *spriteLeft;
    ComponentSprite *spriteRight;
};
Texture *DoubleCrate::crateTexture = nullptr;

// This component is just an invisible box
class Wall : public Actor
{
public:
    Wall() : Actor()
    {
        registerClassName("Wall");
        // Wall is static, it can't be moved, neither checked for collisions with other walls. By default actors are dynamic but not this one
        setPhysicsMotionType(MotionType::Static);
        setZAxisLocked(true);
    }

    void setup(float x, float y, float width, float height)
    {
        transform.setPosition(x, y);

        auto emptyComponent = createComponent<ComponentSprite>();
        emptyComponent->addPhysics2dBox(width, height);
        setFrictionAndRestitution(1.0f, 0.1f);
    }
};

int main()
{
    // Engine setup
    auto engine = RTEngine::createInstance();

    // Set fullscreen through configuration controller
    auto configController = engine->getConfigController();
    auto config = configController->getConfig();
    config->setWindowWidth(1280);
    config->setWindowHeight(800);
    config->setFullscreenState(false);

    // View setup
    auto viewController = engine->getViewController();
    auto view = viewController->createView("Example \"3. Hello Physics\"");

    // Stage setup
    auto stageController = engine->getStageController();
    auto stage = stageController->createStage("Hello Physics");

    // Layers and camera setup
    auto layerActors = stage->createLayerActors("Hello Physics", 0);
    auto camera = layerActors->createActor<CameraOrto>();
    camera->setWidthBasedResolution(240);

    // We need to enable phisics on layer and provide gravity level.
    // That's enough to make physics work - layer will provide all nessary data to existing actors
    // All new actors will be created with initialized physics
    layerActors->enablePhisics(Vector3(0.0f, -20.0f, 0.0f));

    // Textures setup
    auto resourceController = engine->getResourceController();
    Ball::ballTexture = resourceController->addTexture("./data/ball.png");
    Crate::crateTexture = resourceController->addTexture("./data/crate.jpg");
    DoubleCrate::crateTexture = Crate::crateTexture;
    auto background = resourceController->addTexture("./data/background.jpg");

    // Just a background for better look
    auto backgroundActor = layerActors->createActor<Actor>();
    backgroundActor->transform.setScale(0.25f);
    auto backgroundActorSprite = backgroundActor->createComponent<ComponentSprite>();
    backgroundActorSprite->setTexture(background);

    // Here we make a static invisible walls. Actors can't have collision descriptions themselves,
    // So we need to create an empty sprite and put collisions on the component.
    Wall *bottomWall = layerActors->createActor<Wall>();
    bottomWall->setup(0, -80.0f, 320.0f, 20.0f);

    Wall *leftWall = layerActors->createActor<Wall>();
    leftWall->setup(-120.0f, -100.0f, 10.0f, 120.0f);

    Wall *rightWall = layerActors->createActor<Wall>();
    rightWall->setup(120.0f, -100.0f, 10.0f, 120.0f);

    float counter = 1.0f;
    while (!engine->isTerminationIntended())
    {
        float delta = engine->syncFrame();
        counter += delta * 2.0f;
        if (counter > 1.0f)
        {
            Actor *spawn = nullptr;
            counter -= 1.0f;
            int sp = rand() % 10;
            if (sp > 4)
                spawn = layerActors->createActor<Ball>();
            else if (sp > 1)
                spawn = layerActors->createActor<Crate>();
            else
                spawn = layerActors->createActor<DoubleCrate>();

            if (spawn)
                spawn->transform.setPosition((float)((rand() % 200) - 100), 200.0f);
        }
        viewController->processEvents();
        stage->process(delta);
        stage->present(view);
    }
}