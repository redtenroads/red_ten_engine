#pragma once
#include "utils.h"
#include "shadersManager.h"
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

    static void setShadersManager(ShadersManager *shadersManager);

protected:
    void *window = nullptr;
    int width = 640;
    int height = 480;
    bool bIsFullscreen = false;

    unsigned int framebuffer = 0;
    unsigned int renderedTexture = 0;

    static ShadersManager *shadersManager;
};