// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "os/view.h"
#include "opengl/glew.h"
#include "opengl/wglew.h"
#include "common/commonShaders.h"
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <SDL.h>

View::View(int width, int height, bool isFullscreen)
{
    this->width = width;
    this->height = height;
    this->bIsFullscreen = isFullscreen;
}

bool View::makeWindow()
{
    if (window)
        return false;

    auto newWindow = SDL_CreateWindow(windowName.c_str(),
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      width, height,
                                      (bIsFullscreen ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

    if (newWindow)
    {
        SDL_GL_CreateContext(newWindow);
        glewInit();

        CommonShaders::build();

        window = newWindow;

        const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
        const GLubyte *version = glGetString(GL_VERSION);   // version as a string
        printf("Renderer: %s\n", renderer ? (char *)renderer : "no renderer");
        printf("OpenGL version supported %s\n", version ? (char *)version : "no version");

        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        // Color texture
        glGenTextures(1, &renderedTexture);
        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Depth buffer
        glGenRenderbuffers(1, &depthbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        return true;
    }

    window = nullptr;
    return false;
}

void View::prepare()
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
}

void View::swapBuffers()
{
    SDL_GL_SwapWindow((SDL_Window *)window);
}

int View::getWidth()
{
    return width;
}

int View::getHeight()
{
    return height;
}

float View::getHWProportion()
{
    return ((float)height) / ((float)width);
}

bool View::isFullscreen()
{
    return bIsFullscreen;
}

void View::minimize()
{
    if (window)
        SDL_MinimizeWindow((SDL_Window *)window);
}

unsigned int View::getTexture()
{
    return renderedTexture;
}
