// SPDX-FileCopyrightText: 2022 Dmitrii Shashkov
// SPDX-License-Identifier: MIT

#pragma once
#include "common/utils.h"
#include "stage/stage.h"
#include "os/view.h"
#include <list>

class StageController
{
public:
    StageController();

    EXPORT Stage *createStage(std::string name);

protected:
    std::list<Stage *> stages;
    bool isExitIntended = false;
};
