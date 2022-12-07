#include "configController.h"

ConfigController::ConfigController()
{
    config = new Config();
}

Config *ConfigController::getConfig()
{
    return config;
}