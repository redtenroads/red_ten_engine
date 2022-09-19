// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "os/view.h"
#include "controller/inputController.h"
#include <list>

struct GamepadDevice
{
    int index;
    void *gamePad;
};

class ViewController
{
public:
    ViewController();

    EXPORT View *createViewFullscreen(std::string name);
    EXPORT View *createView(std::string name, int resX, int resY, bool isFullscreen);

    EXPORT void processEvents();
    EXPORT bool getIsExitIntended();

    EXPORT static void setInputController(InputController *inputController);

    EXPORT int getPrimaryScreenWidth();
    EXPORT int getPrimaryScreenHeight();

protected:
    std::list<View *> views;
    std::vector<GamepadDevice> gamePads;
    bool isExitIntended = false;
    static InputController *inputController;
};
