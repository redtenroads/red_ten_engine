// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

class Land : public Actor
{
public:
    Land()
    {
        auto landComponent = createComponent<ComponentMesh>();
        landComponent->setMesh(landMesh);
        landComponent->setShader(landShader);

        landComponent->addPhysicsGeometry(landMesh->getGeometry());
        updatePhysics();
    }

    static ResourceMesh *landMesh;
    static PhongShader *landShader;
};
PhongShader *Land::landShader = nullptr;
ResourceMesh *Land::landMesh = nullptr;

class Mushroom : public Actor
{
public:
    Mushroom()
    {
        mushroomComponent = createComponent<ComponentMesh>();
        mushroomComponent->setMesh(mushroomMesh);
        mushroomComponent->setShader(mushroomShader);
        mushroomComponent->transform.setScale(0.15f, 0.15f, 0.15f);

        mushroomComponent->addPhysicsGeometry(mushroomMesh->getGeometry());
        updatePhysics();
    }

    void onProcess(float delta)
    {
        size = fminf(size + delta * 0.5f, 1.0f);
        float fullSize = size * s;
        mushroomComponent->transform.setScale(fullSize, fullSize, fullSize);
    }

    const float s = 0.15f;
    float size = 0.0f;
    ComponentMesh *mushroomComponent;

    static ResourceMesh *mushroomMesh;
    static PhongShader *mushroomShader;
};
PhongShader *Mushroom::mushroomShader = nullptr;
ResourceMesh *Mushroom::mushroomMesh = nullptr;

int main()
{
    // Engine setup
    auto engine = RTEngine::createInstance();

    // We need view controller to get resolution
    auto viewController = engine->getViewController();

    // Set fullscreen through configuration controller
    auto configController = engine->getConfigController();
    auto config = configController->getConfig();
    if (!config->isLoaded())
    {
        // If configuration is new we setup high quality in window
        config->setupByQuality(RenderQuality::High);
        config->setWindowWidth(viewController->getPrimaryScreenWidth() * 0.8f);
        config->setWindowHeight(viewController->getPrimaryScreenHeight() * 0.8f);
        config->setFullscreenState(false);
    }

    // View setup
    auto view = viewController->createView("Example \"14. Hello Mushroom\"");

    // Stage setup
    auto stageController = engine->getStageController();
    auto stage = stageController->createStage("Hello Mushroom");

    // Layers and camera setup
    auto layerActors = stage->createLayerActors("Hello Mushroom", 0);
    layerActors->enablePhisics(Vector3(0.0f, 0.0f, 0.0f));

    auto camera = layerActors->createActor<CameraPerspective>();
    camera->setWidthBasedResolution(1280);
    camera->transform.setPosition(6.0f, 10.0f, 6.0f);
    camera->lookAt(0.0f, -2.0f, 0.0f);

    // Resources
    auto resourceController = engine->getResourceController();
    auto landAlbedoTexture = resourceController->addTexture("./data/3d/land_albedo.jpg");
    auto landNormalTexture = resourceController->addTexture("./data/3d/land_normal.jpg");

    auto mushroomAlbedoTexture = resourceController->addTexture("./data/3d/mushroom_albedo.jpg");

    // our tower
    auto landMesh = resourceController->addMesh("./data/3d/land.fbx");
    landMesh->reload();
    auto mushroomMesh = resourceController->addMesh("./data/3d/mushroom.fbx");
    mushroomMesh->reload();

    Land::landMesh = landMesh;
    Land::landShader = new PhongShader();
    Land::landShader->setTexture(TextureType::Albedo, landAlbedoTexture);
    Land::landShader->setTexture(TextureType::Normal, landNormalTexture);

    Mushroom::mushroomMesh = mushroomMesh;
    Mushroom::mushroomShader = new PhongShader();
    Mushroom::mushroomShader->setTexture(TextureType::Albedo, mushroomAlbedoTexture);

    // land
    auto land = layerActors->createActor<Land>();
    // land->transform.setScale(1.0f, 1.0f, 1.0f);

    // Sun with shadow casting
    auto sun = layerActors->createActor<Actor>();
    auto sunComponent = sun->createComponent<ComponentLight>();
    sunComponent->setupSunLight(Vector3(-1.0f, 2.0f, 1.0f), Vector3(0.9f, 0.9f, 0.9f), true);

    int created = 0;
    while (created < 40)
    {
        float radius = randf(1.0f, 10.0f);
        float angle = randf(0.0f, CONST_PI * 4.0f);
        float x = sin(angle) * radius;
        float z = cos(angle) * radius;

        auto collision = layerActors->castSingleRayCollision(Vector3(x, 10.0f, z), Vector3(0, -20.0f, 0));
        if (collision.hadHit)
        {
            if (collision.actor == land)
            {
                auto mushroom = layerActors->createActor<Mushroom>();
                mushroom->transform.setPosition(x, collision.point.y, z);
                mushroom->transform.setRotation(0.0f, randf(0.0f, CONST_PI * 4.0f), 0.0f);
            }
        }
        created++;
    }

    while (!engine->isTerminationIntended())
    {
        float delta = engine->syncFrame();
        viewController->processEvents();

        stage->process(delta);
        stage->present(view);
    }
}