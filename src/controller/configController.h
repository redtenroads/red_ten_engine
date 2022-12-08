#pragma once
#include "common/config.h"
#include "common/utils.h"
#include "controller/viewController.h"
#include "controller/soundController.h"

class ConfigController
{
public:
    EXPORT ConfigController();
    EXPORT Config *getConfig();
    EXPORT void applyConfig();
    
    void setViewController(ViewController *viewController);
    void setSoundController(SoundController *soundController);

protected:
    Config *config;
    ViewController *viewController;
    SoundController *soundController;
};