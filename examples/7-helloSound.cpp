// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

// Sound player could be component for an actor, same is sprite component
// It allows sound to be panned depending on actor's position
// Btw, Jojo belongs to it's owner and has no relation to Red Ten Roads
class JoJo : public ActorPawn
{
public:
    JoJo() : ActorPawn()
    {
        registerName("JoJo");
        transform.setScale(0.8);
        // Note: here we use z position of an actor, because that's the way to control sound pan in 3d space
        transform.setPosition(0.0f, -50.0f, frontDistance);
        jojoTexture->setMakeBytemapAlpha(true);
        jojoTexture->setBytemapScale(4);
        jojoTexture->reload();

        sprite = createComponent<ComponentSprite>();
        sprite->setTexture(jojoTexture);

        auto inputX = registerAxisCallback(&JoJo::controlX);
        inputX->addInputBinding(InputType::KEYBOARD, 7, 400.0f);
        inputX->addInputBinding(InputType::KEYBOARD, 4, -400.0f);
        inputX->addInputBinding(InputType::KEYBOARD, 79, 400.0f);
        inputX->addInputBinding(InputType::KEYBOARD, 80, -400.0f);
    }

    void onProcess(float delta)
    {
        transform.translate(moveX * delta, 0, 0);
        auto position = transform.getPosition();
        if (position.x > 600.0f)
            transform.setPosition(600.0f, position.y, frontDistance);

        if (position.x < -600.0f)
            transform.setPosition(-600.0f, position.y, frontDistance);
    }

    void controlX(InputType type, int deviceIndex, int index, float axis)
    {
        moveX = axis;
    }

    static Texture *jojoTexture;

    float counter = 0.0f;
    float moveX = 0.0f;
    const float frontDistance = -600;
    LayerActors *layerActors = nullptr;

protected:
    ComponentSprite *sprite;
};
Texture *JoJo::jojoTexture = nullptr;

int main()
{
    // Engine setup
    auto engine = RTEngine::createInstance();

    // View setup
    auto viewController = engine->getViewController();
    auto view = viewController->createView("Example \"7. Hello Sound\"", 1280, 800, false);

    // Stage setup
    auto stageController = engine->getStageController();
    auto stage = stageController->createStage("Hello Sound");

    // Layers and camera setup
    auto layerActors = stage->createLayerActors("Hello Sound", 0);
    auto camera = layerActors->createActor<CameraOrto>();
    camera->setWidthBasedResolution(1280);

    // Textures and sound setup
    auto resourceController = engine->getResourceController();
    JoJo::jojoTexture = resourceController->addTexture("./data/jojo.png");
    auto background = resourceController->addTexture("./data/background.jpg");

    Sound *jojoSong = resourceController->addSound("./data/sound.wav");
    // Note that only mono sounds can be panned. Stereo sounds always being played as is
    // But we can force mono
    jojoSong->setForceMono(true);

    // This is our sound player that playes the song
    SoundPlayer *player = new SoundPlayer();
    player->setSound(jojoSong);
    player->playLoop();

    // Just a background for better look
    auto backgroundActor = layerActors->createActor<Actor>();
    backgroundActor->transform.setScale(1.5f);
    auto backgroundActorSprite = backgroundActor->createComponent<ComponentSprite>();
    backgroundActorSprite->setTexture(background);

    auto jojo = layerActors->createActor<JoJo>();
    jojo->layerActors = layerActors;

    while (!engine->isTerminationIntended())
    {
        float delta = engine->syncFrame();
        viewController->processEvents();
        stage->process(delta);
        stage->present(view);
    }
}