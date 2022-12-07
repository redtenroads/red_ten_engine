#pragma once
#include "common/config.h"

class ConfigController
{
public:
    ConfigController();
    Config *getConfig();
    
protected:
    Config *config;
};