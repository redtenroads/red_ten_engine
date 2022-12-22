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
        registerName("Land");

        auto landComponent = createComponent<ComponentMesh>();
        landComponent->setMesh(landMesh);
        landComponent->setShader(landShader);

        landComponent->addPhysicsGeometry(landMesh->getGeometry());
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
        registerName("Mushroom");
        mushroomComponent = createComponent<ComponentMesh>();
        mushroomComponent->setMesh(mushroomMesh);
        mushroomComponent->setShader(mushroomShader);

        mushroomComponent->addPhysicsGeometry(mushroomMesh->getGeometry());
    }

    void onProcess(float delta)
    {
        size = fminf(size + delta * 0.5f, 1.0f);
        mushroomComponent->transform.setScale(size, size, size);
        if (flyingAway)
        {
            flyingSpeed += delta;
            Vector3 v = transform.getPosition();
            transform.setPosition(v.x, v.y + flyingSpeed, v.z);
            if (v.y > 40.0f)
                destroy();
        }
    }

    void shootIt()
    {
        flyingAway = true;
    }

    float size = 0.0f;
    float flyingSpeed = 0.0f;
    ComponentMesh *mushroomComponent;
    bool flyingAway = false;

    static ResourceMesh *mushroomMesh;
    static PhongShader *mushroomShader;
};
PhongShader *Mushroom::mushroomShader = nullptr;
ResourceMesh *Mushroom::mushroomMesh = nullptr;

class Pointer : public ActorPawn
{
public:
    Pointer()
    {
        auto mousePressInput = registerButtonCallback(&Pointer::mousePress);
        mousePressInput->addMouseButtonBinding(InputTypeMouse::LEFT_BUTTON, 1.0f);

        auto mouseMoveInput = registerAxisCallback(&Pointer::mouseMove);
        mouseMoveInput->addMouseMoveBinding(1.0f);
    }

    void setup(Camera *camera)
    {
        this->camera = camera;
    }

    void mousePress(InputType type, int deviceIndex, int index, bool state)
    {
        if (state && camera)
        {
            PointWithDirection pointWithDirection = camera->screenToWorld(mouseX, mouseY);

            Vector3 v(pointWithDirection.vDirection.x * 100.0f, pointWithDirection.vDirection.y * 100.0f, pointWithDirection.vDirection.z * 100.0f);

            RayCollision r = ((LayerActors *)layer)->castSingleRayCollision(pointWithDirection.vPosition, v);

            if (r.hadHit && r.actor)
            {
                if (r.actor->implements("Land"))
                {
                    auto mushroom = ((LayerActors *)layer)->createActor<Mushroom>();
                    mushroom->transform.setPosition(r.point.x, r.point.y, r.point.z);
                    mushroom->transform.setRotation(0.0f, randf(0.0f, CONST_PI * 4.0f), 0.0f);
                }
                if (r.actor->implements("Mushroom"))
                {
                    ((Mushroom *)r.actor)->shootIt();
                }
            }
        }
    }

    void mouseMove(InputType type, int deviceIndex, int index, float state)
    {
        if (index == (int)InputTypeMouseMove::MOVE_HORIZONTAL)
            mouseX = state;
        if (index == (int)InputTypeMouseMove::MOVE_VERTICAL)
            mouseY = state;
    }

    Camera *camera = nullptr;
    float mouseX = 0.0f, mouseY = 0.0f;
};

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
    layerActors->createActor<Land>();

    // Sun with shadow casting
    auto sun = layerActors->createActor<Actor>();
    auto sunComponent = sun->createComponent<ComponentLight>();
    sunComponent->setupSunLight(Vector3(-1.0f, 2.0f, 1.0f), Vector3(0.9f, 0.9f, 0.9f), true);

    // Pointer to receive input and apply actions
    auto pointer = layerActors->createActor<Pointer>();
    pointer->setup(camera);

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
            if (collision.actor->implements("Land"))
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