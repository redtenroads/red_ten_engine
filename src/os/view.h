// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/renderer.h"
#include "controller/logController.h"
#include <string>

class View
{
public:
    View(Config *config);

    bool makeWindow();
    bool changeMode();

    EXPORT void swapBuffers();

    EXPORT int getWidth();
    EXPORT int getHeight();
    EXPORT int getRefreshRate();
    EXPORT float getHWProportion();
    EXPORT bool isFullscreen();
    EXPORT const char *getOGLVersion();
    EXPORT const char *getVersion();

    EXPORT void minimize();

    EXPORT unsigned int getTexture();
    EXPORT Renderer *getRenderer();
    EXPORT void useFrameBuffer();

    std::string windowName = "unnamed";

protected:
    void updateSuitableDisplayMode();
    void updateFrameBuffer();

    void *window = nullptr;
    int width = 640;
    int height = 480;
    int refreshRate = 0;
    int displayMode = -1;
    bool bIsFullscreen = false;

    const char *oglVersion = nullptr;
    const char *version = nullptr;

    unsigned int framebuffer = 0;
    unsigned int renderedTexture = 0;
    Renderer *renderer = nullptr;
    Config *config = nullptr;
};