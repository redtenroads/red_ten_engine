// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "component/componentSoundPlayer.h"
#include "math/math.h"
#include <al.h>
#include <alc.h>

void ComponentSoundPlayer::render(Matrix4 &vpMatrix, Transformation *tf)
{
    Vector4 v(0, 0, 0, 1.0f);
    Vector4 position = v * *tf->getModelMatrix();
    setPosition(position.x, position.y, position.z);
}
