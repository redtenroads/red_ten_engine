#pragma once
#include <string>
#include <vector>

enum RenderQuality
{
    Fast = 0,
    Balanced,
    High
};

struct ConfigPair
{
    std::string name;
    std::string value;
};

class Config
{
public:
    bool loadConfig();
    bool saveConfig();
    void setConfigFilePath(std::string configFilePath);
    std::string getConfigFilePath();
    void setupByQuality(RenderQuality quality);

    bool isLoaded();
    bool isDirty();

    void setCurrentVudeoDevice(std::string deviceName);
    std::string getCurrentVudeoDevice();

    int getWindowWidth();
    void setWindowWidth(int width);
    int getWindowHeight();
    void setWindowHeight(int height);
    int getRefreshRate();
    void setRefreshRate(int height);
    bool isFullscreen();
    void setFullscreenState(bool isFullscreen);
    int getShadowResolution();

protected:
    bool getPairFromString(char *buffer, int limit, ConfigPair *pair);
    std::string configFilePath = "./cfg";
    bool bIsLoaded = false;
    bool bIsDirty = false;

    int windowWidth = 0, windowHeight = 0;
    int refreshRate = 0;
    bool bIsFullscreen = false;
    std::string videoDevice = "";

    int shadowResolution = 2048;
};
