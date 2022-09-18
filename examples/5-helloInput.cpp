#include "../src/rtengine.h"
#include <math.h>
#pragma comment(lib, "bin/rtengine.lib")

// What a game without controlling your character? The way to make actor receive input is to use ActorPawn
class CratePlayer : public ActorPawn
{
public:
    CratePlayer() : ActorPawn()
    {
        registerName("CratePlayer");

        setPhysicsMotionType(MotionType::Dynamic);
        transform.setScale(0.5f);
        sprite = createComponent<ComponentSprite>();
        sprite->setTexture(crateTexture);
        sprite->addPhysics2dBox(256.0f, 256.0f);
        setFrictionAndRestitution(0.9f, 0.1f);

        setZAxisLocked(true);
        // This way in 2d space you can block rotation of the object
        setZAxisRotationLocked(true);

        // First we create an input type with function to receive the input itself and make an action base on it
        auto inputX = registerAxisCallback(&CratePlayer::controlX);
        // Second we will add the dependences of this input type around your gamepad, mouse, keyboard and other input devices
        // Second parameter is a multiplier of device input making it here in range from -400 to 400
        // Note that keyboard usually have input of 0 to 1, but axises have input from -1 to 1.
        // Also some gamepad axises may have -1 in their rest position, like triggers
        inputX->addInputBinding(InputType::KEYBOARD, 7, 400.0f);
        inputX->addInputBinding(InputType::KEYBOARD, 4, -400.0f);
        inputX->addInputBinding(InputType::KEYBOARD, 79, 400.0f);
        inputX->addInputBinding(InputType::KEYBOARD, 80, -400.0f);
        inputX->addInputBinding(InputType::GAMEPAD_AXIS, 0, 400.0f);

        auto inputY = registerAxisCallback(&CratePlayer::controlY);
        inputY->addInputBinding(InputType::KEYBOARD, 22, -400.0f);
        inputY->addInputBinding(InputType::KEYBOARD, 26, 400.0f);
        inputY->addInputBinding(InputType::KEYBOARD, 81, -400.0f);
        inputY->addInputBinding(InputType::KEYBOARD, 82, 400.0f);
        inputY->addInputBinding(InputType::GAMEPAD_AXIS, 1, -400.0f);

        auto inputHide = registerButtonCallback(&CratePlayer::hideSurrounding);
        inputHide->addInputBinding(InputType::KEYBOARD, 11, 1.0f);
        inputHide->addInputBinding(InputType::GAMEPAD_BUTTON, 0, 1.0f);

        // It's not nessary to provide exact source of input, you can simply register all inputs for some action.
        // It's could be usefull for detection of button ids or to know which input method user prefers
        // But you still need to provide multiplier
        auto inputAll = registerAxisCallback(&CratePlayer::all);
        inputAll->addInputBinding(1.0f);
    }

    void onProcess(float delta)
    {
        transform.translate(Vector2(moveX * delta, moveY * delta));
        transform.setRotation(0);
    }

    void controlX(InputType type, int deviceIndex, int index, float axis)
    {
        moveX = axis;
    }

    void controlY(InputType type, int deviceIndex, int index, float axis)
    {
        moveY = axis;
    }

    void hideSurrounding(InputType type, int deviceIndex, int index, bool state)
    {
        auto layerActors = getLayerActorsList();
        for (auto it = layerActors->begin(); it != layerActors->end(); it++)
            if ((*it)->is("HiddenCrate"))
                (*it)->setVisible(state);
    }

    void all(InputType type, int deviceIndex, int index, float state)
    {
        std::string str = "Input ";
        if (type == InputType::MOUSE)
            str += std::string("from mouse, ");
        if (type == InputType::KEYBOARD)
            str += std::string("from keyboard, ");
        if (type == InputType::GAMEPAD_BUTTON)
            str += std::string("from gamepad button, ");
        if (type == InputType::GAMEPAD_AXIS)
            str += std::string("from gamepad axis, ");

        str += std::string("code: ") + std::to_string(index);
        str += std::string(", value: ") + std::to_string(state);
        printf("%s\n", str.c_str());
    }

    static Texture *crateTexture;
    float moveX = 0.0f, moveY = 0.0f;

protected:
    ComponentSprite *sprite;
};
Texture *CratePlayer::crateTexture = nullptr;

class HiddenCrate : public Actor
{
public:
    HiddenCrate() : Actor()
    {
        registerName("HiddenCrate");
        transform.setScale(0.5f);
        sprite = createComponent<ComponentSprite>();
        sprite->setTexture(crateTexture);
        setZAxisLocked(true);
        setFrictionAndRestitution(0.9f, 0.1f);
        setVisible(false);
    }

    void setup(float x, float y)
    {
        transform.setPosition(x, y);
        sprite->addPhysics2dBox(256.0f, 256.0f);
        updatePhysics();
    }

    static Texture *crateTexture;

protected:
    ComponentSprite *sprite;
};
Texture *HiddenCrate::crateTexture = nullptr;

class Wall : public Actor
{

public:
    Wall() : Actor()
    {
        registerName("Wall");
        setPhysicsMotionType(MotionType::Static);
        setZAxisLocked(true);
    }

    void setup(float x, float y, float width, float height)
    {
        transform.setPosition(x, y);
        auto emptyComponent = createComponent<ComponentSprite>();
        emptyComponent->addPhysics2dBox(width, height);
        setFrictionAndRestitution(1.0f, 0.1f);
        updatePhysics();
    }
};

int main()
{
    // Engine setup
    auto engine = RTEngine::createInstance();

    // View setup
    auto viewManager = engine->getViewManager();
    auto view = viewManager->createView("Example \"3. Hello Physics\"", 1280, 800, false);

    // Stage setup
    auto stageManager = engine->getStageManager();
    auto stage = stageManager->createStage("Hello Physics");

    // Layers and camera setup
    auto layerActors = stage->createLayerActors("Hello Physics", 0);
    auto camera = layerActors->createActor<CameraOrto>();
    camera->setWidthBasedResolution(1280);

    layerActors->enablePhisics(Vector3(0.0f, 0.0f, 0.0f));

    // Textures setup
    auto resourceManager = engine->getResourceManager();
    CratePlayer::crateTexture = resourceManager->addTexture("./data/crate.jpg");
    HiddenCrate::crateTexture = CratePlayer::crateTexture;

    auto background = resourceManager->addTexture("./data/background.jpg");

    // Just a background for better look
    auto backgroundActor = layerActors->createActor<Actor>();
    backgroundActor->transform.setScale(2.0f);
    auto backgroundActorSprite = backgroundActor->createComponent<ComponentSprite>();
    backgroundActorSprite->setTexture(background);

    Wall *bottomWall = layerActors->createActor<Wall>();
    bottomWall->setup(0, 400.0f, 1280.0f, 60.0f);

    Wall *topWall = layerActors->createActor<Wall>();
    topWall->setup(0, -400.0f, 1280.0f, 60.0f);

    Wall *leftWall = layerActors->createActor<Wall>();
    leftWall->setup(-640.0f, 0.0f, 60.0f, 800.0f);

    Wall *rightWall = layerActors->createActor<Wall>();
    rightWall->setup(640.0f, 0.0f, 60.0f, 800.0f);

    auto crate1 = layerActors->createActor<HiddenCrate>();
    crate1->setup(-200.0f, -200.0f);

    auto crate2 = layerActors->createActor<HiddenCrate>();
    crate2->setup(200.0f, -200.0f);

    auto crate3 = layerActors->createActor<HiddenCrate>();
    crate3->setup(-200.0f, 200.0f);

    auto crate4 = layerActors->createActor<HiddenCrate>();
    crate4->setup(200.0f, 200.0f);

    layerActors->createActor<CratePlayer>();

    while (!engine->isTerminationIntended())
    {
        float delta = engine->syncFrame();
        viewManager->processEvents();
        stage->process(delta);
        stage->present(view);
    }
}