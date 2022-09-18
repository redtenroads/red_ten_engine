#include "stageManager.h"

StageManager::StageManager()
{
}

Stage *StageManager::createStage(std::string name)
{
    auto newStage = new Stage(name);
    if (newStage)
    {
        this->stages.push_back(newStage);
    }
    return newStage;
}