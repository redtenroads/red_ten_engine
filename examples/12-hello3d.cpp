// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

#define LIGHT_COUNT 5
#define BUTTON_SWITCH_SHADOW_QUALITY 1
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
    static ResourceMesh *towerMesh;
    static PhongShader *towerShader;
    static PhongShader *floorShader;
};
PhongShader *Town::towerShader = nullptr;
PhongShader *Town::floorShader = nullptr;
Mesh *Town::floorMesh = nullptr;
ResourceMesh *Town::towerMesh = nullptr;

class GUISimpleButton : public ActorGUIElement
{
public:
    GUISimpleButton() : ActorGUIElement()
    {
        registerName("Gui Button");
        setActiveArea(0.0f, 320.0f, -32.0f, 32.0f);

        textSprite = createComponent<ComponentText>();
        textSprite->setFont(font);
        textSprite->setText("Unknown Button");
        textSprite->setColor(200, 200, 200);
        textSprite->setAnchor(0, 0.5f);
    }

    void setText(std::string text)
    {
        textSprite->setText(text);
    }

    void onHover()
    {
        textSprite->setColor(255, 255, 255);
    }

    void onBlur()
    {
        textSprite->setColor(200, 200, 200);
    }

    static Font *font;

protected:
    ComponentText *textSprite;
    int color = 170;
};
Font *GUISimpleButton::font = nullptr;

int main()
{
    // Engine setup
    auto engine = RTEngine::createInstance("ex12cfg.ini");

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
    auto view = viewController->createView("Example \"12. Hello 3D\"");

    // Stage setup
    auto stageController = engine->getStageController();
    auto stage = stageController->createStage("Hello 3D");

    // Layers and camera setup
    auto layerActors = stage->createLayerActors("Hello 3D Layer", 0);
    auto layerGUI = stage->createLayerActors("Hello Gui Layer", 1);

    auto camera = layerActors->createActor<CameraPerspective>();
    camera->setWidthBasedResolution(1280);

    auto guiCamera = layerGUI->createActor<CameraOrto>();
    guiCamera->setWidthBasedResolution(config->getWindowWidth());

    // Resources
    auto resourceController = engine->getResourceController();
    auto concreteAlbedoTexture = resourceController->addTexture("./data/3d/concrete_albedo.jpg");
    auto concreteNormalTexture = resourceController->addTexture("./data/3d/concrete_normal.jpg");

    auto towerAlbedoTexture = resourceController->addTexture("./data/3d/tower_albedo.png");
    auto towerEmissionTexture = resourceController->addTexture("./data/3d/tower_emission.png");
    auto towerNormalTexture = resourceController->addTexture("./data/3d/tower_normal.png");

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
    plainMesh->setupFloatsArray(array, 6, 3, attributeSizes, true);

    // our tower
    auto towerMesh = resourceController->addMesh("./data/3d/tower.fbx");
    towerMesh->reload();

    Town::floorMesh = plainMesh;
    Town::towerMesh = towerMesh;

    Town::floorShader = new PhongShader();
    Town::floorShader->setTexture(TextureType::Albedo, concreteAlbedoTexture);
    Town::floorShader->setTexture(TextureType::Normal, concreteNormalTexture);

    Town::towerShader = new PhongShader();
    Town::towerShader->setTexture(TextureType::Albedo, towerAlbedoTexture);
    Town::towerShader->setTexture(TextureType::Emission, towerEmissionTexture);
    Town::towerShader->setTexture(TextureType::Normal, towerNormalTexture);

    // town
    auto town = layerActors->createActor<Town>();
    town->transform.setScale(2.0f, 2.0f, 2.0f);

    // Sun with shadow casting
    auto sun = layerActors->createActor<Actor>();
    auto sunComponent = sun->createComponent<ComponentLight>();
    sunComponent->setupSunLight(Vector3(-1.0f, 1.0f, -0.5f), Vector3(0.3f, 0.3f, 0.6f), true);

    float cameraRotation = 0.0f;
    float sunRotation = 0.0f;

    // Shadow quality switch GUI
    GUISimpleButton::font = resourceController->addFont("./data/BebasNeue-Regular.ttf", 32);
    auto newButton = layerGUI->createActor<GUISimpleButton>();
    newButton->transform.setPosition(-config->getWindowWidth() / 2 + 60, -config->getWindowHeight() / 2 + 64);
    newButton->setPressID(BUTTON_SWITCH_SHADOW_QUALITY);
    newButton->setText("Toggle shadow quality");

    auto shadowQualityActor = layerGUI->createActor<Actor>();
    shadowQualityActor->transform.setPosition(-config->getWindowWidth() / 2 + 60, -config->getWindowHeight() / 2 + 32);
    auto shadowQualityText = shadowQualityActor->createComponent<ComponentText>();
    shadowQualityText->setFont(GUISimpleButton::font);
    shadowQualityText->setText(std::string("Shadow quality: ") + Config::qualityToString(config->getShadowQuality()));
    shadowQualityText->setColor(170, 170, 170);
    shadowQualityText->setAnchor(0, 0.5f);

    // Update config file if something changed from the initial configuration during setup
    config->saveConfig();

    while (!engine->isTerminationIntended())
    {
        float delta = engine->syncFrame();
        viewController->processEvents();

        cameraRotation += delta * 0.3f;
        sunRotation += delta * 0.1f;

        const float cameraDistance = 3.6f;
        const float cameraHeight = 2.8f;
        camera->transform.setPosition(sinf(cameraRotation) * cameraDistance, cameraHeight, cosf(cameraRotation) * cameraDistance);
        camera->lookAt(0.0f, 0.0f, 0.0f);

        float effectiveLight = fmaxf(sinf(sunRotation), 0.0f);
        layerActors->setAmbientColor(0.05f + effectiveLight * 0.1f, 0.05f + effectiveLight * 0.1f, 0.09f + effectiveLight * 0.1f);
        sunComponent->setupSunLight(
            Vector3(cosf(sunRotation) + 0.5f, sinf(sunRotation), cosf(sunRotation)),
            Vector3(0.7f + (1.0f - effectiveLight) * 0.4f, 0.7f, 0.7f) * effectiveLight,
            true);

        // Controlling the shadow quality
        int firstPressID;
        while ((firstPressID = ActorGUIElement::getFirstPressID()))
        {
            if (firstPressID == BUTTON_SWITCH_SHADOW_QUALITY)
            {
                if (config->getShadowQuality() == RenderQuality::SuperFast)
                    config->setShadowQuality(RenderQuality::Fast);
                else if (config->getShadowQuality() == RenderQuality::Fast)
                    config->setShadowQuality(RenderQuality::Balanced);
                else if (config->getShadowQuality() == RenderQuality::Balanced)
                    config->setShadowQuality(RenderQuality::High);
                else if (config->getShadowQuality() == RenderQuality::High)
                    config->setShadowQuality(RenderQuality::SuperFast);

                shadowQualityText->setText(std::string("Shadow quality: ") + Config::qualityToString(config->getShadowQuality()));
                configController->applyConfig();

                // Saved configuration will be automatically loaded on next run of the engine
                // In this case it will load the quality of shadows
                config->saveConfig();
            }
        }

        stage->process(delta);
        stage->present(view);
    }
}