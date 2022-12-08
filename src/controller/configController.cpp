#include "configController.h"

ConfigController::ConfigController()
{
    config = new Config();
}

Config *ConfigController::getConfig()
{
    return config;
}

void ConfigController::applyConfig()
{
    viewController->update();
    soundController->update();
}

void ConfigController::setViewController(ViewController *viewController)
{
    this->viewController = viewController;
}

void ConfigController::setSoundController(SoundController *soundController) {
    this->soundController = soundController;
}