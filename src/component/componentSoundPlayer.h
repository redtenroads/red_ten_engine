// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "common/soundPlayer.h"
#include "component/component.h"
#include "controller/soundController.h"
#include "resource/sound.h"

class ComponentSoundPlayer : public Component, public SoundPlayer
{
public:
    EXPORT virtual void render(Matrix4 &vpMatrix, Transformation *tf);
};
