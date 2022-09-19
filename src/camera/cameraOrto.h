// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "camera.h"
#include "stage/layer.h"
#include "os/view.h"

class CameraOrto : public Camera
{
public:
    EXPORT CameraOrto();
    EXPORT void prepareToRender(View *view);
    EXPORT void finishRender();
    EXPORT int getWidth();
    EXPORT int getHeight();
    EXPORT float getWidthViewProportion();
    EXPORT float getHeightViewProportion();

    EXPORT void setWidthBasedResolution(float width);
    EXPORT void setHeightBasedResolution(float height);

    View *view;

protected:
    Layer *layer;
    bool useWidthBasedProportion = true;
    float mainLine = 320.0f;
};