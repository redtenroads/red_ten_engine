#pragma once
#include "utils.h"
#include "view.h"
#include "inputManager.h"
#include <list>

struct GamepadDevice
{
    int index;
    void *gamePad;
};

class ViewManager
{
public:
    ViewManager();

    EXPORT View *createViewFullscreen(std::string name);
    EXPORT View *createView(std::string name, int resX, int resY, bool isFullscreen);

    EXPORT void processEvents();
    EXPORT bool getIsExitIntended();

    EXPORT static void setInputManager(InputManager *inputManager);

    EXPORT int getPrimaryScreenWidth();
    EXPORT int getPrimaryScreenHeight();

protected:
    std::list<View *> views;
    std::vector<GamepadDevice> gamePads;
    bool isExitIntended = false;
    static InputManager *inputManager;
};
