// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

class Star : public Actor
{
public:
    Star() : Actor()
    {
        registerName("Star");
        sprite = createComponent<ComponentSprite>();
        sprite->setTexture(starTexture);
    }
    void onProcess(float delta)
    {
        life -= delta;
        if (life < 0.0f)
            destroy();
        sprite->setOpacity(fmaxf(0.0f, life));
    }

    static Texture *starTexture;

protected:
    ComponentSprite *sprite;
    float life = 1.0f;
};
Texture *Star::starTexture = nullptr;

// All the bytemap magic happens in the Actor
// Btw, Jojo belongs to it's owner and has no relation to Red Ten Roads
class JoJo : public Actor
{
public:
    JoJo() : Actor()
    {
        registerName("JoJo");
        transform.setScale(0.8);
        transform.setPosition(0.0f, -50.0f);

        // To be able to use bytemaps we first need to setup them
        // Our bytemap depends on Alpha, bytemap is represented by 1 byte per pixel, not full color.
        // It makes it more compact
        jojoTexture->setMakeBytemapAlpha(true);
        // In addition if we don't need high precision we can scale it down. 
        // For example 4 times in both dimensions reduces memory consumption 16 times
        jojoTexture->setBytemapScale(4);
        // We need to reload texture so bytemap will be created
        jojoTexture->reload();

        sprite = createComponent<ComponentSprite>();
        sprite->setTexture(jojoTexture);
    }

    void onProcess(float delta)
    {
        counter += delta;
        while (counter > 0.02f && layerActors)
        {
            // Here the magic goes
            counter -= 0.02f;
            unsigned char probe = 0;
            int randomX = 0, randomY = 0;

            // first we need to take random point inside texture, that actually has pixel.
            // So we will use this position to put a star on it. This way stars will appear only on JoJo
            while (probe < 128)
            {
                randomX = rand() % jojoTexture->getWidth();
                randomY = rand() % jojoTexture->getHeight();
                probe = jojoTexture->getProbeBytemapAlpha(randomX, randomY);
            }

            auto star = layerActors->createActor<Star>();
            // local space operates points position between 0 and 1, it's already stores texture size transofrmation
            // We need to create vector that holds 0 to 1 positions of our randoms. So just divide it on texture sizes
            Vector4 v(
                (float)randomX / (float)jojoTexture->getWidth(),
                (float)randomY / (float)jojoTexture->getHeight(),
                0.0f,
                1.0f);

            // Now we need to move the vector into local space of the component
            v = sprite->getLocalspaceMatrix() * v;

            // And then to local space of the actor
            v = (*transform.getModelMatrix()) * v;

            // That's it. v now holds the position on the actual scene and we can put our star there!
            star->transform.setPosition(v.x, v.y);
        }
    }

    static Texture *jojoTexture;
    float counter = 0.0f;
    LayerActors *layerActors = nullptr;

protected:
    ComponentSprite *sprite;
};
Texture *JoJo::jojoTexture = nullptr;

// Bytemap is an interesting way of creating effects, where you can detect existance of pixels on certain points of your actor
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
    auto view = viewController->createView("Example \"6. Hello Bytemap\"");

    // Stage setup
    auto stageController = engine->getStageController();
    auto stage = stageController->createStage("Hello Bytemap");

    // Layers and camera setup
    auto layerActors = stage->createLayerActors("Hello Bytemap", 0);
    auto camera = layerActors->createActor<CameraOrto>();
    camera->setWidthBasedResolution(1280);

    // Textures setup
    auto resourceController = engine->getResourceController();
    JoJo::jojoTexture = resourceController->addTexture("./data/jojo.png");
    Star::starTexture = resourceController->addTexture("./data/star.png");
    auto background = resourceController->addTexture("./data/background.jpg");

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