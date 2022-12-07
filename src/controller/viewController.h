// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/withLogger.h"
#include "common/config.h"
#include "os/view.h"
#include "controller/inputController.h"
#include <list>

struct DisplayMode
{
    int width;
    int height;
    int index;
};

struct GamepadDevice
{
    int index;
    void *gamePad;
};

class ViewController : public WithLogger
{
public:
    ViewController(Config *config);

    EXPORT View *createViewFullscreen(std::string name);
    EXPORT View *createView(std::string name, int resX = 0, int resY = 0, bool isFullscreen = false, int refreshRate = 0);
    EXPORT View *createViewUsingConfig(std::string name);

    EXPORT void getAvailableResolutions(std::vector<DisplayMode> *modes, bool onlyNative = true);
    EXPORT void getAvailableRefreshRates(DisplayMode *mode, std::vector<int> *refreshRates);

    EXPORT bool isResolutionAvailable(int width, int height, int refreshRate = 0);
    EXPORT bool isResolutionAvailable(DisplayMode &mode, int refreshRate);

    EXPORT void processEvents();
    EXPORT bool getIsExitIntended();

    EXPORT static void setInputController(InputController *inputController);

    EXPORT int getPrimaryScreenWidth();
    EXPORT int getPrimaryScreenHeight();

protected:
    std::list<View *> views;
    std::vector<GamepadDevice> gamePads;
    bool isExitIntended = false;
    Config *config = nullptr;
    static InputController *inputController;
};
