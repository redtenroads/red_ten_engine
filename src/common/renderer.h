// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once

class Renderer
{
public:
    Renderer(int width, int height);
    ~Renderer();

    unsigned int getPositionTexture();
    unsigned int getNormalTexture();
    unsigned int getAlbedoTexture();
    unsigned int getLightningTexture();
    unsigned int getShadowTexture();

    void setupNewFrame(bool clear = true);
    void setupLightning(bool clear = true);
    void setupShadowHQ(bool clear = true);

protected:
    unsigned int gBuffer, depthbuffer;
    unsigned int gPosition, gNormal, gAlbedoSpec;

    unsigned int lightningBuffer;
    unsigned int lightningPicture;

    unsigned int shadowBuffer;
    unsigned int shadowPicture;

    int width, height;
    int shadowMapWidth = 4096, shadowMapHeight = 4096;
};