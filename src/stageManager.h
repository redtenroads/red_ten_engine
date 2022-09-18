#pragma once
#include "utils.h"
#include "stage.h"
#include "view.h"
#include <list>

class StageManager
{
public:
    StageManager();

    EXPORT Stage *createStage(std::string name);

protected:
    std::list<Stage *> stages;
    bool isExitIntended = false;
};
