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

        // This is our sound player setted up for looping jojo song.
        player = createComponent<ComponentSoundPlayer>();
        player->setSound(jojoSong);
        player->playLoop();

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
    static Sound *jojoSong;

    float counter = 0.0f;
    float moveX = 0.0f;
    const float frontDistance = -600;
    LayerActors *layerActors = nullptr;

protected:
    ComponentSprite *sprite;
    ComponentSoundPlayer *player;
};
Texture *JoJo::jojoTexture = nullptr;
Sound *JoJo::jojoSong = nullptr;

int main()
{
    // Engine setup
    auto engine = RTEngine::createInstance();

    // View setup
    auto viewManager = engine->getViewManager();
    auto view = viewManager->createView("Example \"7. Hello Sound\"", 1280, 800, false);

    // Stage setup
    auto stageManager = engine->getStageManager();
    auto stage = stageManager->createStage("Hello Sound");

    // Layers and camera setup
    auto layerActors = stage->createLayerActors("Hello Sound", 0);
    auto camera = layerActors->createActor<CameraOrto>();
    camera->setWidthBasedResolution(1280);

    // Textures and sound setup
    auto resourceManager = engine->getResourceManager();
    JoJo::jojoTexture = resourceManager->addTexture("./data/jojo.png");
    auto background = resourceManager->addTexture("./data/background.jpg");

    JoJo::jojoSong = resourceManager->addSound("./data/sound.wav");
    // Note that only mono sounds can be panned. Stereo sounds always being played as is
    // But we can force mono
    JoJo::jojoSong->setForceMono(true);

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
        viewManager->processEvents();
        stage->process(delta);
        stage->present(view);
    }
}