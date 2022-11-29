// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

#define LIGHT_COUNT 5

class Town : public Actor
{
public:
    Town()
    {
        auto floorComponent = createComponent<ComponentMesh>();
        floorComponent->setMesh(floorMesh);
        floorComponent->setShader(floorShader);
        floorComponent->transform.setScale(2.5f, 2.5f, 2.5f);

        auto towerComponent = createComponent<ComponentMesh>();
        towerComponent->setMesh(towerMesh);
        towerComponent->setShader(towerShader);
        towerComponent->transform.setScale(0.2f, 0.2f, 0.2f);

        towerComponent = createComponent<ComponentMesh>();
        towerComponent->setMesh(towerMesh);
        towerComponent->setShader(towerShader);
        towerComponent->transform.setScale(0.2f, 0.2f, 0.2f);
        towerComponent->transform.setPosition(0.0f, 0.0f, 0.8f);
        towerComponent->transform.setRotation(0.0f, CONST_PI, 0.0f);

        towerComponent = createComponent<ComponentMesh>();
        towerComponent->setMesh(towerMesh);
        towerComponent->setShader(towerShader);
        towerComponent->transform.setScale(0.2f, 0.2f, 0.2f);
        towerComponent->transform.setPosition(0.0f, 0.0f, -0.8f);
        towerComponent->transform.setRotation(0.0f, CONST_PI, 0.0f);

        for (int i = 0; i < LIGHT_COUNT; i++)
        {
            light[i] = createComponent<ComponentLight>();
            light[i]->setupOmniLight(0.2f + randf(0.0f, 0.1f), Vector3(0.3f + randf(0.0f, 0.7f), 0.3f + randf(0.0f, 0.7f), 0.3f + randf(0.0f, 0.7f)));
        }
    }

    void onProcess(float delta)
    {
        counter += delta * 0.3f;
        transform.rotate(Vector3(0, 0.4f * delta, 0));

        float step = (CONST_PI * 2.0f) / (float)LIGHT_COUNT;
        for (int i = 0; i < LIGHT_COUNT; i++)
        {
            float rotation = (float)i * step + counter;
            light[i]->transform.setPosition(Vector3(sinf(rotation) * 1.2f, 0.10f, cosf(rotation) * 1.2f));
        }
    }

    float counter = 0.0f;
    ComponentLight *light[LIGHT_COUNT];

    static Mesh *floorMesh;
    static Mesh *towerMesh;
    static PhongShader *towerShader;
    static PhongShader *floorShader;
};
PhongShader *Town::towerShader = nullptr;
PhongShader *Town::floorShader = nullptr;
Mesh *Town::floorMesh = nullptr;
Mesh *Town::towerMesh = nullptr;

int main()
{
    // Engine setup
    auto engine = RTEngine::createInstance();

    // View setup
    auto viewController = engine->getViewController();
    auto view = viewController->createView("Example \"12. Hello 3D\"", 1920, 1080, false);

    // Stage setup
    auto stageController = engine->getStageController();
    auto stage = stageController->createStage("Hello 3D");

    // Layers and camera setup
    auto layerActors = stage->createLayerActors("Hello 3D Layer", 0);
    layerActors->setAmbientColor(0.05f, 0.05f, 0.09f);

    auto camera = layerActors->createActor<CameraPerspective>();
    camera->setWidthBasedResolution(1280);
    camera->transform.setPosition(0, -6.0f, 0);
    camera->transform.setRotation(0.7, 0, 0);

    // Resources
    auto resourceController = engine->getResourceController();
    auto concreteTexture = resourceController->addTexture("./data/3d/concrete.jpg");
    auto towerTexture = resourceController->addTexture("./data/3d/tower_defuse.png");
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

    Town::floorShader = new PhongShader();
    Town::floorShader->setTexture(TextureType::Albedo, concreteTexture);

    Town::towerShader = new PhongShader();
    Town::towerShader->setTexture(TextureType::Albedo, towerTexture);

    // town
    auto town = layerActors->createActor<Town>();
    town->transform.setPosition(0.0f, 0.0f, -9.0f);
    town->transform.setScale(2.0f, 2.0f, 2.0f);

    // Sun with shadow casting
    auto sun = layerActors->createActor<Actor>();
    auto sunComponent = sun->createComponent<ComponentLight>();
    sunComponent->setupSunLight(Vector3(-1.0f, 1.0f, -0.5f), Vector3(0.3f, 0.3f, 0.6f), true);

    while (!engine->isTerminationIntended())
    {
        float delta = engine->syncFrame();
        viewController->processEvents();
        stage->process(delta);
        stage->present(view);
    }
}