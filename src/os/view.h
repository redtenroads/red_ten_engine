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
    View(int width, int height, int refreshRate, bool isFullscreen);

    std::string windowName = "unnamed";

    EXPORT bool makeWindow();
    EXPORT bool changeMode(int width, int height, int refreshRate, bool isFullscreen);
    EXPORT void swapBuffers();

    EXPORT int getWidth();
    EXPORT int getHeight();
    EXPORT float getHWProportion();
    EXPORT bool isFullscreen();
    EXPORT const char *getOGLVersion();
    EXPORT const char *getVersion();

    EXPORT void minimize();

    EXPORT unsigned int getTexture();
    EXPORT Renderer *getRenderer();
    EXPORT void useFrameBuffer();

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
};