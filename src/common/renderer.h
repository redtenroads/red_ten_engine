// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once

class Renderer
{
public:
    Renderer(int width, int height);

    unsigned int getPositionTexture();
    unsigned int getNormalTexture();
    unsigned int getAlbedoTexture();
    unsigned int getLightningTexture();

    void setupNewFrame();
    void setupLightning();
    //void setupLithtning(float *ambientColor);

protected:
    unsigned int gBuffer, depthbuffer;
    unsigned int gPosition, gNormal, gAlbedoSpec;
    
    unsigned int lightningBuffer;
    unsigned int lightningPicture;

    int width, height;
};