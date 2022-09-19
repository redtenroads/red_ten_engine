#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

// Actor just draws ball or crate depending on how it was setted up and moves it in circle on every update
class Figure : public Actor
{
public:
    void setup(float circlePosition, bool useBall)
    {
        this->circlePosition = circlePosition;
        transform.setScale(0.5f);
        auto sprite = createComponent<ComponentSprite>();
        sprite->setTexture(useBall ? ballTexture : crateTexture);
    }

    void onProcess(float delta)
    {
        circlePosition += delta;
        float x = sinf(circlePosition) * 500.0f;
        float y = cosf(circlePosition) * 200.0f;
        transform.setPosition(x, y);

        // This parameter determines how the actor will be sorted on the screen before rendering
        zDepth = (int)(-y * 2.0f);
    }

    static Texture *ballTexture;
    static Texture *crateTexture;

protected:
    float circlePosition;
};
Texture *Figure::ballTexture = nullptr;
Texture *Figure::crateTexture = nullptr;

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

    // Ortographic camera in layer disables Z test in OpenGL. It means Z axis doesn't effect which sprite is on the top
    // But you can still sort sprites by enabling manual sort. This way actors will be sorted depending on their inner zIndex before rendering
    auto layerActors = stage->createLayerActors("Hello Actors", 0);
    layerActors->enableSorting();

    auto camera = layerActors->createActor<CameraOrto>();
    camera->setWidthBasedResolution(1280);

    // Textures setup
    auto resourceController = engine->getResourceController();
    Figure::ballTexture = resourceController->addTexture("./data/plate.png");
    Figure::crateTexture = resourceController->addTexture("./data/crate.jpg");

    int figuresAmount = 20;
    float period = (CONST_PI * 2.0f) / (float)figuresAmount;
    for (int i = 0; i < figuresAmount; i++)
    {
        auto figure = layerActors->createActor<Figure>();
        figure->setup((float)i * period, (i % 2) == 0);
    }

    while (!engine->isTerminationIntended())
    {
        float delta = engine->syncFrame();
        viewController->processEvents();
        stage->process(delta);
        stage->present(view);
    }
}