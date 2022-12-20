#include "config.h"
#include <stdio.h>

#define BUFFER_SIZE 2048

bool Config::loadConfig()
{
    std::vector<ConfigPair> configPairs;
    char buffer[BUFFER_SIZE];
    FILE *cfgFile;
    errno_t err = fopen_s(&cfgFile, configFilePath.c_str(), "r");

    if (err == 0)
    {
        while (fgets(buffer, BUFFER_SIZE, cfgFile) != NULL)
        {
            ConfigPair pair;
            if (getPairFromString(buffer, BUFFER_SIZE, &pair))
                configPairs.push_back(pair);
        }

        if (!feof(cfgFile))
        {
            fclose(cfgFile);
            return false;
        }

        for (auto it = configPairs.begin(); it != configPairs.end(); it++)
        {
            if (it->name == "windowWidth" && std::stoi(it->value))
                windowWidth = std::stoi(it->value);
            if (it->name == "windowHeight" && std::stoi(it->value))
                windowHeight = std::stoi(it->value);
            if (it->name == "fullscreen")
                bIsFullscreen = it->value == "true";
            if (it->name == "videoDevice")
                videoDevice = it->value;
            if (it->name == "audioDevice")
                audioDevice = it->value;
            if (it->name == "shadowQuality")
                shadowQuality = stringToQuality(it->value);
        }

        bIsLoaded = true;
        fclose(cfgFile);
        return true;
    }
    logger->logff("Error %i happend on conf file open\n", err);
    return false;
}

bool Config::saveConfig()
{
    std::vector<ConfigPair> configPairs;
    char buffer[2048];
    FILE *cfgFile;
    errno_t err = fopen_s(&cfgFile, configFilePath.c_str(), "w");

    if (err == 0)
    {
        sprintf(buffer, "%s=%d\n", "windowWidth", windowWidth);
        fputs(buffer, cfgFile);
        sprintf(buffer, "%s=%d\n", "windowHeight", windowHeight);
        fputs(buffer, cfgFile);
        sprintf(buffer, "%s=%s\n", "fullscreen", bIsFullscreen ? "true" : "false");
        fputs(buffer, cfgFile);
        sprintf(buffer, "%s=%s\n", "videoDevice", videoDevice.c_str());
        fputs(buffer, cfgFile);
        sprintf(buffer, "%s=%s\n", "audioDevice", audioDevice.c_str());
        fputs(buffer, cfgFile);
        sprintf(buffer, "%s=%s\n", "shadowQuality", qualityToString(shadowQuality).c_str());
        fputs(buffer, cfgFile);

        fclose(cfgFile);
        return true;
    }
    else if (err != 2)
        logger->logff("Error %i happend on conf file open\n", err);
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
    shadowQuality = quality;
}

bool Config::isLoaded()
{
    return bIsLoaded;
}

bool Config::isDirty()
{
    return bIsDirty;
}

void Config::setCurrentVideoDevice(std::string deviceName)
{
    this->videoDevice = deviceName;
}

std::string Config::getCurrentVideoDevice()
{
    return videoDevice;
}

void Config::setCurrentAudioDevice(std::string deviceName)
{
    audioDevice = deviceName;
}

std::string Config::getCurrentAudioDevice()
{
    return audioDevice;
}

int Config::getWindowWidth()
{
    return windowWidth;
}

void Config::setWindowWidth(int width)
{
    if (this->windowWidth != width)
    {
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
    if (this->windowHeight != height)
    {
        this->windowHeight = height;
        bIsDirty = true;
    }
}

int Config::getRefreshRate()
{
    return refreshRate;
}

void Config::setRefreshRate(int refreshRate)
{
    this->refreshRate = refreshRate;
}

bool Config::isFullscreen()
{
    return bIsFullscreen;
}

void Config::setFullscreenState(bool isFullscreen)
{
    this->bIsFullscreen = isFullscreen;
}

RenderQuality Config::getShadowQuality()
{
    return shadowQuality;
}

void Config::setShadowQuality(RenderQuality quality)
{
    this->shadowQuality = quality;
}

std::string Config::qualityToString(RenderQuality quality)
{
    if (quality == RenderQuality::SuperFast)
        return "super fast";
    if (quality == RenderQuality::Fast)
        return "fast";
    if (quality == RenderQuality::High)
        return "high";
    return "balanced";
}

RenderQuality Config::stringToQuality(std::string quality)
{
    if (quality == "super fast")
        return RenderQuality::SuperFast;
    if (quality == "fast")
        return RenderQuality::Fast;
    if (quality == "high")
        return RenderQuality::High;
    return RenderQuality::Balanced;
}

bool Config::getPairFromString(char *buffer, int limit, ConfigPair *pair)
{
    if (strlen(buffer) >= limit)
        return false;

    int iterator = 0;
    bool readingValue = false;
    while (iterator < limit && buffer[iterator] != 0 && buffer[iterator] != 0 && buffer[iterator] != '\n')
    {
        if (buffer[iterator] == '=')
        {
            if (readingValue)
                return false;
            readingValue = true;
            iterator++;
            continue;
        }
        if (readingValue)
            pair->value += buffer[iterator];
        else
            pair->name += buffer[iterator];
        iterator++;
    }
    return true;
}
