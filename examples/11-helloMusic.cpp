// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

// We use ComponentSoundPlayer to play sound as well
// Only difference - it's played as stream inside ComponentSoundPlayer
// It means it loads portions of the song on the go to reduce memory consuption.
// This works only with OGG format files, wav format is always being loaded entirely
// Btw, Jojo belongs to it's owner and has no relation to Red Ten Roads
class JoJo : public ActorPawn
{
public:
    JoJo() : ActorPawn()
    {
        registerClassName("JoJo");
        transform.setScale(0.8);
        transform.setPosition(0.0f, -50.0f);

        sprite = createComponent<ComponentSprite>();
        sprite->setTexture(jojoTexture);
    }

    void controlX(InputType type, int deviceIndex, int index, float axis)
    {
        moveX = axis;
    }

    static Texture *jojoTexture;

    float counter = 0.0f;
    float moveX = 0.0f;

protected:
    ComponentSprite *sprite;
};
Texture *JoJo::jojoTexture = nullptr;

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
    auto view = viewController->createView("Example \"11. Hello Music\"");

    // Stage setup
    auto stageController = engine->getStageController();
    auto stage = stageController->createStage("Hello Music");

    // Layers and camera setup
    auto layerActors = stage->createLayerActors("Hello Music", 0);
    auto camera = layerActors->createActor<CameraOrto>();
    camera->setWidthBasedResolution(1280);

    // Textures setup
    auto resourceController = engine->getResourceController();
    JoJo::jojoTexture = resourceController->addTexture("./data/jojo.png");
    auto background = resourceController->addTexture("./data/background.jpg");

    // Music setup and playing
    Sound *cyberpunkSong = resourceController->addSound("./data/streamable.ogg");
    cyberpunkSong->load();

    SoundPlayer *player = new SoundPlayer();
    player->setSound(cyberpunkSong);
    player->playLoop();

    // Just a background for better look
    auto backgroundActor = layerActors->createActor<Actor>();
    backgroundActor->transform.setScale(1.5f);
    auto backgroundActorSprite = backgroundActor->createComponent<ComponentSprite>();
    backgroundActorSprite->setTexture(background);

    layerActors->createActor<JoJo>();

    while (!engine->isTerminationIntended())
    {
        float delta = engine->syncFrame();
        viewController->processEvents();
        stage->process(delta);
        stage->present(view);
    }
}