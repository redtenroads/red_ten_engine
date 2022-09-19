// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "controller/shadersController.h"
#include <string>

class View
{
public:
    View(int width, int height, bool isFullscreen);

    std::string windowName = "unnamed";

    EXPORT bool makeWindow();
    EXPORT void prepare();
    EXPORT void swapBuffers();

    EXPORT int getWidth();
    EXPORT int getHeight();
    EXPORT float getHWProportion();
    EXPORT bool isFullscreen();
    EXPORT void minimize();

    EXPORT unsigned int getTexture();

    static void setShadersController(ShadersController *shadersController);

protected:
    void *window = nullptr;
    int width = 640;
    int height = 480;
    bool bIsFullscreen = false;

    unsigned int framebuffer = 0;
    unsigned int renderedTexture = 0;

    static ShadersController *shadersController;
};