// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#include "stageController.h"

StageController::StageController()
{
}

Stage *StageController::createStage(std::string name)
{
    auto newStage = new Stage(name);
    if (newStage)
    {
        this->stages.push_back(newStage);
    }
    return newStage;
}