#pragma once
#include "common/withLogger.h"
#include "common/utils.h"
#include <string>
#include <vector>

enum RenderQuality
{
    SuperFast = 0,
    Fast,
    Balanced,
    High
};

struct ConfigPair
{
    std::string name;
    std::string value;
};

class Config : public WithLogger
{
public:
    EXPORT bool loadConfig();
    EXPORT bool saveConfig();
    EXPORT void setConfigFilePath(std::string configFilePath);
    EXPORT std::string getConfigFilePath();
    EXPORT void setupByQuality(RenderQuality quality);

    EXPORT bool isLoaded();
    EXPORT bool isDirty();

    EXPORT void setCurrentVideoDevice(std::string deviceName);
    EXPORT std::string getCurrentVideoDevice();
    EXPORT void setCurrentAudioDevice(std::string deviceName);
    EXPORT std::string getCurrentAudioDevice();

    EXPORT int getWindowWidth();
    EXPORT void setWindowWidth(int width);
    EXPORT int getWindowHeight();
    EXPORT void setWindowHeight(int height);
    EXPORT int getRefreshRate();
    EXPORT void setRefreshRate(int refreshRate);
    EXPORT bool isFullscreen();
    EXPORT void setFullscreenState(bool isFullscreen);
    EXPORT RenderQuality getShadowQuality();
    EXPORT void setShadowQuality(RenderQuality quality);

    EXPORT static std::string qualityToString(RenderQuality quality);
    EXPORT static RenderQuality stringToQuality(std::string quality);

protected:
    bool getPairFromString(char *buffer, int limit, ConfigPair *pair);

    std::string configFilePath = "./cfg";
    bool bIsLoaded = false;
    bool bIsDirty = false;

    int windowWidth = 0, windowHeight = 0;
    int refreshRate = 0;
    bool bIsFullscreen = false;
    std::string videoDevice = "";
    std::string audioDevice = "";

    RenderQuality shadowQuality = RenderQuality::Balanced;
};
