#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

class Town : public Actor
{
public:
    Town()
    {
        auto floorComponent = createComponent<ComponentMesh>();
        floorComponent->setMesh(floorMesh);
        floorComponent->setTexture(concreteTexture);
        floorComponent->transform.setScale(2.0f, 2.0f, 2.0f);

        auto towerComponent = createComponent<ComponentMesh>();
        towerComponent->setMesh(towerMesh);
        towerComponent->setTexture(towerTexture);
        towerComponent->transform.setScale(0.2f, 0.2f, 0.2f);
    }
    void onProcess(float delta)
    {
        transform.rotate(Vector3(0, 1.0f * delta, 0));
    }

    static Texture *concreteTexture;
    static Texture *towerTexture;
    static Mesh *floorMesh;
    static Mesh *towerMesh;
};
Texture *Town::concreteTexture = nullptr;
Texture *Town::towerTexture = nullptr;
Mesh *Town::floorMesh = nullptr;
Mesh *Town::towerMesh = nullptr;

int main()
{
    // Engine setup
    auto engine = RTEngine::createInstance();

    // View setup
    auto viewController = engine->getViewController();
    auto view = viewController->createView("Example \"12. Hello 3D\"", 1280, 800, false);

    // Stage setup
    auto stageController = engine->getStageController();
    auto stage = stageController->createStage("Hello 3D");

    // Layers and camera setup
    auto layerActors = stage->createLayerActors("Hello 3D Layer", 0);
    auto camera = layerActors->createActor<CameraPerspective>();
    camera->setWidthBasedResolution(1280);
    camera->transform.setPosition(0, -4.0f, 0);
    camera->transform.setRotation(0.6, 0, 0);

    // Resources
    auto resourceController = engine->getResourceController();
    Town::concreteTexture = resourceController->addTexture("./data/3d/concrete.jpg");
    Town::towerTexture = resourceController->addTexture("./data/3d/tower_defuse.png");
    auto plainMesh = resourceController->addMesh();

    auto towerMesh = resourceController->addMesh("./data/3d/tower.fbx");

    towerMesh->reload();

    const float array[] = {-1, 0, -1, 0, 1, 0, 0, 0,
                           -1, 0, 1, 0, 1, 0, 0, 3,
                           1, 0, -1, 0, 1, 0, 3, 0,
                           1, 0, -1, 0, 1, 0, 3, 0,
                           -1, 0, 1, 0, 1, 0, 0, 3,
                           1, 0, 1, 0, 1, 0, 3, 3};

    plainMesh->setupByArray8f(array, 8 * 6);
    Town::floorMesh = plainMesh;
    Town::towerMesh = towerMesh;

    auto town = layerActors->createActor<Town>();
    town->transform.setPosition(0, 0, -6);
    town->transform.setScale(2.0f, 2.0f, 2.0f);

    while (!engine->isTerminationIntended())
    {
        float delta = engine->syncFrame();
        viewController->processEvents();
        stage->process(delta);
        stage->present(view);
    }
}