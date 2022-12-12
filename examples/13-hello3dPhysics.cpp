// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

int main()
{
    // Engine setup
    auto engine = RTEngine::createInstance();

    // We need view controller to get resolution
    auto viewController = engine->getViewController();

    auto configController = engine->getConfigController();
    auto config = configController->getConfig();

    // Setup configuration
    config->setWindowWidth(viewController->getPrimaryScreenWidth() * 0.8f);
    config->setWindowHeight(viewController->getPrimaryScreenHeight() * 0.8f);
    config->setFullscreenState(false);

    // View setup
    auto view = viewController->createView("Example \"13. Hello 3D Physics\"");

    // Stage setup
    auto stageController = engine->getStageController();
    auto stage = stageController->createStage("Hello 3D Physics");

    // Layers and camera setup
    auto layerActors = stage->createLayerActors("Hello 3D Layer", 0);

    auto camera = layerActors->createActor<CameraPerspective>();
    camera->setWidthBasedResolution(1280);
    camera->transform.setPosition(6.0f, 6.0f, 6.0f);
    camera->lookAt(0.0f, 0.0f, 0.0f);

    // Resources
    auto resourceController = engine->getResourceController();
    auto concreteAlbedoTexture = resourceController->addTexture("./data/3d/concrete_albedo.jpg");
    auto concreteNormalTexture = resourceController->addTexture("./data/3d/concrete_normal.jpg");

    // our floor
    auto plainMesh = new Mesh();
    const float array[] = {-1, 0, -1, 0, 1, 0, 0, 0,
                           -1, 0, 1, 0, 1, 0, 0, 3,
                           1, 0, -1, 0, 1, 0, 3, 0,
                           1, 0, -1, 0, 1, 0, 3, 0,
                           -1, 0, 1, 0, 1, 0, 0, 3,
                           1, 0, 1, 0, 1, 0, 3, 3};

    // 3 - position, 3 - normal, 2 - UV
    int attributeSizes[3] = {3, 3, 2};
    plainMesh->setupFloatsArray(array, 6, 3, attributeSizes);

    // Sun with shadow casting
    auto sun = layerActors->createActor<Actor>();
    auto sunComponent = sun->createComponent<ComponentLight>();
    sunComponent->setupSunLight(Vector3(-1.0f, 1.0f, -0.5f), Vector3(0.9f, 0.9f, 0.9f), true);

    float spawnCounter = 10.0f;

    while (!engine->isTerminationIntended())
    {
        float delta = engine->syncFrame();
        viewController->processEvents();

        while (spawnCounter > 1.0f){
            spawnCounter--;
            // Spawn new obstacle
        }

        stage->process(delta);
        stage->present(view);
    }
}