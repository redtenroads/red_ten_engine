#include "config.h"
#include <stdio.h>

#define BUFFER_SIZE 2048

bool Config::loadConfig()
{
    std::vector<ConfigPair> configPairs;
    char buffer[BUFFER_SIZE];
    FILE *cfgFile = fopen(configFilePath.c_str(), "r");

    if (cfgFile)
    {
        while (fgets(buffer, BUFFER_SIZE, cfgFile) != NULL)
        {
            ConfigPair pair;
            if (!getPairFromString(buffer, BUFFER_SIZE, &pair))
                return false;
            configPairs.push_back(pair);
        }

        if (!feof(cfgFile))
            return false;

        for (auto it = configPairs.begin(); it != configPairs.end(); it++)
        {
            printf("'%s' = '%s'\n", it->name, it->value);
            int iValue;
            if (it->name == "windowWidth" && std::stoi(it->value))
                windowWidth = std::stoi(it->value);
            if (it->name == "windowHeight" && std::stoi(it->value))
                windowHeight = std::stoi(it->value);
            if (it->name == "fullscreen")
                bIsFullscreen = it->value == "true";
            if (it->name == "videoDevice")
                videoDevice = it->value;
            if (it->name == "shadowResolution" && std::stoi(it->value))
                shadowResolution = std::stoi(it->value);
        }

        fclose(cfgFile);
        return true;
    }
    return false;
}

bool Config::saveConfig()
{
    std::vector<ConfigPair> configPairs;
    char buffer[2048];
    FILE *cfgFile = fopen(configFilePath.c_str(), "w");

    if (cfgFile)
    {
        sprintf(buffer, "%s=%d\n", "windowWidth", windowWidth);
        fputs(buffer, cfgFile);
        sprintf(buffer, "%s=%d\n", "windowHeight", windowHeight);
        fputs(buffer, cfgFile);
        sprintf(buffer, "%s=%s\n", "fullscreen", bIsFullscreen ? "true" : "false");
        fputs(buffer, cfgFile);
        sprintf(buffer, "%s=%s\n", "videoDevice", videoDevice.c_str());
        fputs(buffer, cfgFile);
        sprintf(buffer, "%s=%d\n", "shadowResolution", shadowResolution);
        fputs(buffer, cfgFile);

        fclose(cfgFile);
        return true;
    }
    return false;
}

void Config::setConfigFilePath(std::string configFilePath)
{
    this->configFilePath = configFilePath;
}

std::string Config::getConfigFilePath()
{
    return configFilePath;
}

void Config::setupByQuality(RenderQuality quality)
{
    switch (quality)
    {
    case RenderQuality::Fast:
        shadowResolution = 1024;
        break;

    case RenderQuality::Balanced:
        shadowResolution = 2048;
        break;

    case RenderQuality::High:
        shadowResolution = 4096;
        break;

    default:
        break;
    }
}

bool Config::isLoaded()
{
    return bIsLoaded;
}

bool Config::isDirty()
{
    return bIsDirty;
}

void Config::setCurrentVudeoDevice(std::string deviceName)
{
    this->videoDevice = deviceName;
}

std::string Config::getCurrentVudeoDevice()
{
    return videoDevice;
}

int Config::getWindowWidth()
{
    return windowWidth;
}

void Config::setWindowWidth(int width)
{
    if (this->windowWidth != width){
        this->windowWidth = width;
        bIsDirty = true;
    }
}

int Config::getWindowHeight()
{
    return windowHeight;
}

void Config::setWindowHeight(int height)
{
    if (this->windowHeight != height){
        this->windowHeight = height;
        bIsDirty = true;
    }
}

bool Config::isFullscreen()
{
    return bIsFullscreen;
}

int Config::getShadowResolution()
{
    return shadowResolution;
}

bool Config::getPairFromString(char *buffer, int limit, ConfigPair *pair)
{
    if (strlen(buffer) >= limit)
        return false;
    int iterator = 0;
    bool readingValue = false;
    for (;;)
    {
        if (iterator >= limit)
            return false;
        if (buffer[iterator] == '=')
        {
            if (readingValue)
                return false;
            readingValue = true;
            iterator++;
            continue;
        }
        if (readingValue)
            pair->name += buffer[iterator];
        else
            pair->value += buffer[iterator];
        iterator++;
    }

    return true;
}