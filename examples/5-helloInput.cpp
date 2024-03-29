// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

// What a game without controlling your character? The way to make actor receive input is to use ActorPawn
class CratePlayer : public ActorPawn
{
public:
    CratePlayer() : ActorPawn()
    {
        registerClassName("CratePlayer");

        setPhysicsMotionType(MotionType::Dynamic);
        transform.setScale(0.5f);
        sprite = createComponent<ComponentSprite>();
        sprite->setTexture(crateTexture);
        sprite->addPhysics2dBox(256.0f, 256.0f);
        setFrictionAndRestitution(0.9f, 0.1f);

        setZAxisLocked(true);
        // This way in 2d space you can block rotation of the object
        setZAxisRotationLocked(true);

        // First we create an input type with function to receive the input itself and make an action base on it
        auto inputX = registerAxisCallback(this, &CratePlayer::controlX);
        // Second we will add the dependences of this input type around your gamepad, mouse, keyboard and other input devices
        // Second parameter is a multiplier of device input making it here in range from -400 to 400
        // Note that keyboard usually have input of 0 to 1, but axises have input from -1 to 1.
        // Also some gamepad axises may have -1 in their rest position, like triggers
        inputX->addKeyboardBinding(KeyboardCodes::D, 400.0f);
        inputX->addKeyboardBinding(KeyboardCodes::A, -400.0f);
        inputX->addKeyboardBinding(KeyboardCodes::RIGHT, 400.0f);
        inputX->addKeyboardBinding(KeyboardCodes::LEFT, -400.0f);
        inputX->addGamepadAxisBinding(0, 400.0f);

        auto inputY = registerAxisCallback(this, &CratePlayer::controlY);
        inputY->addKeyboardBinding(KeyboardCodes::S, -400.0f);
        inputY->addKeyboardBinding(KeyboardCodes::W, 400.0f);
        inputY->addKeyboardBinding(KeyboardCodes::DOWN, -400.0f);
        inputY->addKeyboardBinding(KeyboardCodes::UP, 400.0f);
        inputY->addGamepadAxisBinding(1, -400.0f);

        auto inputHide = registerButtonCallback(this, &CratePlayer::hideSurrounding);
        inputHide->addKeyboardBinding(KeyboardCodes::SPACE, 1.0f);
        inputHide->addGamepadButtonBinding(0, 1.0f);
    }

    void onProcess(float delta)
    {
        addLinearVelocity(Vector3(moveX * delta * 2.0f, moveY * delta * 2.0f, 0.0f));
        counter += delta;
        while (counter > 0.10f)
        {
            counter -= 0.10f;
            setLinearVelocity(getLinearVelocity() * 0.8f);
        }
    }

    void controlX(InputType type, int deviceIndex, int index, float axis)
    {
        moveX = axis;
    }

    void controlY(InputType type, int deviceIndex, int index, float axis)
    {
        moveY = axis;
    }

    void hideSurrounding(InputType type, int deviceIndex, int index, bool state)
    {
        auto layerActors = getLayerActorsList();
        for (auto it = layerActors->begin(); it != layerActors->end(); it++)
            if ((*it)->is("HiddenCrate"))
                (*it)->setVisible(state);
    }

    static Texture *crateTexture;
    float moveX = 0.0f, moveY = 0.0f;
    float counter = 0.0f;
protected:
    ComponentSprite *sprite;
};
Texture *CratePlayer::crateTexture = nullptr;

class HiddenCrate : public Actor
{
public:
    HiddenCrate() : Actor()
    {
        registerClassName("HiddenCrate");
        transform.setScale(0.5f);
        sprite = createComponent<ComponentSprite>();
        sprite->setTexture(crateTexture);
        setPhysicsMotionType(MotionType::Static);
        setZAxisLocked(true);
        setFrictionAndRestitution(0.9f, 0.1f);
        setVisible(false);
    }

    void setup(float x, float y)
    {
        transform.setPosition(x, y);
        sprite->addPhysics2dBox(256.0f, 256.0f);
    }

    static Texture *crateTexture;

protected:
    ComponentSprite *sprite;
};
Texture *HiddenCrate::crateTexture = nullptr;

class Wall : public Actor
{

public:
    Wall() : Actor()
    {
        registerClassName("Wall");
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
    auto view = viewController->createView("Example \"5. Hello Input\"");

    // Stage setup
    auto stageController = engine->getStageController();
    auto stage = stageController->createStage("Hello Input");

    // Layers and camera setup
    auto layerActors = stage->createLayerActors("Hello Input", 0);
    auto camera = layerActors->createActor<CameraOrto>();
    camera->setWidthBasedResolution(1280);

    layerActors->enablePhisics(Vector3(0.0f, 0.0f, 0.0f));

    // Textures setup
    auto resourceController = engine->getResourceController();
    CratePlayer::crateTexture = resourceController->addTexture("./data/crate.jpg");
    HiddenCrate::crateTexture = CratePlayer::crateTexture;

    auto background = resourceController->addTexture("./data/background.jpg");

    // Just a background for better look
    auto backgroundActor = layerActors->createActor<Actor>();
    backgroundActor->transform.setScale(2.0f);
    auto backgroundActorSprite = backgroundActor->createComponent<ComponentSprite>();
    backgroundActorSprite->setTexture(background);

    Wall *bottomWall = layerActors->createActor<Wall>();
    bottomWall->setup(0, 400.0f, 1280.0f, 60.0f);

    Wall *topWall = layerActors->createActor<Wall>();
    topWall->setup(0, -400.0f, 1280.0f, 60.0f);

    Wall *leftWall = layerActors->createActor<Wall>();
    leftWall->setup(-640.0f, 0.0f, 60.0f, 800.0f);

    Wall *rightWall = layerActors->createActor<Wall>();
    rightWall->setup(640.0f, 0.0f, 60.0f, 800.0f);

    auto crate1 = layerActors->createActor<HiddenCrate>();
    crate1->setup(-200.0f, -200.0f);

    auto crate2 = layerActors->createActor<HiddenCrate>();
    crate2->setup(200.0f, -200.0f);

    auto crate3 = layerActors->createActor<HiddenCrate>();
    crate3->setup(-200.0f, 200.0f);

    auto crate4 = layerActors->createActor<HiddenCrate>();
    crate4->setup(200.0f, 200.0f);

    layerActors->createActor<CratePlayer>();

    while (!engine->isTerminationIntended())
    {
        float delta = engine->syncFrame();
        viewController->processEvents();
        stage->process(delta);
        stage->present(view);
    }
}