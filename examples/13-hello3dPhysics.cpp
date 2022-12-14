// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

class ActorFactory
{
public:
    ActorFactory(ResourceController *resourceController, LayerActors *layer)
    {
        this->layer = layer;
        sphereMesh = resourceController->addMesh("./data/3d/concrete_ball.fbx");
        platformMesh = resourceController->addMesh("./data/3d/platform.fbx");

        auto sphereAlbedoTexture = resourceController->addTexture("./data/3d/concrete_ball_albedo.jpg");
        auto sphereNormalTexture = resourceController->addTexture("./data/3d/concrete_ball_normal.jpg");

        auto platformAlbedoTexture = resourceController->addTexture("./data/3d/platform_albedo.jpg");

        sphereShader = new PhongShader();
        sphereShader->setTexture(TextureType::Albedo, sphereAlbedoTexture);
        sphereShader->setTexture(TextureType::Normal, sphereNormalTexture);

        platformShader = new PhongShader();
        platformShader->setTexture(TextureType::Albedo, platformAlbedoTexture);
    }

    Actor *spawnSphere(float x, float y, float z)
    {
        Actor *sphere = layer->createActor<Actor>();

        auto sphereComponent = sphere->createComponent<ComponentMesh>();
        sphereComponent->setMesh(sphereMesh);
        sphereComponent->setShader(sphereShader);
        sphereComponent->addPhysicsSphere(0.4f);

        sphere->setFrictionAndRestitution(0.8f, 0.1f);
        sphere->setPhysicsMotionType(MotionType::Dynamic);
        sphere->transform.setPosition(x, y, z);

        sphere->updatePhysics();

        return sphere;
    }

    Actor *spawnPlatform(float x, float y, float z)
    {
        Actor *platform = layer->createActor<Actor>();

        auto platformComponent = platform->createComponent<ComponentMesh>();
        platformComponent->setMesh(platformMesh);
        platformComponent->setShader(platformShader);
        platformComponent->addPhysics3dBox(8.0f, 0.2f, 8.0f);

        platform->setFrictionAndRestitution(0.6f, 0.1f);
        platform->setPhysicsMotionType(MotionType::Static);
        platform->transform.setPosition(x, y, z);

        platform->updatePhysics();

        return platform;
    }

protected:
    LayerActors *layer = nullptr;
    PhongShader *sphereShader = nullptr;
    PhongShader *platformShader = nullptr;
    Mesh *sphereMesh = nullptr;
    Mesh *platformMesh = nullptr;
};

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
    // Also enabling physics for the layer
    auto layerActors = stage->createLayerActors("Hello 3D Layer", 0);
    layerActors->enablePhisics(Vector3(0.0f, -5.0f, 0.0f));

    auto camera = layerActors->createActor<CameraPerspective>();
    camera->setWidthBasedResolution(1280);
    camera->transform.setPosition(2.0f, 8.0f, 8.0f);
    camera->lookAt(0.0f, -1.0f, 0.0f);

    // This factory is able to produce actors we need
    ActorFactory *actorFactory = new ActorFactory(engine->getResourceController(), layerActors);
    actorFactory->spawnPlatform(0.0f, 0.0f, 0.0f);

    // Sun with shadow casting
    auto sun = layerActors->createActor<Actor>();
    auto sunComponent = sun->createComponent<ComponentLight>();
    sunComponent->setupSunLight(Vector3(-1.0f, 1.0f, 1.0f), Vector3(0.9f, 0.9f, 0.9f), true);

    float spawnCounter = 5.0f;

    while (!engine->isTerminationIntended())
    {
        float delta = engine->syncFrame();
        viewController->processEvents();

        spawnCounter += delta * 5.0f;
        while (spawnCounter > 1.0f)
        {
            spawnCounter -= 1.0f;
            // Spawn new sphere
            actorFactory->spawnSphere(randf(-1.0f, 1.0f), randf(6.0f, 9.0f), randf(-1.0f, 1.0f));
        }

        stage->process(delta);
        stage->present(view);
    }
}