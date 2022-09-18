#include "../src/rtengine.h"
#pragma comment(lib, "bin/rtengine.lib")

int main()
{
    // Engine instance is the source of managers and settings of the engine
    auto engine = RTEngine::createInstance();

    // Provides ability to create views, also stores them and update
    // 0 width and height means that with fullscreen (last parameter) it will take the maximum resolution
    auto viewManager = engine->getViewManager();
    auto view = viewManager->createView("Example \"1. Hello World\"", 0, 0, true);

    // Stage is where your game actores are presented
    auto stageManager = engine->getStageManager();
    auto stage = stageManager->createStage("Hello World");
    stage->setClearColor(0, 0, 0);

    // Layer with actors contain sprites and camera to view them
    // To make 2d games you should use ortographic camera that has no perspective distorsion
    // Second parameter "index" is being used to sort the layers
    auto layerActors = stage->createLayerActors("Hello Actors", 0);
    auto camera = layerActors->createActor<CameraOrto>();
    // There are physical and logical resolutions exist in the engine.
    // Physical is the width and height of your view in pixels.
    // Logical is the when you use to draw. If you set height based resolution of 640 your logical screen height will always be 640
    // But width will be calculated depending on the view size. You also can base logical resolution on width if prefer
    camera->setHeightBasedResolution(640);

    // To put sprite onto our stage layer we first need to load image for this sprite
    auto resourceManager = engine->getResourceManager();
    auto helloTexture = resourceManager->addTexture("./data/hello.png");

    // First we need to create Actor. Actor is representation of some affecting environment entity in the scene
    auto sprite = layerActors->createActor<Actor>();

    // Entity without components rendering nothing. Components are the bridges between actor and various APIs
    // ComponentSprite is rendering texture on the screen
    auto component = sprite->createComponent<ComponentSprite>();
    component->setTexture(helloTexture);
    // We want to put picture on the bottom of the screen and to avoid calculating shift depending on the size of the texture
    // We can simply move anchor drawing point from center to bottom 
    // (second parameter is vertical axis Y, first is horizontal - sprite will stay in the center)
    component->setAnchor(0.5f, 0.0f);

    // Now we will set position of actor in the center-bottom of the screen. Our screen height is alaways 640,
    // but 0-0 position is actually the center of the screen, not the upper left corner as in many game engines redners in 2d space
    // So the bottom will be height / -2, bottom of Y axis is negative
    sprite->transform.setPosition(0.0f, -320.0f);

    while (!engine->isTerminationIntended())
    {
        // Sync frame calculates the delta between frames
        // You can use your own counter if you prefer, it wont break anything
        float delta = engine->syncFrame();
        // Process operating system events coming to all created windows
        viewManager->processEvents();
        // Processes all layers, actors and physics if enabled
        stage->process(delta);
        // Renders all actor components on all layers
        stage->present(view);
    }
}